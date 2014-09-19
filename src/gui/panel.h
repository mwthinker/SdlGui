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
		// Creates a empty panel. The default LayoutManager is FlowLayout.
		Panel();

		virtual ~Panel() {
		}

		// Adds the component to the container. Takes the ownership.
		// I.e. Deallocates the component when the panel is deallocated.
		void add(const std::shared_ptr<Component>& component);

		// Adds the component to the container using the layout spcified 
		// by the layout manager and the layoutIndex. Takes the ownership.
		// I.e. Deallocates the component when the panel is deallocated.
		void add(const std::shared_ptr<Component>& component, int layoutIndex);

		// Same as add(Component* component) but added to 
		// a traversal group too.
		void addToGroup(const std::shared_ptr<Component>& component);

		// Same as add(Component* component, int layoutIndex) but added to 
		// a traversal group too.
		void addToGroup(const std::shared_ptr<Component>& component, int layoutIndex);

		// Sets the layouyt manager. Takes ower the ownership of the layoutManager.
		// The old layoutManager are dealloted.
		void setLayout(const std::shared_ptr<LayoutManager>& layoutManager);
		
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

		void setChildsParent(const std::shared_ptr<Component>& thisPanel) override;
#if MW_OPENGLES2
		void setWindowMatrixPtr(const WindowMatrixPtr& windowMatrix) override;
#endif // MW_OPENGLES2
	private:
		std::vector<std::shared_ptr<Component>> components_;
		std::shared_ptr<LayoutManager> layoutManager_;
		TraversalGroup group_;

		std::shared_ptr<Component> mouseMotionInsideComponent_;
		std::shared_ptr<Component> mouseDownInsideComponent_;
	};

} // Namespace gui.

#endif // GUI_PANEL_H
