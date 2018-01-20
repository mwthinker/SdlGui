#include "component.h"
#include "panel.h"

#include <mw/matrix44.h>

#include <cassert>

namespace gui {

	void Component::setLocation(float x, float y) {
		setLocation(Point(x, y));
	}

	void Component::setLocation(const Point& point) {
		location_ = point;
		if (parent_ != nullptr) {
			model_ = parent_->model_;
		} else {
			model_ = mw::Matrix44<GLfloat>::I;
		}
		mw::translate2D(model_, point.x_, point.y_);
	}

	void Component::setPreferredSize(float width, float height) {
		preferedDimension_ = Dimension(width, height);
		validateParent();
	}

	void Component::setPreferredSize(const Dimension& dimension) {
		preferedDimension_ = dimension;
		validateParent();
	}

	void Component::setVisible(bool visible) {
		if (visible_ != visible) {
			visible_ = visible;
			validateParent();
		}
	}

	mw::signals::Connection Component::addKeyListener(const KeyListener::Callback& callback) {
		return keyListener_.connect(callback);
	}

	mw::signals::Connection Component::addMouseListener(const MouseListener::Callback& callback) {
		return mouseListener_.connect(callback);
	}

	mw::signals::Connection Component::addFocusListener(const FocusListener::Callback& callback) {
		return focusListener_.connect(callback);
	}

	mw::signals::Connection Component::addActionListener(const ActionListener::Callback& callback) {
		return actionListener_.connect(callback);
	}

	mw::signals::Connection Component::addPanelChangeListener(const PanelChangeListener::Callback& callback) {
		return panelChangeListener_.connect(callback);
	}

	mw::signals::Connection Component::addDrawListener(const DrawListener::Callback& callback) {
		return drawListener_.connect(callback);
	}

	void Component::setFocus(bool focus) {
		// Change?
		if (focus_ != focus) {
			focus_ = focus;
			focusListener_(*this);
		}
	}

	bool Component::hasFocus() const {
		return focus_;
	}
	
	void Component::draw(const Graphic& graphic, double deltaTime) {
		// Draw panel background.
		graphic.setColor(backgroundColor_);
		graphic.drawSquare(0, 0, dimension_.width_, dimension_.height_);
		graphic.drawSprite(background_, 0, 0, dimension_.width_, dimension_.height_);
		graphic.setColor(borderColor_);
		graphic.drawBorder(0, 0, dimension_.width_, dimension_.height_);
	}

	void Component::doAction() {
		actionListener_(*this);
	}

	void Component::panelChanged(bool active) {
		panelChangeListener_(*this, active);
	}

	int Component::getLayoutIndex() const {
		return layoutIndex_;
	}

	void Component::setLayoutIndex(int layoutIndex) {
		layoutIndex_ = layoutIndex;
	}
	
	Component::Component() : parent_(nullptr), ancestor_(nullptr),
		borderColor_(0, 0, 0), backgroundColor_(1, 1, 1), layoutIndex_(0), visible_(true),
		focus_(false), isAdded_(false),
		model_(mw::Matrix44<GLfloat>::I) {

	}

	void Component::handleMouse(const SDL_Event& mouseEvent) {
		mouseListener_(*this, mouseEvent);
	}

	void Component::handleKeyboard(const SDL_Event& keyEvent) {
		keyListener_(*this, keyEvent);
	}

	void Component::validateParent() {
		if (parent_ != nullptr) {
			parent_->validate();
		}
	}

	void Component::demandPriority() {
		if (getParent() != nullptr) {
			getParent()->demandPriority(shared_from_this());
		}
	}

	void Component::releasePriority() {
		if (getParent() != nullptr) {
			getParent()->releasePriority(shared_from_this());
		}
	}

	void Component::drawListener(Frame& frame, double deltaTime) {
		drawListener_(frame, deltaTime);
	}

} // Namespace gui.
