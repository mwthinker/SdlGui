#include "checkbox.h"

#include <functional>
#include <string>

namespace gui {

	namespace {

		void doActionSelect(CheckBox* checkBox) {
			checkBox->setSelected(!checkBox->isSelected());
		}

		void drawComponentSprite(Component& c, const mw::Sprite& sprite, float w, float h) {
			const mw::Texture& texture = sprite.getTexture();
			texture.bindTexture();
			if (texture.isValid()) {
#if MW_OPENGLES2
				// Centered square in ORIGO.
				GLfloat aPos[] = {
					0, 0,
					w, 0,
					0, h,
					w, h};

				// Map the sprite out from the texture.
				GLfloat aTex[] = {
					sprite.getX() / texture.getWidth(), sprite.getY() / texture.getHeight(),
					(sprite.getX() + sprite.getWidth()) / texture.getWidth(), sprite.getY() / texture.getHeight(),
					sprite.getX() / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight(),
					(sprite.getX() + sprite.getWidth()) / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight()};

				c.setGlTextureU(true);

				// Load the vertex data.
				c.setGlPosA(2, aPos);
				c.setGlTexA(2, aTex);

				// Upload the attributes and draw the sprite.
				mw::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#else // MW_OPENGLES2
				glEnable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
				glTexCoord2f(sprite.getX() / texture.getWidth(), sprite.getY() / texture.getHeight());
				glVertex2f(0, 0);

				glTexCoord2f((sprite.getX() + sprite.getWidth()) / texture.getWidth(), sprite.getY() / texture.getHeight());
				glVertex2f(w, 0);

				glTexCoord2f((sprite.getX() + sprite.getWidth()) / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight());
				glVertex2f(w, h);

				glTexCoord2f(sprite.getX() / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight());
				glVertex2f(0, h);
				glEnd();
				glDisable(GL_TEXTURE_2D);
#endif // MW_OPENGLES2
				mw::checkGlError();
			}
		}

	}

	CheckBox::CheckBox(float characterSize, const mw::Sprite& box, const mw::Sprite& check) :
		textColor_(0, 0, 0),
		boxColor_(0, 0, 0),
		checkColor_(0, 0, 0),
		box_(box),
		check_(check),
		boxSize_(characterSize),
		selected_(false),
		mouseDown_(false),
		pushed_(false) {

		addActionListener(std::bind(doActionSelect, this));
		setPreferredSize(boxSize_, boxSize_);
		setBorderColor(1, 1, 1, 0);
		setBackgroundColor(1, 1, 1, 0);
	}

	CheckBox::CheckBox(std::string text, const mw::Font& font, const mw::Sprite& box, const mw::Sprite& check) :
		text_(text, font),
		textColor_(0, 0, 0),
		boxColor_(0, 0, 0),
		checkColor_(0, 0, 0),
		box_(box),
		check_(check),
		boxSize_((float) font.getCharacterSize()),
		selected_(false),
		mouseDown_(false),
		pushed_(false) {

		addActionListener(std::bind(doActionSelect, this));
		setPreferredSize(boxSize_ + text_.getWidth() + 1, text_.getHeight() + 2);
		setBorderColor(1, 1, 1, 0);
		setBackgroundColor(1, 1, 1, 0);
	}

	CheckBox::CheckBox(const mw::Text& text, const mw::Sprite& box, const mw::Sprite& check) :
		text_(text),
		textColor_(0, 0, 0),
		boxColor_(0, 0, 0),
		checkColor_(0, 0, 0),
		box_(box),
		check_(check),
		boxSize_(text.getCharacterSize()),
		selected_(false),
		mouseDown_(false),
		pushed_(false) {

		addActionListener(std::bind(doActionSelect, this));
		setPreferredSize(boxSize_ + text_.getWidth() + 1, text_.getHeight() + 2);
		setBorderColor(1, 1, 1, 0);
		setBackgroundColor(1, 1, 1, 0);
	}

	void CheckBox::draw(Uint32 deltaTime) {
		Component::draw(deltaTime);
#if MW_OPENGLES2
		mw::glEnable(GL_BLEND);
		mw::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		setGlColorU(boxColor_);
		drawComponentSprite(*this, box_, boxSize_, boxSize_);
		if (selected_) {
			setGlColorU(checkColor_);
			drawComponentSprite(*this, check_, boxSize_, boxSize_);
		}
		mw::glDisable(GL_BLEND);
		setGlColorU(textColor_);
		text_.draw(boxSize_, 0);
#else // MW_OPENGLES2		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		boxColor_.glColor4f();
		drawComponentSprite(*this, box_, boxSize_, boxSize_);
		if (selected_) {
			checkColor_.glColor4f();
			drawComponentSprite(*this, check_, boxSize_, boxSize_);
		}
		glDisable(GL_BLEND);
		textColor_.glColor4f();
		text_.draw(boxSize_, 0);
#endif // MW_OPENGLES2
		mw::checkGlError();
	}

	void CheckBox::setTextColor(const mw::Color& color) {
		textColor_ = color;
	}

	void CheckBox::setTextColor(float red, float green, float blue, float alpha) {
		textColor_ = mw::Color(red, green, blue, alpha);
	}

	void CheckBox::setBoxColor(const mw::Color& color) {
		boxColor_ = color;
	}

	void CheckBox::setBoxColor(float red, float green, float blue, float alpha) {
		boxColor_ = mw::Color(red, green, blue, alpha);
	}

	void CheckBox::setCheckColor(const mw::Color& color) {
		checkColor_ = color;
	}

	void CheckBox::setCheckColor(float red, float green, float blue, float alpha) {
		checkColor_ = mw::Color(red, green, blue, alpha);
	}

	void CheckBox::handleMouse(const SDL_Event& mouseEvent) {
		switch (mouseEvent.type) {
			case SDL_MOUSEBUTTONDOWN:
				switch (mouseEvent.button.button) {
					case SDL_BUTTON_LEFT:
						pushed_ = true;
						mouseDown_ = true;
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (mouseEvent.button.button) {
					case SDL_BUTTON_LEFT:
						if (mouseDown_) {
							doAction();
							mouseDown_ = false;
							pushed_ = false;
						}
						break;
				}
				break;
		}
	}

} // Namespace gui.
