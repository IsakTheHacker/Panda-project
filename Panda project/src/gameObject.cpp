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
	object::object(std::string configPath, WindowFramework*& window, PandaFramework& framework, bool shouldLogInConsole, bool shouldLogToFile, NodePath parent) {
		id = current_id;
		current_id++;
		object_quantity++;
		this->configPath = configPath;

		if (knownConfigs.find(configPath) != knownConfigs.end()) {
			config = knownConfigs[configPath];
		} else {
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
			knownConfigs[configPath] = config;		//Add to knownConfigs
		}

		model = NodePath("model");

		initConfig(window, framework);

		//model.reparent_to(rbcnp);
		if (parent.get_name() == "__unspecifiedParent__") {
			parent = window->get_render();
		}
		model.reparent_to(parent);

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
	object::operator NodePath() {
		return this->model;
	}
	void object::initConfig(WindowFramework*& window, PandaFramework& framework) {
		if (config.find("collidable") != config.end()) {
			if (std::stoi(config["collidable"]) == 1) {
				CollisionNode* collisionNode = new CollisionNode("Box");
				collisionNode->add_solid(new CollisionBox(0, std::stod(config["collision-x"]), std::stod(config["collision-y"]), std::stod(config["collision-z"])));
				this->collisionNodePath = model.attach_new_node(collisionNode);
			}
		} else {
			config["collidable"] = "0";
		}

		if (config.find("hp") != config.end()) {
			this->hp = std::stod(config["hp"]);
		}

		if (config.find("transparency") != config.end()) {
			model.set_transparency(TransparencyAttrib::M_alpha);
			model.set_alpha_scale(std::stod(config["transparency"]));
		}

		if (config.find("plights") != config.end()) {
			PT(PointLight) plight = new PointLight("plight");
			plight->set_attenuation(LVecBase3(0, 0, 0.01));
			NodePath plnp = model.attach_new_node(plight);
			window->get_render().set_light(plnp);
			lights.push_back(plnp);
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
				} else {
					texture = TexturePool::get_global_ptr()->load_cube_map("models/textures/png/grass-#.png");
				}

				texture->set_minfilter(SamplerState::FilterType::FT_nearest);
				texture->set_magfilter(SamplerState::FilterType::FT_nearest);
				NodePath subobject = window->load_model(framework.get_models(), subobjectOptions["model"]);
				if (subobjectOptions.find("texture") != subobjectOptions.end()) {
					subobject.get_child(0).set_texture(texture, 1);
				}
				subobjects.push_back(subobject);
			}

			//Reparent subobjects to model
			for (std::size_t i = 0; i < subobjects.size(); i++) {
				subobjects[i].reparent_to(model);
			}

			//Texture settings
			model.set_tex_gen(TextureStage::get_default(), RenderAttrib::M_world_position);
			model.set_tex_projector(TextureStage::get_default(), window->get_render(), model);
		}
	}
	int object::current_id = 0;
	int object::object_quantity = 0;
	std::map<std::string, std::map<std::string, std::string>> object::knownConfigs;

	//Entity class
	entity::entity(bool shouldLogInConsole, bool shouldLogToFile) : object { shouldLogInConsole, shouldLogToFile } {

	}
	entity::entity(std::string configPath, WindowFramework*& window, PandaFramework& framework, bool shouldLogInConsole, bool shouldLogToFile) : object { configPath, window, framework, shouldLogInConsole, shouldLogToFile } {
		
	}
	entity::~entity() {
		
	}
	void entity::update() {
		this->model.set_x(this->model, 0.01);
	}

	//Player class
	Player::Player(bool shouldLogInConsole, bool shouldLogToFile) : entity { shouldLogInConsole, shouldLogToFile } {
		this->onGround = false;
		this->sneaking = false;
		this->flying = false;
	}
	Player::Player(std::string configPath, WindowFramework*& window, PandaFramework& framework, bool shouldLogInConsole, bool shouldLogToFile) : entity { configPath, window, framework, shouldLogInConsole, shouldLogToFile } {
		this->onGround = false;
		this->sneaking = false;
		this->flying = false;
		this->playerName = (*Player::options)["player-name"];

		camera_x_speed = std::stof((*Player::options)["camera_x_speed"]);
		camera_y_speed = std::stof((*Player::options)["camera_y_speed"]);

		//Add first person camera
		firstPerson = window->make_camera();
		firstPerson.node()->set_name("firstPerson");
		firstPerson.reparent_to(model);

		//Add third person camera
		thirdPerson = window->make_camera();
		thirdPerson.node()->set_name("thirdPerson");
		thirdPerson.set_pos(thirdPerson, 0, -15, 7.5);
		thirdPerson.reparent_to(model);

		//Picker Ray
		pickerHandler = new CollisionHandlerQueue();								//Create Handler
		PT(CollisionNode) pickerNode = new CollisionNode("mouseRay");				//Create CollisionNode
		PT(CollisionRay) pickerRay = new CollisionRay();							//Create CollisionRay
		NodePath pickerNP = firstPerson.attach_new_node(pickerNode);			//Create NodePath for the attached new node
		pickerNode->set_from_collide_mask(GeomNode::get_default_collide_mask());	//Set from collide mask to use
		pickerNode->add_solid(pickerRay);											//Add solid to CollisionNode
		pickerNode->set_into_collide_mask(0);										//Disable into-collisions
		pickerTraverser.add_collider(pickerNP, pickerHandler);						//Add collider to traverser
		pickerRay->set_from_lens(window->get_camera(0), 0, 0);						//Adjust pickerRay with set_from_lens method
	}
	Player::~Player() {

	}
	void Player::doCameraControl(WindowFramework* window) {
		if (window->get_display_region_3d()->get_camera() == thirdPerson) {
			thirdPerson.look_at(model);
		} else if (window->get_display_region_3d()->get_camera() == firstPerson) {
			if (mouseInGame && window->get_graphics_window()) {
				if (window->get_graphics_window()->get_pointer(0).get_in_window()) {
					center_x = window->get_graphics_window()->get_x_size() / static_cast<double>(2);
					center_y = window->get_graphics_window()->get_y_size() / static_cast<double>(2);

					double move_x = std::floor(center_x - window->get_graphics_window()->get_pointer(0).get_x());
					double move_y = std::floor(center_y - window->get_graphics_window()->get_pointer(0).get_y());

					offset_h += move_x / camera_x_speed;
					player.model.set_h(std::fmod(offset_h, 360));

					offset_p += move_y / camera_y_speed;

					if (offset_p < 90 && offset_p > -90) {
						player.firstPerson.set_p(offset_p);
					} else {
						offset_p -= move_y / 5;
					}
					window->get_graphics_window()->move_pointer(0, center_x, center_y);		//Reset pointer to 0, 0
				}
			}
		} else {
			game::errorOut("Camera error!");
		}
	}
	void Player::setThirdPersonCamera(WindowFramework* window) {
		window->get_display_region_3d()->set_camera(thirdPerson);
		game::logOut("Switched to third person style camera.");
	}
	void Player::setFirstPersonCamera(WindowFramework* window) {
		window->get_display_region_3d()->set_camera(firstPerson);
		game::logOut("Switched to first person camera!");
	}
	std::map<std::string, std::string>* Player::options;


	void testIfPlayerOnGround(const Event* theEvent, void* data) {
		bool in_out_pattern = (bool)data;

		TypedWritableReferenceCount* value = theEvent->get_parameter(0).get_ptr();
		PT(CollisionEntry) entry = DCAST(CollisionEntry, value);
		nassertv(entry != NULL);

		if (!in_out_pattern) {
			if (std::round(entry->get_into_node_path().get_parent().get_z()) <= std::round(entry->get_from_node_path().get_parent().get_z())) {
				player.onGround = true;
				player.flying = false;
			} else {
				player.onGround = false;
			}
		} else {
			player.onGround = false;
		}
	}
	void getCollidedNodePath(const Event* theEvent, void* data) {
		TypedWritableReferenceCount* value = theEvent->get_parameter(0).get_ptr();
		PT(CollisionEntry) entry = DCAST(CollisionEntry, value);
		nassertv(entry != NULL);

		if (player.onGround) {
			player.collidedNodePath = entry->get_into_node_path().get_parent();
		}
	}
}