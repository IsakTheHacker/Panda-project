#include "gameClasses.h"

namespace game {
	
	//Item class
	item::item(std::string configPath, unsigned int stackedItems) {
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
	windObject::windObject(WindowFramework*& window, PandaFramework& framework, double mx, double my, double mz, double velocity, PN_stdfloat sx, PN_stdfloat sy, PN_stdfloat sz, bool shouldLogInConsole, bool shouldLogToFile) {
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
	entity::entity(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool collidable, bool shouldLogInConsole, bool shouldLogToFile) : object{ window, framework, modelpath, collidable, shouldLogInConsole, shouldLogToFile } {
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
	player::player(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool shouldLogInConsole, bool shouldLogToFile) : entity{ window, framework, modelpath, shouldLogInConsole, shouldLogToFile } {
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

	//Chunk class
	chunk::chunk(std::vector<object> objects, int x, int y) {
		this->objects = objects;
		this->x = x;
		this->y = y;
	}
	chunk::chunk(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int chunk::reset() {
		chunk::objects.clear();
		return 0;
	}

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
				z_levels[object.model.get_z()][(object.model.get_x() - x) / 2 - 1][(object.model.get_y() - y) / 2 - 1] = object;
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

	int generateChunk(WindowFramework*& window, PandaFramework& framework, chunk chunk, PerlinNoise3 perlinNoise) {
		int x = chunk.x;
		int y = chunk.y;
		TexturePool* texturePool = TexturePool::get_global_ptr();
		std::vector<object> blocks;
		std::vector<NodePath> subobjects;
		game::object object;

		game::warningOut(object);

		for (size_t i = 1; i < 2; i++) {
			std::cout << "z: " << i << std::endl;
			for (int j = x; j < 16; j+=2) {
				//std::cout << "x: " << j << std::endl;
				for (int k = y; k < 16; k+=2) {
					//std::cout << "y: " << k << std::endl;
					TextureStage* textureStage = new TextureStage("textureStage2");
					textureStage->set_sort(0);
					textureStage->set_mode(TextureStage::M_replace);
					NodePath object2 = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/" + (std::string)"block.egg");
					subobjects.clear();

					Texture* texture = texturePool->load_cube_map(gamePath + (std::string)"models/textures/png/grass-#.png");
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);
					object2.set_texture(texture, 1);

					subobjects.push_back(object2);

					object = game::object(window, framework, subobjects, true, false);
					object.model.set_pos(j, k, i);
					//std::cout << object.model.get_pos() << std::endl;

					object.model.set_tex_gen(textureStage->get_default(), RenderAttrib::M_world_position);
					object.model.set_tex_projector(textureStage->get_default(), window->get_render(), object.model);
					object.model.set_tag("chunk", std::to_string(game::chunks.size()));
					object.model.set_tag("id", std::to_string(object.id));
					object.model.set_tag("chunkObjectId", std::to_string(blocks.size()));

					blocks.push_back(object);
				}
			}
		}
		game::chunk newChunk(blocks, x, y);
		blocks.clear();
		std::cout << "Before size: " << game::chunks.size() << std::endl;
		game::chunks.push_back(newChunk);
		std::cout << "After size: " << game::chunks.size() << std::endl;
		game::importantInfoOut("Finsihed generating: " + std::to_string(x) + "   " + std::to_string(y));
		game::importantInfoOut(newChunk.objects.size());
		newChunk.reset();
		game::importantInfoOut("After reset: " + std::to_string(newChunk.objects.size()));
		return 0;
	}

	std::vector<windObject> winds;
	std::vector<chunk> chunks;
}