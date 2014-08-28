#ifndef GUI_WINDOWMATRIX_H
#define GUI_WINDOWMATRIX_H

#include <mw/matrix.h>
#include <mw/sprite.h>
#include <mw/opengl.h>

#include <memory>

namespace gui {

	class WindowMatrix;
	typedef std::shared_ptr<WindowMatrix> WindowMatrixPtr;

	class WindowMatrix {
	public:
		WindowMatrix() : proj_(mw::I_44), model_(mw::I_44) {
		}

		mw::Matrix44 getProjection() const {
			return proj_;
		}

		mw::Matrix44 getModel() const {
			return model_;
		}

		void setProjection(const mw::Matrix44& proj) {
			proj_ = proj;
			mw::Sprite::getShaderPtr()->glUseProgram();
			mw::glUniformMatrix4fv(mw::Sprite::getShaderPtr()->getUniformLocation(mw::SHADER_UNIFORM_MAT4_PROJ), 1, false, proj_.transpose().data());
		}

		void setModel(const mw::Matrix44& model) {
			model_ = model;
			mw::Sprite::getShaderPtr()->glUseProgram();
			mw::glUniformMatrix4fv(mw::Sprite::getShaderPtr()->getUniformLocation(mw::SHADER_UNIFORM_MAT4_MODEL), 1, false, model_.transpose().data());
		}

		void setVertexPosition(GLint dimension, float* data) {
			mw::glVertexAttribPointer(mw::Sprite::getShaderPtr()->getAttributeLocation(mw::SHADER_ATTRIBUTE_VEC4_POSITION), dimension, GL_FLOAT, GL_FALSE, 0, data);
			mw::glEnableVertexAttribArray(mw::Sprite::getShaderPtr()->getAttributeLocation(mw::SHADER_ATTRIBUTE_VEC4_POSITION));
		}

		void setTexturePosition(GLint dimension, float* data) {
			mw::glVertexAttribPointer(mw::Sprite::getShaderPtr()->getAttributeLocation(mw::SHADER_ATTRIBUTE_VEC2_TEXCOORD), dimension, GL_FLOAT, GL_FALSE, 0, data);
			mw::glEnableVertexAttribArray(mw::Sprite::getShaderPtr()->getAttributeLocation(mw::SHADER_ATTRIBUTE_VEC2_TEXCOORD));
		}

	private:
		mw::Matrix44 model_, proj_;
	};

} // Namespace gui.

#endif // GUI_WINDOWMATRIX_H
