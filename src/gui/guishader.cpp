#include "guishader.h"

namespace gui {

	GuiShader::GuiShader() {
		aPosIndex_ = -1;
		aTexIndex_ = -1;

		uProjIndex_ = -1;
		uModelIndex_ = -1;
		uIsTexIndex_ = -1;
		uColorIndex_ = -1;
	}

	GuiShader::GuiShader(std::string vShaderFile, std::string fShaderFile) {
		shader_.bindAttribute("aPos");
		shader_.bindAttribute("aTex");
		shader_.loadAndLinkFromFile(vShaderFile, fShaderFile);

		shader_.glUseProgram();

		// Collect the vertex buffer attributes indexes.
		aPosIndex_ = shader_.getAttributeLocation("aPos");
		aTexIndex_ = shader_.getAttributeLocation("aTex");

		// Collect the vertex buffer uniforms indexes.
		uProjIndex_ = shader_.getUniformLocation("uProj");
		uModelIndex_ = shader_.getUniformLocation("uModel");
		uIsTexIndex_ = shader_.getUniformLocation("uIsTex");
		uColorIndex_ = shader_.getUniformLocation("uColor");
	}

	void GuiShader::glUseProgram() const {
		shader_.glUseProgram();
	}

	// Vertex buffer Attributes. ---------------------------

	void GuiShader::setGlPosA(GLint size, const GLvoid* data) const {
		glEnableVertexAttribArray(aPosIndex_);
		glVertexAttribPointer(aPosIndex_, size, GL_FLOAT, GL_FALSE, 0, data);
	}

	void GuiShader::setGlPosA(GLint size, GLsizei stride, const GLvoid* data) const {
		glEnableVertexAttribArray(aPosIndex_);
		glVertexAttribPointer(aPosIndex_, size, GL_FLOAT, GL_FALSE, stride, data);
	}

	void GuiShader::setGlTexA(GLint size, const GLvoid* data) const {
		glEnableVertexAttribArray(aTexIndex_);
		glVertexAttribPointer(aTexIndex_, size, GL_FLOAT, GL_FALSE, 0, data);
	}

	void GuiShader::setGlTexA(GLint size, GLsizei stride, const GLvoid* data) const {
		glEnableVertexAttribArray(aTexIndex_);
		glVertexAttribPointer(aTexIndex_, size, GL_FLOAT, GL_FALSE, stride, data);
	}

	// Uniforms. -------------------------------------------

	void GuiShader::setGlProjU(const mw::Matrix44& matrix) const {
		glUniformMatrix4fv(uProjIndex_, 1, false, matrix.data());
	}

	void GuiShader::setGlModelU(const mw::Matrix44& matrix) const {
		glUniformMatrix4fv(uModelIndex_, 1, false, matrix.data());
	}

	void GuiShader::setGlTextureU(bool texture) const {
		glUniform1f(uIsTexIndex_, texture);
	}

	void GuiShader::setGlColorU(const mw::Color& color) const {
		glUniform4f(uColorIndex_, color.red_, color.green_, color.blue_, color.alpha_);
	}

	void GuiShader::setGlColorU(float red, float green, float blue, float alpha) const {
		glUniform4f(uColorIndex_, red, green, blue, alpha);
	}

} // Namespace gui.
