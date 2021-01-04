#include "gameClasses.h"

namespace game {
	
	//Item class
	item::item(const std::string& configPath, const unsigned int& stackedItems) {
		std::ifstream file(configPath);
		if (!file) {
			errorOut("Specified a configPath for an item that doesn't exist!");
		}
		std::string line;
		std::string delimiter = "=";
		while (std::getline(file, line)) {
			size_t pos = 0;
			std::string token;
			while ((pos = line.find(delimiter)) != std::string::npos) {
				token = line.substr(0, pos);
				line.erase(0, pos + delimiter.length());
			}

			options[token] = line;
		}
		this->configPath = configPath;
		this->stackedItems = stackedItems;
	}

	item emptyItem("emptyItem", 0);

	//Wind Object class
	windObject::windObject(WindowFramework*& window, PandaFramework& framework, const double& mx, const double& my, const double& mz, const double& velocity, const PN_stdfloat& sx, const PN_stdfloat& sy, const PN_stdfloat& sz, bool shouldLogInConsole, bool shouldLogToFile) {
		id = current_id;
		current_id++;
		object_quantity++;
		direction = LPoint3(mx, my, mz);
		this->velocity = velocity;

		model = NodePath("windObject");
		model.reparent_to(window->get_render());

		CollisionNode* collisionNode = new CollisionNode("Box");
		collisionNode->add_solid(new CollisionBox(0, sx, sy, sz));
		NodePath collisionNodePath = model.attach_new_node(collisionNode);

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
	windObject::~windObject() {
		object_quantity--;

		if (shouldLogInConsoleIntern) {
			game::logOut("Succesfully destroyed object: " + std::to_string(id));
		}
		if (shouldLogToFileIntern) {
			logToFile("game.log", "Log: Succesfully destroyed object: " + std::to_string(id));
		}
	}
	int windObject::current_id = 0;
	int windObject::object_quantity = 0;

	std::vector<windObject> winds;
}