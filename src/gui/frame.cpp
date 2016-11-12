#include "frame.h"
#include "borderlayout.h"

#include <mw/font.h>
#include <mw/window.h>
#include <mw/exception.h>

#include <string>
#include <iostream>

namespace gui {

	Frame::Frame(int x, int y, int width, int height, bool resizeable,
		std::string title, std::string icon, bool borderless) :
		Frame(x, y, width, height, resizeable, title, icon, borderless, []() {

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		const int MAJOR_VERSION = 2;
		const int MINOR_VERSION = 1;

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, MAJOR_VERSION);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, MINOR_VERSION);

		if (SDL_GL_LoadLibrary(0) != 0) {
			std::cerr << "SDL_GL_LoadLibrary failed: " << SDL_GetError() << std::endl;
			std::cerr << "Failed to OpenGL version" << MAJOR_VERSION << "." << MINOR_VERSION << std::endl;
			std::exit(1);
		}
	}) {
	}

	Frame::Frame(int x, int y, int width, int height, bool resizeable,
		std::string title, std::string icon, bool borderless, std::function<void()> initGl) :
		mw::Window(x, y, width, height, resizeable, title, icon, borderless, initGl),
		graphic_("gui.ver.glsl", "gui.fra.glsl"),
		defaultClosing_(false),
		currentPanel_(0) {

		// Default layout for Frame.
		addPanelBack();

		getCurrentPanel()->setLayout<BorderLayout>();

		// Initialization the OpenGL settings.
		resize(getWidth(), getHeight());

		getCurrentPanel()->setBackgroundColor(1, 1, 1);
		getCurrentPanel()->setSize((float) getWidth(), (float) getHeight());
		getCurrentPanel()->setPreferredSize((float) getWidth(), (float) getHeight());
	}

	int Frame::addPanelBack() {
		auto p = std::make_shared<Panel>();
		p->setLayout<BorderLayout>();
		return pushBackPanel(p);
	}

	int Frame::pushBackPanel(const std::shared_ptr<Panel>& panel) {
		panel->setChildsParent();
		panels_.push_back(panel);
		return panels_.size() - 1;
	}

	std::shared_ptr<Component> Frame::addDefault(const std::shared_ptr<Component>& component) {
		getCurrentPanel()->add(DEFAULT_INDEX, component);
		return component;
	}

	std::shared_ptr<Component> Frame::add(int layoutIndex, const std::shared_ptr<Component>& component) {
		getCurrentPanel()->add(layoutIndex, component);
		return component;
	}

	std::shared_ptr<Component> Frame::addDefaultToGroup(const std::shared_ptr<Component>& component) {
		getCurrentPanel()->addToGroup(DEFAULT_INDEX, component);
		return component;
	}

	std::shared_ptr<Component> Frame::addToGroup(int layoutIndex, const std::shared_ptr<Component>& component) {
		getCurrentPanel()->addToGroup(layoutIndex, component);
		return component;
	}

	std::shared_ptr<LayoutManager> Frame::setLayout(const std::shared_ptr<LayoutManager>& layoutManager) {
		getCurrentPanel()->setLayout(layoutManager);
		return layoutManager;
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
			resize(getWidth(), getHeight());
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

	void Frame::resize(int width, int height) {
		graphic_.setProj(mw::getOrthoProjectionMatrix44<GLfloat>(0, (GLfloat) width, 0, (GLfloat) height));
		getCurrentPanel()->setPreferredSize((float) width, (float) height);
		getCurrentPanel()->setSize((float) width, (float) height);
		getCurrentPanel()->setLocation(0, 0);
		getCurrentPanel()->validate();
		glViewport(0, 0, width, height);
	}

	void Frame::update(double deltaTime) {
		getCurrentPanel()->drawFirst(*this, graphic_, deltaTime);

		// Perform non critical event updates.
		while (!eventQueue_.empty()) {
			SDL_Event sdlEvent = eventQueue_.front();
			eventQueue_.pop();

			sdlEventListener_(*this, sdlEvent);
			switch (sdlEvent.type) {
				case SDL_WINDOWEVENT:
					windowListener_(*this, sdlEvent);
					switch (sdlEvent.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
							resize(sdlEvent.window.data1, sdlEvent.window.data2);
							break;
						case SDL_WINDOWEVENT_LEAVE:
							getCurrentPanel()->mouseMotionLeave();
							break;
						case SDL_WINDOWEVENT_CLOSE:
							if (defaultClosing_) {
								quit();
							}
					}
					break;
				case SDL_MOUSEMOTION:
					// Reverse y-axis.
					sdlEvent.motion.yrel *= -1;
					sdlEvent.motion.y = getHeight() - sdlEvent.motion.y;
					getCurrentPanel()->handleMouse(sdlEvent);
					break;
				case SDL_MOUSEBUTTONDOWN:
					// Fall through!
				case SDL_MOUSEBUTTONUP:
					sdlEvent.button.y = getHeight() - sdlEvent.motion.y;
					getCurrentPanel()->handleMouse(sdlEvent);
					break;
				case SDL_TEXTINPUT:
					// Fall through.
				case SDL_TEXTEDITING:
					// Fall through.
				case SDL_KEYDOWN:
					// Fall through.
				case SDL_KEYUP:
					getCurrentPanel()->handleKeyboard(sdlEvent);
					switch (sdlEvent.key.keysym.sym) {
						case SDLK_ESCAPE:
							if (defaultClosing_) {
								quit();
							}
							break;
					}
					break;
			}
		}
		getCurrentPanel()->draw(graphic_, deltaTime);
		getCurrentPanel()->drawLast(*this, graphic_, deltaTime);
	}

	void Frame::eventUpdate(const SDL_Event& windowEvent) {
		eventQueue_.push(windowEvent);
	}

} // Namespace gui.
