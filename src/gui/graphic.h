#ifndef GUI_GRAPHIC
#define GUI_GRAPHIC

#include <mw/opengl.h>
#include <mw/matrix44.h>
#include <mw/shader.h>
#include <mw/text.h>
#include <mw/color.h>
#include <mw/sprite.h>
#include <mw/vertexbufferobject.h>

#include "guishader.h"
#include "guivertexdata.h"

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

		inline const mw::Matrix44f& getProjectionMatrix() const {
			return proj_;
		}

		void setColor(const mw::Color& color) const;

		void setColor(float red, float green, float blue, float alpha = 1) const;
	
	private:
		inline void useProgram() const {
			guiShader_.useProgram();
		}

		void draw() const;

		void setModel(const mw::Matrix44f& model) const;

		void setProj(const mw::Matrix44f& proj);

		mw::Matrix44f proj_;

		GuiShader guiShader_;
		std::shared_ptr<GuiVertexData> guiVertexData_;
	};

} // Namespace gui.

#endif // GUI_GRAPHIC
