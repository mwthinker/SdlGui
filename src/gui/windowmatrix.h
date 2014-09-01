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
		WindowMatrix() : proj_(mw::I_44), model_(mw::I_44) {
			shader_ = mw::Shader::getDefaultShader();
		}

		mw::Matrix44 getProjection() const {
			return proj_;
		}

		mw::Matrix44 getModel() const {
			return model_;
		}

		void useShader() {
			shader_->glUseProgram();
		}		

		void setProjection(const mw::Matrix44& proj) {
			proj_ = proj;
			mw::glUniformMatrix4fv(shader_->getUniformLocation(mw::SHADER_U_MAT4_PROJ), 1, false, proj_.transpose().data());
		}

		void setModel(const mw::Matrix44& model) {
			model_ = model;
			mw::glUniformMatrix4fv(shader_->getUniformLocation(mw::SHADER_U_MAT4_MODEL), 1, false, model_.transpose().data());
		}

		void setVertexPosition(GLint dimension, float* data) {			
			mw::glVertexAttribPointer(shader_->getAttributeLocation(mw::SHADER_A_VEC4_POSITION), dimension, GL_FLOAT, GL_FALSE, 0, data);
			mw::glEnableVertexAttribArray(shader_->getAttributeLocation(mw::SHADER_A_VEC4_POSITION));
		}

		void setTexturePosition(GLint dimension, float* data) {
			mw::glVertexAttribPointer(shader_->getAttributeLocation(mw::SHADER_A_VEC2_TEXCOORD), dimension, GL_FLOAT, GL_FALSE, 0, data);
			mw::glEnableVertexAttribArray(shader_->getAttributeLocation(mw::SHADER_A_VEC2_TEXCOORD));
		}

		void setTexture(bool texture) {
			if (texture) {
				mw::glUniform1f(shader_->getUniformLocation(mw::SHADER_U_FLOAT_TEXTURE), 1);
			} else {
				mw::glUniform1f(shader_->getUniformLocation(mw::SHADER_U_FLOAT_TEXTURE), 0);
			}
		}

		void setColor(const mw::Color& color) {
			mw::glUniform4f(shader_->getUniformLocation(mw::SHADER_U_VEC4_COLOR), color.red_, color.green_, color.blue_, color.alpha_);
		}

		void setColor(float red, float green, float blue, float alpha = 1.f) {
			mw::glUniform4f(shader_->getUniformLocation(mw::SHADER_U_VEC4_COLOR), red, green, blue, alpha);
		}

		void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
			mw::glDrawArrays(mode, first, count);
		}
		
	private:
		mw::Matrix44 model_, proj_;
		mw::ShaderPtr shader_;
	};

} // Namespace gui.

#endif // MW_OPENGLES2
#endif // GUI_WINDOWMATRIX_H
