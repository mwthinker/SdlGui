#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include "dimension.h"
#include "guishader.h"

#include <mw/matrix.h>
#include <mw/signal.h>
#include <mw/color.h>
#include <mw/sprite.h>
#include <mw/text.h>

#include <SDL.h>

#include <memory>

namespace gui {

	class Component;
	class Panel;

	using FocusListener = mw::Signal<Component&>;
	using KeyListener = mw::Signal<Component&, const SDL_Event&>;
	using MouseListener = KeyListener;
	using ActionListener = mw::Signal<Component&>;
	using PanelChangeListener = mw::Signal<Component&, bool>;

	class Component : public std::enable_shared_from_this<Component> {
	public:
		friend class Frame;
		friend class Panel;

		virtual ~Component() {
		}

		// Return the component's location in the parent's coordinate space.
		inline Point getLocation() const {
			return location_;
		}

		void setLocation(float x, float y);

		void setLocation(const Point& point);

		void setPreferredSize(float width, float height);

		void setPreferredSize(const Dimension& dimension);

		inline Dimension getPreferredSize() const {
			return preferedDimension_;
		}

		// Return the component size.
		inline Dimension getSize() const {
			return dimension_;
		}

		inline void setSize(float width, float height) {
			dimension_ = Dimension(width, height);
		}

		inline void setSize(const Dimension& dimension) {
			dimension_ = dimension;
		}

		void setVisible(bool visible);

		inline bool isVisible() const {
			return visible_;
		}

		mw::signals::Connection addKeyListener(const KeyListener::Callback& callback);
		mw::signals::Connection addMouseListener(const MouseListener::Callback& callback);
		mw::signals::Connection addFocusListener(const FocusListener::Callback& callback);
		mw::signals::Connection addActionListener(const ActionListener::Callback& callback);
		mw::signals::Connection addPanelChangeListener(const PanelChangeListener::Callback& callback);		

		// If set to true makes the component to always have focus. All parents will
		// also be set to have focus. If the current state is the same as the change,
		// nothing happens. If set to false, it will stop grabbing focus unless there are
		// children that want to grab focus.
		void setGrabFocus(bool grabFocus);

		bool isGrabFocus() const;

		// Set the focus for the component.
		// The focus listener is called if the focus changes.
		virtual void setFocus(bool focus);

		// Return the focus for the component.
		bool hasFocus() const;

		inline std::shared_ptr<Panel> getParent() const {
			return parent_;
		}

		inline std::shared_ptr<Panel> getAncestor() const {
			return parent_;
		}

		inline void setBackground(const mw::Sprite& background) {
			background_ = background;
		}

		inline const mw::Sprite& getBackground() const {
			return background_;
		}

		inline void setBackgroundColor(const mw::Color& color) {
			backgroundColor_ = color;
		}

		inline void setBackgroundColor(float red, float green, float blue, float alpha = 1) {
			backgroundColor_ = mw::Color(red, green, blue, alpha);
		}

		inline const mw::Color& getBackgroundColor() const {
			return backgroundColor_;
		}

		inline void setBorderColor(const mw::Color& color) {
			borderColor_ = color;
		}

		inline void setBorderColor(float red, float green, float blue, float alpha = 1) {
			borderColor_ = mw::Color(red, green, blue, alpha);
		}

		inline const mw::Color& getBorderColor() const {
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

		inline const mw::Matrix44& getModelMatrix() {
			return model_;
		}

		inline const mw::Matrix44& getProjectionMatrix() {
			return proj;
		}

		void drawArrays(GLenum mode, GLfloat* data, int size, bool drawTexture) const;

		void enableGlTransparancy() const;
		void disableGlTransparancy() const;

		void glUseProgram() const;
		void setGlPosA(GLint size, const GLvoid* data) const;
		void setGlPosA(GLint size, GLsizei stride, const GLvoid* data) const;

		void setGlTexA(GLint size, const GLvoid* data) const;
		void setGlTexA(GLint size, GLsizei stride, const GLvoid* data) const;

		void setGlTextureU(bool texture) const;

		void setGlModelU(const mw::Matrix44& matrix) const;
		void setGlColorU(const mw::Color& color) const;
		void setGlColorU(float red, float green, float blue, float alpha = 1) const;

		void drawSquare(float x, float y, float w, float h) const;
		void drawSprite(const mw::Sprite& sprite, float x, float y, float w, float h) const;
		void drawText(const mw::Text& text, float x, float y) const;
		void drawBorder(float x, float y, float w, float h) const;
		
	protected:
		Component();

		virtual void drawBorder();

		// Draw the background color.
		// Should be derived and should then draw the
		// component in the size defined by getSize().
		virtual void draw(Uint32 deltaTime);

	
		// Is called in order to signal the parent component that
		// the children's sizes must be recalculated.
		void validateParent();

		inline virtual void setChildsParent() {
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
		inline virtual void validate() {
		}

		// Only called when the mouse leave the component.
		inline virtual void mouseMotionLeave() {
		}

		// Only called when the mouse up was inside the component.
		// And the up event was outside.
		inline virtual void mouseOutsideUp() {
		}

		virtual void panelChanged(bool active);

		inline virtual void drawFirst(Frame& frame, Uint32 deltaTime) {
		}

		inline virtual void drawLast(Frame& frame, Uint32 deltaTime) {
		}

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

		bool visible_;
		bool focus_;
		bool grabFocus_;
		int nbrChildGrabFocus_;

		bool isAdded_;

		GuiShader guiShader_;

		mw::Matrix44 model_;
		static mw::Matrix44 proj;
	};

} // Namespace gui.

#endif // GUI_COMPONENT_H
