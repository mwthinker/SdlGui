#ifndef GUISHADER_H
#define GUISHADER_H

#include <mw/opengl.h>
#include <mw/matrix.h>
#include <mw/shader.h>

#include <mw/color.h>
#include <mw/sprite.h>

namespace gui {

	class GuiShader {
	public:
		GuiShader();
		GuiShader(std::string vShaderFile, std::string fShaderFile);

		void useProgram() const;

		// Vertex buffer Attributes. ---------------------------

		void setPosA(GLint size, const GLvoid* data) const;
		void setPosA(GLint size, GLsizei stride, const GLvoid* data) const;

		void setTexA(GLint size, const GLvoid* data) const;
		void setTexA(GLint size, GLsizei stride, const GLvoid* data) const;

		// Uniforms. -------------------------------------------

		void setProjU(const mw::Matrix44& matrix) const;

		void setModelU(const mw::Matrix44& matrix) const;

		void setTextureU(bool texture) const;

		void setColorU(const mw::Color& color) const;
		void setColorU(float red, float green, float blue, float alpha = 1) const;

	private:
		mw::Shader shader_;

		int aPosIndex_;
		int aTexIndex_;

		int uProjIndex_;
		int uModelIndex_;
		int uIsTexIndex_;
		int uColorIndex_;
	};

} // Namespace gui.

#endif // GUISHADER_H
