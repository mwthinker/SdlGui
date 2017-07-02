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

		virtual ~ComboBox() = default;

		void addItem(std::string item);

		std::string getChosenItemText() const {
			return chosenItem_.getText();
		}

		void draw(const Graphic& graphic, double deltaTime) override;

		void setTextColor(const mw::Color<GLfloat>& color) {
			textColor_ = color;
		}

		void setTextColor(float red, float green, float blue, float alpha = 1) {
			textColor_ = mw::Color<GLfloat>(red, green, blue, alpha);
		}

		mw::Color<GLfloat> getTextColor() const {
			return textColor_;
		}
				
		void setMarkColor(const mw::Color<GLfloat>& color) {
			markColor_ = color;
		}
		
		void setMarkColor(float red, float green, float blue, float alpha = 1) {
			markColor_ = mw::Color<GLfloat>(red, green, blue, alpha);
		}

		mw::Color<GLfloat> getMarkColor() const {
			return markColor_;
		}

		int getItemCount() const {
			return items_.size();
		}

		void drawLast(Frame& frame, const Graphic& graphic, double deltaTime) override;

		// Returns true when the mouse is inside the button. Else 
		// it returns false.
		bool isMouseInside() const;

		// Returns true if the button was pushed. Else 
		// it returns false.
		bool isPushed() const;

		// Returns true if the mouse left button is hold down when the 
		// mouse hovers the button. Else it returns false.
		bool isMouseDown() const;

		void mouseMotionLeave() override;

		void mouseOutsideUp() override;

	private:
		void handleKeyboard(const SDL_Event& keyEvent) override;

		void handleMouse(const SDL_Event& mouseEvent) override;
		
		std::vector<mw::Text> items_;
		mw::Color<GLfloat> textColor_, markColor_;
		mw::Color<GLfloat> hoverColor_;
		mw::Color<GLfloat> focusColor_;
		mw::Color<GLfloat> pushColor_;

		mw::Text chosenItem_;
		bool addedDrawFunction_;
		bool mouseInside_, pushed_;
		bool mouseDown_;
	};

} // Namespace gui.

#endif // GUI_COMBOBOX_H
