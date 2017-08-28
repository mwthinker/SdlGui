#ifndef GUI_GUIVERTEXDATA_H
#define GUI_GUIVERTEXDATA_H

#include "guishader.h"

#include <mw/vertexdata.h>

namespace gui {

	class GuiVertexData : public mw::VertexData {
	public:
		GuiVertexData() {
		}

		GuiVertexData(const GuiShader& guiShader) : guiShader_(guiShader) {
		}

		virtual ~GuiVertexData() = default;

		unsigned int vertexSizeInFloat() const override {
			return guiShader_.vertexSizeInFloat();
		}

		void setVertexAttribPointer() const override {
			guiShader_.setVertexAttribPointer();
		}

		void useProgram() const override {
			guiShader_.useProgram();
		}

		void setColor(const mw::Color& color) const {
			guiShader_.setUColor(color);
		}

		void setColor(float red, float green, float blue, float alpha = 1) const {
			guiShader_.setUColor(mw::Color(red, green, blue, alpha));
		}

		void setTexture(bool texture) const {
			guiShader_.setUIsTex(texture ? 1.f : 0.f);
		}

		void drawTRIANGLE_STRIP() {
			drawMode(GL_TRIANGLE_STRIP);
		}

		void addSquareTRIANGLE_STRIP(float x, float y, float w, float h) {
			addVertex(x, y);
			addVertex(x + w, y);
			addVertex(x, h + y);
			addVertex(x + w, h + y);
		}

		void addTriangleTRIANGLES(
			float x1, float y1,
			float x2, float y2,
			float x3, float y3) {

			addVertex(x1, y1);
			addVertex(x2, y2);
			addVertex(x3, y3);
		}

		void addVertex(float x, float y) {
			VertexData::addVertex<2>({x, y});
		}

	private:
		GuiShader guiShader_;
	};

} // gui.
	
#endif // GUI_GUIVERTEXDATA_H
