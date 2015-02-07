#ifndef GUISHADER_H
#define GUISHADER_H
#if MW_OPENGLES2

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

		void glUseProgram() const;

		// Vertex buffer Attributes. ---------------------------

		void setGlPosA(GLint size, const GLvoid* data) const;
		void setGlPosA(GLint size, GLsizei stride, const GLvoid* data) const;

		void setGlTexA(GLint size, const GLvoid* data) const;
		void setGlTexA(GLint size, GLsizei stride, const GLvoid* data) const;

		// Uniforms. -------------------------------------------

		void setGlProjU(const mw::Matrix44& matrix) const;

		void setGlModelU(const mw::Matrix44& matrix) const;

		void setGlTextureU(bool texture) const;

		void setGlColorU(const mw::Color& color) const;
		void setGlColorU(float red, float green, float blue, float alpha = 1) const;

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

#endif // MW_OPENGLES2
#endif // GUISHADER_H
