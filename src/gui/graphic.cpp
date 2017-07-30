#include "graphic.h"

#include <mw/buffer.h>

#include <array>

namespace gui {

	Graphic::Graphic() {
	}

	Graphic::Graphic(std::string vShaderFile, std::string fShaderFile) {
		guiShader_ = GuiShader(vShaderFile, fShaderFile);

		guiVertexData_ = std::make_shared<GuiVertexData>(guiShader_);
		guiVertexData_->begin();
		guiVertexData_->addSquareTRIANGLE_STRIP(0, 0, 1, 1);
		guiVertexData_->end();
		
		mw::Buffer buffer(mw::Buffer::STATIC);
		buffer.addVertexData(guiVertexData_);
		buffer.uploadToGraphicCard();
	}

	// Uniforms. -------------------------------------------
	void Graphic::setColor(const mw::Color& color) const {
		guiShader_.setUColor(color);
	}

	void Graphic::setColor(float red, float green, float blue, float alpha) const {
		guiShader_.setUColor(mw::Color(red, green, blue, alpha));
	}

	void Graphic::drawBorder(float x, float y, float w, float h) const {
		// North.
		drawSquare(x, y + h - 1, w, 1);
		// West.
		drawSquare(x, y, 1, h);
		// East.
		drawSquare(x + w - 1, y, 1, h);
		// South.
		drawSquare(x, y, w, 1);
	}

	void Graphic::drawSquare(float x, float y, float w, float h) const {
		mw::Matrix44<GLfloat> uPos = mw::Matrix44<GLfloat>::I;
		mw::translate2D(uPos, x, y);
		mw::scale2D(uPos, w, h);
		guiShader_.setUPos(uPos);

		// Use as non texture!
		guiShader_.setUIsTex(false);
		draw();
	}

	void Graphic::drawSprite(const mw::Sprite& sprite, float x, float y, float w, float h) const {
		const mw::Texture& texture = sprite.getTexture();
		if (texture.isValid()) {
			guiShader_.useProgram();
			texture.bindTexture();

			mw::Matrix44<GLfloat> uPos(
				w, 0, 0, x,
				0, h, 0, y,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			guiShader_.setUPos(uPos);

			mw::Matrix44<GLfloat> uTex(
				sprite.getWidth() / texture.getWidth(), 0, 0, sprite.getX() / texture.getWidth(),
				0, sprite.getHeight() / texture.getHeight(), 0, sprite.getY() / texture.getHeight(),
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			guiShader_.setUTex(uTex);

			// Use as texture!
			guiShader_.setUIsTex(true);
			draw();
		}
	}

	void Graphic::drawText(const mw::Text& text, float x, float y) const {
		if (text.isValid()) {
			guiShader_.useProgram();
			text.bindTexture();

			mw::Matrix44<GLfloat> uPos(
				text.getWidth(), 0, 0, x,
				0, text.getHeight(), 0, y,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			guiShader_.setUPos(uPos);

			// Send the whole texture => I_44.
			guiShader_.setUTex(mw::Matrix44<GLfloat>::I);

			// Use as texture!
			guiShader_.setUIsTex(true);
			draw();
		}
	}

	void Graphic::setModel(const mw::Matrix44<GLfloat>& model) const {
		guiShader_.setUModel(model);
	}

	void Graphic::setProj(const mw::Matrix44<GLfloat>& proj) {
		proj_ = proj;
		guiShader_.setUProj(proj);
	}

	void Graphic::draw() const {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		guiVertexData_->drawTRIANGLE_STRIP();

		glDisable(GL_BLEND);
	}

} // Namespace gui.
