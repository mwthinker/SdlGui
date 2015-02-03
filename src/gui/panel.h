#ifndef GUI_PANEL_H
#define GUI_PANEL_H

#include "component.h"
#include "traversalgroup.h"

#include <vector>
#include <cassert>

namespace gui {

	class LayoutManager;

	// Creates a panel able to contain other components. It manage the
	// the layout of the components based on the layout manager.
	class Panel : public Component {
	public:
		friend class Frame;

		// Creates a empty panel. The default LayoutManager is FlowLayout.
		Panel();

		virtual ~Panel() {
		}		

		// Adds the component to the container using the layout spcified 
		// by the layout manager and the layoutIndex. Takes the ownership.
		// I.e. Deallocates the component when the panel is deallocated.
		template <class Comp, class... Args>
		std::shared_ptr<Comp> add(int layoutIndex, Args... args) {
			std::shared_ptr<Comp> c = std::make_shared<Comp>(args...);
			add(layoutIndex, c);
			return c;
		}

		// Adds the component to the container using the layout spcified 
		// by the layout manager and the layoutIndex. Takes the ownership.
		// I.e. Deallocates the component when the panel is deallocated.
		void add(int layoutIndex, const std::shared_ptr<Component>& component);

		// Same as add(Component* component, int layoutIndex) but added to 
		// a traversal group too.
		template <class Comp, class... Args>
		std::shared_ptr<Comp> addToGroup(int layoutIndex, Args... args) {
			std::shared_ptr<Comp> c = std::make_shared<Comp>(args...);
			addToGroup(layoutIndex, c);
			return c;
		}

		// Same as add(Component* component, int layoutIndex) but added to 
		// a traversal group too.
		void addToGroup(int layoutIndex, const std::shared_ptr<Component>& component);

		// Sets the layouyt manager. Takes ower the ownership of the layoutManager.
		// The old layoutManager are dealloted.
		void setLayout(const std::shared_ptr<LayoutManager>& layoutManager);

		template <class LManager, class... Args>
		std::shared_ptr<LManager> setLayout(Args... args) {
			std::shared_ptr<LManager> m = std::make_shared<LManager>(args...);
			setLayout(layoutManager);
			return m;
		}

		// Gets the current layout manager. Do not deallocate the layout manager
		// the panel takes care of that!
		std::shared_ptr<LayoutManager> getLayout() const;

		std::vector<std::shared_ptr<Component>>::iterator begin();
		std::vector<std::shared_ptr<Component>>::iterator end();

		// Returns the number of components contained.
		int nbrOfComponents() const;

		// Gets the list holding all contained components.
		const std::vector<std::shared_ptr<Component>>& getComponents() const;

		void draw(Uint32 deltaTime) override;

		void handleMouse(const SDL_Event& mouseEvent) override;

		void handleKeyboard(const SDL_Event&) override;
		
		void mouseMotionLeave() override;

		void mouseOutsideUp() override;

		void panelChanged(bool active) override;

		void validate() override;

		void setFocus(bool focus) override;
	
	protected:
		void setChildsParent() override;

	private:

		std::vector<std::shared_ptr<Component>> components_;
		std::shared_ptr<LayoutManager> layoutManager_;
		TraversalGroup group_;

		std::shared_ptr<Component> mouseMotionInsideComponent_;
		std::shared_ptr<Component> mouseDownInsideComponent_;
	};

} // Namespace gui.

#endif // GUI_PANEL_H
