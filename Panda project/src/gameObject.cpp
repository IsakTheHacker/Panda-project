#include "gameObject.h"

namespace game {
	//Object class
	object::object(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool collidable, bool shouldLogInConsole, bool shouldLogToFile) {
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
			modelpath = gamePath + (std::string)"models/egg/blocky.egg";			//Changing modelpath to standard model!
		}

		model = window->load_model(framework.get_models(), modelpath);
		model.reparent_to(window->get_render());

		if (collidable) {
			CollisionNode* collisionNode = new CollisionNode("Box");
			collisionNode->add_solid(new CollisionBox(0, 1, 1, 1));
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
	object::object(WindowFramework*& window, PandaFramework& framework, std::vector<NodePath> subobjects, bool collidable, bool shouldLogInConsole, bool shouldLogToFile) {
		id = current_id;
		current_id++;
		object_quantity++;

		model = NodePath("model");
		for (std::size_t i = 0; i < subobjects.size(); i++) {
			subobjects[i].reparent_to(model);
		}
		model.reparent_to(window->get_render());

		if (collidable) {
			CollisionNode* collisionNode = new CollisionNode("Box");
			collisionNode->add_solid(new CollisionBox(0, 1, 1, 1));
			NodePath collisionNodePath = model.attach_new_node(collisionNode);
		}

		//Setting internal class variables
		shouldLogInConsoleIntern = shouldLogInConsole;
		shouldLogToFileIntern = shouldLogToFile;

		if (shouldLogInConsole) {
			game::logOut("Succesfully created object: " + std::to_string(id));
		}
		if (shouldLogToFile) {
			logToFile("game.log", "Log: Succesfully created object: " + std::to_string(id));
		}
	}
	object::object(bool shouldLogInConsole, bool shouldLogToFile) {
		id = current_id;
		current_id++;
		object_quantity++;
		empty = true;

		//Setting internal class variables
		shouldLogInConsoleIntern = shouldLogInConsole;
		shouldLogToFileIntern = shouldLogToFile;

		if (shouldLogInConsole) {
			game::logOut("Succesfully created empty object: " + std::to_string(id));
		}
		if (shouldLogToFile) {
			logToFile("game.log", "Log: Succesfully created empty object: " + std::to_string(id));
		}
	}
	object::~object() {
		object_quantity--;

		if (shouldLogInConsoleIntern) {
			game::logOut("Succesfully destroyed object: " + std::to_string(id));
		}
		if (shouldLogToFileIntern) {
			logToFile("game.log", "Log: Succesfully destroyed object: " + std::to_string(id));
		}
	}
	object::operator std::string() {
		std::string stringObject =
			"Stringobject of game::object:\n"
			"    id: " + std::to_string(object::id) + "\n"
			"    empty: " + std::to_string(object::empty) + "\n"
			"    shouldLogInConsole: " + std::to_string(object::shouldLogInConsoleIntern) + "\n"
			"    shouldLogToFile: " + std::to_string(object::shouldLogToFileIntern) + ""
			;
		return stringObject;
	}
	int object::current_id = 0;
	int object::object_quantity = 0;

	//Entity class
	entity::entity(WindowFramework*& window, PandaFramework& framework, const std::string& modelpath, bool collidable, bool shouldLogInConsole, bool shouldLogToFile) : object{ window, framework, modelpath, collidable, shouldLogInConsole, shouldLogToFile } {
		/*if (shouldLogInConsole) {
			game::logOut("Succesfully created the player! id: " + std::to_string(id));
		}
		if (shouldLogToFile) {
			logToFile("game.log", "Log: Succesfully created the player! id: " + std::to_string(id));
		}*/
	}

	entity::~entity() {
		/*if (shouldLogInConsoleIntern) {
			game::logOut("Succesfully destroyed the player! id: " + std::to_string(id));
		}
		if (shouldLogToFileIntern) {
			logToFile("game.log", "Log: Succesfully destroyed the player! id: " + std::to_string(id));
		}*/
	}

	//Player class
	player::player(WindowFramework*& window, PandaFramework& framework, const std::string& modelpath, bool shouldLogInConsole, bool shouldLogToFile) : entity{ window, framework, modelpath, shouldLogInConsole, shouldLogToFile } {
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

	player::~player() {
		/*if (shouldLogInConsoleIntern) {
			game::logOut("Succesfully destroyed the player! id: " + std::to_string(id));
		}
		if (shouldLogToFileIntern) {
			logToFile("game.log", "Log: Succesfully destroyed the player! id: " + std::to_string(id));
		}*/
	}
}