#include "checkbox.h"

#include <functional>
#include <string>

namespace gui {

	namespace {

		void doActionSelect(CheckBox* checkBox) {
			checkBox->setSelected(!checkBox->isSelected());
		}

	}

	CheckBox::CheckBox(int characterSize) {
		addActionListener(std::bind(doActionSelect, this));
		boxSize_ = (float) characterSize;
		selected_ = false;
		setPreferredSize(boxSize_ + 2, boxSize_ + 3);
		setBorderColor(1, 1, 1, 0);
		setBackgroundColor(1, 1, 1, 0);
	}

	CheckBox::CheckBox(std::string text, const mw::Font& font) : Button(text, font) {
		addActionListener(std::bind(doActionSelect, this));
		boxSize_ = (float) font.getCharacterSize();
		selected_ = false;
		setPreferredSize(boxSize_ + text_.getWidth() + 3, text_.getHeight() + 2);
		setBorderColor(1, 1, 1, 0);
		setBackgroundColor(1, 1, 1, 0);
	}

	CheckBox::CheckBox(mw::Text text) : Button(text) {
		addActionListener(std::bind(doActionSelect, this));
		boxSize_ = text.getCharacterSize();
		selected_ = false;
		setPreferredSize(boxSize_ + text_.getWidth() + 3, text_.getHeight() + 2);
		setBorderColor(1, 1, 1, 0);
		setBackgroundColor(1, 1, 1, 0);
	}

	void CheckBox::draw(Uint32 deltaTime) {
		Component::draw(deltaTime);
#if MW_OPENGLES2
		auto wM = getWindowMatrixPtr();
		wM->useShader();
		wM->setColor(textColor_);
		wM->setTexture(false);
		mw::Matrix44 oldModel = wM->getModel();
		mw::Matrix44 newModel = oldModel * mw::getTranslateMatrix(1, 1);
		wM->setModel(newModel);
		float vertices[] = {
			0, 0,
			boxSize_, 0,
			boxSize_, boxSize_,
			0, boxSize_,
			0, 0,
		};
		wM->setVertexPosition(2, vertices); // vec2, i.e. dimension = 2.
		wM->glDrawArrays(GL_LINE_STRIP, 0, 5); // 5 vertices.
		if (selected_) {
			float vertices[] = {
				boxSize_ * 0.1f, boxSize_* 0.9f,
				boxSize_ * 0.2f, boxSize_* 0.1f,
				boxSize_ * 0.9f, boxSize_ * 0.9f
			};
			wM->setVertexPosition(2, vertices); // vec2, i.e. dimension = 2.
			wM->glDrawArrays(GL_LINE_STRIP, 0, 3); // 3 vertices.
		}
		newModel = oldModel * mw::getTranslateMatrix(boxSize_ + 2, 1);
		wM->setModel(newModel);
		text_.draw();
		wM->setModel(oldModel);
#else // MW_OPENGLES2
		textColor_.glColor4f();
		glPushMatrix();
		glTranslatef(1, 1, 0);

		// Draw box.
		glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(boxSize_, 0);

		glVertex2f(boxSize_, 0);
		glVertex2f(boxSize_, boxSize_);

		glVertex2f(boxSize_, boxSize_);
		glVertex2f(0, boxSize_);

		glVertex2f(0, boxSize_);
		glVertex2f(0, 0);
				
		if (selected_) {
			glVertex2f(boxSize_ * 0.2f, boxSize_* 0.1f);
			glVertex2f(boxSize_ * 0.1f, boxSize_* 0.9f);

			glVertex2f(boxSize_ * 0.2f, boxSize_ * 0.1f);
			glVertex2f(boxSize_ * 0.9f, boxSize_ * 0.9f);
		}

		glEnd();

		glPopMatrix();

		glPushMatrix();
		glTranslatef(boxSize_ + 2, 1, 0);
		text_.draw();
		glPopMatrix();
#endif // MW_OPENGLES2
	}

} // Namespace gui.
