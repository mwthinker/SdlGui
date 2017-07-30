#include "combobox.h"
#include "panel.h"

namespace gui {

	namespace {
	
		float calculateTextAlignDeltaX(ComboBox::HorizontalAlign horizontalAlign, const mw::Text& text, Dimension dim) {
			float x = 0;
			switch (horizontalAlign) {
				case ComboBox::HorizontalAlign::LEFT:
					x = 2;
					break;
				case ComboBox::HorizontalAlign::MIDDLE:
					x = (dim.width_ - text.getWidth() - dim.height_) * 0.5f; // Ignore the sprite at the right (dim.height_)
					break;
				case ComboBox::HorizontalAlign::RIGHT:
					x = dim.width_ - text.getWidth() - dim.height_ - 2; // Ignore the sprite at the right (dim.height_)
					break;
			}
			return x;
		}
	
	}

	ComboBox::ComboBox(const mw::Font& font, const mw::Sprite& showDropDown, HorizontalAlign horizontalAlign) :
		font_(font),
		showDropDown_(showDropDown),
		textHorizontalAlign_(horizontalAlign),
		dropDownOpen_(false),
		selectedItem_(-1),
		currentItem_(-1),
		textColor_(0, 0, 0),
		mouseInside_(false), pushed_(false),
		focusColor_(0, 0, 0, 0.05f),
		showDropDownColor_(0, 0, 0, 1),
		originalSize_(-1, -1),
		selectedBackgroundColor_(0.5f, 0.5f, 0.5f) {

		setBorderColor(0.4f, 0, 0);
		setBackgroundColor(0.9f, 0.9f, 0.9f);
		selectedTextColor_ = getBackgroundColor();

		setPreferredSize(150, 20);
	}

	int ComboBox::addItem(std::string item) {
		items_.emplace_back(item, font_);
		return items_.size() - 1;
	}

	void ComboBox::draw(const Graphic& graphic, double deltaTime) {
		// Draw panel background.
		Dimension dim = originalSize_;
		if (dim.width_ < 0) {
			dim = getSize();
		}

		float x = 0, y = 0;
		if (pushed_) {
			y = originalSize_.height_ * (items_.size());
		}

		if (hasFocus()) {
			graphic.setColor(focusColor_);
		} else {
			graphic.setColor(getBackgroundColor());
		}
		graphic.drawSquare(x, y, dim.width_, dim.height_);
		graphic.drawSprite(getBackground(), x, y, dim.width_, dim.height_);

		graphic.setColor(getBorderColor());
		graphic.drawBorder(x, y, dim.width_, dim.height_);

		graphic.setColor(textColor_);
		if (selectedItem_ >= 0 && selectedItem_ < (int) items_.size()) {
			graphic.drawText(items_[selectedItem_], calculateTextAlignDeltaX(textHorizontalAlign_, items_[selectedItem_], originalSize_), y);
		}

		float spriteSize = dim.height_;
		graphic.setColor(showDropDownColor_);
		graphic.drawSprite(showDropDown_,
			x + dim.width_ - spriteSize, y,
			spriteSize, spriteSize
		);

		if (pushed_) {
			auto dim = getSize();
			int size = items_.size();
			for (int i = 0; i < size; ++i) {
				float y = originalSize_.height_ * (size - i - 1);
				if (currentItem_ == i) {
					graphic.setColor(selectedBackgroundColor_);
				} else {
					graphic.setColor(getBackgroundColor());
				}
				graphic.drawSquare(0, y, dim.width_, originalSize_.height_);

				graphic.setColor(getBorderColor());
				graphic.drawBorder(0, y, dim.width_, originalSize_.height_);

				if (currentItem_ == i) {
					graphic.setColor(selectedTextColor_);
				} else {
					graphic.setColor(textColor_);
				}
				graphic.drawText(items_[i], calculateTextAlignDeltaX(textHorizontalAlign_, items_[i], originalSize_), y);
			}
		}
	}

	void ComboBox::handleKeyboard(const SDL_Event& keyEvent) {
		SDL_Keycode key = keyEvent.key.keysym.sym;
		switch (keyEvent.type) {
			case SDL_KEYDOWN:
				// Has focus and pressed enter?
				if (hasFocus()) {
					switch (keyEvent.key.keysym.sym) {
						case SDLK_DOWN:
							selectedItem_ = (selectedItem_ + 1) % items_.size();
							currentItem_ = selectedItem_;
							doAction();
							break;
						case SDLK_UP:
							selectedItem_ = (selectedItem_ + items_.size() - 1) % items_.size();
							currentItem_ = selectedItem_;
							doAction();
							break;
					}
				}
				break;
		}
	}

	void ComboBox::handleMouse(const SDL_Event& mouseEvent) {
		switch (mouseEvent.type) {
			case SDL_MOUSEMOTION:
				mouseInside_ = true;
				if (pushed_) {
					currentItem_ = (int) (items_.size() - mouseEvent.motion.y / originalSize_.height_);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (mouseEvent.button.button) {
					case SDL_BUTTON_LEFT:
						pushed_ = true;
						demandPriority();
						currentItem_ = selectedItem_;
						originalSize_ = getSize();
						originalLocation_ = getLocation();
						setLocation(originalLocation_.x_, originalLocation_.y_ - items_.size() * originalSize_.height_);
						setSize(originalSize_.width_, (items_.size() + 1) * originalSize_.height_);
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (mouseEvent.button.button) {
					case SDL_BUTTON_LEFT:
						if (pushed_) {
							setSelectedItem(currentItem_); // Select the item, plus make sure that no invalid item is selected.
							doAction();
							pushed_ = false;
							setLocation(originalLocation_);
							setSize(originalSize_);
						}
						break;
				}
				break;
		}
	}

	bool ComboBox::isMouseInside() const {
		return mouseInside_;
	}

	bool ComboBox::isPushed() const {
		return pushed_;
	}

	void ComboBox::mouseMotionLeave() {
		mouseInside_ = false;
	}

	void ComboBox::mouseOutsideUp() {
		pushed_ = false;
		setLocation(originalLocation_);
		setSize(originalSize_);
	}

	void ComboBox::setShowDropDownColor(const mw::Color& color) {
		showDropDownColor_ = color;
	}

	void ComboBox::setShowDropDownColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
		showDropDownColor_ = mw::Color(red, green, blue, alpha);
	}

} // Namespace gui.
