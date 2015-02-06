#ifndef GUISHADERFUNCTIONS_H
#define GUISHADERFUNCTIONS_H

#include "guishader.h"

#include <mw/sprite.h>
#include <mw/color.h>

void addSquareToGuiShader(GLfloat* data, int& index,
	float x, float y,
	float w, float h,
	const mw::Sprite& sprite, const mw::Color& color = mw::Color(1, 1, 1));

void addSquareToGuiShader(GLfloat* data, int& index,
	float x, float y,
	float w, float h);

void addSquareToGuiShader(GLfloat* data, int& index,
	const mw::Color& color);

void setGuiShaderVertexAttribPointer(const GuiShader& shader);

#endif // GUISHADERFUNCTIONS_H
