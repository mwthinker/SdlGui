#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include "component.h"

#include <mw/signal.h>
#include <mw/text.h>

#include <functional>
#include <string>

namespace gui {
	
	class Button : public Component {
	public:
		enum HorizontalAlignment {
			LEFT,
			HCENTER,
			RIGHT
		};

		enum VerticalAlignment {
			TOP,
			VCENTER,
			BOTTOM
		};

		Button();
		Button(std::string text, const mw::Font& font);
		Button(mw::Text text);

		virtual ~Button() = default;

		// Get the label of the Button instance.
		std::string getLabel() const;

		// Sets the label of the Button instance.
		void setLabel(std::string buttonLabel);

		// Sets the font used by the label.
		void setFont(const mw::Font& font);

		// Returns true when the mouse is inside the button. Else 
		// it returns false.
		bool isMouseInside() const;

		// Returns true if the button was pushed. Else 
		// it returns false.
		bool isPushed() const;

		// Returns true if the mouse left button is hold down when the 
		// mouse hovers the button. Else it returns false.
		bool isMouseDown() const;
		
		// Sets the vertical alignment for the text label.
		void setVerticalAlignment(VerticalAlignment alignment);
		
		// Sets the horizontal alignment for the text label.
		void setHorizontalAlignment(HorizontalAlignment alignment);

		const mw::Color& getTextColor() const {
			return textColor_;
		}

		// Sets the color for the text label.
		void setTextColor(const mw::Color& textColor);

		void setTextColor(float red, float green, float blue, float alpha = 1);

		virtual void sizeToFitText();

		void setAutoSizeToFitText(bool autoFit);

		bool isAutoSizeToFitText() const;

		const mw::Color& getHoverColor() const {
			return hoverColor_;
		}

		void setHoverColor(const mw::Color& color) {
			hoverColor_ = color;
		}

		void setHoverColor(float red, float green, float blue, float alpha = 1) {
			hoverColor_ = mw::Color(red, green, blue, alpha);
		}

		const mw::Color& getFocusColor() const {
			return focusColor_;
		}

		void setFocusColor(float red, float green, float blue, float alpha = 1) {
			focusColor_ = mw::Color(red, green, blue, alpha);
		}

		void setFocusColor(const mw::Color& color) {
			focusColor_ = color;
		}

		const mw::Color& getPushColor() const {
			return pushColor_;
		}

		void setPushColor(float red, float green, float blue, float alpha = 1) {
			pushColor_ = mw::Color(red, green, blue, alpha);
		}

		void setPushColor(const mw::Color& color) {
			pushColor_ = color;
		}

	protected:
		// Is drawn if this butten has focus while the mouse 
		// button was is outside this button.
		virtual void drawOnFocus(const Graphic& graphic);
		// Is drawn if the mouse is hovering and 
		// the mouse is not pushed down.
		virtual void drawOnMouseHover(const Graphic& graphic);
		// Is drawn if the button was pushed.
		virtual void drawOnPush(const Graphic& graphic);
		// Draws the label.
		virtual void drawLabel(const Graphic& graphic);

		virtual void draw(const Graphic& graphic, double deltaTime) override;

		virtual void mouseMotionLeave() override;

		virtual void mouseOutsideUp() override;

		mw::Text text_;

		mw::Color textColor_;
		mw::Color hoverColor_;
		mw::Color focusColor_;
		mw::Color pushColor_;

		VerticalAlignment vTextAlignment_;
		HorizontalAlignment hTextAlignment_;

		bool autoFit_;

	private:
		void handleMouse(const SDL_Event&) override;
		void handleKeyboard(const SDL_Event&) override;
		
		bool pushed_;
		bool mouseDown_;
		bool mouseInside_;
	};

} // Namespace gui.

#endif // GUI_BUTTON_H
