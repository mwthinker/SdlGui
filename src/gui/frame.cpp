#include "frame.h"
#include "borderlayout.h"
#include "guishader.h"

#include <mw/font.h>
#include <mw/window.h>
#include <mw/exception.h>

#include <string>

namespace gui {

	Frame::Frame(const int majorGlVersion, const int minorGlVersion, const bool glProfileEs,
		int x, int y, int width, int height, bool resizeable,
		std::string title, std::string icon, bool borderless) :
		mw::Window(majorGlVersion, minorGlVersion, glProfileEs, x, y, width, height, resizeable, title, icon, borderless),
		guiShader_("gui.ver.glsl", "gui.fra.glsl"),
		defaultClosing_(false),
		currentPanel_(0) {

		SDL_StartTextInput();

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
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		p->guiShader_ = guiShader_;
		return pushBackPanel(p);
	}

	int Frame::pushBackPanel(const std::shared_ptr<Panel>& panel) {
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		panel->guiShader_ = guiShader_;
        panel->setChildsParent();
		panels_.push_back(panel);
		return panels_.size() - 1;
	}

	std::shared_ptr<Component> Frame::addDefault(const std::shared_ptr<Component>& component) {
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		component->guiShader_ = guiShader_;
		getCurrentPanel()->add(DEFAULT_INDEX, component);
		return component;
	}

	std::shared_ptr<Component> Frame::add(int layoutIndex, const std::shared_ptr<Component>& component) {
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		component->guiShader_ = guiShader_;
		getCurrentPanel()->add(layoutIndex, component);
		return component;
	}

	std::shared_ptr<Component> Frame::addDefaultToGroup(const std::shared_ptr<Component>& component) {
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		component->guiShader_ = guiShader_;
		getCurrentPanel()->addToGroup(DEFAULT_INDEX, component);
		return component;
	}

	std::shared_ptr<Component> Frame::addToGroup(int layoutIndex, const std::shared_ptr<Component>& component) {
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		component->guiShader_ = guiShader_;
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
		mw::ortho2D(Component::proj, 0, (float) width, 0, (float) height);
		getCurrentPanel()->setPreferredSize((float) width, (float) height);
		getCurrentPanel()->setSize((float) width, (float) height);
		getCurrentPanel()->setLocation(0, 0);
		getCurrentPanel()->validate();
		guiShader_.glUseProgram();
		glViewport(0, 0, width, height);
		guiShader_.setGlProjU(Component::proj);
		guiShader_.setGlModelU(mw::I_44);
	}

	void Frame::update(Uint32 deltaTime) {
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		getCurrentPanel()->drawFirst(*this, deltaTime);

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
		getCurrentPanel()->draw(deltaTime);
		getCurrentPanel()->drawLast(*this, deltaTime);
	}

	void Frame::eventUpdate(const SDL_Event& windowEvent) {
		eventQueue_.push(windowEvent);
	}

} // Namespace gui.
