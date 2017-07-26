#ifndef GUI_TRAVERSALGROUP_H
#define GUI_TRAVERSALGROUP_H

#include "component.h"

#include <vector>

namespace gui {

	class TraversalGroup {
	public:
		TraversalGroup();

		void add(const std::shared_ptr<Component>& component);

		void remove(const std::shared_ptr<Component>& component);
		void removeAll();

		void handleKeyboard(const SDL_Event& keyEvent);

		void sort();

	private:
		// Changes to next item if next is true else changes to the last item.
		void changeToNext(bool nextItem);

		int lastFocusIndex_;
		std::vector<std::shared_ptr<Component>> components_;
	};

} // Namespace gui.

#endif // GUI_TRAVERSALGROUP_H
