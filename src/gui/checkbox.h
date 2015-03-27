#ifndef GUI_CHECKBOX_H
#define GUI_CHECKBOX_H

#include "component.h"

#include <mw/font.h>
#include <mw/text.h>
#include <mw/sprite.h>

#include <string>

namespace gui {

	class CheckBox : public Component {
	public:
		CheckBox(float characterSize, const mw::Sprite& box, const mw::Sprite& check);
		CheckBox(std::string text, const mw::Font& font, const mw::Sprite& box, const mw::Sprite& check);
		CheckBox(const mw::Text& text, const mw::Sprite& box, const mw::Sprite& check);

		virtual void draw(std::chrono::high_resolution_clock::duration) override;

		inline bool isSelected() const {
			return selected_;
		}

		inline void setSelected(bool select) {
			selected_ = select;
		}

		// Set the color for the text label.
		void setTextColor(const mw::Color& color);

		// Set the color for the text label.
		void setTextColor(float red, float green, float blue, float alpha = 1);

		// Set the box color.
		void setBoxColor(const mw::Color& color);

		// Set the box color.
		void setBoxColor(float red, float green, float blue, float alpha = 1);

		// Set the check color.
		void setCheckColor(const mw::Color& color);

		// Set the check color.
		void setCheckColor(float red, float green, float blue, float alpha = 1);

	private:
		void handleMouse(const SDL_Event&) override;

		mw::Text text_;
		mw::Color textColor_, boxColor_, checkColor_;
		mw::Sprite box_, check_;

		float boxSize_;
		bool selected_;
		bool mouseDown_, pushed_;
	};

} // Namespace gui.

#endif // GUI_CHECKBOX_H
