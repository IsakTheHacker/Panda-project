#pragma once

namespace game {

	// Item class
	class item {
		private:
		public:
			std::string configPath;
			unsigned int stackedItems;
			std::map<std::string, std::string> options;

			item(std::string configPath, unsigned int stackedItems = 1) {
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
	};

	item emptyItem("emptyItem", 0);

	// Inventory class
	class inventory {
		private:
			std::vector<item> items;
		public:
			unsigned int slots;
			unsigned int maximumStackSize; //Set to 0 for infinite

			inventory(const unsigned int slots = 33, unsigned int maximumStackSize = 256) {
				this->slots = slots;
				this->maximumStackSize = maximumStackSize;

				for (unsigned int i = 0; i < slots; i++) {
					items.push_back(emptyItem);
				}
			}

			//int addItem(unsigned int slot, std::string configPath, unsigned int stackedItems = 1) {
			//	std::ifstream file(configPath);
			//	if (!file) {
			//		errorOut("Specified a configPath for an item that doesn't exist!");
			//		return 1;
			//	}
	
			//	if (slot > slots) {
			//		errorOut("Tried to assign an item to a slot that doesn't exist!");
			//		return 1;
			//	} else if (items[slot].itemName != itemName) {
			//		//Another item already assigned at that slot!
			//		return 1;
			//	}
			//	
			//	item item(itemName, texturePath, stackedItems);
			//	items[slot] = item;
			//	return 0;
			//}

