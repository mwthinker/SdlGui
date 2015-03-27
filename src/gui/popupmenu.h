#ifndef GUI_POPUPMENU_H
#define GUI_POPUPMENU_H

#include "component.h"

namespace gui {

	class PopUpMenu : public Component {
	public:
		virtual void draw(std::chrono::high_resolution_clock::duration) override;

	private:
	};

} // Namespace gui.

#endif // GUI_POPUPMENU_H
