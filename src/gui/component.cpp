#include "component.h"
#include "panel.h"

#if MW_OPENGLES2
#include <mw/matrix.h>
#endif // MW_OPENGLES2

namespace gui {

	void Component::setLocation(float x, float y) {
		setLocation(Point(x, y));
	}

	void Component::setLocation(const Point& point) {
		location_ = point;
		if (parent_ != nullptr) {
			model_ = parent_->model_;
		} else {
			model_ = mw::I_44;
		}
		mw::translate2D(model_, point.x_, point.y_);
	}

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
		useGlShader();
		setGlModelMatrix(model_);
		
		GLfloat aVertices[] = {
			0, 0,
			dim.width_, 0,
			0, dim.height_,
			dim.width_, dim.height_
		};

		setGlColor(backgroundColor_);
		setGlVer2dCoords(aVertices);
		setGlTexture(false);
		
		mw::glEnable(GL_BLEND);
		mw::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		mw::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		drawSprite(background_);
		drawBorder();

		mw::glDisable(GL_BLEND);
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
		mw::checkGlError();
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

	void Component::setGlColor(float red, float green, float blue, float alpha) const {
#if MW_OPENGLES2
		windowMatrix_.setColor(red, green, blue, alpha);
#else // MW_OPENGLES2
		glColor4f(red, green, blue, alpha);
#endif // MW_OPENGLES2
	}
	
	void Component::setGlColor(const mw::Color& color) const {
#if MW_OPENGLES2
		windowMatrix_.setColor(color);
#else // MW_OPENGLES2
		glColor4f(color.red_, color.green_, color.blue_, color.alpha_);
#endif // MW_OPENGLES2
	}
	
	void Component::setGlModelMatrix(const mw::Matrix44& matrix) const {
#if MW_OPENGLES2
		windowMatrix_.setModel(matrix);
#else // MW_OPENGLES2
		glColor4f(color.red_, color.green_, color.blue_, color.alpha_);
#endif // MW_OPENGLES2
	}

	void Component::setGlProjectionMatrix(const mw::Matrix44& matrix) const {
#if MW_OPENGLES2
		windowMatrix_.setProjection(matrix);
#else // MW_OPENGLES2
		glColor4f(color.red_, color.green_, color.blue_, color.alpha_);
#endif // MW_OPENGLES2
	}

#if MW_OPENGLES2
	void Component::setGlVer2dCoords(const GLvoid* data) const {
		windowMatrix_.setVertexPosition(2, data);
	}

	void Component::setGlVer2dCoords(GLsizei stride, const GLvoid* data) const {
		windowMatrix_.setVertexPosition(2, stride, data);
	}

	void Component::setGlTexCoords(const GLvoid* data) const {
		windowMatrix_.setTexturePosition(2, data);
	}

	void Component::setGlTexCoords(GLsizei stride, const GLvoid* data) const {
		windowMatrix_.setTexturePosition(2, stride, data);
	}

	void Component::setGlTexture(bool texture) const {
		windowMatrix_.setTexture(texture);
	}

	void Component::useGlShader() const {
		windowMatrix_.useShader();
	}
#endif // MW_OPENGLES2

	Component::Component() : parent_(nullptr), borderColor_(0,0,0), layoutIndex_(0), visible_(true),
		focus_(false), grabFocus_(false), nbrChildGrabFocus_(0), isAdded_(false),
		model_(mw::I_44) {

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
		useGlShader();
		setGlColor(borderColor_);
		setGlTexture(false);

		GLfloat border[] = {
			// North.
			0, dimension_.height_ - 1,
			dimension_.width_, dimension_.height_ - 1,
			0, dimension_.height_,
			0, dimension_.height_,
			dimension_.width_, dimension_.height_ - 1,
			dimension_.width_, dimension_.height_,
			// West.
			0, 0,
			1, 0,
			0, dimension_.height_ - 1,
			0, dimension_.height_ - 1,
			1, 0,
			1, dimension_.height_ - 1,
			// East.
			dimension_.width_ - 1, 1,
			dimension_.width_, 1,
			dimension_.width_ - 1, dimension_.height_ - 1,
			dimension_.width_ - 1, dimension_.height_ - 1,
			dimension_.width_, 1,
			dimension_.width_, dimension_.height_ - 1,
			// South.
			0, 0,
			dimension_.width_, 0,
			0, 1,
			0, 1,
			dimension_.width_, 0,
			dimension_.width_, 1
		};

		setGlVer2dCoords(border);
		mw::glDrawArrays(GL_TRIANGLES, 0, 6*4);
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
		mw::checkGlError();
	}

	void Component::drawSprite(const mw::Sprite& sprite) const {
		const mw::Texture& texture = sprite.getTexture();
		texture.bindTexture();
		if (texture.isValid()) {
#if MW_OPENGLES2
			// Centered square in ORIGO.
			GLfloat aVertices[] = {
				0, 0,
				dimension_.width_, 0,
				0, dimension_.height_,
				dimension_.width_, dimension_.height_};

			// Map the sprite out from the texture.
			GLfloat aTexCoord[] = {
				sprite.getX() / texture.getWidth(), sprite.getY() / texture.getHeight(),
				(sprite.getX() + sprite.getWidth()) / texture.getWidth(), sprite.getY() / texture.getHeight(),
				sprite.getX() / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight(),
				(sprite.getX() + sprite.getWidth()) / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight()};

			setGlTexture(true);

			// Load the vertex data.
			setGlVer2dCoords(aVertices);
			setGlTexCoords(aTexCoord);

			// Upload the attributes and draw the sprite.
			mw::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#else // MW_OPENGLES2
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(GL_QUADS);
			glTexCoord2f(sprite.getX() / texture.getWidth(), sprite.getY() / texture.getHeight());
			glVertex2f(0, 0);

			glTexCoord2f((sprite.getX() + sprite.getWidth()) / texture.getWidth(), sprite.getY() / texture.getHeight());
			glVertex2f(dimension_.width_, 0);

			glTexCoord2f((sprite.getX() + sprite.getWidth()) / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight());
			glVertex2f(dimension_.width_, dimension_.height_);

			glTexCoord2f(sprite.getX() / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight());
			glVertex2f(0, dimension_.height_);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
#endif // MW_OPENGLES2
			mw::checkGlError();
		}
	}

} // Namespace gui.
