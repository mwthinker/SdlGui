#include "panel.h"
#include "component.h"
#include "flowlayout.h"
#include "frame.h"

#include <list>
#include <cassert>

namespace gui {

	Panel::Panel() : layoutManager_(std::make_shared<FlowLayout>()) {
		setPreferredSize(50, 50);
		setBorderColor(1, 1, 1, 0);
	}

	void Panel::setChildsParent() {
	    for (auto& c : components_) {
			c->parent_ = std::static_pointer_cast<Panel>(shared_from_this());
			if (ancestor_ != nullptr) {
				c->ancestor_ = ancestor_;
			} else {
				c->ancestor_ = c->parent_;
			}
			c->setChildsParent();
		}
	}

	std::shared_ptr<Component> Panel::addDefault(const std::shared_ptr<Component>& component) {
		add(DEFAULT_INDEX, component);
		return component;
	}

    std::shared_ptr<Component> Panel::add(int layoutIndex, const std::shared_ptr<Component>& component) {
		// Was already added?
		assert(!component->isAdded_);
		component->init();
		component->isAdded_ = true;
		component->setLayoutIndex(layoutIndex);
		components_.push_back(component);
		setChildsParent();
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

	void Panel::remove(const std::shared_ptr<Component>& component) {
		auto it = std::find(components_.begin(), components_.end(), component);
		if (it != components_.end()) {
			// Remove component.
			components_.erase(it);
		}
		group_.remove(component);
	}

	void Panel::removeAll() {
		components_.clear();
		group_.removeAll();
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

	const std::vector<std::shared_ptr<Component>>& Panel::getComponents() const {
		return components_;
	}

	void Panel::draw(const Graphic& graphic, double deltaTime) {
		graphic.useProgram();
		graphic.setModel(Component::model_);
		Component::draw(graphic, deltaTime);

		// Draw the components.
		for (auto& component : *this) {
			if (component != priorityComponent_ && component->isVisible()) {
				graphic.useProgram();
				graphic.setModel(component->model_);
				component->draw(graphic, deltaTime);
			}
		}

		if (priorityComponent_) {
			graphic.useProgram();
			graphic.setModel(priorityComponent_->model_);
			priorityComponent_->draw(graphic, deltaTime);
		}
	}

	void Panel::handleMouse(const SDL_Event& mouseEvent) {
		std::shared_ptr<Component> currentComponent;
		switch (mouseEvent.type) {
			case SDL_MOUSEMOTION:
				handleMouseMotionEvent(mouseEvent);
				break;
			case SDL_MOUSEBUTTONDOWN:
				// Fall through!
			case SDL_MOUSEBUTTONUP:
				handleMouseButtonEvent(mouseEvent);
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

	void Panel::drawListener(Frame& frame, double deltaTime) {
		Component::drawListener(frame, deltaTime);
		for (auto& child : *this) {
			if (child->isVisible()) {
				child->drawListener(frame, deltaTime);
			}
		}
	}

	void Panel::validate() {
		// Validate!
		layoutManager_->layoutContainer(*this);
		for (auto& child : *this) {
			child->validate();
		}

		group_.sort();
	}

	void Panel::handleMouseMotionEvent(SDL_Event mouseEvent) {
		std::shared_ptr<Component> currentComponent = priorityComponent_;
		unsigned int index = 0;
		while (index < components_.size()) {
			if (currentComponent == nullptr) {
				currentComponent = components_[index];
				++index;
			}
			if (!currentComponent->isVisible()) {
				++index;
				continue;
			}
			Point p = currentComponent->getLocation();
			Dimension d = currentComponent->getSize();
			if (p.x_ <= mouseEvent.motion.x && p.x_ + d.width_ > mouseEvent.motion.x &&
				p.y_ <= mouseEvent.motion.y && p.y_ + d.height_ > mouseEvent.motion.y) {
				SDL_Event motionEvent = mouseEvent;
				motionEvent.motion.x -= (Sint32) p.x_;
				motionEvent.motion.y -= (Sint32) p.y_;
				currentComponent->handleMouse(motionEvent);
				break;
			}
			currentComponent = nullptr;
		}

		if (mouseMotionInsideComponent_ != nullptr && mouseMotionInsideComponent_ != currentComponent) {
			mouseMotionInsideComponent_->mouseMotionLeave();
		}

		mouseMotionInsideComponent_ = currentComponent;
		Component::handleMouse(mouseEvent);
	}

	void Panel::handleMouseButtonEvent(SDL_Event mouseEvent) {
		std::shared_ptr<Component> currentComponent = priorityComponent_;
		unsigned int index = 0;
		// Send the mouseEvent through to the correct component.
		while (index < components_.size()) {
			if (currentComponent == nullptr) {
				currentComponent = components_[index];
				++index;
			}
			if (!currentComponent->isVisible()) {
				++index;
				continue;
			}
			Point p = currentComponent->getLocation();
			Dimension d = currentComponent->getSize();
			if (p.x_ <= mouseEvent.button.x && p.x_ + d.width_ > mouseEvent.button.x &&
				p.y_ <= mouseEvent.button.y && p.y_ + d.height_ > mouseEvent.button.y) {
				SDL_Event buttonEvent = mouseEvent;
				buttonEvent.button.x -= (Sint32) p.x_;
				buttonEvent.button.y -= (Sint32) p.y_;
				currentComponent->handleMouse(buttonEvent);
				break; // Abort, components should not overlap!
			}
			currentComponent = nullptr;
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
	}

	void Panel::demandPriority(const std::shared_ptr<Component>& component) {
		if (priorityComponent_ != nullptr && component != nullptr) {
			priorityComponent_->priorityChanged(false);
		}
		if (component != nullptr) {
			priorityComponent_ = component;
			priorityComponent_->priorityChanged(true);
		}
	}

	void Panel::releasePriority(const std::shared_ptr<Component>& component) {
		if (priorityComponent_ != nullptr && component == priorityComponent_) {
			priorityComponent_->priorityChanged(false);
			priorityComponent_ = nullptr;
		}
	}

} // Namespace gui.
