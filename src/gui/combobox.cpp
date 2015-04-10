#include "combobox.h"
#include "panel.h"

namespace gui {

	ComboBox::ComboBox(const mw::Font& font) :
		chosenItem_("", font), textColor_(0, 0, 0), addedDrawFunction_(false),
		mouseInside_(false), pushed_(false) {

		setBorderColor(0, 0, 0);
		setBackgroundColor(1, 1, 1);

		setPreferredSize(150, 20);
	}

	void ComboBox::addItem(std::string item) {
		items_.emplace_back(item, chosenItem_.getFont());
	}

	void ComboBox::draw(const Graphic& graphic, double deltaTime) {
		Component::draw(graphic, deltaTime);
		graphic.setColor(textColor_);
		graphic.drawText(chosenItem_, 2, 0);
	}

	void ComboBox::drawLast(Frame& frame, const Graphic& graphic, double deltaTime) {
		if (pushed_) {
			auto dim = getSize();
			float y = 2;
			for (auto& text : items_) {
				y += text.getHeight();
				graphic.setColor(getBackgroundColor());
				graphic.drawSquare(0, -y, dim.width_, text.getHeight());
				graphic.setColor(getBorderColor());
				graphic.drawBorder(0, -y, dim.width_, text.getHeight());
				graphic.setColor(textColor_);
				graphic.drawText(text, 2, -y);
			}
		}
	}

	void ComboBox::handleKeyboard(const SDL_Event& keyEvent) {
		// TODO!
	}

	void ComboBox::handleMouse(const SDL_Event& mouseEvent) {
		switch (mouseEvent.type) {
			case SDL_MOUSEMOTION:
				mouseInside_ = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (mouseEvent.button.button) {
					case SDL_BUTTON_LEFT:
						pushed_ = !pushed_;
						break;
				}
				break;
		}
	}

} // Namespace gui.