			int remItem(unsigned int slot, unsigned int items = 1) {
				this->items[slot] = emptyItem;
				return 0;
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

	// Wind Object class
	class windObject {
		protected:
			bool shouldLogInConsoleIntern;
			bool shouldLogToFileIntern;
		public:
			NodePath model;
			static int current_id;
			static int object_quantity;
			unsigned int id;
			std::string name;
			LPoint3 direction;
			double velocity;

			windObject(WindowFramework*& window, PandaFramework& framework, double mx, double my, double mz, double velocity, PN_stdfloat sx = 1, PN_stdfloat sy = 1, PN_stdfloat sz = 1, bool shouldLogInConsole = false, bool shouldLogToFile = false) {
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

			~windObject() {
				object_quantity--;

				if (shouldLogInConsoleIntern) {
					game::logOut("Succesfully destroyed object: " + std::to_string(id));
				}
				if (shouldLogToFileIntern) {
					logToFile("game.log", "Log: Succesfully destroyed object: " + std::to_string(id));
				}
			}
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
			bool empty = false;
			NodePath model = NodePath("");
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

			object(WindowFramework*& window, PandaFramework& framework, std::vector<NodePath> subobjects, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false) {
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

			//Empty game::object constructor
			object(bool shouldLogInConsole = false, bool shouldLogToFile = false) {
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

	//Chunk class
	class chunk {
	public:
		int x;
		int y;
		std::vector<object> objects;

		chunk(std::vector<object> objects, int x, int y) {
			this->objects = objects;
			this->x = x;
			this->y = y;
		}
	};

	// Initialize static members of object class
	int object::current_id = 0;
	int object::object_quantity = 0;

	// Initialize static members of windObject class
	int windObject::current_id = 0;
	int windObject::object_quantity = 0;

	//Creating vectors for the classes
	std::vector<object> objects;
	std::vector<windObject> winds;
	std::vector<chunk> chunks;
	std::vector<entity> entities;
	std::vector<player> players;

	/// <summary> Reads a chunk from the specified path. </summary>
	/// <param name="window">- The window object</param>
	/// <param name="framework">- The framework object</param>
	/// <param name="path">- The path where the chunk file is located</param>
	/// <param name="x">- The chunk's x-pos</param>
	/// <param name="y">- The chunk's y-pos</param>
	/// <returns> 0 if successful, nonzero if not! </returns>
	int readChunk(WindowFramework*& window, PandaFramework& framework, std::string path, int x, int y) {
		
		//Initalize variables
		std::ifstream file(path);
		std::string line;
		int x_level = x;
		int y_level = y;
		int z_level;
		std::vector<std::string> block_list;
		std::vector<std::string> block_attributes;
		std::vector<std::string> attribute_array;
		std::string block_model;
		std::string block_texture;
		std::map<std::string, std::string> placeholder = {
			{"model",""},
			{"texture",""},
			{"texture-scale",""}
		};
		NodePath object = NodePath("");
		std::vector<game::object> blocks;

		while (std::getline(file, line)) {
			if (line.find("z") != std::string::npos) {
				findReplaceFirst(line, "z", "");
				z_level = std::stoi(line);
				x_level = x;
			} else {
				x_level += 2;
				y_level = y;
				block_list = split(line, ",");

				for (std::string block : block_list) {
					y_level += 2;
					if (block == "{empty}") {
						continue;
					}
					findReplaceFirst(block, "{", "");
					findReplaceFirst(block, "}", "");
					block_attributes = split(block, "|");
					for (std::string attribute : block_attributes) {
						attribute_array = split(attribute, ":");
						if (placeholder.find(attribute_array[0]) != placeholder.end()) {
							placeholder[attribute_array[0]] = attribute_array[1];
						}
					}
					TexturePool* texturePool = TexturePool::get_global_ptr();
					TextureStage* textureStage = new TextureStage("textureStage2");
					textureStage->set_sort(0);
					textureStage->set_mode(TextureStage::M_replace);
					std::vector<NodePath> subobjects;

					NodePath object2 = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/" + (std::string)"block.egg");
					/*if (placeholder["texture"] != "") {
						game::setTexture(object, placeholder["texture"]);
					}
					if (placeholder["texture-scale"] != "") {
						game::setTextureScale(object, std::stoi(placeholder["texture-scale"]));
					}*/
					/*object.clear_texture();*/

					Texture* texture = texturePool->load_cube_map(gamePath + (std::string)"models/textures/png/grass-#.png");
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);

					object2.set_texture(texture, 1);

					subobjects.push_back(object2);

					game::object object = game::object(window, framework, subobjects, true, false);
					object.model.set_pos(x_level, y_level, z_level);

					object.model.set_tex_gen(textureStage->get_default(), RenderAttrib::M_world_position);
					object.model.set_tex_projector(textureStage->get_default(), window->get_render(), object.model);
					object.model.set_tag("chunk", std::to_string(game::chunks.size()));
					object.model.set_tag("id", std::to_string(object.id));
					object.model.set_tag("chunkObjectId", std::to_string(blocks.size()));

					blocks.push_back(object);
				}
			}
		}
		game::chunk chunk(blocks, x, y);
		game::chunks.push_back(chunk);

		file.close();
		return 0;
	}

	

	/// <summary> Saves a specified chunk to it's destination. </summary>
	/// <param name="chunk">- your specifed chunk object</param>
	/// <returns> 0 if successful, nonzero if not! </returns>
	int saveChunk(game::chunk chunk) {
		int x = chunk.x;
		int y = chunk.y;
		std::string path = std::to_string(x) + "." + std::to_string(y) + ".chunk";

		std::set<int> z;
		for (game::object object : chunk.objects) {
			z.insert(object.model.get_z());
		}

		game::object emptyObject = {};
		std::vector<game::object> y_levels(8, emptyObject);
		std::vector<std::vector<game::object>> x_levels(8, y_levels);
		std::map<int, std::vector<std::vector<game::object>>> z_levels;

		for (auto i : z) {
			z_levels[i] = x_levels;
		}

		for (game::object object : chunk.objects) {
			//std::cout << object.model.get_pos() << "  -  " << (object.model.get_x()-x)/2-1 << " "<< (object.model.get_y()-y)/2-1 <<  " " <<  object.model.get_z() << std::endl;
			if (!object.empty) {
				z_levels[object.model.get_z()][(object.model.get_x()-x)/2-1][(object.model.get_y()-y)/2-1] = object;
			}
		}

		bool x_level_empty = false;
		bool y_level_empty = false;
		bool z_level_empty = false;
		bool shouldBreak = false;
		std::string final;
		for (std::pair<const int, std::vector<std::vector<game::object>>> pair : z_levels) {
			std::vector<std::vector<game::object>> z_level = pair.second;
			shouldBreak = false;
			for (std::vector<game::object> x_level : z_level) {
				for (game::object y_level : x_level) {
					if (!y_level.empty) {
						final.append("z" + std::to_string(static_cast<int>(y_level.model.get_z())) + "\n");
						shouldBreak = true;
						break;
					}
				}
				if (shouldBreak) {
					break;
				}
			}
			for (std::vector<game::object> x_level : z_level) {
				for (game::object y_level : x_level) {
					if (!y_level.empty) {
						final.append("{model:block.egg},");
					} else {
						final.append("{empty},");
						y_level_empty = true;
					}
				}
				//if (!y_level_empty) {
					final.append("\n");
				//} else {
					//x_level_empty = true;
				//}
			}
		}
		std::ofstream file("universes/Test/" + path);
		file << final;
		
		return 0;
	}
}