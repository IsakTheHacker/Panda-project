#pragma once

namespace game {
	class object {
		private:
			int x = 0;
			int y = 0;
			std::string modelpathIntern;
			bool shouldLogInConsoleIntern;
			bool shouldLogToFileIntern;
		public:
			NodePath model;

			object(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool shouldLogInConsole = true, bool shouldLogToFile = false) {
				model = window->load_model(framework.get_models(), modelpath);
				model.reparent_to(window->get_render());

				//Setting internal class variables
				modelpathIntern = modelpath;
				shouldLogInConsoleIntern = shouldLogInConsole;
				shouldLogToFileIntern = shouldLogToFile;

				if (shouldLogInConsole) {
					game::logOut("Succesfully created an object with modelpath: " + modelpath);
				}
				if (shouldLogToFile) {
					logToFile("game.log", "Log: Succesfully created an object with modelpath: " + modelpath);
				}
			}

			~object() {
				if (shouldLogInConsoleIntern) {
					game::logOut("Succesfully destroyed an object with modelpath: " + modelpathIntern);
				}
				if (shouldLogToFileIntern) {
					logToFile("game.log", "Log: Succesfully destroyed an object with modelpath: " + modelpathIntern);
				}
			}
	};

	class key {
	public:
		const char* keyname;
		bool pressed;
		key(const char* x, bool y) {
			keyname = x;
			pressed = y;
		}
	};
}