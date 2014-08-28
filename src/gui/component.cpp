#include "component.h"
#include "panel.h"

#if MW_OPENGLES2
#include <mw/matrix.h>
#endif // MW_OPENGLES2

namespace gui {

#if MW_OPENGLES2

	namespace {

		mw::Texture createWhiteTexture() {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			Uint32 rmask = 0xff000000;
			Uint32 gmask = 0x00ff0000;
			Uint32 bmask = 0x0000ff00;
			Uint32 amask = 0x000000ff;
#else
			Uint32 rmask = 0x000000ff;
			Uint32 gmask = 0x0000ff00;
			Uint32 bmask = 0x00ff0000;
			Uint32 amask = 0xff000000;
#endif
			SDL_Surface* surface = SDL_CreateRGBSurface(0, 2, 2, 32, rmask, gmask, bmask, amask);
			// Fill with white color.
			SDL_FillRect(surface, 0, SDL_MapRGB(surface->format, 255, 255, 255));
			// Texture takes ownership of surface.
			return mw::Texture(surface);
		}
		
	} // Anonymous namespace.

	const mw::Texture Component::WHITE_TEXTURE = createWhiteTexture();

#endif // MW_OPENGLES2

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

	void Component::setGrabFocus(bool grabFocus) {
		if (grabFocus_ != grabFocus) {
			// State is changed.
			grabFocus_ = grabFocus;

			if (grabFocus) {
				setFocus(true);
				if (parent_ != nullptr) {
					// This component is one more.
					++parent_->nbrChildGrabFocus_;
				}
			} else {
				// Have children that wants focus?
				if (nbrChildGrabFocus_ > 1) {
					// Has a parent?
					if (parent_ != nullptr) {
						// One component less.
						--parent_->nbrChildGrabFocus_;
					}
				}
			}
		}
	}

	bool Component::isGrabFocus() const {
		return grabFocus_;
	}

	void Component::setFocus(bool focus) {
		// Change?
		if (focus_ != focus) {
			if (focus || grabFocus_ || (!focus && nbrChildGrabFocus_ < 1)) {
				focus_ = focus;
				focusListener_(*this);
			}
		}
	}

	bool Component::hasFocus() const {
		return focus_;
	}

	std::shared_ptr<Panel> Component::getParent() const {
		return parent_;
	}

	void Component::draw(Uint32 deltaTime) {
		// Draw panel background.
		backgroundColor_.glColor4f();
		Dimension dim = getSize();
#if MW_OPENGLES2
		auto& sprite = background_;
		const auto& texture = background_.getTexture();
		mw::glEnable(GL_BLEND);
		mw::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mw::glEnable(GL_TEXTURE_2D);
		mw::Matrix44 m;
		background_.bind();
		// Centered square in ORIGO.
		GLfloat aVertices[] = {
			0, 0,
			dim.width_, 0,
			0, dim.height_,
			dim.width_, dim.height_};

		// Map the sprite out from the texture.
		GLfloat aTexCoord[] = {
			sprite.getX() / texture.getWidth(), sprite.getY() / texture.getHeight(),
			(sprite.getX() + sprite.getWidth()) / texture.getWidth(), sprite.getY() / texture.getHeight(),
			sprite.getX() / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight(),
			(sprite.getX() + sprite.getWidth()) / texture.getWidth(), (sprite.getY() + sprite.getHeight()) / texture.getHeight()};

		// Use the program object
		auto& shader = sprite.getShaderPtr();
		shader->glUseProgram();

		// Load the vertex data
		mw::glVertexAttribPointer(shader->getAttributeLocation(mw::SHADER_ATTRIBUTE_VEC4_POSITION), 2, GL_FLOAT, GL_FALSE, 0, aVertices);
		mw::glVertexAttribPointer(shader->getAttributeLocation(mw::SHADER_ATTRIBUTE_VEC2_TEXCOORD), 2, GL_FLOAT, GL_FALSE, 0, aTexCoord);
		mw::glEnableVertexAttribArray(shader->getAttributeLocation(mw::SHADER_ATTRIBUTE_VEC4_POSITION));
		mw::glEnableVertexAttribArray(shader->getAttributeLocation(mw::SHADER_ATTRIBUTE_VEC2_TEXCOORD));

		// Upload the attributes and draw the sprite.
		mw::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		mw::glDisable(GL_TEXTURE_2D);
		mw::glDisable(GL_BLEND);
		drawBorder();
#else // MW_OPENGLES2
		glPushMatrix();
		glScaled(dim.width_, dim.height_, 1);
		glTranslated(0.5, 0.5, 0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glEnd();
		glDisable(GL_BLEND);

		background_.draw();
		glPopMatrix();
		drawBorder();
#endif // MW_OPENGLES2
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

	Component::Component() : parent_(nullptr), layoutIndex_(0), visible_(true),
		focus_(false), grabFocus_(false), isAdded_(false) {
		borderColor_ = mw::Color(0, 0, 0);
		nbrChildGrabFocus_ = 0;
#if MW_OPENGLES2
		background_ = mw::Sprite(WHITE_TEXTURE);
#endif // MW_OPENGLES2
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

	void Component::drawBorder() {
#if MW_OPENGLES2
#else // MW_OPENGLES2
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		borderColor_.glColor4f();

		glBegin(GL_QUADS);

		// South.
		glVertex2d(0, 0);
		glVertex2d(dimension_.width_, 0);
		glVertex2d(dimension_.width_, 1);
		glVertex2d(0, 1);

		// East.
		glVertex2d(dimension_.width_ - 1, 0);
		glVertex2d(dimension_.width_, 0);
		glVertex2d(dimension_.width_, dimension_.height_);
		glVertex2d(dimension_.width_ - 1, dimension_.height_);

		// North.
		glVertex2d(0, dimension_.height_ - 1);
		glVertex2d(dimension_.width_ - 1, dimension_.height_ -1);
		glVertex2d(dimension_.width_ - 1, dimension_.height_);
		glVertex2d(0, dimension_.height_);

		// West.
		glVertex2d(0, 0);
		glVertex2d(1, 0);
		glVertex2d(1, dimension_.height_);
		glVertex2d(0, dimension_.height_);

		glEnd();
		glDisable(GL_BLEND);
#endif // MW_OPENGLES2
	}

} // Namespace gui.
