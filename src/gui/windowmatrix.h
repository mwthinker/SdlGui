#ifndef GUI_WINDOWMATRIX_H
#define GUI_WINDOWMATRIX_H
#if MW_OPENGLES2

#include <mw/matrix.h>
#include <mw/shader.h>
#include <mw/opengl.h>
#include <mw/color.h>

#include <memory>

namespace gui {

	class WindowMatrix;
	typedef std::shared_ptr<WindowMatrix> WindowMatrixPtr;

	class WindowMatrix {
	public:
		WindowMatrix() : proj44_(mw::I_44), model44_(mw::I_44) {
			shader_ = mw::Shader::getDefaultShader();
			shader_->glUseProgram();
			uModelIndex_ = shader_->getUniformLocation(mw::SHADER_U_MAT4_MODEL);
			uProjIndex_ = shader_->getUniformLocation(mw::SHADER_U_MAT4_PROJ);
			uColorIndex_ = shader_->getUniformLocation(mw::SHADER_U_VEC4_COLOR);
			uIsTexIndex_ = shader_->getUniformLocation(mw::SHADER_U_FLOAT_TEXTURE);

			aVertexIndex_ = shader_->getAttributeLocation(mw::SHADER_A_VEC4_POSITION);
			aTexIndex_ = shader_->getAttributeLocation(mw::SHADER_A_VEC2_TEXCOORD);
		}

		mw::Matrix44 getProjection() const {
			return proj44_;
		}

		mw::Matrix44 getModel() const {
			return model44_;
		}

		void useShader() {
			shader_->glUseProgram();
		}

		void setProjection(const mw::Matrix44& proj) {
			proj44_ = proj;
			mw::glUniformMatrix4fv(uProjIndex_, 1, false, proj44_.data());
		}

		void setModel(const mw::Matrix44& model) {
			model44_ = model;
			mw::glUniformMatrix4fv(uModelIndex_, 1, false, model44_.data());
		}
		
		void setVertexPosition(GLint dimension, const GLvoid* data) {
			mw::glVertexAttribPointer(aVertexIndex_, dimension, GL_FLOAT, GL_FALSE, 0, data);
			mw::glEnableVertexAttribArray(aVertexIndex_);
		}

		void setTexturePosition(GLint dimension, const GLvoid* data) {
			mw::glVertexAttribPointer(aTexIndex_, dimension, GL_FLOAT, GL_FALSE, 0, data);
			mw::glEnableVertexAttribArray(aTexIndex_);
		}

		void setVertexPosition(GLint dimension, GLsizei stride, const GLvoid* data) {
			mw::glVertexAttribPointer(aVertexIndex_, dimension, GL_FLOAT, GL_FALSE, stride, data);
			mw::glEnableVertexAttribArray(aVertexIndex_);
		}

		void setTexturePosition(GLint dimension, GLsizei stride, const GLvoid* data) {
			mw::glVertexAttribPointer(aTexIndex_, dimension, GL_FLOAT, GL_FALSE, stride, data);
			mw::glEnableVertexAttribArray(aTexIndex_);
		}

		void setTexture(bool texture) {
			if (texture) {
				mw::glUniform1f(uIsTexIndex_, 1);
			} else {
				mw::glUniform1f(uIsTexIndex_, 0);
			}
		}

		void setColor(const mw::Color& color) {
			mw::glUniform4f(uColorIndex_, color.red_, color.green_, color.blue_, color.alpha_);
		}

		void setColor(float red, float green, float blue, float alpha = 1.f) {
			mw::glUniform4f(uColorIndex_, red, green, blue, alpha);
		}

		void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
			mw::glDrawArrays(mode, first, count);
		}

		void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices) {
			mw::glDrawElements(mode, count, type, indices);
		}
		
	private:
		mw::Matrix44 model44_, proj44_;
		int aVertexIndex_;
		int aTexIndex_;
		int uIsTexIndex_;
		int uColorIndex_;
		int uModelIndex_;
		int uProjIndex_;
		mw::ShaderPtr shader_;
	};

} // Namespace gui.

#endif // MW_OPENGLES2
#endif // GUI_WINDOWMATRIX_H
