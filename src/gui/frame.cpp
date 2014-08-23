#include "frame.h"
#include "borderlayout.h"

#include <mw/font.h>
#include <mw/window.h>
#include <mw/exception.h>

#include <string>

namespace gui {
	
	Frame::Frame() : mw::Window(-1, -1, 512, 512, true, "Frame", "") {
		init();
	}

	Frame::Frame(int x, int y, int width, int height, bool resizeable, std::string title, std::string icon, bool borderless) : mw::Window(x, y, width, height, resizeable, title, icon, borderless) {
		init();
	}

	int Frame::addPanelBack() {
		std::shared_ptr<Panel> p = std::make_shared<Panel>();
		p->setLayout(std::make_shared<BorderLayout>());
		p->setChildsParent(p);
		return push_back(p);
	}

	int Frame::push_back(const std::shared_ptr<Panel>& panel) {
		panels_.push_back(panel);
		panel->setChildsParent(panel);
		return panels_.size() - 1;
	}

	void Frame::add(const std::shared_ptr<Component>& component) {
		getCurrentPanel()->add(component);
		component->setChildsParent(component);
	}

	void Frame::add(const std::shared_ptr<Component>& component, int layoutIndex) {
		getCurrentPanel()->add(component, layoutIndex);
		component->setChildsParent(component);
	}

	void Frame::addToGroup(const std::shared_ptr<Component>& component) {
		getCurrentPanel()->addToGroup(component);
		component->setChildsParent(component);
	}

	void Frame::addToGroup(const std::shared_ptr<Component>& component, int layoutIndex) {
		getCurrentPanel()->addToGroup(component, layoutIndex);
		component->setChildsParent(component);
	}

	void Frame::setLayout(const std::shared_ptr<LayoutManager>& layoutManager) {
		getCurrentPanel()->setLayout(layoutManager);
	}

	std::shared_ptr<LayoutManager> Frame::getLayout() const {
		return getCurrentPanel()->getLayout();
	}

	std::vector<std::shared_ptr<Panel>>::iterator Frame::begin() {
		return panels_.begin();
	}

	std::vector<std::shared_ptr<Panel>>::iterator Frame::end() {
		return panels_.end();
	}

	void Frame::setCurrentPanel(int index) {
		if (currentPanel_ != index) {
			getCurrentPanel()->panelChanged(false);
			currentPanel_ = index;
			resize();
			getCurrentPanel()->panelChanged(true);
		}
	}

	mw::signals::Connection Frame::addWindowListener(const WindowListener::Callback& callback) {
		return windowListener_.connect(callback);
	}

	mw::signals::Connection Frame::addSdlEventListener(const SdlEventListener::Callback& callback) {
		return sdlEventListener_.connect(callback);
	}

	void Frame::setDefaultClosing(bool defaultClosing) {
		defaultClosing_ = true;
	}

	bool Frame::isDefaultClosing() const {
		return defaultClosing_;
	}

	void Frame::resize() {
		getCurrentPanel()->setPreferredSize((float) getWidth(), (float) getHeight());
		getCurrentPanel()->setSize((float) getWidth(), (float) getHeight());
		getCurrentPanel()->setLocation(0, 0);
		getCurrentPanel()->validate();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, getWidth(), getHeight());
		glOrtho(0, getWidth(), 0, getHeight(), -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	
	void Frame::update(Uint32 deltaTime) {
		updateListener_(*this, deltaTime);

		// Perform non critical event updates.
		while (!eventQueue_.empty()) {
			SDL_Event sdlEvent = eventQueue_.front();
			eventQueue_.pop();

			sdlEventListener_(*this, sdlEvent);
			switch (sdlEvent.type) {
				case SDL_WINDOWEVENT:
					if (sdlEvent.window.windowID == SDL_GetWindowID(getSdlWindow())) {
						windowListener_(*this, sdlEvent);
						switch (sdlEvent.window.event) {
							case SDL_WINDOWEVENT_RESIZED:
								resize();
								break;
							case SDL_WINDOWEVENT_LEAVE:
								getCurrentPanel()->mouseMotionLeave();
								break;
							case SDL_WINDOWEVENT_CLOSE:
								if (defaultClosing_) {
									quit();
								}
						}
					}
					break;
				case SDL_MOUSEMOTION:
					if (sdlEvent.motion.windowID == SDL_GetWindowID(getSdlWindow())) {
						// Reverse y-axis.
						sdlEvent.motion.yrel *= -1;
						sdlEvent.motion.y = getHeight() - sdlEvent.motion.y;
						getCurrentPanel()->handleMouse(sdlEvent);
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					// Fall through!
				case SDL_MOUSEBUTTONUP:
					if (sdlEvent.button.windowID == SDL_GetWindowID(getSdlWindow())) {
						sdlEvent.button.y = getHeight() - sdlEvent.motion.y;
						getCurrentPanel()->handleMouse(sdlEvent);
					}
					break;
				case SDL_TEXTINPUT:
					// Fall through.
				case SDL_TEXTEDITING:
					// Fall through.
				case SDL_KEYDOWN:
					// Fall through.
				case SDL_KEYUP:
					if (sdlEvent.key.windowID == SDL_GetWindowID(getSdlWindow())) {
						getCurrentPanel()->handleKeyboard(sdlEvent);
						switch (sdlEvent.key.keysym.sym) {
							case SDLK_ESCAPE:
								if (defaultClosing_) {
									quit();
								}
								break;
						}
					}
					break;
			}
		}
		getCurrentPanel()->draw(deltaTime);
	}
	
	void Frame::eventUpdate(const SDL_Event& windowEvent) {
		eventQueue_.push(windowEvent);
	}

	void Frame::init() {
		currentPanel_ = 0;
		// Default layout for Frame.
		push_back(std::make_shared<Panel>());

		getCurrentPanel()->setLayout(std::make_shared<BorderLayout>());

		// Init the opengl settings.
		resize();

		getCurrentPanel()->setBackgroundColor(1, 1, 1);
		getCurrentPanel()->setSize((float) getWidth(), (float) getHeight());
		getCurrentPanel()->setPreferredSize((float) getWidth(), (float) getHeight());
		defaultClosing_ = false;
	}

} // Namespace gui.
