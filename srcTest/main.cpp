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

#include <cassert>
#include <iostream>

void testBorderLayout() {
	gui::Frame frame(-1, -1, 512, 512);
	frame.setDefaultClosing(true);
	frame.add(std::make_shared<gui::Button>(), gui::BorderLayout::NORTH);
	
	std::shared_ptr<gui::Panel> panel = std::make_shared<gui::Panel>();
	panel->setLayout(std::make_shared<gui::VerticalLayout>());
	frame.add(panel, gui::BorderLayout::CENTER);
	frame.add(std::make_shared<gui::Button>(), gui::BorderLayout::WEST);
	frame.add(std::make_shared<gui::Button>(), gui::BorderLayout::EAST);
	frame.add(std::make_shared<gui::Button>(), gui::BorderLayout::SOUTH);
	
	panel->addToGroup(std::make_shared<gui::Button>());
	panel->addToGroup(std::make_shared<gui::Button>());
	panel->addToGroup(std::make_shared<gui::Button>());
	panel->addToGroup(std::make_shared<gui::Button>());
	frame.startLoop();
}

void testFlowLayout() {
	gui::Frame frame(-1, -1, 512, 512);
	frame.setDefaultClosing(true);
	mw::Font font("Ubuntu-B.ttf", 16);
	frame.setLayout(std::make_shared<gui::FlowLayout>(gui::FlowLayout::LEFT));
	frame.add(std::make_shared<gui::Button>("Button 1", font));
	frame.add(std::make_shared<gui::Button>("Button 2", font));
	std::shared_ptr<gui::Button> button = std::make_shared<gui::Button>("Button 3", font);
	button->setVerticalAlignment(gui::Button::TOP);
	frame.add(button);
	frame.add(std::make_shared<gui::Button>("Button 4", font));
	frame.add(std::make_shared<gui::CheckBox>("CheckBox 5", font, mw::Sprite("box.png"), mw::Sprite("check.png")));
	std::shared_ptr<gui::TextField> field = std::make_shared<gui::TextField>("Text:", font);
	field->setGrabFocus(true);
	frame.add(field);
	frame.add(std::make_shared<gui::TextField>("", font));
	frame.add(std::make_shared<gui::Label>("JAjajaj", font));

	std::shared_ptr<gui::Panel> panel = std::make_shared<gui::Panel>();
	panel->setBackgroundColor(mw::Color(0, 1, 0));
	frame.add(panel);

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
