#include "guishader.h"

namespace gui {
	
	GuiShader::GuiShader() {
		aPos_ = -1;
	}
		
	GuiShader::GuiShader(std::string vShaderFile, std::string fShaderFile) {
		shader_.bindAttribute("aPos");
		shader_.loadAndLinkFromFile(vShaderFile, fShaderFile);
		
		shader_.useProgram();
	
		aPos_ = shader_.getAttributeLocation("aPos");

		// Collect the vertex buffer uniforms indexes.
		uProj_ = shader_.getUniformLocation("uProj");
		uModel_ = shader_.getUniformLocation("uModel");
		uPos_ = shader_.getUniformLocation("uPos");
		uTex_ = shader_.getUniformLocation("uTex");
		uColor_ = shader_.getUniformLocation("uColor");
		uIsTex_ = shader_.getUniformLocation("uIsTex");
	}
	
	void GuiShader::useProgram() const {
		shader_.useProgram();
	}
	
	void GuiShader::setVertexAttribPointer() const {
		glEnableVertexAttribArray(aPos_);
		glVertexAttribPointer(aPos_, 2, GL_FLOAT, GL_FALSE, vertexSizeInBytes(), (GLvoid*) (sizeof(GLfloat) * 0));
	}
	
	void GuiShader::setUProj(const mw::Matrix44<float>& matrix) const {
		shader_.useProgram();
		glUniformMatrix4fv(uProj_, 1, false, matrix.data());
	}
	
	void GuiShader::setUModel(const mw::Matrix44<float>& matrix) const {
		shader_.useProgram();
		glUniformMatrix4fv(uModel_, 1, false, matrix.data());
	}
	
	void GuiShader::setUPos(const mw::Matrix44<float>& matrix) const {
		shader_.useProgram();
		glUniformMatrix4fv(uPos_, 1, false, matrix.data());
	}
	
	void GuiShader::setUTex(const mw::Matrix44<float>& matrix) const {
		shader_.useProgram();
		glUniformMatrix4fv(uTex_, 1, false, matrix.data());
	}
	
	void GuiShader::setUColor(const mw::Color& color) const {
		shader_.useProgram();
		glUniform4f(uColor_, color.red_, color.green_, color.blue_, color.alpha_);
	}
	
	void GuiShader::setUIsTex(float value) const {
		shader_.useProgram();
		glUniform1f(uIsTex_, value);
	}

} // Namespace gui
