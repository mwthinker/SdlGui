#ifndef GUI_DIMENSION_H
#define GUI_DIMENSION_H

#include <mw/vec2.h>

namespace gui {

	using Point = mw::Vec2<float>;

	class Dimension {
	public:
		Dimension() : width_(0), height_(0) {
		}

		Dimension(float width, float height) : width_(width), height_(height) {
		}

		Dimension operator+(const Dimension& dim) {
			return Dimension(dim.width_ + width_, dim.height_ + height_);
		}

		float width_;
		float height_;
	};

} // Namespace gui.

#endif // GUI_DIMENSION_H
