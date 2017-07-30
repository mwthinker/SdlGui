#ifndef GUI_LABEL_H
#define GUI_LABEL_H

#include "component.h"

#include <mw/text.h>

namespace gui {

	class Label : public Component {
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

		Label(std::string text, const mw::Font& font);
		Label(mw::Text text);

		virtual ~Label() = default;

		virtual void draw(const Graphic& graphic, double deltaTime) override;

		// Sets the font used by the label.
		void setFont(const mw::Font& font);

		std::string getText() const;
		void setText(std::string text);

		HorizontalAlignment getHorizontalAlignment() const;
		void setHorizontalAlignment(HorizontalAlignment alignment);

		VerticalAlignment getVerticalAlignment() const;
		void setVerticalAlignment(VerticalAlignment alignment);

		// Sets the color for the text.
		void setTextColor(const mw::Color& textColor);

		// Sets the color for the text.
		void setTextColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1);

	private:
		mw::Text text_;
		mw::Color textColor_;
		VerticalAlignment vTextAlignment_;
		HorizontalAlignment hTextAlignment_;
	};

} // Namespace gui.

#endif // GUI_LABEL_H
