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
		PT(PGButton) pgButton = new PGButton("MyButton");
		CardMaker cardMaker = CardMaker("cardMaker");
	public:
		button(PandaFramework& framework);
		~button();
	};
}