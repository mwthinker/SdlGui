#ifndef GUISHADER_H
#define GUISHADER_H

#include <mw/opengl.h>
#include <mw/matrix.h>
#include <mw/shader.h>
#include <mw/text.h>
#include <mw/color.h>
#include <mw/sprite.h>
#include <mw/vertexbufferobject.h>

namespace gui {

	class Graphic {
	public:
		friend class Frame;
		friend class Panel;

		Graphic();
		Graphic(std::string vShaderFile, std::string fShaderFile);

		void drawSquare(float x, float y, float w, float h) const;
		void drawSprite(const mw::Sprite& sprite, float x, float y, float w, float h) const;
		void drawText(const mw::Text& text, float x, float y) const;
		void drawBorder(float x, float y, float w, float h) const;

		inline const mw::Matrix44& getProjectionMatrix() const {
			return proj_;
		}

		void setColor(const mw::Color& color) const;

		void setColor(float red, float green, float blue, float alpha = 1) const;

	private:
		inline void useProgram() const {
			shader_.useProgram();
		}

		void draw() const;

		void setModel(const mw::Matrix44& model) const;

		void setProj(const mw::Matrix44& proj);

		mw::Matrix44 proj_;

		mw::Shader shader_;
		mw::VertexBufferObject vbo_;

		int aPosIndex_;

		int uProjIndex_;
		int uModelIndex_;
		int uPosIndex_;
		int uTexIndex_;
		int uIsTexIndex_;
		int uColorIndex_;
	};

} // Namespace gui.

#endif // GUISHADER_H
