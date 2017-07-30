#ifndef GUISHADER_H
#define GUISHADER_H

#include <mw/shader.h>
#include <mw/matrix44.h>
#include <mw/color.h>

#include <string>

namespace gui {	
	
	class GuiShader {
	public:
		inline static constexpr unsigned int vertexSizeInBytes() {
			return vertexSizeInFloat() * sizeof(GLfloat);
		}
	
		inline static constexpr unsigned int vertexSizeInFloat() {
			return 2;
		}
	
		GuiShader();
	
		GuiShader(std::string vShaderFile, std::string fShaderFile);
	
		void useProgram() const;
	
		void setVertexAttribPointer() const;
	
		void setUProj(const mw::Matrix44<float>& matrix) const;
		void setUModel(const mw::Matrix44<float>& matrix) const;
		void setUPos(const mw::Matrix44<float>& matrix) const;
		void setUTex(const mw::Matrix44<float>& matrix) const;
		
		void setUColor(const mw::Color& color) const;
		void setUIsTex(float value) const;
	
	private:
		int aPos_;
		int uProj_;
		int uModel_;
		int uPos_;
		int uTex_;
		int uColor_;
		int uIsTex_;

		mw::Shader shader_;
	};

} // Namespace gui.

#endif // GUISHADER_H
