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
		enum class HorizontalAlign {
			LEFT, MIDDLE, RIGHT
		};

		ComboBox(const mw::Font& font, const mw::Sprite& showDropDown = mw::Sprite(), HorizontalAlign horizontalAlign = HorizontalAlign::LEFT);

		virtual ~ComboBox() = default;

		int addItem(std::string item);

		int getSelectedItem() const;

		void setSelectedItem(int item);

		std::string getChosenItemText() const;

		void setTextColor(const mw::Color<GLfloat>& color);

		void setTextColor(float red, float green, float blue, float alpha = 1);

		mw::Color<GLfloat> getTextColor() const;

		inline const mw::Color<GLfloat>& getFocusColor() const;

		void setFocusColor(float red, float green, float blue, float alpha = 1);

		void setFocusColor(const mw::Color<GLfloat>& color);

		void setSelectedBackgroundColor(float red, float green, float blue, float alpha = 1);

		void setSelectedBackgroundColor(const mw::Color<GLfloat>& color);

		void setSelectedTextColor(float red, float green, float blue, float alpha = 1);

		void setSelectedTextColor(const mw::Color<GLfloat>& color);

		int getItemCount() const;

		// Set the check color.
		void setShowDropDownColor(const mw::Color<GLfloat>& color);

		// Set the check color.
		void setShowDropDownColor(float red, float green, float blue, float alpha = 1);

		// Returns true when the mouse is inside the button. Else 
		// it returns false.
		bool isMouseInside() const;

		// Returns true if the button was pushed. Else 
		// it returns false.
		bool isPushed() const;

	protected:
		virtual void handleKeyboard(const SDL_Event& keyEvent) override;

		virtual void handleMouse(const SDL_Event& mouseEvent) override;

		virtual void mouseMotionLeave() override;

		virtual void mouseOutsideUp() override;

		virtual void draw(const Graphic& graphic, double deltaTime) override;

		virtual void drawLast(Frame& frame, const Graphic& graphic, double deltaTime) override;

	private:
		std::vector<mw::Text> items_;
		mw::Color<GLfloat> textColor_, markColor_;
		mw::Color<GLfloat> hoverColor_;
		mw::Color<GLfloat> focusColor_;
		mw::Color<GLfloat> pushColor_;

		mw::Color<GLfloat> selectedBackgroundColor_;
		mw::Color<GLfloat> selectedTextColor_;
		mw::Color<GLfloat> showDropDownColor_;


		mw::Font font_;
		mw::Sprite showDropDown_;
		HorizontalAlign textHorizontalAlign_;

		int selectedItem_;
		int currentItem_;
		bool mouseInside_, pushed_;
		bool dropDownOpen_;

		Dimension originalSize_;
		Point originalLocation_;
	};


	inline int ComboBox::getSelectedItem() const {
		return selectedItem_;
	}

	inline void ComboBox::setSelectedItem(int item) {
		if (item >= 0 && item < (int) items_.size()) {
			if (selectedItem_ != item) {
				selectedItem_ = item;
				doAction();
			}
		}
	}

	inline std::string ComboBox::getChosenItemText() const {
		if (selectedItem_ >= 0 && selectedItem_ < (int) items_.size()) {
			return items_[selectedItem_].getText();
		}
		return "";
	}


	inline void ComboBox::setTextColor(const mw::Color<GLfloat>& color) {
		textColor_ = color;
	}

	inline void ComboBox::setTextColor(float red, float green, float blue, float alpha) {
		textColor_ = mw::Color<GLfloat>(red, green, blue, alpha);
	}

	inline mw::Color<GLfloat> ComboBox::getTextColor() const {
		return textColor_;
	}

	inline const mw::Color<GLfloat>& ComboBox::getFocusColor() const {
		return focusColor_;
	}

	inline void ComboBox::setFocusColor(float red, float green, float blue, float alpha) {
		focusColor_ = mw::Color<GLfloat>(red, green, blue, alpha);
	}

	inline void ComboBox::setFocusColor(const mw::Color<GLfloat>& color) {
		focusColor_ = color;
	}

	inline void ComboBox::setSelectedBackgroundColor(float red, float green, float blue, float alpha) {
		selectedBackgroundColor_ = mw::Color<GLfloat>(red, green, blue, alpha);
	}

	inline void ComboBox::setSelectedBackgroundColor(const mw::Color<GLfloat>& color) {
		selectedBackgroundColor_ = color;
	}

	inline void ComboBox::setSelectedTextColor(float red, float green, float blue, float alpha) {
		selectedTextColor_ = mw::Color<GLfloat>(red, green, blue, alpha);
	}

	inline void ComboBox::setSelectedTextColor(const mw::Color<GLfloat>& color) {
		selectedTextColor_ = color;
	}

	inline int ComboBox::getItemCount() const {
		return items_.size();
	}

} // Namespace gui.

#endif // GUI_COMBOBOX_H
