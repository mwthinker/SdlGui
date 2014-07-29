#include "button.h"

#include <functional>
#include <string>

namespace gui {

	Button::Button() {
		init();
	}

	Button::Button(std::string text, const mw::Font& font) : text_(text , font) {
		init();
	}

	Button::Button(mw::Text text) : text_(text) {
		init();
	}

	// Draws the button. The button is drawn by calls
	// to different functions.
	// By default the background is drawn first and then the border. 
	// Then one of the following functions is called: 
	// drawOnMouseHover(), drawOnFocus() or drawOnPush().
	// On top of everything is the drawing of the label.
	void Button::draw(Uint32 deltaTime) {
		Component::draw(deltaTime);
		if (pushed_ && mouseInside_) {
			drawOnPush();
		} else if (mouseInside_) {
			drawOnMouseHover();
		} else if (hasFocus()) {
			drawOnFocus();
		}
		drawLabel();
	}

	// Get the label of this Button instance.
	std::string Button::getLabel() const {
		return text_.getText();
	}

	// Set the label of this Button instance.
	void Button::setLabel(std::string label) {
		text_.setText(label);
		if (autoFit_) {
			sizeToFitText();
		}
	}

	void Button::setFont(const mw::Font& font) {
		text_ = mw::Text(text_.getText(), font);
		toWide_ = mw::Text(toWide_.getText(), font);
		if (autoFit_) {
			sizeToFitText();
		}
	}

	void Button::handleKeyboard(const SDL_Event& keyEvent) {
		SDL_Keycode key = keyEvent.key.keysym.sym;
		switch (keyEvent.type) {
			case SDL_KEYDOWN:
				// Has focus and pressed enter?
				if (hasFocus() && key == SDLK_RETURN && key == SDLK_KP_ENTER) {
					doAction();
				}
				break;
		}
	}

	void Button::handleMouse(const SDL_Event& mouseEvent) {
		switch (mouseEvent.type) {
			case SDL_MOUSEMOTION:
				mouseInside_ = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (mouseEvent.button.button) {
					case SDL_BUTTON_LEFT:
						pushed_ = true;
						mouseDown_ = true;
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (mouseEvent.button.button) {
					case SDL_BUTTON_LEFT:
						if (mouseDown_) {
							doAction();
							mouseDown_ = false;
							pushed_ = false;
						}
						break;
				}
				break;
		}
	}

	bool Button::isMouseInside() const {
		return mouseInside_;
	}

	bool Button::isPushed() const {
		return pushed_;
	}

	bool Button::isMouseDown() const {
		return mouseDown_;
	}

	void Button::mouseMotionLeave() {
		mouseInside_ = false;
	}

	void Button::mouseOutsideUp() {
		mouseDown_ = false;
		pushed_ = false;
	}

	void Button::setVerticalAlignment(VerticalAlignment alignment) {
		vTextAlignment_ = alignment;
	}

	void Button::setHorizontalAlignment(HorizontalAlignment alignment) {
		hTextAlignment_ = alignment;
	}

	void Button::setTextColor(const mw::Color& textColor) {
		textColor_ = textColor;
	}

	void Button::setTextColor(float red, float green, float blue, float alpha) {
		textColor_ = mw::Color(red, green, blue, alpha);
	}

	void Button::sizeToFitText() {
		if (text_.getWidth() > 1 && text_.getHeight() > 1) {
			setPreferredSize(2 + text_.getWidth(), 2 + text_.getHeight());
		}
	}

	void Button::setAutoSizeToFitText(bool autoFit) {
		if (autoFit_ != autoFit) {
			sizeToFitText();
			autoFit_ = autoFit;
		}
	}

	bool Button::isAutoSizeToFitText() const {
		return autoFit_;
	}

	void Button::drawLabel() {
		Dimension dim = getSize();

		float x = 0.0;
		switch (hTextAlignment_) {
			case HorizontalAlignment::LEFT:
				x = 0;
				break;
			case HorizontalAlignment::HCENTER:
				x = dim.width_ * 0.5f - text_.getWidth() * 0.5f;
				break;
			case HorizontalAlignment::RIGHT:
				x = dim.width_ - text_.getWidth();
				break;
		}
		float y = 0.0;
		switch (vTextAlignment_) {
			case VerticalAlignment::BOTTOM:
				y = 0;
				break;
			case VerticalAlignment::VCENTER:
				y = dim.height_ * 0.5f - text_.getHeight() * 0.5f;
				break;
			case VerticalAlignment::TOP:
				y = dim.height_ - text_.getHeight();
				break;
		}
		textColor_.glColor4f();
		glPushMatrix();
		glTranslatef(x, y, 0);

		if (text_.getWidth() < dim.width_) {
			text_.draw();
		} else {
			toWide_.draw();
		}

		glPopMatrix();
	}

	void Button::drawOnMouseHover() {
		Dimension dim = getSize();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		hoverColor_.glColor4f();
		glBegin(GL_QUADS);
		glVertex2d(0, 0);
		glVertex2d(dim.width_, 0);
		glVertex2d(dim.width_, dim.height_);
		glVertex2d(0, dim.height_);
		glEnd();
		glDisable(GL_BLEND);
	}

	void Button::drawOnFocus() {
		Dimension dim = getSize();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		focusColor_.glColor4f();
		glBegin(GL_QUADS);
		glVertex2d(0, 0);
		glVertex2d(dim.width_, 0);
		glVertex2d(dim.width_, dim.height_);
		glVertex2d(0, dim.height_);
		glEnd();
		glDisable(GL_BLEND);
	}

	void Button::drawOnPush() {
		Dimension dim = getSize();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		pushColor_.glColor4f();
		glBegin(GL_QUADS);
		glVertex2d(0, 0);
		glVertex2d(dim.width_, 0);
		glVertex2d(dim.width_, dim.height_);
		glVertex2d(0, dim.height_);
		glEnd();
		glDisable(GL_BLEND);
	}

	void Button::init() {
		setPreferredSize(50, 50);
		pushed_ = false;
		mouseDown_ = false;
		mouseInside_ = false;
		vTextAlignment_ = VerticalAlignment::VCENTER;
		hTextAlignment_ = HorizontalAlignment::HCENTER;
		setBackgroundColor(0.9f, 0.9f, 0.9f);
		textColor_ = mw::Color(0, 0, 0);
		focusColor_ = mw::Color(0, 0, 0, 0.05f);
		hoverColor_ = mw::Color(0, 0, 0, 0.1f);
		pushColor_ = mw::Color(0, 0, 0, 0.15f);

		autoFit_ = false;
		toWide_ = text_;
		toWide_.setText(". . .");
	}
	
} // Namespace gui.
