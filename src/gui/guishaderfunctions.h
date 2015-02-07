#ifndef GUISHADERFUNCTIONS_H
#define GUISHADERFUNCTIONS_H

#include "guishader.h"

#include <mw/sprite.h>
#include <mw/color.h>

void addSquare(GLfloat* data, int& index,
	float x, float y,
	float w, float h,
	const mw::Sprite& sprite);

void addSquare(GLfloat* data, int& index,
	float x, float y,
	float w, float h);

#endif // GUISHADERFUNCTIONS_H
