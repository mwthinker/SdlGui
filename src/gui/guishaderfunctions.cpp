#include "guishaderfunctions.h"

#include <cassert>
#include <algorithm>

namespace {
	
	const int VERTEX_BYTES = 4;
	const int TRIANGLES_VERTICES = 6;

	inline void addVertex(GLfloat* data, int& index,
		float x, float y) {

		data[index] = x;
		data[index + 1] = y;
		index += VERTEX_BYTES;
	}

	inline void addVertex(GLfloat* data, int& index,
		float x, float y,
		float xTex, float yTex) {

		data[index++] = x;
		data[index++] = y;

		data[index++] = xTex;
		data[index++] = yTex;
	}
	
	// Add a triangle, GL_TRIANGLES, i.e. 3 vertices.
	inline void addTriangle(GLenum mode, GLfloat* data, int& index,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3) {

		switch (mode) {
			case GL_TRIANGLES:
				addVertex(data, index, x1, y1);
				addVertex(data, index, x2, y2);
				addVertex(data, index, x3, y3);
				break;
			case GL_TRIANGLE_FAN:
				assert(0);
				break;
			case GL_TRIANGLE_STRIP:
				assert(0);
				break;
			default:
				break;
		}
	}

	// Add a triangle, GL_TRIANGLES, i.e. 3 vertices.
	inline void addTriangle(GLenum mode, GLfloat* data, int& index,
		float x1, float y1,
		float x2, float y2,
		float x3, float y3,
		float xTex1, float yTex1,
		float xTex2, float yTex2,
		float xTex3, float yTex3) {
		switch (mode) {
			case GL_TRIANGLES:
				addVertex(data, index, x1, y1, xTex1, yTex1);
				addVertex(data, index, x2, y2, xTex2, yTex2);
				addVertex(data, index, x3, y3, xTex3, yTex3);
				break;
			case GL_TRIANGLE_FAN:
				assert(0);
				break;
			case GL_TRIANGLE_STRIP:
				assert(0);
				break;
		}
	}

} // Namespace mw.

void addSquare(GLenum mode, GLfloat* data, int& index,
	float x, float y,
	float w, float h,
	const mw::Sprite& sprite, const mw::Color& color) {
	int textureW = sprite.getTexture().getWidth();
	int textureH = sprite.getTexture().getHeight();

	// Left triangle |_
	addTriangle(mode, data, index,
		x, y,
		x + w, y,
		x, y + h,
		sprite.getX() / textureW, sprite.getY() / textureH,
		(sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH,
		sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH);
	//                _
	// Right triangle  |
	addTriangle(mode, data, index,
		x, y + h,
		x + w, y,
		x + w, y + h,
		sprite.getX() / textureW, (sprite.getY() + sprite.getHeight()) / textureH,
		(sprite.getX() + sprite.getWidth()) / textureW, sprite.getY() / textureH,
		(sprite.getX() + sprite.getWidth()) / textureW, (sprite.getY() + sprite.getHeight()) / textureH);
}

void addSquare(GLenum mode, GLfloat* data, int& index,
	float x, float y,
	float w, float h) {

	// Left triangle |_
	addTriangle(mode, data, index,
		x, y,
		x + w, y,
		x, y + h);
	//                _
	// Right triangle  |
	addTriangle(mode, data, index,
		x, y + h,
		x + w, y,
		x + w, y + h);
}
