#include "frame.h"
#include "borderlayout.h"
#include "guishader.h"

#include <mw/font.h>
#include <mw/window.h>
#include <mw/exception.h>

#include <string>

namespace gui {

	Frame::Frame(int x, int y, int width, int height, bool resizeable,
		std::string title, std::string icon, bool borderless) :
		mw::Window(x, y, width, height, resizeable, title, icon, borderless),
#if MW_OPENGLES2
		guiShader_("gui.ver.glsl", "gui.fra.glsl"),
#endif // MW_OPENGLES2
		defaultClosing_(false),
		currentPanel_(0) {

		SDL_StartTextInput();

		// Default layout for Frame.
		pushBackPanel(std::make_shared<Panel>());

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
#if MW_OPENGLES2
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		p->guiShader_ = guiShader_;
#endif // MW_OPENGLES2
		p->setChildsParent();
		return pushBackPanel(p);
	}

	int Frame::pushBackPanel(const std::shared_ptr<Panel>& panel) {
		panels_.push_back(panel);
#if MW_OPENGLES2
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		panel->guiShader_ = guiShader_;
#endif // MW_OPENGLES2
		panel->setChildsParent();
		return panels_.size() - 1;
	}

	std::shared_ptr<Component> Frame::addDefault(const std::shared_ptr<Component>& component) {
		getCurrentPanel()->add(DEFAULT_INDEX, component);
#if MW_OPENGLES2
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		component->guiShader_ = guiShader_;
#endif // MW_OPENGLES2
		component->setChildsParent();
		return component;
	}

	std::shared_ptr<Component> Frame::add(int layoutIndex, const std::shared_ptr<Component>& component) {
		getCurrentPanel()->add(layoutIndex, component);
#if MW_OPENGLES2
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		component->guiShader_ = guiShader_;
#endif // MW_OPENGLES2
		component->setChildsParent();
		return component;
	}

	std::shared_ptr<Component> Frame::addDefaultToGroup(const std::shared_ptr<Component>& component) {
		getCurrentPanel()->addToGroup(DEFAULT_INDEX, component);
#if MW_OPENGLES2
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		component->guiShader_ = guiShader_;
#endif // MW_OPENGLES2
		component->setChildsParent();
		return component;
	}

	std::shared_ptr<Component> Frame::addToGroup(int layoutIndex, const std::shared_ptr<Component>& component) {
		getCurrentPanel()->addToGroup(layoutIndex, component);
#if MW_OPENGLES2
        // Must be called before setChildsParent() in order to give
        // all components the shader.
		component->guiShader_ = guiShader_;
#endif // MW_OPENGLES2
		component->setChildsParent();
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
#if MW_OPENGLES2
		guiShader_.glUseProgram();
		mw::glViewport(0, 0, width, height);
		guiShader_.setGlProjU(Component::proj);
		guiShader_.setGlModelU(mw::I_44);
#else // MW_OPENGLES2
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, getWidth(), getHeight());
		glOrtho(0, getWidth(), 0, getHeight(), -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
#endif // MW_OPENGLES2
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
	}

	void Frame::eventUpdate(const SDL_Event& windowEvent) {
		eventQueue_.push(windowEvent);
	}

} // Namespace gui.
