#include "chunk.h"

namespace game {
	chunk::chunk(const std::vector<object>& objects, const int& x, const int& y) {
		this->objects = objects;
		this->x = x;
		this->y = y;
	}
	chunk::chunk(const int& x, const int& y) {
		this->x = x;
		this->y = y;
		chunk::index.insert(std::pair<int, int>(x, y));
	}
	int chunk::reset() {
		chunk::objects.clear();
		return 0;
	}
	void chunk::setDefaultPerlinNoise3(const PerlinNoise3 perlinNoise) {
		chunk::perlinNoise = perlinNoise;
	}
	PerlinNoise3 chunk::getDefaultPerlinNoise3() {
		return chunk::perlinNoise;
	}
	int chunk::generateChunk(WindowFramework*& window, PandaFramework& framework, const PerlinNoise3& perlinNoise = chunk::perlinNoise) {
		int start_x = this->x * 16;
		int start_y = this->y * 16;

		std::vector<object> blocks;
		std::vector<NodePath> subobjects;

		//game::warningOut(object);

		for (size_t i = 1; i < 2; i++) {
			for (int j = start_x; j < start_x + 16; j += 2) {
				//std::cout << "x: " << j << std::endl;
				for (int k = start_y; k < start_y + 16; k += 2) {
					//std::cout << "y: " << k << std::endl;
					TextureStage* textureStage = new TextureStage("textureStage2");
					textureStage->set_sort(0);
					textureStage->set_mode(TextureStage::M_replace);
					NodePath object2 = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/" + (std::string)"block.egg");
					subobjects.clear();

					Texture* texture = TexturePool::get_global_ptr()->load_cube_map(gamePath + (std::string)"models/textures/png/grass-#.png");
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);
					object2.set_texture(texture, 1);

					subobjects.push_back(object2);

					game::object object(window, framework, subobjects, true, false);
					double object_z = (int)(perlinNoise.noise(j, k, i) * 100 + 0.5);
					object_z = (double)object_z / 100;
					object.model.set_pos(j, k, std::round(object_z * 50 / 2) * 2);
					//std::cout << object.model.get_pos() << std::endl;
					//game::importantInfoOut(std::to_string(perlinNoise.noise(j, k, i)) + " -> " + std::to_string(object_z) + " -> " + std::to_string(std::round(object_z * 50 / 2) * 2));

					object.model.set_tex_gen(textureStage->get_default(), RenderAttrib::M_world_position);
					object.model.set_tex_projector(textureStage->get_default(), window->get_render(), object.model);
					object.model.set_tag("chunk", std::to_string(game::chunks.size()));
					object.model.set_tag("id", std::to_string(object.id));
					object.model.set_tag("chunkObjectId", std::to_string(blocks.size()));

					blocks.push_back(object);
				}
			}
		}
		this->objects = blocks;															//Push the generated blocks to vector objects of this chunk
		//this->index.insert(std::pair<int, int>(start_x, start_y));									//Register that this chunk has been generated
		//std::cout << "Before size: " << game::chunks.size() << std::endl;
		//std::cout << "After size: " << game::chunks.size() << std::endl;
		if (devMode) {
			std::string fancyDebugOutput =
				"Finished generating chunk:\n"
				"    XY: " + std::to_string(start_x) + ", " + std::to_string(start_y) + "\n"
				"    Chunk Objects Size: " + std::to_string(this->objects.size());
			;
			game::logOut(fancyDebugOutput);
		}
		//game::importantInfoOut("After reset: " + std::to_string(newChunk.objects.size()));
		return 0;
	}
	int chunk::saveChunk() const {
		int start_x = this->x * 16;
		int start_y = this->y * 16;

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
				z_levels[object.model.get_z()][x_value][y_value] = object;
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
		std::ofstream file(universePath + path);
		file << final;

		return 0;
	}

	//Initalize static members
	std::set<std::pair<int, int>> chunk::index;
	PerlinNoise3 chunk::perlinNoise;

	//Creating vector for chunk class
	std::vector<chunk> chunks;

	int readChunk(WindowFramework*& window, PandaFramework& framework, const std::string& path,int x, int y) {

		if (game::chunk::index.find(std::pair<int, int>(x, y)) != game::chunk::index.end()) {
			return 0;																					//Chunk is already loaded
		}

		//Initalize variables
		std::ifstream file(path);

		if (file.fail()) {
			game::warningOut("Chunk index file specified a chunk which could not be found. Skipping...");
			return 0;
		}

		std::string line;
		int x_level = x * 16;
		int y_level = y * 16;
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
				x_level = x * 16;
			} else {
				x_level += 2;
				y_level = y * 16;
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
					object.model.set_pos(x_level-2, y_level-2, z_level);

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
		chunk::index.insert(std::pair<int, int>(x, y));
		game::chunks.push_back(chunk);

		file.close();
		return 0;
	}
}