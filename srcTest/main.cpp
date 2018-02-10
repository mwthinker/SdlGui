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
#include <gui/traversalgroup.h>
#include <gui/progressbar.h>

#include <cassert>
#include <iostream>

void testBorderLayout() {
	gui::Frame frame;
	frame.setOpenGlVersion(2, 1);
	frame.setWindowSize(512, 512);
	frame.setDefaultClosing(true);
	mw::Font font("Ubuntu-B.ttf", 16);
	auto b = frame.add<gui::Button>(gui::BorderLayout::NORTH, "Loop delay", font);
	b->setTextColor(1, 0, 0);
	b->addActionListener([&](gui::Component&) {
		if (frame.getLoopSleepingTime() < 0) {
			frame.setLoopSleepingTime(10);
		} else {
			frame.setLoopSleepingTime(-1);
		}
	});
	auto panel = std::make_shared<gui::Panel>();
	panel->setLayout<gui::VerticalLayout>();
	frame.add(gui::BorderLayout::CENTER, panel);
	frame.add<gui::Button>(gui::BorderLayout::WEST);
	frame.add<gui::Button>(gui::BorderLayout::EAST);
	frame.add<gui::Button>(gui::BorderLayout::SOUTH);

	frame.addKeyListener([&](gui::Component& c, const SDL_Event& keyEvent) {
		switch (keyEvent.type) {
			case SDL_KEYUP:
				std::cout << "KeyUp";
				frame.getCurrentPanel()->setFocus(true);
				break;
		}
	});

	gui::TraversalGroup group;
	group.setVerticalArrows(true);

	group.add(panel->addDefault(std::make_shared<gui::Button>()));
	group.add(panel->addDefault(std::make_shared<gui::Button>()));

	auto comboBox = panel->addDefault<gui::ComboBox>(font, mw::Sprite("triangle.png"));
	group.add(comboBox);
	comboBox->addItem("Option 1");
	comboBox->addItem("Option 2");
	comboBox->addItem("Option 3");

	b = panel->addDefault<gui::Button>("Hello", font);
	group.add(b);
	b->addMouseListener([](gui::Component& c, const SDL_Event& mouseEvent) {
		switch (mouseEvent.type) {
			case SDL_MOUSEMOTION:
				std::cout << "Motion\n";
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (mouseEvent.button.button) {
					case SDL_BUTTON_LEFT:
						std::cout << "Hello pushed\n";
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (mouseEvent.button.button) {
					case SDL_BUTTON_LEFT:
						break;
				}
				break;
		}
	});
	group.add(panel->add<gui::Button>(gui::DEFAULT_INDEX));

	frame.addKeyListener([&](gui::Component&, const SDL_Event& sdlEvent) {
		group.handleKeyboard(sdlEvent);
	});

	panel->add<gui::ProgressBar>(gui::DEFAULT_INDEX);

	frame.startLoop();
}

void testFlowLayout() {
	gui::Frame frame;
	frame.setOpenGlVersion(3, 1);
	frame.setWindowSize(512, 512);

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
	field->setFocus(true);
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
