#include "checkbox.h"

#include <functional>
#include <string>

namespace gui {

	namespace {

		void doActionSelect(CheckBox* checkBox) {
			checkBox->setSelected(!checkBox->isSelected());
		}

	}

	CheckBox::CheckBox(float characterSize, const mw::Sprite& box, const mw::Sprite& check) :
		textColor_(0, 0, 0),
		boxColor_(0, 0, 0),
		checkColor_(0, 0, 0),
		box_(box),
		check_(check),
		boxSize_(characterSize),
		selected_(false),
		mouseDown_(false),
		pushed_(false) {

		addActionListener(std::bind(doActionSelect, this));
		setPreferredSize(boxSize_, boxSize_);
		setBorderColor(1, 1, 1, 0);
		setBackgroundColor(1, 1, 1, 0);
	}

	CheckBox::CheckBox(std::string text, const mw::Font& font, const mw::Sprite& box, const mw::Sprite& check) :
		text_(text, font),
		textColor_(0, 0, 0),
		boxColor_(0, 0, 0),
		checkColor_(0, 0, 0),
		box_(box),
		check_(check),
		boxSize_((float) font.getCharacterSize()),
		selected_(false),
		mouseDown_(false),
		pushed_(false) {

		addActionListener(std::bind(doActionSelect, this));
		setPreferredSize(boxSize_ + text_.getWidth() + 1, text_.getHeight() + 2);
		setBorderColor(1, 1, 1, 0);
		setBackgroundColor(1, 1, 1, 0);
	}

	CheckBox::CheckBox(const mw::Text& text, const mw::Sprite& box, const mw::Sprite& check) :
		text_(text),
		textColor_(0, 0, 0),
		boxColor_(0, 0, 0),
		checkColor_(0, 0, 0),
		box_(box),
		check_(check),
		boxSize_(text.getCharacterSize()),
		selected_(false),
		mouseDown_(false),
		pushed_(false) {

		addActionListener(std::bind(doActionSelect, this));
		setPreferredSize(boxSize_ + text_.getWidth() + 1, text_.getHeight() + 2);
		setBorderColor(1, 1, 1, 0);
		setBackgroundColor(1, 1, 1, 0);
	}

	void CheckBox::draw(double deltaTime) {
		Component::draw(deltaTime);
		enableGlTransparancy();
		setGlColorU(boxColor_);
		drawSprite(box_, 0, 0, boxSize_, boxSize_);
		if (selected_) {
			setGlColorU(checkColor_);
			drawSprite(check_, 0, 0, boxSize_, boxSize_);
		}
		setGlColorU(textColor_);
		drawText(text_, boxSize_, 0);
	}

	void CheckBox::setTextColor(const mw::Color& color) {
		textColor_ = color;
	}

	void CheckBox::setTextColor(float red, float green, float blue, float alpha) {
		textColor_ = mw::Color(red, green, blue, alpha);
	}

	void CheckBox::setBoxColor(const mw::Color& color) {
		boxColor_ = color;
	}

	void CheckBox::setBoxColor(float red, float green, float blue, float alpha) {
		boxColor_ = mw::Color(red, green, blue, alpha);
	}

	void CheckBox::setCheckColor(const mw::Color& color) {
		checkColor_ = color;
	}

	void CheckBox::setCheckColor(float red, float green, float blue, float alpha) {
		checkColor_ = mw::Color(red, green, blue, alpha);
	}

	void CheckBox::handleMouse(const SDL_Event& mouseEvent) {
		switch (mouseEvent.type) {
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

} // Namespace gui.
