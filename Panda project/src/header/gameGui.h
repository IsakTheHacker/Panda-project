#pragma once

//Panda3D libraries
#include "pgButton.h"
#include "cardMaker.h"
#include "mouseButton.h"

#include "pandaIncludes.h"

//My libraries
#include "gameFunctions.h"

namespace game {
	class button {
	private:
		CardMaker cardMaker = CardMaker("cardMaker");
	public:
		PT(PGButton) pgButton = new PGButton("pgButton");
		NodePath pgButtonNP = NodePath("pgButtonNP");

		PT(TextNode) textNode = new TextNode("textNode");
		NodePath textNP = NodePath("textNP");

		button(PandaFramework& framework, EventHandler::EventCallbackFunction callbackFunction, PN_stdfloat x, PN_stdfloat y, PN_stdfloat textX, PN_stdfloat textY, std::string text);
		button(PandaFramework& framework, EventHandler::EventCallbackFunction callbackFunction, PN_stdfloat x, PN_stdfloat y, std::string text);
		~button();
	};
}