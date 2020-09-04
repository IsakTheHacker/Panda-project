#pragma once

namespace game {
	class object {
		protected:
			std::string modelpathIntern;
			std::string oldModelpath;
			bool shouldLogInConsoleIntern;
			bool shouldLogToFileIntern;
			bool modelNotFound;
		public:
			NodePath model;
			static int current_id;
			unsigned int id;
			std::string name;

			object(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool shouldLogInConsole = true, bool shouldLogToFile = false) {
				id = current_id;
				current_id++;

				std::string convertedModelpath = modelpath;
				findReplaceAll(convertedModelpath, "/c", "C:");
				findReplaceAll(convertedModelpath, "/", "\\");
				std::ifstream modelFile(convertedModelpath);
				if (modelFile.fail()) {
					modelNotFound = true;
					oldModelpath = modelpath;
					modelpath = "/c/dev/Panda project/Panda project/models/egg/blocky.egg";			//Changing modelpath to standard model!
				}

				model = window->load_model(framework.get_models(), modelpath);
				model.reparent_to(window->get_render());

				//Setting internal class variables
				modelpathIntern = modelpath;
				shouldLogInConsoleIntern = shouldLogInConsole;
				shouldLogToFileIntern = shouldLogToFile;

				if (shouldLogInConsole) {
					if (modelNotFound) {
						game::warningOut("Could not find model for object: " + std::to_string(id) + " with modelpath: " + oldModelpath + " - Standard model was used!");
					} else {
						game::logOut("Succesfully created object: " + std::to_string(id));
					}
				}
				if (shouldLogToFile) {
					if (modelNotFound) {
						logToFile("game.log", "Warning: Could not find model for object: " + std::to_string(id) + " with modelpath: " + oldModelpath + " - Standard model was used!");
					} else {
						logToFile("game.log", "Log: Succesfully created object: " + std::to_string(id));
					}
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

	//Creating vector for storing the object class
	std::vector<game::object> objects;
}

// Initialize static members of object class
int game::object::current_id = 0;