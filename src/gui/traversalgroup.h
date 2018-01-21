#ifndef GUI_TRAVERSALGROUP_H
#define GUI_TRAVERSALGROUP_H

#include "component.h"

#include <vector>

namespace gui {

	class Component;

	class TraversalGroup {
	public:
		using iterator = std::vector<std::shared_ptr<Component>>::iterator;
		using const_iterator = std::vector<std::shared_ptr<Component>>::const_iterator;

		TraversalGroup();

		void add(const std::shared_ptr<Component>& component);

		void remove(const std::shared_ptr<Component>& component);
		void removeAll();

		void handleKeyboard(const SDL_Event& keyEvent);

		// Sort the components from up to down. 
		void verticalSort();

		// Sort the components from left to right. 
		void horizontalSort();

		void setVerticalArrows(bool vertical);

		bool isVerticalArrows() const;

		void setHorizontalArrows(bool horizontal);

		bool isHorizontalArrows() const;

		void setTab(bool tab);

		bool isTab() const;

		// Changes to next item if next is true else changes to the last item.
		void changeToNext(bool nextItem);

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;

		const_iterator cbegin() const;
		const_iterator cend() const;

	private:
		int lastFocusIndex_;
		std::vector<std::shared_ptr<Component>> components_;
		bool verticalArrows_;
		bool horizontalArrows_;
		bool tab_;
	};

	inline void TraversalGroup::setVerticalArrows(bool vertical) {
		verticalArrows_ = vertical;
	}

	inline bool TraversalGroup::isVerticalArrows() const {
		return verticalArrows_;
	}

	inline void TraversalGroup::setHorizontalArrows(bool horizontal) {
		horizontalArrows_ = horizontal;
	}

	inline bool TraversalGroup::isHorizontalArrows() const {
		return horizontalArrows_;
	}

	inline void TraversalGroup::setTab(bool tab) {
		tab_ = tab;
	}

	inline bool TraversalGroup::isTab() const {
		return tab_;
	}

	inline TraversalGroup::iterator TraversalGroup::begin() {
		return components_.begin();
	}

	inline TraversalGroup::iterator TraversalGroup::end() {
		return components_.end();
	}

	inline TraversalGroup::const_iterator TraversalGroup::begin() const {
		return components_.begin();
	}

	inline TraversalGroup::const_iterator TraversalGroup::end() const {
		return components_.end();
	}

	inline TraversalGroup::const_iterator TraversalGroup::cbegin() const {
		return components_.begin();
	}

	inline TraversalGroup::const_iterator TraversalGroup::cend() const {
		return components_.end();
	}

} // Namespace gui.

#endif // GUI_TRAVERSALGROUP_H
