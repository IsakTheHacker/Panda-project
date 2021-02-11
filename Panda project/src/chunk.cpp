#include "chunk.h"

namespace game {
	chunk::chunk(const std::vector<object>& objects, const int& x, const int& y) {
		this->x = x;
		this->y = y;
		this->objects = objects;
	}
	chunk::chunk(const int& x, const int& y) {
		this->x = x;
		this->y = y;
	}
	chunk::chunk(std::string path, const int& x, const int& y, WindowFramework*& window, PandaFramework& framework) {
		this->x = x;
		this->y = x;
		readChunk(window, framework, path, x, y);
	}
	int chunk::reset() {
		chunk::objects.clear();
		return 0;
	}
	int chunk::generateChunk(WindowFramework*& window, PandaFramework& framework, const PerlinNoise3& perlinNoise = chunk::perlinNoise) {
		int chunksize = std::stoi(universeOptions["chunksize"]);
		int start_x = this->x * chunksize;
		int start_y = this->y * chunksize;

		std::vector<object> blocks;
		std::vector<NodePath> subobjects;

		//game::warningOut(object);

		for (size_t i = 1; i < 2; i++) {
			for (int j = start_x; j < start_x + chunksize; j += 2) {
				for (int k = start_y; k < start_y + chunksize; k += 2) {

					double object_z = (int)(perlinNoise.noise(j, k, i) * 100 + 0.5);
					object_z = (double)object_z / 100;
					object_z = std::round(object_z * 50 / 2) * 2;

					game::object object("data/assets/blockproperties/grass.blockproperties", window, framework, false, false);
					if (object_z < 1) {
						object = game::object("data/assets/blockproperties/sand.blockproperties", window, framework, false, false);
						if (object_z < 0) {
							for (int u = object_z + 2; u < 1; u += 2) {
								game::object water("data/assets/blockproperties/water.blockproperties", window, framework, false, false);
								water.model.set_pos(j, k, u);

								water.model.set_tag("chunk", std::to_string(this->x) + "," + std::to_string(this->y));
								water.model.set_tag("id", std::to_string(water.id));
								water.model.set_tag("chunkObjectId", std::to_string(blocks.size()));

								blocks.push_back(water);
							}
						}
					} else {
						//Tree generating
						if (rand() % 50 == 49) {
							for (size_t i = 2; i < 10; i += 2) {		//Trunk
								game::object object("data/assets/blockproperties/log.blockproperties", window, framework, false, false);
								object.model.set_pos(j, k, object_z + i);

								object.model.set_tag("chunk", std::to_string(this->x) + "," + std::to_string(this->y));
								object.model.set_tag("id", std::to_string(object.id));
								object.model.set_tag("chunkObjectId", std::to_string(blocks.size()));

								blocks.push_back(object);
							}

							//Leaves
							for (int x = -2; x < 4; x += 2) {		//Trunk
								for (int y = -2; y < 4; y += 2) {
									game::object object("data/assets/blockproperties/stone.blockproperties", window, framework, false, false);
									object.model.set_pos(j + x, k + y, object_z + 10);

									object.model.set_tag("chunk", std::to_string(this->x) + "," + std::to_string(this->y));
									object.model.set_tag("id", std::to_string(object.id));
									object.model.set_tag("chunkObjectId", std::to_string(blocks.size()));

									blocks.push_back(object);
								}
							}
							for (int x = 0; x < 2; x += 2) {		//Trunk
								for (int y = 0; y < 2; y += 2) {
									game::object object("data/assets/blockproperties/stone.blockproperties", window, framework, false, false);
									object.model.set_pos(j + x, k + y, object_z + 12);

									object.model.set_tag("chunk", std::to_string(this->x) + "," + std::to_string(this->y));
									object.model.set_tag("id", std::to_string(object.id));
									object.model.set_tag("chunkObjectId", std::to_string(blocks.size()));

									blocks.push_back(object);
								}
							}
						}
					}

					object.model.set_pos(j, k, object_z);

					object.model.set_tag("chunk", std::to_string(this->x) + "," + std::to_string(this->y));
					object.model.set_tag("id", std::to_string(object.id));
					object.model.set_tag("chunkObjectId", std::to_string(blocks.size()));

					blocks.push_back(object);
				}
			}
		}
		this->objects = blocks;															//Push the generated blocks to vector objects of this chunk
		this->loaded_chunks.insert(std::pair<int, int>(x, y));							//Register that this chunk has been generated
		/*Texture* texture = TexturePool::get_global_ptr()->load_cube_map("models/textures/png/grass-#.png");
		texture->set_minfilter(SamplerState::FilterType::FT_nearest);
		texture->set_magfilter(SamplerState::FilterType::FT_nearest);
		oneMesh.get_child(0).get_child(1).get_child(0).set_texture(texture);
		oneMesh.flatten_strong();
		oneMesh.reparent_to(window->get_render());*/
		if (devMode) {
			std::string fancyDebugOutput =
				"Finished generating chunk:\n"
				"    XY: " + std::to_string(start_x) + ", " + std::to_string(start_y) + "\n"
				"    Chunk Objects Size: " + std::to_string(this->objects.size());
			;
			game::logOut(fancyDebugOutput);
		}
		if (std::stoi((*this->options)["save_newly_created_chunks"])) {
			this->saveChunk();		//Save chunk
		}
		//DCAST(RigidBodyCombiner, rbcnp.node())->collect();
		return 0;
	}
	int chunk::saveChunk() const {
		int chunksize = std::stoi(universeOptions["chunksize"]);
		int start_x = this->x * chunksize;
		int start_y = this->y * chunksize;

		std::string path = std::to_string(this->x) + "." + std::to_string(this->y) + ".chunk";

		std::set<int> z;
		for (game::object object : this->objects) {
			z.insert(object.model.get_z());
		}

		game::object emptyObject = {};
		std::vector<game::object> y_levels(8, emptyObject);
		std::vector<std::vector<game::object>> x_levels(8, y_levels);
		std::map<int, std::vector<std::vector<game::object>>> z_levels;

		for (int i : z) {
			z_levels[i] = x_levels;
		}

		for (game::object object : this->objects) {
			//std::cout << object.model.get_pos() << "  -  " << (object.model.get_x()-x)/2-1 << " "<< (object.model.get_y()-y)/2-1 <<  " " <<  object.model.get_z() << std::endl;
			if (!object.empty) {
				int x_value = parsePositive((object.model.get_x() - start_x) / 2);
				//int x_value = ((parsePositive(object.model.get_x()) - parsePositive(start_x)) / 2);
				int y_value = parsePositive((object.model.get_y() - start_y) / 2);
				//int y_value = ((parsePositive(object.model.get_y()) - parsePositive(start_y)) / 2);
				try {
					z_levels.at(object.model.get_z()).at(x_value).at(y_value) = object;
				} catch (const std::out_of_range& outOfRange) {
					game::errorOut(std::string("Could not save universe! Out of range error: ") + outOfRange.what());
					return 1;
				}
				//z_levels[object.model.get_z()][x + object.model.get_x() - 1][y + object.model.get_y() - 1] = object;
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
						final.append("{" + y_level.configPath.substr(y_level.configPath.find_last_of("/")+1, y_level.configPath.substr(y_level.configPath.find_last_of("/")+1).size() - std::string(".blockproperties").size()) + "},");
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
		std::ofstream file(universePath + path);
		file << final;

		return 0;
	}
	int chunk::readChunk(WindowFramework*& window, PandaFramework& framework, std::string path, int x, int y) {
		int chunksize = std::stoi(universeOptions["chunksize"]);

		if (game::chunk::loaded_chunks.find(std::pair<int, int>(x, y)) != game::chunk::loaded_chunks.end()) {
			return 1;																					//Chunk is already loaded
		}

		//Initalize variables
		std::ifstream file(path);

		if (file.fail()) {
			game::warningOut("Chunk index file specified a chunk which could not be found. Skipping...");
			file.close();
			return 1;
		}

		std::string line;
		int x_level = x * chunksize;
		int y_level = y * chunksize;
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
				x_level = x * chunksize;
			} else {
				x_level += 2;
				y_level = y * chunksize;
				block_list = split(line, ",");

				for (std::string block : block_list) {
					y_level += 2;
					if (block == "{empty}") {
						continue;
					}
					findReplaceFirst(block, "{", "");
					findReplaceFirst(block, "}", "");
					//block_attributes = split(block, "|");
					/*for (std::string attribute : block_attributes) {
						attribute_array = split(attribute, ":");
						if (placeholder.find(attribute_array[0]) != placeholder.end()) {
							placeholder[attribute_array[0]] = attribute_array[1];
						}
					}*/

					game::object object("data/assets/blockproperties/" + block + ".blockproperties", window, framework, false, false);
					object.model.set_pos(x_level-2, y_level-2, z_level);

					object.model.set_tag("chunk", std::to_string(x) + "," + std::to_string(y));
					object.model.set_tag("id", std::to_string(object.id));
					object.model.set_tag("chunkObjectId", std::to_string(blocks.size()));

					blocks.push_back(object);
				}
			}
		}
		this->objects = blocks;
		game::chunk::index[std::pair<int, int>(x, y)] = game::chunks.size();
		chunk::loaded_chunks.insert(std::pair<int, int>(x, y));

		file.close();
		return 0;
	}

	//Initalize static members
	std::set<std::pair<int, int>> chunk::loaded_chunks;
	std::map<std::pair<int, int>, int> chunk::index;
	std::map<std::string, std::string>* chunk::options;
	int chunk::chunksize;
	PerlinNoise3 chunk::perlinNoise;
	WindowFramework* chunk::window;
	PandaFramework* chunk::framework;

	//Creating vector for chunk class
	std::vector<chunk> chunks;
}