#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "dimension.h"
#include "graphic.h"

#include <mw/matrix44.h>
#include <mw/signal.h>
#include <mw/color.h>
#include <mw/sprite.h>
#include <mw/text.h>
#include <mw/vertexbufferobject.h>

#include <SDL.h>

#include <memory>

namespace gui {

	class Component;
	class Panel;
	class Frame;

	using FocusListener = mw::Signal<Component&>;
	using KeyListener = mw::Signal<Component&, const SDL_Event&>;
	using MouseListener = KeyListener;
	using ActionListener = mw::Signal<Component&>;
	using PanelChangeListener = mw::Signal<Component&, bool>;
	using DrawListener = mw::Signal<Frame&, double>;

	class Component : public std::enable_shared_from_this<Component> {
	public:
		friend class Frame;
		friend class Panel;

		virtual ~Component() = default;

		// Return the component's location in the parent's coordinate space.
		Point getLocation() const {
			return location_;
		}

		void setLocation(float x, float y);

		void setLocation(const Point& point);

		void setPreferredSize(float width, float height);

		void setPreferredSize(const Dimension& dimension);

		Dimension getPreferredSize() const {
			return preferedDimension_;
		}

		// Return the component size.
		Dimension getSize() const {
			return dimension_;
		}

		void setSize(float width, float height) {
			dimension_ = Dimension(width, height);
		}

		void setSize(const Dimension& dimension) {
			dimension_ = dimension;
		}

		void setVisible(bool visible);

		bool isVisible() const {
			return visible_;
		}

		mw::signals::Connection addKeyListener(const KeyListener::Callback& callback);
		mw::signals::Connection addMouseListener(const MouseListener::Callback& callback);
		mw::signals::Connection addFocusListener(const FocusListener::Callback& callback);
		mw::signals::Connection addActionListener(const ActionListener::Callback& callback);
		mw::signals::Connection addPanelChangeListener(const PanelChangeListener::Callback& callback);		
		mw::signals::Connection addDrawListener(const DrawListener::Callback& callback);

		// Set the focus for the component.
		// The focus listener is called if the focus changes.
		virtual void setFocus(bool focus);

		// Return the focus for the component.
		bool hasFocus() const;

		std::shared_ptr<Panel> getParent() const {
			return parent_;
		}

		std::shared_ptr<Panel> getAncestor() const {
			return parent_;
		}

		void setBackground(const mw::Sprite& background) {
			background_ = background;
		}

		const mw::Sprite& getBackground() const {
			return background_;
		}

		void setBackgroundColor(const mw::Color& color) {
			backgroundColor_ = color;
		}

		void setBackgroundColor(float red, float green, float blue, float alpha = 1) {
			backgroundColor_ = mw::Color(red, green, blue, alpha);
		}

		const mw::Color& getBackgroundColor() const {
			return backgroundColor_;
		}

		void setBorderColor(const mw::Color& color) {
			borderColor_ = color;
		}

		void setBorderColor(float red, float green, float blue, float alpha = 1) {
			borderColor_ = mw::Color(red, green, blue, alpha);
		}

		const mw::Color& getBorderColor() const {
			return borderColor_;
		}
		
		// Do action.
		void doAction();		

		// Get the layout index for the component.
		// 0 is the default value.
		int getLayoutIndex() const;

		// Define the layout for the component.
		// Should correspond to the active LayoutManager in order to work.
		void setLayoutIndex(int layoutIndex);

		const mw::Matrix44<GLfloat>& getModelMatrix() {
			return model_;
		}
		
	protected:
		Component();

		virtual void init() {
		}

		// Draw the background color.
		// Should be derived and should then draw the
		// component in the size defined by getSize().
		virtual void draw(const Graphic& graphic, double deltaTime);
	
		// Is called in order to signal the parent component that
		// the children's sizes must be recalculated.
		void validateParent();

		virtual void setChildsParent() {
		}

		// Take care of all mouse events. And send it through to
		// all mouse listener callbacks.
		// Mouse events: SDL_MOUSEMOTION, SDL_MOUSEBUTTONDOWN and SDL_MOUSEBUTTONUP.
		virtual void handleMouse(const SDL_Event& mouseEvent);

		// Take care of all key events. And send it through to
		// all key listener callbacks.
		// Key events: SDL_TEXTINPUT, SDL_TEXTEDITING, SDL_KEYDOWN and SDL_KEYUP.
		virtual void handleKeyboard(const SDL_Event& keyEvent);

		// Fix all child components sizes. Based on the layout manager and
		// the preferred component size.
		// If there are no children nothing happens.
		virtual void validate() {
		}

		// Only called when the mouse leave the component.
		virtual void mouseMotionLeave() {
		}

		// Only called when the mouse up was inside the component.
		// And the up event was outside.
		virtual void mouseOutsideUp() {
		}

		virtual void panelChanged(bool active);

		virtual void priorityChanged(bool newPriority) {
		}

		void demandPriority();

		void releasePriority();

		virtual void drawListener(Frame& frame, double deltaTime);

	private:
		std::shared_ptr<Panel> parent_;
		std::shared_ptr<Panel> ancestor_;

		mw::Sprite background_;
		mw::Color backgroundColor_;
		mw::Color borderColor_;
		Point location_;
		Dimension dimension_;
		Dimension preferedDimension_;
		int layoutIndex_;

		FocusListener focusListener_;
		KeyListener keyListener_;
		MouseListener mouseListener_;
		ActionListener actionListener_;
		PanelChangeListener panelChangeListener_;
		DrawListener drawListener_;

		bool visible_;
		bool focus_;

		bool isAdded_;
		mw::Matrix44<GLfloat> model_;
	};

} // Namespace gui.

#endif // GUI_COMPONENT_H
