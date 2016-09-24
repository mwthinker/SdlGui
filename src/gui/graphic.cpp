#include "graphic.h"

#include <array>

namespace gui {

	Graphic::Graphic() {
		aPosIndex_ = -1;

		uProjIndex_ = -1;
		uModelIndex_ = -1;
		uPosIndex_ = -1;
		uTexIndex_ = -1;
		uIsTexIndex_ = -1;
		uColorIndex_ = -1;
	}

	Graphic::Graphic(std::string vShaderFile, std::string fShaderFile) {
		shader_.bindAttribute("aPos");
		shader_.loadAndLinkFromFile(vShaderFile, fShaderFile);

		shader_.useProgram();

		// Collect the vertex buffer attributes indexes.
		aPosIndex_ = shader_.getAttributeLocation("aPos");

		// Collect the vertex buffer uniforms indexes.
		uProjIndex_ = shader_.getUniformLocation("uProj");
		uModelIndex_ = shader_.getUniformLocation("uModel");
		uPosIndex_ = shader_.getUniformLocation("uPos");
		uTexIndex_ = shader_.getUniformLocation("uTex");
		uIsTexIndex_ = shader_.getUniformLocation("uIsTex");
		uColorIndex_ = shader_.getUniformLocation("uColor");

		// A vertex quad defined as a GL_TRIANGLE_STRIP.
		std::array<GLfloat, 8> data = {0, 0, 1, 0, 0, 1, 1, 1};

		vbo_.bindBufferData(GL_ARRAY_BUFFER, sizeof(data), data.data(), GL_STATIC_DRAW);
	}

	// Uniforms. -------------------------------------------
	void Graphic::setColor(const mw::Color<GLfloat>& color) const {
		shader_.useProgram();
		glUniform4f(uColorIndex_, color.red_, color.green_, color.blue_, color.alpha_);
	}

	void Graphic::setColor(float red, float green, float blue, float alpha) const {
		shader_.useProgram();
		glUniform4f(uColorIndex_, red, green, blue, alpha);
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
		shader_.useProgram();
		mw::Matrix44<GLfloat> uPos = mw::Matrix44<GLfloat>::I;
		mw::translate2D(uPos, x, y);
		mw::scale2D(uPos, w, h);
		glUniformMatrix4fv(uPosIndex_, 1, false, uPos.data());
		mw::checkGlError();

		// Use as non texture!
		glUniform1f(uIsTexIndex_, 0);
		mw::checkGlError();

		draw();
	}

	void Graphic::drawSprite(const mw::Sprite& sprite, float x, float y, float w, float h) const {
		const mw::Texture& texture = sprite.getTexture();
		if (texture.isValid()) {
			shader_.useProgram();
			texture.bindTexture();

			mw::Matrix44<GLfloat> uPos(
				w, 0, 0, x,
				0, h, 0, y,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			glUniformMatrix4fv(uPosIndex_, 1, false, uPos.data());
			mw::checkGlError();

			mw::Matrix44<GLfloat> uTex(
				sprite.getWidth() / texture.getWidth(), 0, 0, sprite.getX() / texture.getWidth(),
				0, sprite.getHeight() / texture.getHeight(), 0, sprite.getY() / texture.getHeight(),
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			glUniformMatrix4fv(uTexIndex_, 1, false, uTex.data());
			mw::checkGlError();

			// Use as texture!
			glUniform1f(uIsTexIndex_, 1);
			mw::checkGlError();

			draw();
		}
	}

	void Graphic::drawText(const mw::Text& text, float x, float y) const {
		if (text.isValid()) {
			shader_.useProgram();
			text.bindTexture();

			mw::Matrix44<GLfloat> uPos(
				text.getWidth(), 0, 0, x,
				0, text.getHeight(), 0, y,
				0, 0, 1, 0,
				0, 0, 0, 1
			);

			glUniformMatrix4fv(uPosIndex_, 1, false, uPos.data());
			mw::checkGlError();

			// Send the whole texture => I_44.
			glUniformMatrix4fv(uTexIndex_, 1, false, mw::Matrix44<GLfloat>::I.data());
			mw::checkGlError();

			// Use as texture!
			glUniform1f(uIsTexIndex_, 1);
			mw::checkGlError();

			draw();
		}
	}

	void Graphic::setModel(const mw::Matrix44<GLfloat>& model) const {
		shader_.useProgram();
		glUniformMatrix4fv(uModelIndex_, 1, false, model.data());
	}

	void Graphic::setProj(const mw::Matrix44<GLfloat>& proj) {
		proj_ = proj;
		shader_.useProgram();
		glUniformMatrix4fv(uProjIndex_, 1, false, proj.data());
	}

	void Graphic::draw() const {
		vbo_.bindBuffer();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mw::checkGlError();

		glEnableVertexAttribArray(aPosIndex_);
		glVertexAttribPointer(aPosIndex_, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableVertexAttribArray(aPosIndex_);
		mw::checkGlError();

		glDisable(GL_BLEND);
		vbo_.unbindBuffer();
	}

} // Namespace gui.
