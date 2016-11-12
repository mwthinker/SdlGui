#include <gui/frame.h>
#include <gui/flowlayout.h>
#include <gui/borderlayout.h>
#include <gui/verticallayout.h>
#include <gui/panel.h>
#include <gui/component.h>
#include <gui/label.h>
#include <gui/button.h>
#include <gui/textfield.h>
#include <gui/checkbox.h>
#include <gui/horizontallayout.h>
#include <gui/gridlayout.h>
#include <gui/combobox.h>

#include <cassert>
#include <iostream>

void testBorderLayout() {
	gui::Frame frame(-1, -1, 512, 512);
	frame.setDefaultClosing(true);
	mw::Font font("Ubuntu-B.ttf", 16);
	auto b = frame.add<gui::Button>(gui::BorderLayout::NORTH, "Hello", font);
	b->setTextColor(1,0,0);
	auto panel = std::make_shared<gui::Panel>();
	panel->setLayout<gui::VerticalLayout>();
	frame.add(gui::BorderLayout::CENTER, panel);
	frame.add<gui::Button>(gui::BorderLayout::WEST);
	frame.add<gui::Button>(gui::BorderLayout::EAST);
	frame.add<gui::Button>(gui::BorderLayout::SOUTH);

	panel->addDefaultToGroup(std::make_shared<gui::Button>());
	panel->addDefaultToGroup(std::make_shared<gui::Button>());

	auto comboBox = panel->addDefaultToGroup<gui::ComboBox>(font);
	comboBox->addItem("Option 1");
	comboBox->addItem("Option 2");
	comboBox->addItem("Option 3");
	
	panel->addDefaultToGroup<gui::Button>("Hello", font);
	panel->addToGroup<gui::Button>(gui::DEFAULT_INDEX);
	frame.startLoop();
}

void testFlowLayout() {
	gui::Frame frame(-1, -1, 512, 512);
	frame.setDefaultClosing(true);
	mw::Font font("Ubuntu-B.ttf", 16);
	frame.setLayout(std::make_shared<gui::FlowLayout>(gui::FlowLayout::LEFT));
	frame.addDefault<gui::Button>("Button 1", font);
	frame.addDefault(std::make_shared<gui::Button>("Button 2", font));
	auto button = frame.add<gui::Button>(gui::DEFAULT_INDEX, "Button 3", font);
	button->setVerticalAlignment(gui::Button::TOP);
	frame.add(gui::DEFAULT_INDEX, std::make_shared<gui::Button>("Button 4", font));
	frame.add(gui::DEFAULT_INDEX, std::make_shared<gui::CheckBox>("CheckBox 5", font, mw::Sprite("box.png"), mw::Sprite("check.png")));
	auto field = frame.addDefault<gui::TextField>("Text:", font);
	field->setGrabFocus(true);
	frame.add(gui::DEFAULT_INDEX, std::make_shared<gui::TextField>("", font));
	frame.add(gui::DEFAULT_INDEX, std::make_shared<gui::Label>("JAjajaj", font));

	auto panel = std::make_shared<gui::Panel>();
	panel->setBackgroundColor(0, 1, 0);
	frame.add(gui::DEFAULT_INDEX, panel);

	frame.addMouseListener([](gui::Component& c, const SDL_Event& sdlEvent) {
		switch (sdlEvent.type) {
			case SDL_MOUSEBUTTONDOWN:
				std::cout << "\n" << sdlEvent.button.x << " " << sdlEvent.button.y;
				break;
		}
	});
	frame.startLoop();
}

int main(int argc, char** argv) {
	testBorderLayout();
	testFlowLayout();
	return 0;
}
