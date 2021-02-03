#include "gameGui.h"

namespace game {
	button::button(PandaFramework& framework, EventHandler::EventCallbackFunction callbackFunction, PN_stdfloat x, PN_stdfloat y, PN_stdfloat textX, PN_stdfloat textY) {

		WindowFramework* window = framework.get_window(0);

		PT(PandaNode) readyCard = cardMaker.generate();
		PT(PandaNode) pressCard = cardMaker.generate();
		PT(PandaNode) rollCard = cardMaker.generate();
		PT(PandaNode) inactCard = cardMaker.generate();

		NodePath readyNodePath(readyCard);
		game::setTexture(readyNodePath, gamePath + "models/textures/png/regular-button.png");
		readyNodePath.set_transparency(TransparencyAttrib::M_binary);

		NodePath pressNodePath(pressCard);
		game::setTexture(pressNodePath, gamePath + "models/textures/png/regular-button.png");
		pressNodePath.set_transparency(TransparencyAttrib::M_binary);

		NodePath rollNodePath(rollCard);
		game::setTexture(rollNodePath, gamePath + "models/textures/png/regular-button-hover.png");
		rollNodePath.set_transparency(TransparencyAttrib::M_binary);

		NodePath inactNodePath(inactCard);
		game::setTexture(inactNodePath, gamePath + "models/textures/png/regular-button.png");
		inactNodePath.set_transparency(TransparencyAttrib::M_binary);


		pgButton->setup(readyNodePath, pressNodePath, rollNodePath, inactNodePath);
		framework.define_key(pgButton->get_click_event(MouseButton::one()), "button press", callbackFunction, pgButton);

		pgButtonNP = window->get_aspect_2d().attach_new_node(pgButton);
		pgButtonNP.set_sx(0.64);
		pgButtonNP.set_sz(0.16);
		pgButtonNP.set_pos(x - pgButtonNP.get_sx() / 2, 0, y - pgButtonNP.get_sz() / 2);

		//Set up text for the button
		textNode->set_text("Quit and save");
		textNP = window->get_aspect_2d().attach_new_node(textNode);
		textNP.set_scale(0.07);
		textNP.set_pos((textX - textNode->get_width() * 0.07) / 2, 0, (textY - textNode->get_height() * 0.07 * 0.64) / 2);
	}
	button::button(PandaFramework& framework, EventHandler::EventCallbackFunction callbackFunction, PN_stdfloat x, PN_stdfloat y) {
		button(framework, callbackFunction, x, y, x, y);
	}

	button::~button() {

	}
}