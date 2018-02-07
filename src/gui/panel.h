#ifndef GUI_PANEL_H
#define GUI_PANEL_H

#include "component.h"
#include "layoutmanager.h"

#include <vector>

namespace gui {

	class LayoutManager;

	// Creates a panel able to contain other components. It manage the
	// the layout of the components based on the layout manager.
	class Panel : public Component {
	public:
		using iterator = std::vector<std::shared_ptr<Component>>::iterator;
		using const_iterator = std::vector<std::shared_ptr<Component>>::const_iterator;

		friend class Frame;

		// Create a empty panel. The default LayoutManager is FlowLayout.
		Panel();
		virtual ~Panel() = default;

		// Add the component, with the provided layoutIndex.
		// Will assert if the component already added.
		template <class Comp, class... Args>
		std::shared_ptr<Comp> addDefault(Args... args);

		// Add the component, with the default index layout.
		// Will assert if the component already added.
		std::shared_ptr<Component> addDefault(const std::shared_ptr<Component>& component);

		// Adds the component to the container using the layout specified
		// by the layout manager and the layoutIndex. Takes the ownership.
		// I.e. Deallocates the component when the panel is deallocated.
		std::shared_ptr<Component> add(int layoutIndex, const std::shared_ptr<Component>& component);

		// Add the component to the container using the layout specified
		// by the layout manager and the layoutIndex. Takes the ownership.
		// I.e. Deallocates the component when the panel is deallocated.
		template <class Comp, class... Args>
		std::shared_ptr<Comp> add(int layoutIndex, Args... args);

		void remove(const std::shared_ptr<Component>& component);
		void removeAll();

		// Set the current layout manager.
		std::shared_ptr<LayoutManager> setLayout(const std::shared_ptr<LayoutManager>& layoutManager);

		template <class LManager, class... Args>
		std::shared_ptr<LManager> setLayout(Args... args);

		// Get the current layout manager.
		std::shared_ptr<LayoutManager> getLayout() const;

		iterator begin();
		iterator end();

		const_iterator begin() const;
		const_iterator end() const;
		
		const_iterator cbegin() const;
		const_iterator cend() const;

		// Return the number of components contained.
		int getComponentsCount() const;

		// Get all contained components.
		const std::vector<std::shared_ptr<Component>>& getComponents() const;

		void setFocus(bool focus) override;

		void demandPriority(const std::shared_ptr<Component>& component);

		void releasePriority(const std::shared_ptr<Component>& component);

		void validate() override;

	protected:
		// @Component Make the focus propagete upwards, from child to parent.
		void setFocus(bool focus, const std::shared_ptr<Component>& child) override;

		void setChildsParent() override;

		virtual void draw(const Graphic& graphic, double deltaTime) override;

		void handleMouse(const SDL_Event& mouseEvent) override;

		void handleKeyboard(const SDL_Event&) override;

		void mouseMotionLeave() override;

		void mouseOutsideUp() override;

		void panelChanged(bool active) override;

		void drawListener(Frame& frame, double deltaTime) override final;

	private:
		void handleMouseMotionEvent(SDL_Event mouseEvent);

		void handleMouseButtonEvent(SDL_Event mouseEvent);

		std::vector<std::shared_ptr<Component>> components_;
		std::shared_ptr<Component> priorityComponent_;
		std::shared_ptr<LayoutManager> layoutManager_;

		std::shared_ptr<Component> mouseMotionInsideComponent_;
		std::shared_ptr<Component> mouseDownInsideComponent_;
	};

	template <class LManager, class... Args>
	std::shared_ptr<LManager> Panel::setLayout(Args... args) {
		auto m = std::make_shared<LManager>(args...);
		setLayout(m);
		return m;
	}

	template <class Comp, class... Args>
	std::shared_ptr<Comp> Panel::addDefault(Args... args) {
		auto c = std::make_shared<Comp>(args...);
		add(DEFAULT_INDEX, c);
		return c;
	}

	template <class Comp, class... Args>
	std::shared_ptr<Comp> Panel::add(int layoutIndex, Args... args) {
		auto c = std::make_shared<Comp>(args...);
		add(layoutIndex, c);
		return c;
	}

	inline int Panel::getComponentsCount() const {
		return components_.size();
	}

	inline Panel::iterator Panel::begin() {
		return components_.begin();
	}

	inline Panel::iterator Panel::end() {
		return components_.end();
	}

	inline Panel::const_iterator Panel::begin() const {
		return components_.begin();
	}

	inline Panel::const_iterator Panel::end() const {
		return components_.end();
	}

	inline Panel::const_iterator Panel::cbegin() const {
		return components_.begin();
	}

	inline Panel::const_iterator Panel::cend() const {
		return components_.end();
	}

} // Namespace gui.

#endif // GUI_PANEL_H
