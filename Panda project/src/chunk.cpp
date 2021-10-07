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
	chunk::chunk(std::string path, const int& x, const int& y, WindowFramework* window, PandaFramework& framework) {
		this->x = x;
		this->y = x;
		readChunk(window, framework, path, x, y);
	}
	int chunk::reset() {
		chunk::objects.clear();
		return 0;
	}
	//You can't normalize inline so this is a helper function
	LVector3 normalized(double x, double y, double z) {
		LVector3 myVec(x, y, z);
		myVec.normalize();
		return myVec;
	}
	PT(Geom) makeSquare(double x1, double x2, double z1, double z2, double y1, double y2) {
		CPT(GeomVertexFormat) format = GeomVertexFormat::get_v3n3cpt2();
		PT(GeomVertexData) vdata = new GeomVertexData("square", format, Geom::UH_dynamic);

		GeomVertexWriter vertex(vdata, "vertex");
		GeomVertexWriter normal(vdata, "normal");
		GeomVertexWriter color(vdata, "color");
		GeomVertexWriter texcoord(vdata, "texcoord");

		if (x1 != x2) {
			vertex.add_data3(x1, y1, z1);
			vertex.add_data3(x2, y1, z1);
			vertex.add_data3(x2, y2, z2);
			vertex.add_data3(x1, y2, z2);

			normal.add_data3(normalized(2 * x1 - 1, 2 * y1 - 1, 2 * z1 - 1));
			normal.add_data3(normalized(2 * x2 - 1, 2 * y1 - 1, 2 * z1 - 1));
			normal.add_data3(normalized(2 * x2 - 1, 2 * y2 - 1, 2 * z2 - 1));
			normal.add_data3(normalized(2 * x1 - 1, 2 * y2 - 1, 2 * z2 - 1));
		} else {
			vertex.add_data3(x1, y1, z1);
			vertex.add_data3(x2, y2, z1);
			vertex.add_data3(x2, y2, z2);
			vertex.add_data3(x1, y1, z2);

			normal.add_data3(normalized(2 * x1 - 1, 2 * y1 - 1, 2 * z1 - 1));
			normal.add_data3(normalized(2 * x2 - 1, 2 * y2 - 1, 2 * z1 - 1));
			normal.add_data3(normalized(2 * x2 - 1, 2 * y2 - 1, 2 * z2 - 1));
			normal.add_data3(normalized(2 * x1 - 1, 2 * y1 - 1, 2 * z2 - 1));
		}

		//Adding different colors to the vertex for visibility
		color.add_data4f(1.0, 0.0, 0.0, 1.0);
		color.add_data4f(0.0, 1.0, 0.0, 1.0);
		color.add_data4f(0.0, 0.0, 1.0, 1.0);
		color.add_data4f(1.0, 0.0, 1.0, 1.0);

		texcoord.add_data2f(0.0, 1.0);
		texcoord.add_data2f(0.0, 0.0);
		texcoord.add_data2f(1.0, 0.0);
		texcoord.add_data2f(1.0, 1.0);

		//Quads aren't directly supported by the Geom interface
		//you might be interested in the CardMaker class if you are
		//interested in rectangle though
		PT(GeomTriangles) tris = new GeomTriangles(Geom::UH_dynamic);
		tris->add_vertices(0, 1, 3);
		tris->add_vertices(1, 2, 3);

		PT(Geom) square = new Geom(vdata);
		square->add_primitive(tris);
		return square;
	}
	int chunk::generateChunk(WindowFramework* window, PandaFramework& framework, const PerlinNoise3& perlinNoise = chunk::perlinNoise) {
		int chunksize = std::stoi(universeOptions["chunksize"]);
		int start_x = this->x * chunksize;
		int start_y = this->y * chunksize;

		//Note: it isn't particularly efficient to make every face as a separate Geom.
		//instead, it would be better to create one Geom holding all of the faces.
		PT(Geom) square0 = game::makeSquare(-1, -1, -1, 1, -1, 1);
		PT(Geom) square1 = game::makeSquare(-1, 1, -1, 1, 1, 1);
		PT(Geom) square2 = game::makeSquare(-1, 1, 1, 1, -1, 1);
		PT(Geom) square3 = game::makeSquare(-1, 1, -1, 1, -1, -1);
		PT(Geom) square4 = game::makeSquare(-1, -1, -1, -1, 1, 1);
		PT(Geom) square5 = game::makeSquare(1, -1, -1, 1, 1, 1);
		PT(GeomNode) snode = new GeomNode("square");
		snode->add_geom(square0);
		snode->add_geom(square1);
		snode->add_geom(square2);
		snode->add_geom(square3);
		snode->add_geom(square4);
		snode->add_geom(square5);
		NodePath cube = window->get_render().attach_new_node(snode);
		//cube.set_two_sided(true);
		cube.set_pos(start_x, start_y, 10);
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
	int chunk::readChunk(WindowFramework* window, PandaFramework& framework, std::string path, int x, int y) {
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