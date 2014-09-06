#include "component.h"
#include "panel.h"

#if MW_OPENGLES2
#include <mw/matrix.h>
#endif // MW_OPENGLES2

namespace gui {

	void Component::setPreferredSize(float width, float height) {
		preferedDimension_ = Dimension(width, height);
		validateParent();
	}

	void Component::setPreferredSize(const Dimension& dimension) {
		preferedDimension_ = dimension;
		validateParent();
	}

	void Component::setVisible(bool visible) {
		if (visible_ != visible) {
			visible_ = visible;
			validateParent();
		}
	}

	mw::signals::Connection Component::addKeyListener(const KeyListener::Callback& callback) {
		return keyListener_.connect(callback);
	}

	mw::signals::Connection Component::addMouseListener(const MouseListener::Callback& callback) {
		return mouseListener_.connect(callback);
	}

	mw::signals::Connection Component::addFocusListener(const FocusListener::Callback& callback) {
		return focusListener_.connect(callback);
	}

	mw::signals::Connection Component::addActionListener(const ActionListener::Callback& callback) {
		return actionListener_.connect(callback);
	}

	mw::signals::Connection Component::addPanelChangeListener(const PanelChangeListener::Callback& callback) {
		return panelChangeListener_.connect(callback);
	}

	void Component::setGrabFocus(bool grabFocus) {
		if (grabFocus_ != grabFocus) {
			// State is changed.
			grabFocus_ = grabFocus;

			if (grabFocus) {
				setFocus(true);
				if (parent_ != nullptr) {
					// This component is one more.
					++parent_->nbrChildGrabFocus_;
				}
			} else {
				// Have children that wants focus?
				if (nbrChildGrabFocus_ > 1) {
					// Has a parent?
					if (parent_ != nullptr) {
						// One component less.
						--parent_->nbrChildGrabFocus_;
					}
				}
			}
		}
	}

	bool Component::isGrabFocus() const {
		return grabFocus_;
	}

	void Component::setFocus(bool focus) {
		// Change?
		if (focus_ != focus) {
			if (focus || grabFocus_ || (!focus && nbrChildGrabFocus_ < 1)) {
				focus_ = focus;
				focusListener_(*this);
			}
		}
	}

	bool Component::hasFocus() const {
		return focus_;
	}

	std::shared_ptr<Panel> Component::getParent() const {
		return parent_;
	}

	void Component::draw(Uint32 deltaTime) {
		// Draw panel background.
		Dimension dim = getSize();
#if MW_OPENGLES2
		auto wM = getWindowMatrixPtr();
		wM->useShader();
		mw::Matrix44 oldModel = wM->getModel();
		mw::Matrix44 newModel = oldModel * mw::getScaleMatrix(dim.width_, dim.height_) * mw::getTranslateMatrix(0.5f, 0.5f);
		wM->setModel(newModel);
		static GLfloat aVertices[] = {
			-0.5f, -0.5f,
			0.5f, -0.5f,
			-0.5f,  0.5f,
			0.5f,  0.5f};
		mw::glEnable(GL_BLEND);
		mw::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		wM->setColor(backgroundColor_);
		wM->setVertexPosition(2, aVertices);
		wM->setTexture(false);
		wM->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		mw::glDisable(GL_BLEND);
		background_.draw();
		wM->setModel(oldModel);
		drawBorder();
#else // MW_OPENGLES2
		glPushMatrix();
		backgroundColor_.glColor4f();
		glScalef(dim.width_, dim.height_, 1);
		glTranslatef(0.5f, 0.5f, 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glEnd();
		glDisable(GL_BLEND);

		background_.draw();
		glPopMatrix();
		drawBorder();
#endif // MW_OPENGLES2
	}

	void Component::doAction() {
		actionListener_(*this);
	}

	void Component::panelChanged(bool active) {
		panelChangeListener_(*this, active);
	}

	int Component::getLayoutIndex() const {
		return layoutIndex_;
	}

	void Component::setLayoutIndex(int layoutIndex) {
		layoutIndex_ = layoutIndex;
	}

	Component::Component() : parent_(nullptr), layoutIndex_(0), visible_(true),
		focus_(false), grabFocus_(false), isAdded_(false) {
		borderColor_ = mw::Color(0, 0, 0);
		nbrChildGrabFocus_ = 0;
	}

	void Component::handleMouse(const SDL_Event& mouseEvent) {
		mouseListener_(*this, mouseEvent);
	}

	void Component::handleKeyboard(const SDL_Event& keyEvent) {
		keyListener_(*this, keyEvent);
	}

	void Component::validateParent() {
		if (parent_ != nullptr) {
			parent_->validate();
		}
	}

	void Component::drawBorder() {
#if MW_OPENGLES2
		mw::glEnable(GL_BLEND);
		mw::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		auto wM = getWindowMatrixPtr();
		wM->setColor(borderColor_);
		wM->setTexture(false);
		GLfloat north[] = {
			0, dimension_.height_ - 1,
			dimension_.width_, dimension_.height_ - 1,
			0, dimension_.height_,
			dimension_.width_, dimension_.height_};
		wM->setVertexPosition(2, north);
		wM->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		GLfloat west[] = {
			0, 0,
			1, 0,
			0, dimension_.height_ - 1,
			1, dimension_.height_ - 1};
		wM->setVertexPosition(2, west);
		wM->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		GLfloat east[] = {
			dimension_.width_ - 1, 1,
			dimension_.width_, 1,
			dimension_.width_ - 1, dimension_.height_ - 1,
			dimension_.width_, dimension_.height_ - 1};
		wM->setVertexPosition(2, east);
		wM->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		GLfloat south[] = {
			0, 0,
			dimension_.width_, 0,
			0, 1,
			dimension_.width_, 1};
		wM->setVertexPosition(2, south);
		wM->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		mw::glDisable(GL_BLEND);
#else // MW_OPENGLES2
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		borderColor_.glColor4f();

		glBegin(GL_QUADS);

		// South.
		glVertex2d(0, 0);
		glVertex2d(dimension_.width_, 0);
		glVertex2d(dimension_.width_, 1);
		glVertex2d(0, 1);

		// East.
		glVertex2d(dimension_.width_ - 1, 0);
		glVertex2d(dimension_.width_, 0);
		glVertex2d(dimension_.width_, dimension_.height_);
		glVertex2d(dimension_.width_ - 1, dimension_.height_);

		// North.
		glVertex2d(0, dimension_.height_ - 1);
		glVertex2d(dimension_.width_ - 1, dimension_.height_ -1);
		glVertex2d(dimension_.width_ - 1, dimension_.height_);
		glVertex2d(0, dimension_.height_);

		// West.
		glVertex2d(0, 0);
		glVertex2d(1, 0);
		glVertex2d(1, dimension_.height_);
		glVertex2d(0, dimension_.height_);

		glEnd();
		glDisable(GL_BLEND);
#endif // MW_OPENGLES2
	}

} // Namespace gui.
