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

	std::shared_ptr<Panel> Component::getParent() const {
		return parent_;
	}

	void Component::draw(Uint32 deltaTime) {
		// Draw panel background.
		Dimension dim = getSize();
		enableGlTransparancy();
		glUseProgram();
		setGlModelU(model_);
		setGlColorU(backgroundColor_);
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
#if MW_OPENGLES2
		setGlTextureU(drawTexture);
		setGlPosA(2, sizeof(GLfloat) * 4, (GLvoid*) data);
		setGlTexA(2, sizeof(GLfloat) * 4, (GLvoid*) (data + 2));
		mw::glDrawArrays(mode, 0, size / 4);
#else // MW_OPENGLES2
		if (drawTexture) {
			glEnable(GL_TEXTURE_2D);
			glBegin(mode);
			for (int i = 0; i < size; i += 4) {
				glTexCoord2f(data[i + 2], data[i + 3]);
				glVertex2f(data[i], data[i + 1]);
			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		} else {
			glBegin(mode);
			for (int i = 0; i < size; i += 4) {
				glVertex2f(data[i], data[i + 1]);
			}
			glEnd();
		}
#endif // MW_OPENGLES2
		mw::checkGlError();
	}

	void Component::enableGlTransparancy() const {
#if MW_OPENGLES2
		mw::glEnable(GL_BLEND);
		mw::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#else // MW_OPENGLES2
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif // MW_OPENGLES2
	}

	void Component::disableGlTransparancy() const {
#if MW_OPENGLES2
		mw::glDisable(GL_BLEND);
#else // MW_OPENGLES2
		glDisable(GL_BLEND);
#endif // MW_OPENGLES2
	}

	void Component::glUseProgram() const {
#if MW_OPENGLES2
		guiShader_.glUseProgram();
#endif // MW_OPENGLES2
	}

#if MW_OPENGLES2
	void Component::setGlPosA(GLint size, const GLvoid* data) const {
		guiShader_.setGlPosA(size, data);
	}

	void Component::setGlPosA(GLint size, GLsizei stride, const GLvoid* data) const {
		guiShader_.setGlPosA(size, stride, data);
	}

	void Component::setGlTexA(GLint size, const GLvoid* data) const {
		guiShader_.setGlTexA(size, data);
	}

	void Component::setGlTexA(GLint size, GLsizei stride, const GLvoid* data) const {
		guiShader_.setGlTexA(size, stride, data);
	}

	void Component::setGlTextureU(bool texture) const {
		guiShader_.setGlTextureU(texture);
	}

	void Component::setGlModelU(const mw::Matrix44& matrix) const {
		guiShader_.setGlModelU(matrix);
	}

	void Component::setGlColorU(const mw::Color& color) const {
		guiShader_.setGlColorU(color);
	}

	void Component::setGlColorU(float red, float green, float blue, float alpha) const {
		guiShader_.setGlColorU(red, green, blue, alpha);
	}

#else // MW_OPENGLES2
	void Component::setGlColorU(float red, float green, float blue, float alpha) const {
		glColor4f(red, green, blue, alpha);
	}

	void Component::setGlColorU(const mw::Color& color) const {
		glColor4f(color.red_, color.green_, color.blue_, color.alpha_);
	}

	void Component::setGlModelU(const mw::Matrix44& matrix) const {
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(matrix.data());
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
		GLfloat data[6 * 4 * 4] = {
			// North.
			0, dimension_.height_ - 1, 0, 0,
			dimension_.width_, dimension_.height_ - 1, 0, 0,
			0, dimension_.height_, 0, 0,
			0, dimension_.height_, 0, 0,
			dimension_.width_, dimension_.height_ - 1, 0, 0,
			dimension_.width_, dimension_.height_, 0, 0,
			// West.
			0, 0, 0, 0,
			1, 0, 0, 0,
			0, dimension_.height_ - 1, 0, 0,
			0, dimension_.height_ - 1, 0, 0,
			1, 0, 0, 0,
			1, dimension_.height_ - 1, 0, 0,
			// East.
			dimension_.width_ - 1, 1, 0, 0,
			dimension_.width_, 1, 0, 0,
			dimension_.width_ - 1, dimension_.height_ - 1, 0, 0,
			dimension_.width_ - 1, dimension_.height_ - 1, 0, 0,
			dimension_.width_, 1, 0, 0,
			dimension_.width_, dimension_.height_ - 1, 0, 0,
			// South.
			0, 0, 0, 0,
			dimension_.width_, 0, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			dimension_.width_, 0, 0, 0,
			dimension_.width_, 1, 0, 0
		};
		
		setGlColorU(borderColor_);
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
		const mw::Texture& texture = text.getTexture();
		if (!text.getText().empty() && texture.isValid()) {
			texture.bindTexture();

			// Lower left corner is in ORIGO.
			GLfloat data[16] = {
				x, y,
				0, 0,
				x + texture.getWidth(), y,
				1, 0,
				x, texture.getHeight() + y,
				0, 1,
				x + texture.getWidth(), texture.getHeight() + y,
				1, 1};

			// Upload the attributes and draw the sprite.
			drawArrays(GL_TRIANGLE_STRIP, data, 16, true);
		}
	}

} // Namespace gui.
