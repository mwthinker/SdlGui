#include "progressbar.h"

#include <functional>
#include <string>

namespace gui {

	ProgressBar::ProgressBar() : barColor_(0.7f, 0.7f, 0.7f), barWidth_(30), barSpeed_(80.f) {
		setPreferredSize(150, 20);
		setBackgroundColor(0.8f, 0.8f, 0.8f);
	}

	void ProgressBar::draw(const Graphic& graphic, double deltaTime) {
		Component::draw(graphic, deltaTime);
		auto dim = Component::getSize();
		
		xPostion_ += (float) deltaTime * barSpeed_;
		if (xPostion_ < 1) {
			xPostion_ = 1;
			barSpeed_ *= -1;
		} else if (xPostion_ + barWidth_ > dim.width_ - 1) {
			xPostion_ = dim.width_ - 1 - barWidth_;
			barSpeed_ *= -1;
		}

		// Draw moving bar inside the 1 pixel border.
		graphic.setColor(barColor_);
		graphic.drawSquare(xPostion_, 1, barWidth_, dim.height_ - 2);
	}

} // Namespace gui.
