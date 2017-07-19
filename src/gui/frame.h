#ifndef GUI_FRAME_H
#define GUI_FRAME_H

#include "panel.h"
#include "layoutmanager.h"
#include "graphic.h"

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
	using DrawListener = mw::Signal<Frame&, double>;

	class Frame : public mw::Window {
	public:
		using iterator = std::vector<std::shared_ptr<Panel>>::iterator;
		using const_iterator = std::vector<std::shared_ptr<Panel>>::const_iterator;

		Frame();
		virtual ~Frame() = default;

		// Add the default panel in the back of the internal vector.
		// The panel index is returned.
		int addPanelBack();

		// Add the provided panel in the back of the internal vector.
		// The panel index is returned.
		int pushBackPanel(const std::shared_ptr<Panel>& panel);

        // Add the component, with the provided layoutIndex.
		// Will assert if the component already added.
		template <class Comp, class... Args>
		std::shared_ptr<Comp> addDefault(Args... args);

		// Add the component, with the default index layout.
		// Will assert if the component already added.
		std::shared_ptr<Component> addDefault(const std::shared_ptr<Component>& component);

		// Add the component, with the provided layoutIndex.
		// Will assert if the component already added.
		template <class Comp, class... Args>
		std::shared_ptr<Comp> add(int layoutIndex, Args... args);

		// Add the component, with the default index layout.
		// Will assert if the component already added.
		std::shared_ptr<Component> add(int layoutIndex, const std::shared_ptr<Component>& component);

		// Same as add(int layoutIndex, Component* component) but added to
		// a traversal group too.
		template <class Comp, class... Args>
		std::shared_ptr<Comp> addDefaultToGroup(Args... args);

		// Same as add(int layoutIndex, Component* component) but added to
		// a traversal group too.
		std::shared_ptr<Component> addDefaultToGroup(const std::shared_ptr<Component>& component);

		// Same as add(int layoutIndex, Component* component) but added to
		// a traversal group too.
		template <class Comp, class... Args>
		std::shared_ptr<Comp> addToGroup(int layoutIndex, Args... args);

		// Same as add(int layoutIndex, Component* component) but added to
		// a traversal group too.
		std::shared_ptr<Component> addToGroup(int layoutIndex, const std::shared_ptr<Component>& component);

		// Set the layout manager.
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

		// Add a window listener. A callback be be made when a SDL_WINDOWEVENT is called
		// for this window.
		mw::signals::Connection addWindowListener(const WindowListener::Callback& callback);

		// Add a SDL event listener. A callback be be made when any SDL event is called
		// for any window.
		mw::signals::Connection addSdlEventListener(const SdlEventListener::Callback& callback);

		// Set the default closing true or false.
		// Makes the window's close by clicking the quit button or by
		// pressing the ESQ key if set to true. if set to false
		// nothing happens.
		void setDefaultClosing(bool defaultClosing);

		// Returns if the default closing is active or not.
		bool isDefaultClosing() const;

		int getPanelsCount() const;

		int getCurrentPanelIndex() const;

		void setCurrentPanel(int index);

		std::shared_ptr<Panel> getCurrentPanel() const;

		// Add a key listener to the current panel.
		mw::signals::Connection addKeyListener(const KeyListener::Callback& callback);

		// Add a mouse listener to the current panel.
		mw::signals::Connection addMouseListener(const MouseListener::Callback& callback);

		// Add a focus listener to the current panel.
		mw::signals::Connection addFocusListener(const FocusListener::Callback& callback);

		// Add a action listener to the current panel.
		mw::signals::Connection addActionListener(const ActionListener::Callback& callback);

		// Add a panel change listener to the current panel.
		mw::signals::Connection addPanelChangeListener(const PanelChangeListener::Callback& callback);

		// Add a update listener to the current panel.
		mw::signals::Connection addDrawListener(const DrawListener::Callback& callback);

	protected:
		virtual void initPreLoop() override;

	private:
		// Override mw::Window.
		virtual void update(double deltaTime) override final;

		// Override mw::Window.
		virtual void eventUpdate(const SDL_Event& windowEvent) override final;

		void resize(int width, int height);

		std::queue<SDL_Event> eventQueue_;
		WindowListener windowListener_;
		SdlEventListener sdlEventListener_;
		Graphic graphic_;

		bool defaultClosing_;
		std::vector<std::shared_ptr<Panel>> panels_;
		int currentPanel_;
	};

	// Add the component, with the provided layoutIndex.
	// Will assert if the component already added.
	template <class Comp, class... Args>
	std::shared_ptr<Comp> Frame::addDefault(Args... args) {
		auto c = std::make_shared<Comp>(args...);
		add(DEFAULT_INDEX, c);
		return c;
	}

	// Add the component, with the provided layoutIndex.
	// Will assert if the component already added.
	template <class Comp, class... Args>
	std::shared_ptr<Comp> Frame::add(int layoutIndex, Args... args) {
		auto c = std::make_shared<Comp>(args...);
		add(layoutIndex, c);
		return c;
	}

	// Same as add(int layoutIndex, Component* component) but added to
	// a traversal group too.
	template <class Comp, class... Args>
	std::shared_ptr<Comp> Frame::addDefaultToGroup(Args... args) {
		auto c = std::make_shared<Comp>(args...);
		addToGroup(DEFAULT_INDEX, c);
		return c;
	}
		
	// Same as add(int layoutIndex, Component* component) but added to
	// a traversal group too.
	template <class Comp, class... Args>
	std::shared_ptr<Comp> Frame::addToGroup(int layoutIndex, Args... args) {
		auto c = std::make_shared<Comp>(args...);
		addToGroup(layoutIndex, c);
		return c;
	}

	template <class LManager, class... Args>
	std::shared_ptr<LManager> Frame::setLayout(Args... args) {
		auto m = std::make_shared<LManager>(args...);
		setLayout(m);
		return m;
	}

	inline int Frame::getPanelsCount() const {
		return panels_.size();
	}

	inline int Frame::getCurrentPanelIndex() const {
		return currentPanel_;
	}


	inline std::shared_ptr<Panel> Frame::getCurrentPanel() const {
		return panels_[currentPanel_];
	}

	// Add a key listener to the current panel.
	inline mw::signals::Connection Frame::addKeyListener(const KeyListener::Callback& callback) {
		return getCurrentPanel()->addKeyListener(callback);
	}

	// Add a mouse listener to the current panel.
	inline mw::signals::Connection Frame::addMouseListener(const MouseListener::Callback& callback) {
		return getCurrentPanel()->addMouseListener(callback);
	}

	// Add a focus listener to the current panel.
	inline mw::signals::Connection Frame::addFocusListener(const FocusListener::Callback& callback) {
		return getCurrentPanel()->addFocusListener(callback);
	}

	// Add a action listener to the current panel.
	inline mw::signals::Connection Frame::addActionListener(const ActionListener::Callback& callback) {
		return getCurrentPanel()->addActionListener(callback);
	}

	// Add a panel change listener to the current panel.
	inline mw::signals::Connection Frame::addPanelChangeListener(const PanelChangeListener::Callback& callback) {
		return getCurrentPanel()->addPanelChangeListener(callback);
	}

	// Add a update listener to the current panel.
	inline mw::signals::Connection Frame::addDrawListener(const DrawListener::Callback& callback) {
		return getCurrentPanel()->addDrawListener(callback);
	}

	inline Frame::iterator Frame::begin() {
		return panels_.begin();
	}

	inline Frame::iterator Frame::end() {
		return panels_.end();
	}

	inline Frame::const_iterator Frame::begin() const {
		return panels_.begin();
	}

	inline Frame::const_iterator Frame::end() const {
		return panels_.end();
	}

	inline Frame::const_iterator Frame::cbegin() const {
		return panels_.begin();
	}

	inline Frame::const_iterator Frame::cend() const {
		return panels_.end();
	}

} // Namespace gui.

#endif // GUI_FRAME_H
