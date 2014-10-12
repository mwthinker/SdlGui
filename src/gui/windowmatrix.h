#ifndef GUI_WINDOWMATRIX_H
#define GUI_WINDOWMATRIX_H
#if MW_OPENGLES2

#include <mw/matrix.h>
#include <mw/shader.h>
#include <mw/opengl.h>
#include <mw/color.h>

namespace gui {

	class WindowMatrix {
	public:
		WindowMatrix() {
			shader_ = mw::Shader::getDefaultShader();
			shader_.glUseProgram();
			uModelIndex_ = shader_.getUniformLocation(mw::SHADER_U_MAT4_MODEL);
			uProjIndex_ = shader_.getUniformLocation(mw::SHADER_U_MAT4_PROJ);
			uColorIndex_ = shader_.getUniformLocation(mw::SHADER_U_VEC4_COLOR);
			uIsTexIndex_ = shader_.getUniformLocation(mw::SHADER_U_FLOAT_TEXTURE);

			aVertexIndex_ = shader_.getAttributeLocation(mw::SHADER_A_VEC4_POSITION);
			aTexIndex_ = shader_.getAttributeLocation(mw::SHADER_A_VEC2_TEXCOORD);
		}

		void useShader() const {
			shader_.glUseProgram();
		}

		void setProjection(const mw::Matrix44& proj) const {
			mw::glUniformMatrix4fv(uProjIndex_, 1, false, proj.data());
		}

		void setModel(const mw::Matrix44& model) const {
			mw::glUniformMatrix4fv(uModelIndex_, 1, false, model.data());
		}
		
		void setVertexPosition(GLint dimension, const GLvoid* data) const {
			mw::glVertexAttribPointer(aVertexIndex_, dimension, GL_FLOAT, GL_FALSE, 0, data);
			mw::glEnableVertexAttribArray(aVertexIndex_);
		}

		void setTexturePosition(GLint dimension, const GLvoid* data) const {
			mw::glVertexAttribPointer(aTexIndex_, dimension, GL_FLOAT, GL_FALSE, 0, data);
			mw::glEnableVertexAttribArray(aTexIndex_);
		}

		void setVertexPosition(GLint dimension, GLsizei stride, const GLvoid* data) const {
			mw::glVertexAttribPointer(aVertexIndex_, dimension, GL_FLOAT, GL_FALSE, stride, data);
			mw::glEnableVertexAttribArray(aVertexIndex_);
		}

		void setTexturePosition(GLint dimension, GLsizei stride, const GLvoid* data) const {
			mw::glVertexAttribPointer(aTexIndex_, dimension, GL_FLOAT, GL_FALSE, stride, data);
			mw::glEnableVertexAttribArray(aTexIndex_);
		}

		void setTexture(bool texture) const {
			if (texture) {
				mw::glUniform1f(uIsTexIndex_, 1);
			} else {
				mw::glUniform1f(uIsTexIndex_, 0);
			}
		}

		void setColor(const mw::Color& color) const {
			mw::glUniform4f(uColorIndex_, color.red_, color.green_, color.blue_, color.alpha_);
		}

		void setColor(float red, float green, float blue, float alpha = 1.f) const {
			mw::glUniform4f(uColorIndex_, red, green, blue, alpha);
		}
		
	private:
		int aVertexIndex_;
		int aTexIndex_;
		int uIsTexIndex_;
		int uColorIndex_;
		int uModelIndex_;
		int uProjIndex_;
		mw::Shader shader_;
	};

} // Namespace gui.

#endif // MW_OPENGLES2
#endif // GUI_WINDOWMATRIX_H
