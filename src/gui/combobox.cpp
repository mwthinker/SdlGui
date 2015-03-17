#include "combobox.h"

namespace gui {

	ComboBox::ComboBox(const mw::Font& font) : font_(font) {
	}

	ComboBox::ComboBox(const std::vector<std::string>&  items, const mw::Font& font) : font_(font) {
		for (auto& text : items) {
			items_.emplace_back(text, font);
		}
	}

	void ComboBox::addItem(std::string item) {
		items_.emplace_back(item, font_);
	}

	void ComboBox::draw(Uint32 deltaTime) {
		// TODO!
	}

	void ComboBox::handleKeyboard(const SDL_Event& keyEvent) {
		// TODO!
	}

} // Namespace gui.
