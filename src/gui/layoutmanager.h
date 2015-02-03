#ifndef GUI_LAYOUTMANAGER_H
#define GUI_LAYOUTMANAGER_H

#include "dimension.h"

namespace gui {

	static const int DEFAULT_INDEX = 0;

	class Panel;

	class LayoutManager {
	public:

		virtual ~LayoutManager() {
		}

		// Sets the size of childs based on the current
		// size of the parent.
		virtual void layoutContainer(Panel& parent) = 0;
	};

} // Namespace gui.

#endif // GUI_LAYOUTMANAGER_H
