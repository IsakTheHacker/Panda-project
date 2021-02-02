#include "gameGui.h"

namespace game {
	button::button(PandaFramework& framework, EventHandler::EventCallbackFunction callbackFunction{

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
		game::setTexture(rollNodePath, gamePath + "models/textures/png/regular-button.png");
		rollNodePath.set_transparency(TransparencyAttrib::M_binary);

		NodePath inactNodePath(inactCard);
		game::setTexture(inactNodePath, gamePath + "models/textures/png/regular-button.png");
		inactNodePath.set_transparency(TransparencyAttrib::M_binary);


		pgButton->setup(readyNodePath, pressNodePath, rollNodePath, inactNodePath);
		framework.define_key(pgButton->get_click_event(MouseButton::one()), "button press", callbackFunction, pgButton);

		NodePath defbutNP = window->get_aspect_2d().attach_new_node(pgButton);
		defbutNP.set_sx(0.64);
		defbutNP.set_sz(0.16);
		defbutNP.set_pos(0 - defbutNP.get_sx() / 2, 0, 0 - defbutNP.get_sz() / 2);

		//Set up text for the button
		PT(TextNode) textNode = new TextNode("textNode");
		textNode->set_text("Quit and save");
		NodePath textNP = window->get_aspect_2d().attach_new_node(textNode);
		textNP.set_scale(0.07);
		textNP.set_pos((0 - textNode->get_width() * 0.07) / 2, 0, (0 - textNode->get_height() * 0.07 * 0.64) / 2);
	}

	button::~button() {

	}
}