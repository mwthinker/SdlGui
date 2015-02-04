#ifndef GUI_FRAME_H
#define GUI_FRAME_H

#include "panel.h"

#include <mw/window.h>
#include <mw/color.h>
#include <mw/signal.h>

#include <SDL.h>

#include <string>
#include <queue>

namespace gui {

	class Frame;
	using WindowListener = mw::Signal<Frame&, const SDL_Event&>;
	using SdlEventListener = mw::Signal<Frame&, const SDL_Event&>;
	using UpdateListener = mw::Signal<Frame&, Uint32>;

	class Frame : public mw::Window {
	public:
		Frame(int x, int y, int width, int height, bool resizeable = true, std::string title = "Frame", std::string icon = "", bool borderless = false);

		// Add the default panel in the back of the internal vector.
		// The panel index is returned.
		int addPanelBack();

		// Add the provided panel in the back of the internal vector.
		// The panel index is returned.
		int pushBackPanel(const std::shared_ptr<Panel>& panel);

		// Add the component, with the provided layoutIndex.
		// Will assert if the component already added.
		template <class Comp, class... Args>
		std::shared_ptr<Comp> add(int layoutIndex, Args... args) {
			std::shared_ptr<Comp> c = std::make_shared<Comp>(args...);
			add(layoutIndex, c);
			return c;
		}

		// Adds component, with the provided layoutIndex.
		// Will assert if the component already added.
		void add(int layoutIndex, const std::shared_ptr<Component>& component);

		// Same as add(int layoutIndex, Component* component) but added to
		// a traversal group too.
		template <class Comp, class... Args>
		std::shared_ptr<Comp> addToGroup(int layoutIndex, Args... args) {
			std::shared_ptr<Comp> c = std::make_shared<Comp>(args...);
			addToGroup(layoutIndex, c);
			return c;
		}

		// Same as add(int layoutIndex, Component* component) but added to
		// a traversal group too.
		void addToGroup(int layoutIndex, const std::shared_ptr<Component>& component);

		// Set the layout manager.
		void setLayout(const std::shared_ptr<LayoutManager>& layoutManager);

		template <class LManager, class... Args>
		std::shared_ptr<LManager> setLayout(Args... args) {
			std::shared_ptr<LManager> m = std::make_shared<LManager>(args...);
			setLayout(m);
			return m;
		}

		// Get the current layout manager.
		std::shared_ptr<LayoutManager> getLayout() const;

		std::vector<std::shared_ptr<Panel>>::iterator begin();
		std::vector<std::shared_ptr<Panel>>::iterator end();

		// Add a window listener. A callback be be made when a SDL_WINDOWEVENT is called
		// for this window.
		mw::signals::Connection addWindowListener(const WindowListener::Callback& callback);

		// Add a sdl event listener. A callback be be made when any sdl event is called
		// for any window.
		mw::signals::Connection addSdlEventListener(const SdlEventListener::Callback& callback);

		// Set the default closing true or false.
		// Makes the window's close by clicking the quit button or by
		// pressing the ESQ key if set to true. if set to false
		// nothing happens.
		void setDefaultClosing(bool defaultClosing);

		// Returns if the default closing is active or not.
		bool isDefaultClosing() const;

		inline int getNbrOfPanels() const {
			return panels_.size();
		}

		inline int getCurrentPanelIndex() const {
			return currentPanel_;
		}

		void setCurrentPanel(int index);

		inline std::shared_ptr<Panel> getCurrentPanel() const {
			return panels_[currentPanel_];
		}

		// Add a key listener to the current panel.
		inline mw::signals::Connection addKeyListener(const KeyListener::Callback& callback) {
			return getCurrentPanel()->addKeyListener(callback);
		}

		// Add a mouse listener to the current panel.
		inline mw::signals::Connection addMouseListener(const MouseListener::Callback& callback) {
			return getCurrentPanel()->addMouseListener(callback);
		}

		// Add a focus listener to the current panel.
		inline mw::signals::Connection addFocusListener(const FocusListener::Callback& callback) {
			return getCurrentPanel()->addFocusListener(callback);
		}

		// Add a action listener to the current panel.
		inline mw::signals::Connection addActionListener(const ActionListener::Callback& callback) {
			return getCurrentPanel()->addActionListener(callback);
		}

		// Add a panel change listener to the current panel.
		inline mw::signals::Connection addPanelChangeListener(const PanelChangeListener::Callback& callback) {
			return getCurrentPanel()->addPanelChangeListener(callback);
		}

		// Add a uppdate listener to the current panel.
		inline mw::signals::Connection addUpdateListener(const UpdateListener::Callback& callback) {
			return updateListener_.connect(callback);
		}

	private:
		// Override mw::Window.
		virtual void update(Uint32 deltaTime) override final;

		// Override mw::Window.
		virtual void eventUpdate(const SDL_Event& windowEvent) override final;

		void resize(int width, int height);

		std::queue<SDL_Event> eventQueue_;
		WindowListener windowListener_;
		SdlEventListener sdlEventListener_;
		UpdateListener updateListener_;

		bool defaultClosing_;
		std::vector<std::shared_ptr<Panel>> panels_;
		int currentPanel_;
	};

} // Namespace gui.

#endif // GUI_FRAME_H
