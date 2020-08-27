#pragma once

namespace game {
	class object {
		protected:
			std::string modelpathIntern;
			bool shouldLogInConsoleIntern;
			bool shouldLogToFileIntern;
			bool modelNotFound;
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
					game::logOut("Succesfully created object: " + std::to_string(id));
				}
				if (shouldLogToFile) {
					logToFile("game.log", "Log: Succesfully created object: " + std::to_string(id));
				}

				std::ifstream modelFile(modelpath);
				if (modelFile.fail()) {
					modelNotFound = true;
					game::warningOut("Could not find model for object: " + std::to_string(id) + " with modelpath: " + modelpath);
					logToFile("game.log", "Warning: Could not find model for object: " + std::to_string(id) + " with modelpath: " + modelpath);
				}
			}

			~object() {
				if (shouldLogInConsoleIntern) {
					game::logOut("Succesfully destroyed object: " + std::to_string(id));
				}
				if (shouldLogToFileIntern) {
					logToFile("game.log", "Log: Succesfully destroyed object: " + std::to_string(id));
				}
			}
	};

	class player : public object {
		public:
			player(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool shouldLogInConsole = true, bool shouldLogToFile = false) : object{ window, framework, modelpath, shouldLogInConsole, shouldLogToFile } {
				/*if (shouldLogInConsole) {
					game::logOut("Succesfully created the player! id: " + std::to_string(id));
				}
				if (shouldLogToFile) {
					logToFile("game.log", "Log: Succesfully created the player! id: " + std::to_string(id));
				}*/
			}

			~player() {
				/*if (shouldLogInConsoleIntern) {
					game::logOut("Succesfully destroyed the player! id: " + std::to_string(id));
				}
				if (shouldLogToFileIntern) {
					logToFile("game.log", "Log: Succesfully destroyed the player! id: " + std::to_string(id));
				}*/
			}
	};
}

// Initialize static member of object class
int game::object::current_id = 0;