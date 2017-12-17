#ifndef GUI_PROGRESSBAR_H
#define GUI_PROGRESSBAR_H

#include "component.h"

#include <mw/font.h>
#include <mw/text.h>
#include <mw/sprite.h>

#include <string>

namespace gui {

	class ProgressBar : public Component {
	public:
		ProgressBar();

		virtual ~ProgressBar() = default;

		virtual void draw(const Graphic& graphic, double deltaTime) override;

		// Set the color for the moving bar.
		void setBarColor(const mw::Color& color) {
			barColor_ = color;
		}

		// Set the color for the moving bar.
		mw::Color getBarColor() const {
			return barColor_;
		}

		void setBarWidth(float width) {
			barWidth_ = width;
		}

		float getBarWidth() const {
			return barWidth_;
		}

		void setBarSpeed(float speed) {
			barSpeed_ = speed;
		}

		float getBarSpeed() const {
			return barSpeed_;
		}

	private:
		mw::Color barColor_;
		float barWidth_;
		float barSpeed_;
		float xPostion_;
	};

} // Namespace gui.

#endif // GUI_PROGRESSBAR_H
