#include "guishaderfunctions.h"

#include <algorithm>

namespace {

	inline void addVertex(GLfloat* data, int& index, const mw::Color& color) {
		data[index + 4] = color.red_;
		data[index + 5] = color.green_;
		data[index + 6] = color.blue_;
		data[index + 7] = color.alpha_;
		index += 8;
	}

	inline void addVertex(GLfloat* data, int& index,
		float x, float y) {

		data[index] = x;
		data[index + 1] = y;
		index += 8;
	}

	inline void addVertex(GLfloat* data, int& index,
		float x, float y,
		float xTex, float yTex,
		const mw::Color& color) {

		data[index++] = x;
		data[index++] = y;

		data[index++] = xTex;
		data[index++] = yTex;

		data[index++] = color.red_;
		data[index++] = color.green_;
		data[index++] = color.blue_;
		data[index++] = color.alpha_;
	}

	// Add a triangle, GL_TRIANGLES, i.e. 3 vertices.
	inline void addTriangle(GLfloat* data, int& index,
		const mw::Color& color) {

		addVertex(data, index, color);
		addVertex(data, index, color);
		addVertex(data, index, color);
	}

	// Add a triangle, GL_TRIANGLES, i.e. 3 vertices.
	inline void addTriangle(GLfloat* data, int& index,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3) {

		addVertex(data, index, x1, y1);
		addVertex(data, index, x2, y2);
		addVertex(data, index, x3, y3);
	}

	// Add a triangle, GL_TRIANGLES, i.e. 3 vertices.
	inline void addTriangle(GLfloat* data, int& index,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3,
		float xTex1, float yTex1,
		float xTex2, float yTex2,
		float xTex3, float yTex3,
		const mw::Color& color) {

		addVertex(data, index, x1, y1, xTex1, yTex1, color);
		addVertex(data, index, x2, y2, xTex2, yTex2, color);
		addVertex(data, index, x3, y3, xTex3, yTex3, color);
	}

} // Namespace mw.

void addSquareToGuiShader(GLfloat* data, int& index,
	float x, float y,
	float w, float h,
	const mw::Sprite& sprite, const mw::Color& color) {
	int textureW = sprite.getTexture().getWidth();
	int textureH = sprite.getTexture().getHeight();

	// Left triangle |_
	addTriangle(data, index,
		x, y,
		x + w, y,
		x, y + h,
		sprite.getX() / textureW, sprite.getY() / textureH,
		(sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH,
		sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH,
		color);
	//                _
	// Right triangle  |
	addTriangle(data, index,
		x, y + h,
		x + w, y,
		x + w, y + h,
		sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH,
		(sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH,
		(sprite.getX() + sprite.getWidth()) / textureW, (sprite.getY() + sprite.getHeight()) / textureH,
		color);
}

void addSquareToGuiShader(GLfloat* data, int& index,
	float x, float y,
	float w, float h) {

	// Left triangle |_
	addTriangle(data, index,
		x, y,
		x + w, y,
		x, y + h);
	//                _
	// Right triangle  |
	addTriangle(data, index,
		x, y + h,
		x + w, y,
		x + w, y + h);
}

void addSquareToGuiShader(GLfloat* data, int& index,
	const mw::Color& color) {

	// Left triangle |_
	addTriangle(data, index, color);
	//                _
	// Right triangle  |
	addTriangle(data, index, color);
}

void setGuiShaderVertexAttribPointer(const GuiShader& shader) {
	shader.setGlPosA(2, sizeof(GLfloat) * 8, (GLvoid*) 0);
	shader.setGlTexA(2, sizeof(GLfloat) * 8, (GLvoid*) (sizeof(GLfloat) * 2));
	shader.setGlColorA(4, sizeof(GLfloat) * 8, (GLvoid*) (sizeof(GLfloat) * 4));
}
