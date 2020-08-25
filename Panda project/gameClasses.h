#pragma once

namespace game {
	class object {
		protected:
			int x = 0;
			int y = 0;
			std::string modelpathIntern;
			bool shouldLogInConsoleIntern;
			bool shouldLogToFileIntern;
		public:
			NodePath model;
			static int current_id;
			unsigned int id;

			object(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool shouldLogInConsole = true, bool shouldLogToFile = false) {
				id = current_id;
				current_id++;

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

	class player : public object {
		public:
			player(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool shouldLogInConsole = true, bool shouldLogToFile = false) : object{ window, framework, modelpath, shouldLogInConsole, shouldLogToFile } {
				model = window->load_model(framework.get_models(), modelpath);
				model.reparent_to(window->get_render());

				//Setting internal class variables
				modelpathIntern = modelpath;
				shouldLogInConsoleIntern = shouldLogInConsole;
				shouldLogToFileIntern = shouldLogToFile;

				if (shouldLogInConsole) {
					game::logOut("Succesfully created the player! Modelpath: " + modelpath);
				}
				if (shouldLogToFile) {
					logToFile("game.log", "Log: Succesfully created the player! Modelpath: " + modelpath);
				}
			}

			~player() {
				if (shouldLogInConsoleIntern) {
					game::logOut("Succesfully destroyed the player! MOdelpath: " + modelpathIntern);
				}
				if (shouldLogToFileIntern) {
					logToFile("game.log", "Log: Succesfully destroyed the player! Modelpath: " + modelpathIntern);
				}
			}
	};
}

// Initialize static member of object class
int game::object::current_id = 0;