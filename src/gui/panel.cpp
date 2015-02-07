#include "panel.h"
#include "component.h"
#include "flowlayout.h"

#include <list>
#include <cassert>

namespace gui {

	Panel::Panel() : mouseMotionInsideComponent_(false), mouseDownInsideComponent_(false), layoutManager_(std::make_shared<FlowLayout>()) {
		setPreferredSize(50, 50);
		setBorderColor(1, 1, 1, 0);
	}

	void Panel::setChildsParent() {
		for (auto& c : components_) {
			c->parent_ = std::static_pointer_cast<Panel>(shared_from_this());
			c->setChildsParent();
		}
	}

	std::shared_ptr<Component> Panel::addDefault(const std::shared_ptr<Component>& component) {
		add(DEFAULT_INDEX, component);
		component->setChildsParent();
		return component;
	}

    std::shared_ptr<Component> Panel::add(int layoutIndex, const std::shared_ptr<Component>& component) {
		// Was already added?
		assert(!component->isAdded_);
#if MW_OPENGLES2
		component->guiShader_ = guiShader_;
#endif // MW_OPENGLES2
		component->isAdded_ = true;
		component->setLayoutIndex(layoutIndex);
		components_.push_back(component);
		setChildsParent();
		if (component->isGrabFocus()) {
			setFocus(true);
			++nbrChildGrabFocus_;
		}
		validate();
		return component;
	}

	std::shared_ptr<Component> Panel::addDefaultToGroup(const std::shared_ptr<Component>& component) {
		add(DEFAULT_INDEX, component);
		group_.add(component);
		return component;
	}

	std::shared_ptr<Component> Panel::addToGroup(int layoutIndex, const std::shared_ptr<Component>& component) {
		add(layoutIndex, component);
		group_.add(component);
		return component;
	}

	std::shared_ptr<LayoutManager> Panel::setLayout(const std::shared_ptr<LayoutManager>& layoutManager) {
		layoutManager_ = layoutManager;
		return layoutManager_;
	}

	void Panel::setFocus(bool focus) {
		Component::setFocus(focus);
		if (!focus) {
			for (auto& component : *this) {
				component->setFocus(false);
			}
		}
	}

	std::shared_ptr<LayoutManager> Panel::getLayout() const {
		return layoutManager_;
	}

	std::vector<std::shared_ptr<Component>>::iterator Panel::begin() {
		return components_.begin();
	}

	std::vector<std::shared_ptr<Component>>::iterator Panel::end() {
		return components_.end();
	}

	int Panel::nbrOfComponents() const {
		return components_.size();
	}

	const std::vector<std::shared_ptr<Component>>& Panel::getComponents() const {
		return components_;
	}

	void Panel::draw(Uint32 deltaTime) {
		Component::draw(deltaTime);

		// Draw the components.
		for (auto& component : *this) {
			if (component->isVisible()) {
				component->draw(deltaTime);
			}
		}
	}

	// Todo! Reverse y-axis!
	void Panel::handleMouse(const SDL_Event& mouseEvent) {
		std::shared_ptr<Component> currentComponent;
		switch (mouseEvent.type) {
			case SDL_MOUSEMOTION:
				for (auto& component : *this) {
					if (!component->isVisible()) {
						continue;
					}
					Point p = component->getLocation();
					Dimension d = component->getSize();
					if (p.x_ <= mouseEvent.motion.x && p.x_ + d.width_ > mouseEvent.motion.x &&
						p.y_ <= mouseEvent.motion.y && p.y_ + d.height_ > mouseEvent.motion.y) {
						SDL_Event motionEvent = mouseEvent;
						motionEvent.motion.x -= (Sint32) p.x_;
						motionEvent.motion.y -= (Sint32) p.y_;
						component->handleMouse(motionEvent);
						currentComponent = component;
						break;
					}
				}
				if (mouseMotionInsideComponent_ != currentComponent) {
					if (mouseMotionInsideComponent_ != nullptr) {
						mouseMotionInsideComponent_->mouseMotionLeave();
					}
				}
				mouseMotionInsideComponent_ = currentComponent;
				Component::handleMouse(mouseEvent);
				break;
			case SDL_MOUSEBUTTONDOWN:
				// Fall through!
			case SDL_MOUSEBUTTONUP:
				// Send the mouseEvent through to the correct component.
				for (auto& component : *this) {
					if (!component->isVisible()) {
						continue;
					}
					Point p = component->getLocation();
					Dimension d = component->getSize();

					// Mouse is inside the component?
					if (p.x_ <= mouseEvent.button.x && p.x_ + d.width_ > mouseEvent.button.x &&
						p.y_ <= mouseEvent.button.y && p.y_ + d.height_ > mouseEvent.button.y) {
						SDL_Event buttonEvent = mouseEvent;
						buttonEvent.motion.x -= (Sint32) p.x_;
						buttonEvent.motion.y -= (Sint32) p.y_;
						currentComponent = component;
						component->handleMouse(buttonEvent);
						break; // Abort, components should not overlap!
					}
				}

				if (mouseEvent.type == SDL_MOUSEBUTTONDOWN) {
					// Set all components focus to false except
					// the component used.
					for (auto& component : *this) {
						if (!component->isVisible()) {
							continue;
						}
						if (currentComponent == component) {
							component->setFocus(true);
						} else {
							component->setFocus(false);
						}
					}
				}

				// Call the component if it was pushed and released outside
				// the component.
				if (mouseEvent.type == SDL_MOUSEBUTTONUP) {
					if (mouseDownInsideComponent_ != nullptr && mouseDownInsideComponent_ != currentComponent) {
						mouseDownInsideComponent_->mouseOutsideUp();
						mouseDownInsideComponent_ = nullptr;
					}
				} else if (mouseEvent.type == SDL_MOUSEBUTTONDOWN) {
					mouseDownInsideComponent_ = currentComponent;
				}

				Component::handleMouse(mouseEvent);
				break;
		}
	}

	void Panel::handleKeyboard(const SDL_Event& keyEvent) {
		for (auto& component : *this) {
			if (component->hasFocus()) {
				component->handleKeyboard(keyEvent);
			}
		}
		Component::handleKeyboard(keyEvent);
		group_.handleKeyboard(keyEvent);
	}

	void Panel::mouseMotionLeave() {
		for (auto& component : *this) {
			component->mouseMotionLeave();
		}
	}

	void Panel::mouseOutsideUp() {
		for (auto& component : *this) {
			component->mouseOutsideUp();
		}
	}

	void Panel::panelChanged(bool active) {
		if (active) {
			validate();
		}

		for (auto& component : *this) {
			component->panelChanged(active);
		}
		Component::panelChanged(active);
	}

	void Panel::validate() {
		// Validate!
		layoutManager_->layoutContainer(*this);
		for (auto& child : *this) {
			child->validate();
		}

		group_.sort();
	}

} // Namespace gui.
