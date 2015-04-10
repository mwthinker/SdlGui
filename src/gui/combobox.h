#ifndef GUI_COMBOBOX_H
#define GUI_COMBOBOX_H

#include "component.h"

#include <mw/text.h>
#include <mw/font.h>

#include <vector>
#include <memory>

namespace gui {

	class ComboBox : public Component {
	public:
		ComboBox(const mw::Font& font);

		void addItem(std::string item);

		std::string getChosenItemText() const {
			return chosenItem_.getText();
		}

		void draw(const Graphic& graphic, double deltaTime) override;

		void setTextColor(const mw::Color& color) {
			textColor_ = color;
		}

		void setTextColor(float red, float green, float blue, float alpha = 1) {
			textColor_ = mw::Color(red, green, blue, alpha);
		}

		inline mw::Color getTextColor() const {
			return textColor_;
		}
				
		void setMarkColor(const mw::Color& color) {
			markColor_ = color;
		}
		
		void setMarkColor(float red, float green, float blue, float alpha = 1) {
			markColor_ = mw::Color(red, green, blue, alpha);
		}

		inline mw::Color getMarkColor() const {
			return markColor_;
		}

		inline int getItemCount() const {
			return items_.size();
		}

		void drawLast(Frame& frame, const Graphic& graphic, double deltaTime) override;

	private:
		void handleKeyboard(const SDL_Event& keyEvent) override;

		void handleMouse(const SDL_Event& mouseEvent) override;
		
		std::vector<mw::Text> items_;
		mw::Color textColor_, markColor_;
		mw::Text chosenItem_;
		bool addedDrawFunction_;
		bool mouseInside_, pushed_;
	};

} // Namespace gui.

#endif // GUI_COMBOBOX_H
