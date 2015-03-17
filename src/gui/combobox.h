#ifndef GUI_COMBOBOX_H
#define GUI_COMBOBOX_H

#include "component.h"

#include <mw/text.h>
#include <mw/font.h>

#include <vector>

namespace gui {

	class ComboBox : public Component {
	public:
		ComboBox(const mw::Font& font);

		ComboBox(const std::vector<std::string>&  items, const mw::Font& font);

		void addItem(std::string item);

		std::string getChosenItemText() const {
			return chosenItem_;
		}

		void draw(Uint32 deltaTime) override;

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

	private:
		void handleKeyboard(const SDL_Event& keyEvent) override;

		mw::Color textColor_, markColor_;
		mw::Font font_;
		std::vector<mw::Text> items_;
		std::string chosenItem_;
	};

} // Namespace gui.

#endif // GUI_COMBOBOX_H
