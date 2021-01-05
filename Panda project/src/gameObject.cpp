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
	object::object(std::string configPath, WindowFramework*& window, PandaFramework& framework, bool shouldLogInConsole, bool shouldLogToFile) {
		id = current_id;
		current_id++;
		object_quantity++;
		this->configPath = configPath;
		
		std::ifstream file(configPath);
		if (file.fail()) {
			errorOut("Specified a configPath that doesn't exist!");
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

			config[token] = line;
		}

		model = NodePath("model");

		initConfig(window, framework);

		model.reparent_to(window->get_render());

		//Setting internal class variables
		shouldLogInConsoleIntern = shouldLogInConsole;
		shouldLogToFileIntern = shouldLogToFile;
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
	void object::initConfig(WindowFramework*& window, PandaFramework& framework) {
		if (config.find("collidable") != config.end()) {
			if (std::stoi(config["collidable"]) == 1) {
				CollisionNode* collisionNode = new CollisionNode("Box");
				collisionNode->add_solid(new CollisionBox(0, std::stoi(config["collision-x"]), std::stoi(config["collision-y"]), std::stoi(config["collision-z"])));
				NodePath collisionNodePath = model.attach_new_node(collisionNode);
			}
		} else {
			config["collidable"] = "0";
		}

		if (config.find("subobjects") != config.end()) {
			std::vector<NodePath> subobjects;

			std::vector<std::string> stringSubobjects = game::split(config["subobjects"], ",");
			for (std::string stringSubobject : stringSubobjects) {

				std::vector<std::string> stringSubobjectOptions = game::split(stringSubobject, "|");
				std::map<std::string, std::string> subobjectOptions;
				Texture* texture;
				for (std::string stringSubobjectOption : stringSubobjectOptions) {
					subobjectOptions[game::split(stringSubobjectOption, ":")[0]] = game::split(stringSubobjectOption, ":")[1];
				}
				if (subobjectOptions.find("texture") != subobjectOptions.end()) {
					texture = TexturePool::get_global_ptr()->load_cube_map(subobjectOptions["texture"]);
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);
				} else {
					texture = TexturePool::get_global_ptr()->load_cube_map("models/textures/png/grass-#.png");
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);
				}

				NodePath subobject = window->load_model(framework.get_models(), subobjectOptions["model"]);
				if (subobjectOptions.find("texture") != subobjectOptions.end()) {
					subobject.set_texture(texture, 1);
				}
				subobjects.push_back(subobject);
			}

			//Reparent subobjects to model
			for (std::size_t i = 0; i < subobjects.size(); i++) {
				subobjects[i].reparent_to(model);
			}
		}
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