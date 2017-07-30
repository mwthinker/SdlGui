#ifndef GUI_TEXTFIELD_H
#define GUI_TEXTFIELD_H

#include "component.h"
#include "inputformatter.h"

#include <mw/text.h>
#include <mw/font.h>

#include <string>

namespace gui {

	class TextField : public Component {
	public:
		enum Alignment {
			LEFT,
			CENTER,
			RIGHT
		};

		TextField(const mw::Font& font);

		TextField(std::string initialText , const mw::Font& font);

		~TextField() = default;

		// Get the current text.
		std::string getText() const;

		// Set the current text.
		void setText(std::string text);

		// Set the textfield to be editable or not.
		void setEditable(bool editable);

		bool isEditable() const;

		virtual void draw(const Graphic& graphic, double deltaTime) override;

		Alignment getAlignment() const;
		void setAlignment(Alignment alignment);

		// Set the color for the text.
		void setTextColor(const mw::Color& textColor);

		// Set the color for the text.
		void setTextColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha = 1);

	private:
		void handleKeyboard(const SDL_Event& keyEvent) override;

		bool editable_;
		mw::Text text_;
		float markerWidth_;
		bool markerChanged_;
		mw::Font font_;

		mw::Color textColor_;
		Alignment alignment_;
		InputFormatter inputFormatter_;
		double markerDeltaTime_;
	};

} // Namespace gui.

#endif // GUI_TEXTFIELD_H
