#pragma once

namespace game {

	// Item class
	class item {
		private:
			unsigned int slots;
			unsigned int maximumStackSize; //Set to 0 for infinite
			unsigned int items;
		public:

			item(unsigned int slots = 30, unsigned int maximumStackSize = 256) {
				this->slots = slots;
				this->maximumStackSize = maximumStackSize;
			}
	};

	// Inventory class
	class inventory {
		private:
			unsigned int slots;
			unsigned int maximumStackSize; //Set to 0 for infinite
		public:

			inventory(unsigned int slots = 30, unsigned int maximumStackSize = 256) {
				this->slots = slots;
				this->maximumStackSize = maximumStackSize;
			}
	};

	//Class for pauseMenu event parameters
	class pauseMenuEventParameters {
		public:
			WindowFramework* window;
			MouseWatcher* mouseWatcher;
	};

	//Class for collisionRay event parameters
	class collisionRayEventParameters {
		public:
			WindowFramework* window;
			CollisionRay* pickerRay;
			MouseWatcher* mouseWatcher;
	};

	// Class for computer specs
	class computerSpecs {
		public:
			std::string platform;
	};

	// Object class
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
			static int object_quantity;
			unsigned int id;
			std::string name;

			object(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false) {
				id = current_id;
				current_id++;
				object_quantity++;

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

				if (collidable) {
					CollisionNode* collisionNode = new CollisionNode("Box");
					collisionNode->add_solid(new CollisionBox(0, 2, 2, 2));
					NodePath collisionNodePath = model.attach_new_node(collisionNode);
				}

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
				object_quantity--;

				if (shouldLogInConsoleIntern) {
					game::logOut("Succesfully destroyed object: " + std::to_string(id));
				}
				if (shouldLogToFileIntern) {
					logToFile("game.log", "Log: Succesfully destroyed object: " + std::to_string(id));
				}
			}
	};

	// Entity class
	class entity : public object {
		public:
			entity(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false) : object{ window, framework, modelpath, collidable, shouldLogInConsole, shouldLogToFile } {
				/*if (shouldLogInConsole) {
					game::logOut("Succesfully created the player! id: " + std::to_string(id));
				}
				if (shouldLogToFile) {
					logToFile("game.log", "Log: Succesfully created the player! id: " + std::to_string(id));
				}*/
			}

			~entity() {
				/*if (shouldLogInConsoleIntern) {
					game::logOut("Succesfully destroyed the player! id: " + std::to_string(id));
				}
				if (shouldLogToFileIntern) {
					logToFile("game.log", "Log: Succesfully destroyed the player! id: " + std::to_string(id));
				}*/
			}
	};

	// Player class
	class player : public entity {
		public:
			NodePath collisionNodePath;

			player(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool shouldLogInConsole = true, bool shouldLogToFile = false) : entity{ window, framework, modelpath, shouldLogInConsole, shouldLogToFile } {
				CollisionNode* collisionNode = new CollisionNode("Box");
				collisionNode->add_solid(new CollisionBox(0, 2, 2, 4));
				collisionNodePath = model.attach_new_node(collisionNode);
				collisionNodePath.show();

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

	//Creating vectors for the classes
	std::vector<object> objects;
	std::vector<entity> entities;
	std::vector<player> players;
}

// Initialize static members of object class
int game::object::current_id = 0;
int game::object::object_quantity = 0;