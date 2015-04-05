#include "component.h"
#include "panel.h"

#include <mw/matrix.h>

#include <cassert>

namespace gui {

	mw::Matrix44 Component::proj;

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
	
	void Component::draw(double deltaTime) {
		// Draw panel background.
		Dimension dim = getSize();
		enableTransparancy();
		useProgram();
		setModelU(model_);
		setColorU(backgroundColor_);
		drawSquare(0, 0, dim.width_, dim.height_);
		drawSprite(background_, 0, 0, dim.width_, dim.height_);
		drawBorder();
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

	void Component::drawArrays(GLenum mode, GLfloat* data, int size, bool drawTexture) const {
		switch (mode) {
			case GL_TRIANGLES:
				assert((size % 4) == 0 && (size % 6 * 4) == 0);
				break;
			case GL_TRIANGLE_FAN:
				assert((size % 4) == 0);
				break;
			case GL_TRIANGLE_STRIP:
				assert((size % 4) == 0);
				break;
			default:
				assert(0);
				break;
		}
		mw::checkGlError();
		setTextureU(drawTexture);
		setPosA(2, sizeof(GLfloat) * 4, (GLvoid*) data);
		setTexA(2, sizeof(GLfloat) * 4, (GLvoid*) (data + 2));
		glDrawArrays(mode, 0, size / 4);
		mw::checkGlError();
	}

	void Component::enableTransparancy() const {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Component::disableTransparancy() const {
		glDisable(GL_BLEND);
	}

	void Component::useProgram() const {
		guiShader_.useProgram();
	}

	void Component::setPosA(GLint size, const GLvoid* data) const {
		guiShader_.setPosA(size, data);
	}

	void Component::setPosA(GLint size, GLsizei stride, const GLvoid* data) const {
		guiShader_.setPosA(size, stride, data);
	}

	void Component::setTexA(GLint size, const GLvoid* data) const {
		guiShader_.setTexA(size, data);
	}

	void Component::setTexA(GLint size, GLsizei stride, const GLvoid* data) const {
		guiShader_.setTexA(size, stride, data);
	}

	void Component::setTextureU(bool texture) const {
		guiShader_.setTextureU(texture);
	}

	void Component::setModelU(const mw::Matrix44& matrix) const {
		guiShader_.setModelU(matrix);
	}

	void Component::setColorU(const mw::Color& color) const {
		guiShader_.setColorU(color);
	}

	void Component::setColorU(float red, float green, float blue, float alpha) const {
		guiShader_.setColorU(red, green, blue, alpha);
	}
	
	Component::Component() : parent_(nullptr), ancestor_(nullptr),
		borderColor_(0, 0, 0), backgroundColor_(1, 1, 1), layoutIndex_(0), visible_(true),
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
		setColorU(borderColor_);
		drawBorder(0, 0, dimension_.width_, dimension_.height_);
	}

	void Component::drawBorder(float x, float y, float w, float h) const {
		GLfloat data[6 * 4 * 4] = {
			// North.
			x, y + h - 1, 0, 0,
			x + w, y + h - 1, 0, 0,
			x, y + h, 0, 0,
			x, y + h, 0, 0,
			x + w, y + h - 1, 0, 0,
			x + w, y + h, 0, 0,
			// West.
			x, y, 0, 0,
			x + 1, y, 0, 0,
			x, y + h - 1, 0, 0,
			x, y + h - 1, 0, 0,
			x + 1, y, 0, 0,
			x + 1, y + h - 1, 0, 0,
			// East.
			x + w - 1, y + 1, 0, 0,
			x + w, y + 1, 0, 0,
			x + w - 1, y + h - 1, 0, 0,
			x + w - 1, y + h - 1, 0, 0,
			x + w, y + 1, 0, 0,
			x + w, y + h - 1, 0, 0,
			// South.
			x, y, 0, 0,
			x + w, y, 0, 0,
			x, y + 1, 0, 0,
			x, y + 1, 0, 0,
			x + w, y, 0, 0,
			x + w, y + 1, 0, 0
		};
		drawArrays(GL_TRIANGLES, data, 6 * 4 * 4, false);
	}

	void Component::drawSquare(float x, float y, float w, float h) const {
		// Centered square in ORIGO.
		GLfloat data[16] = {
			x, y,
			0, 0,
			x + w, y,
			0, 0,
			x, y + h,
			0, 0,
			x + w, y + h,
			0, 0};

		// Upload the attributes and draw the sprite.
		drawArrays(GL_TRIANGLE_STRIP, data, 16, false);
	}

	void Component::drawSprite(const mw::Sprite& sprite, float x, float y, float w, float h) const {
		const mw::Texture& texture = sprite.getTexture();
		if (texture.isValid()) {
			texture.bindTexture();

			// Centered square in ORIGO.
			GLfloat data[16] = {
				x, y,
				sprite.getX() / texture.getWidth(), sprite.getY() / texture.getHeight(),
				x + w, y,
				(sprite.getX() + sprite.getWidth()) / texture.getWidth(), sprite.getY() / texture.getHeight(),
				x, y + h,
				sprite.getX() / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight(),
				x + w, y + h,
				(sprite.getX() + sprite.getWidth()) / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight()};

			// Upload the attributes and draw the sprite.
			drawArrays(GL_TRIANGLE_STRIP, data, 16, true);
		}
	}

	void Component::drawText(const mw::Text& text, float x, float y) const {
		if (text.isValid()) {
			text.bindTexture();

			// Lower left corner is in ORIGO.
			GLfloat data[16] = {
				x, y,
				0, 0,
				x + text.getWidth(), y,
				1, 0,
				x, text.getHeight() + y,
				0, 1,
				x + text.getWidth(), text.getHeight() + y,
				1, 1};

			// Upload the attributes and draw the sprite.
			drawArrays(GL_TRIANGLE_STRIP, data, 16, true);
		}
	}

} // Namespace gui.
