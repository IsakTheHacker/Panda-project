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

		button(PandaFramework& framework, EventHandler::EventCallbackFunction callbackFunction);
		~button();
	};
}