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

		vao_ = 0;
	}

	Graphic::Graphic(std::string vShaderFile, std::string fShaderFile) {
		shader_.bindAttribute("aPos");
		shader_.bindAttribute("aTex");
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
		
		glGenVertexArrays(1, &vao_);
		glBindVertexArray(vao_);
		// A vertex quad defined as a GL_TRIANGLE_STRIP.
		std::array<float, 8> data = {0, 0, 1, 0, 0, 1, 1, 1};

		vbo_.bindBufferData(GL_ARRAY_BUFFER, sizeof(data), data.data(), GL_STATIC_DRAW);
	}

	// Uniforms. -------------------------------------------
	void Graphic::setColor(const mw::Color& color) const {
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
		mw::Matrix44 uPos = mw::I_44;
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

			mw::Matrix44 uPos = mw::I_44;
			mw::translate2D(uPos, x, y);
			mw::scale2D(uPos, w, h);
			glUniformMatrix4fv(uPosIndex_, 1, false, uPos.data());
			mw::checkGlError();

			mw::Matrix44 uTex = mw::I_44;
			mw::translate2D(uTex, sprite.getX(), sprite.getY());
			mw::scale2D(uTex, sprite.getWidth() / texture.getWidth(), sprite.getHeight() / texture.getHeight());
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

			mw::Matrix44 uPos = mw::I_44;
			mw::translate2D(uPos, x, y);
			mw::scale2D(uPos, text.getWidth(), text.getHeight());
			glUniformMatrix4fv(uPosIndex_, 1, false, uPos.data());
			mw::checkGlError();
			
			// Send the whole texture => I_44.
			glUniformMatrix4fv(uTexIndex_, 1, false, mw::I_44.data());
			mw::checkGlError();

			// Use as texture!
			glUniform1f(uIsTexIndex_, 1);
			mw::checkGlError();

			draw();
		}
	}

	void Graphic::setModel(const mw::Matrix44& model) const {
		shader_.useProgram();
		glUniformMatrix4fv(uModelIndex_, 1, false, model.data());
	}

	void Graphic::setProj(const mw::Matrix44& proj) {
		proj_ = proj;
		shader_.useProgram();
		glUniformMatrix4fv(uProjIndex_, 1, false, proj.data());
	}

	void Graphic::draw() const {
		vbo_.bindBuffer();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mw::checkGlError();

		glBindVertexArray(vao_);
		glEnableVertexAttribArray(aPosIndex_);
		glVertexAttribPointer(aPosIndex_, 2, GL_FLOAT, GL_FALSE, 0, 0);		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisableVertexAttribArray(aPosIndex_);
		mw::checkGlError();
		
		glDisable(GL_BLEND);
		vbo_.unbindBuffer();
	}

} // Namespace gui.
