#include "guishader.h"

GuiShader::GuiShader() {
	aPosIndex_ = -1;
	aTexIndex_ = -1;
	aColorIndex_ = -1;

	// Collect the vertex buffer uniforms indexes.
	uMatrixIndex_ = -1;
}

GuiShader::GuiShader(std::string vShaderFile, std::string fShaderFile) {
	shader_.bindAttribute("aPos");
	shader_.bindAttribute("aTex");
	shader_.bindAttribute("aColor");
	shader_.loadAndLinkFromFile(vShaderFile, fShaderFile);

	shader_.glUseProgram();

	// Collect the vertex buffer attributes indexes.
	aPosIndex_ = shader_.getAttributeLocation("aPos");
	aTexIndex_ = shader_.getAttributeLocation("aTex");
	aColorIndex_ = shader_.getAttributeLocation("aColor");

	// Collect the vertex buffer uniforms indexes.
	uMatrixIndex_ = shader_.getUniformLocation("uMat");
}

void GuiShader::glUseProgram() const {
	shader_.glUseProgram();
}

// Vertex buffer Attributes. ---------------------------

void GuiShader::setGlPosA(GLint size, const GLvoid* data) const {
	mw::glEnableVertexAttribArray(aPosIndex_);
	mw::glVertexAttribPointer(aPosIndex_, size, GL_FLOAT, GL_FALSE, 0, data);
}

void GuiShader::setGlPosA(GLint size, GLsizei stride, const GLvoid* data) const {
	mw::glEnableVertexAttribArray(aPosIndex_);
	mw::glVertexAttribPointer(aPosIndex_, size, GL_FLOAT, GL_FALSE, stride, data);
}

void GuiShader::setGlTexA(GLint size, const GLvoid* data) const {
	mw::glEnableVertexAttribArray(aTexIndex_);
	mw::glVertexAttribPointer(aTexIndex_, size, GL_FLOAT, GL_FALSE, 0, data);
}

void GuiShader::setGlTexA(GLint size, GLsizei stride, const GLvoid* data) const {
	mw::glEnableVertexAttribArray(aTexIndex_);
	mw::glVertexAttribPointer(aTexIndex_, size, GL_FLOAT, GL_FALSE, stride, data);
}

void GuiShader::setGlColorA(GLint size, const GLvoid* data) const {
	mw::glEnableVertexAttribArray(aColorIndex_);
	mw::glVertexAttribPointer(aColorIndex_, size, GL_FLOAT, GL_FALSE, 0, data);
}

void GuiShader::setGlColorA(GLint size, GLsizei stride, const GLvoid* data) const {
	mw::glEnableVertexAttribArray(aColorIndex_);
	mw::glVertexAttribPointer(aColorIndex_, size, GL_FLOAT, GL_FALSE, stride, data);
}

// Uniforms. -------------------------------------------

void GuiShader::setGlMatrixU(const mw::Matrix44& matrix) const {
	mw::glUniformMatrix4fv(uMatrixIndex_, 1, false, matrix.data());
}
