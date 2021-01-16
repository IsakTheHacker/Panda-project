
//Specify build options
#define game_console		//Comment this line to prevent console window from being created at startup

//Process build options
#ifndef game_console
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")		//This will remove the console window
#endif


#include "pandaIncludes.h"
//#include <direct.h>

int handInventoryIndex;
std::map<std::string, bool> keys;
std::map<std::string, std::string> universeOptions;
bool shouldRun = true;
bool terrainAnimationShouldRun;
bool devMode = false;
bool mouseInGame = true;
NodePath collidedNodePath;
std::string gamePath = "./";
std::string universePath = "universes/Test/";
bool player_sneaking = false;

//My libraries
#include "cppExtension.h"
#include "gameVars.h"
#include "gameFunctions.h"
#include "gameLanguage.h"
#include "constantVars.h"
#include "chunk.h"
#include "gameObject.h"

// Global stuff
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject) globalClock = ClockObject::get_global_clock();
PT(TextNode) text = new TextNode("textnode");
WindowFramework* window;

// Cool stuff
PT(MouseWatcher) mouseWatcher;
PT(CollisionRay) pickerRay;
CollisionTraverser myTraverser = CollisionTraverser("ctraverser");
PT(CollisionHandlerQueue) myHandler;
PT(CollisionNode) pickerNode;
NodePath pickerNP;

#include "gameClasses.h"

void pauseMenu(const Event* theEvent, void* data) {
	game::pauseMenuEventParameters* parameters = (game::pauseMenuEventParameters*)data;
	
	if (mouseInGame) {
		mouseInGame = false;
		WindowProperties props = parameters->window->get_graphics_window()->get_properties();
		props.set_cursor_hidden(false);
		parameters->window->get_graphics_window()->request_properties(props);
	} else {
		double center_x = parameters->window->get_graphics_window()->get_x_size() / static_cast<double>(2);
		double center_y = parameters->window->get_graphics_window()->get_y_size() / static_cast<double>(2);
		parameters->window->get_graphics_window()->move_pointer(0, center_x, center_y);
		WindowProperties props = parameters->window->get_graphics_window()->get_properties();
		props.set_cursor_hidden(true);
		parameters->window->get_graphics_window()->request_properties(props);
		mouseInGame = true;
	}
}
void pauseMenu(WindowFramework* window) {

	if (mouseInGame) {
		mouseInGame = false;
		WindowProperties props = window->get_graphics_window()->get_properties();
		props.set_cursor_hidden(false);
		window->get_graphics_window()->request_properties(props);
	} else {
		double center_x = window->get_graphics_window()->get_x_size() / static_cast<double>(2);
		double center_y = window->get_graphics_window()->get_y_size() / static_cast<double>(2);
		window->get_graphics_window()->move_pointer(0, center_x, center_y);
		WindowProperties props = window->get_graphics_window()->get_properties();
		props.set_cursor_hidden(true);
		window->get_graphics_window()->request_properties(props);
		mouseInGame = true;
	}
}

int main(int argc, char* argv[]) {

	//if (PStatClient::is_connected()) {
	//	PStatClient::disconnect();
	//}

	//std::string host = ""; // Empty = default config var value
	//int port = -1; // -1 = default config var value
	//if (!PStatClient::connect(host, port)) {
	//	std::cout << "Could not connect to PStat server." << std::endl;
	//}

	//Set gamePath to the directory of executable
	Filename exefile = argv[0];
	//chdir(exefile.get_fullpath().substr(0, exefile.get_fullpath().find_last_of("\\") + 1).c_str());
	//gamePath = exefile.get_fullpath().substr(0, exefile.get_fullpath().find_last_of("\\") + 1);

	//Checking if any arguments was given at startup
	if (argc > 3) {
		game::warningOut("Too many arguments was given, 1-2 arguments are allowed!");
	} else if (argc > 1) {
		for (size_t i = 1; i < argc; i++) {
			if (std::find(std::begin(game::allowed_parameters), std::end(game::allowed_parameters), argv[i]) != std::end(game::allowed_parameters)) {
				if (argv[i] == game::allowed_parameters[0]) {
					devMode = true;
					game::importantInfoOut("Game was started in devmode!");
				} else if (argv[i] == game::allowed_parameters[1]) {
					gamePath = "../../../Panda Project/";
					devMode = true;
					game::importantInfoOut("Game was started in Visual Studio devmode!");
				}
			} else {
				std::ifstream ulink(argv[i]);
				if (ulink.fail()) {
					game::warningOut("An unknown argument was given!");
					continue;
				}

				universePath = "";
				std::string line;
				while (std::getline(ulink, line)) {
					universePath += line;
				}
				game::errorOut(universePath);
				ulink.close();
				
			}
		}
	}

	//Starting debug input thread if game was started in devmode!
	//std::thread debugInputThread(game::takeDebugInput);

	//Creating folders and files
	game::runPyScript("data/scripts/makeDirectories.py");
	game::runPyScript("data/scripts/createOptionsFile.py");

	//Loading config files
	load_prc_file("data/Confauto.prc");
	load_prc_file("data/Config.prc");

	//Reading options
	std::map<std::string, std::string> options;
	game::readOptions(options, "data/options.txt");
	std::map<std::string, std::string> scripting_options;
	game::readOptions(scripting_options, "data/scripting_options.txt");

	game::setHeading(options["console-heading"]);
	game::logOut("Starting...");

	game::listOptions(options);
	game::listOptions(scripting_options, "Scripting options:");

	// Open a new window framework and set the title
	PandaFramework framework;
	framework.open_framework(argc, argv);
	framework.set_window_title("The Panda Project: Prealpha 0.1.1");

	// Open the window
	WindowFramework* window = framework.open_window();
	game::Player player("data/assets/playerproperties/standard.playerproperties", window, framework, false, false);
	window->get_camera(0)->get_lens()->set_fov(std::stod(options["fov"]));

	//Enable shader generation for the game
	/*window->get_render().set_shader_auto();*/

	//Set default window instance to use for chunk class
	game::chunk::setDefaultWindow(window);
	game::chunk::setDefaultFramework(framework);

	// Cool stuff
	pickerNode = new CollisionNode("mouseRay");
	pickerNP = player.camera.attach_new_node(pickerNode);
	pickerNP.show();
	pickerNP.show_bounds();
	pickerNP.show_tight_bounds();
	pickerNP.show_through();
	pickerNode->set_from_collide_mask(GeomNode::get_default_collide_mask());
	pickerRay = new CollisionRay();
	pickerNode->add_solid(pickerRay);
	myHandler = new CollisionHandlerQueue();
	myTraverser.add_collider(pickerNP, myHandler);

	//Setting up frame rate meter
	if (!std::stoi(options["hide_fps"])) {
		PT(FrameRateMeter) meter;
		meter = new FrameRateMeter("frame_rate_meter");
		meter->setup_window(window->get_graphics_window());
	}

	//Mouse input
	MouseWatcher* mouseWatcher = DCAST(MouseWatcher, window->get_mouse().node());
	WindowProperties props = window->get_graphics_window()->get_properties();
	props.set_cursor_hidden(std::stoi(options["hidden_cursor"]));
	props.set_mouse_mode(WindowProperties::M_relative);
	window->get_graphics_window()->request_properties(props);


	// This makes the ray's origin the camera and makes the ray point
	// to the screen coordinates of the mouse.

	//LPoint2 mpos = mouseWatcher->get_mouse();

	pickerRay->set_from_lens(window->get_camera(0), 0, 0);
	//pickerRay->set_from_lens(window->get_camera(0), mpos.get_x(), mpos.get_y());

	// Change background color to black
	window->get_graphics_window()->get_active_display_region(0)->set_clear_color(LColorf(0, 0, 0, 1));

	//Keyboard input
	window->enable_keyboard();
	#include "keyDefinitions.h"

	PT(TextNode) text;
	text = new TextNode("node name");
	NodePath textNodePath = window->get_aspect_2d().attach_new_node(text);
	textNodePath.set_scale(0.07);
	textNodePath.set_pos(-1.65,0,0.9);

	PT(TextNode) text2;
	text2 = new TextNode("node name2");
	NodePath textNodePath2 = window->get_aspect_2d().attach_new_node(text2);
	textNodePath2.set_scale(0.07);
	textNodePath2.set_pos(-1.65, 0, 0.65);

	PT(TextNode) text3;
	text3 = new TextNode("node name3");
	NodePath textNodePath3 = window->get_aspect_2d().attach_new_node(text3);
	textNodePath3.set_scale(0.07);
	textNodePath3.set_pos(-1.65, 0, 0.40);

	PT(TextNode) fovText;
	fovText = new TextNode("node name3");
	NodePath fovTextNodePath = window->get_aspect_2d().attach_new_node(fovText);
	fovTextNodePath.set_scale(0.07);
	fovTextNodePath.set_pos(-1.65, 0, 0.15);

	// Crosshair
	CardMaker cardmaker("crosshair");
	NodePath cursor(cardmaker.generate());
	game::setTexture(cursor, gamePath + (std::string)"models/textures/png/crosshair.png");
	cursor.set_sx(0.06);
	cursor.set_sz(0.06);
	cursor.set_pos(0 - cursor.get_sx() / 2, 0, 0 - cursor.get_sz() / 2);
	cursor.set_transparency(TransparencyAttrib::M_alpha);
	cursor.reparent_to(window->get_aspect_2d());

	// Inventory
	CardMaker e_inventory_card("Inventory");
	NodePath e_inventory(e_inventory_card.generate());
	game::setTexture(e_inventory, gamePath + (std::string)"models/textures/png/inventory.png");
	e_inventory.set_sx(2.2);
	e_inventory.set_sz(0.9);
	e_inventory.set_pos(0 - e_inventory.get_sx() / 2, 0, 0 - e_inventory.get_sz() / 2);
	e_inventory.set_transparency(TransparencyAttrib::M_binary);
	e_inventory.reparent_to(window->get_aspect_2d());
	e_inventory.hide();

	std::vector<NodePath> inventory;
	std::vector<NodePath> tool;

	for (int i = -5; i < 6; i++) {
		CardMaker hand_inventory("hand_inventory" + i);
		NodePath hand_inventoryNode(hand_inventory.generate());
		game::setTexture(hand_inventoryNode, gamePath + (std::string)"models/textures/png/hand-inventory-all.png");
		hand_inventoryNode.set_sx(0.2);
		hand_inventoryNode.set_sz(0.2);
		hand_inventoryNode.set_pos(i/static_cast<float>(5) - hand_inventoryNode.get_sx() / 2, 0, -0.85 - hand_inventoryNode.get_sz() / 2);
		hand_inventoryNode.set_transparency(TransparencyAttrib::M_alpha);
		hand_inventoryNode.reparent_to(window->get_aspect_2d());
		if (i == -5) {
			i++;
			tool.push_back(hand_inventoryNode);
		} else {
			inventory.push_back(hand_inventoryNode);
		}
	}

	//Loading chunks
	game::readOptions(universeOptions, universePath + "universe");
	int chunksize = std::stoi(universeOptions["chunksize"]);
	unsigned long seed = std::stoul(universeOptions["seed"]);
	bool ignore_lock = std::stoi(universeOptions["ignore-lock"]);
	if (game::fileExists(universePath + "lock")) {
		if (!ignore_lock) {
			game::importantInfoOut("This universe is opened by another instance of Panda Project! Therefore, you can't open it.");
			exit(0);
		} else {
			game::importantInfoOut("This universe is locked and you are ignoring it.");
		}
	} else {
		std::ofstream lockFile(universePath + "lock");
		lockFile.close();
	}
	if (game::isOdd(chunksize)) {
		game::errorOut("Chunksize must be even, not odd!");
		exit(1);
	}
	{
		std::ifstream index(universePath + "index");
		if (index.fail()) {
			game::warningOut("Could not find an index file for the universe. Creating one...");
			std::ofstream createIndex(universePath + "index");
			createIndex.close();
		} else {
			terrainAnimationShouldRun = true;
			std::thread terrain_animation_thread(game::terrainAnimation, "Loading terrain");
			std::string line;
			while (std::getline(index, line)) {
				int x = std::stoi(game::split(line, ".")[0]);
				int y = std::stoi(game::split(line, ".")[1]);
				if (game::chunk::loaded_chunks.find(std::pair<int, int>(x, y)) == game::chunk::loaded_chunks.end()) {
					game::chunk chunk(x, y);
					if (chunk.readChunk(window, framework, universePath + line, x, y) == 0) {
						game::chunk::loaded_chunks.insert(std::pair<int, int>(x, y));
						game::chunks.push_back(chunk);
					}
				}
			}
			index.close();
			terrainAnimationShouldRun = false;
			terrain_animation_thread.join();
		}
	}

	//
	/*NodePath test = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/blocky.egg");
	test.set_scale(0.5);
	test.set_h(45);
	test.set_p(45);
	test.set_r(45);
	test.reparent_to(window->get_aspect_2d());*/


	NodePath blocky = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/blocky.egg");
	blocky.set_scale(0.5);
	blocky.set_pos(0, 0, 100);
	blocky.reparent_to(window->get_render());
	blocky.set_shader_off(10);

	CollisionNode* cSphere_node2 = new CollisionNode("Sphere");
	cSphere_node2->add_solid(new CollisionSphere(0, 0, 0, 4));
	NodePath blockyC = blocky.attach_new_node(cSphere_node2);
	blockyC.show();
	
	player.camera.set_z(30);

	NodePath panda = NodePath("panda");
	panda.set_scale(0.5);
	panda.set_pos(0, 0, 0);
	panda.reparent_to(window->get_render());
	panda.hide();


	CollisionNode* cSphere_node = new CollisionNode("Sphere");
	cSphere_node->add_solid(new CollisionBox(0, 0.8, 0.8, 3));
	NodePath cameraC = player.camera.attach_new_node(cSphere_node);

	CollisionHandlerPusher pusher;
	pusher.add_in_pattern("Something");
	pusher.add_out_pattern("Something2");
	//pusher.add_again_pattern("%fn-into-%in");

	framework.define_key("Something", "", game::testIfPlayerOnGround, 0);
	framework.define_key("Something2", "", game::testIfPlayerOnGround, (void*)1);

	framework.define_key("Something", "", game::getCollidedNodePath, 0);
	framework.define_key("Something2", "", game::getCollidedNodePath, (void*)1);

	//window->get_render().ls();

	//CollisionHandlerQueue queue;
	CollisionTraverser* traverser = new CollisionTraverser();

	traverser->add_collider(cameraC, &pusher);
	pusher.add_collider(cameraC, player.camera);

	traverser->traverse(window->get_render());
	//traverser->show_collisions(window->get_render());

	//Lights
	PT(AmbientLight) alight = new AmbientLight("alight");
	alight->set_color(0.2);
	NodePath alnp = window->get_render().attach_new_node(alight);
	window->get_render().set_light(alnp);

	PT(DirectionalLight) d_light = new DirectionalLight("my d_light");
	d_light->set_color(LColor(0.8, 0.8, 0.5, 1));
	//d_light->set_shadow_caster(true, 512, 512);
	NodePath dlnp = window->get_render().attach_new_node(d_light);
	dlnp.set_hpr(0, -90, 0);
	dlnp.set_pos(0, 0, 100);
	dlnp.show_tight_bounds();
	window->get_render().set_light(dlnp);

	//PNMImage pnmImage("images/noise_0000.png");
	//for (size_t i = 0; i < pnmImage.get_x_size(); i++) {
	//	for (size_t j = 0; j < pnmImage.get_y_size(); j++) {
	//		auto pixel = pnmImage.get_pixel(i, j);
	//		//std::cout << "Pixel: " << i << "," << j << " " << pixel << std::endl;
	//	}
	//}

	std::vector<double> doubles;
	PerlinNoise3 perlinNoise(128, 128, 128, 256, seed);
	for (size_t i = 0; i < 100; i++) {
		for (size_t j = 0; j < 100; j++) {
			for (size_t k = 0; k < 100; k++) {
				//std::cout << "Perlin: " << perlinNoise.noise(i, j, k) << std::endl;
				doubles.push_back(perlinNoise.noise(i, j, k));
			}
		}
	}
	std::cout << *std::min_element(doubles.begin(), doubles.end()) << std::endl;
	std::cout << *std::max_element(doubles.begin(), doubles.end()) << std::endl;


	//Reading settings from settings map
	double camera_x_speed = std::stof(options["camera_x_speed"]);
	double camera_y_speed = std::stof(options["camera_y_speed"]);

	double x_speed = std::stof(options["x_speed"]);
	double y_speed = std::stof(options["y_speed"]);
	double z_speed = std::stof(options["z_speed"]);

	double offset_h = 0.0;
	double offset_p = 0.0;
	double offset_r = 0.0;

	double center_x = 0.0;
	double center_y = 0.0;

	double x = 0.0;
	double y = 0.0;

	std::string chunk_x;
	std::string chunk_y;

	double sneak_distance = std::stod(options["sneak-distance"]);
	bool chunk_exists = false;

	std::string sad = "Hello World";
	/*myTraverser.show_collisions(window->get_render());

	framework.show_collision_solids(window->get_render());*/

	NodePath block;

	double heading;
	double pitch;

	double velocity = 0.0;
	double velocityModifier = 1.1;

	//Testing entities
	game::entity entity("data/assets/entityproperties/test.entityproperties", window, framework, false);
	entity.model.set_pos(0, 0, 15);
	entity.model.reparent_to(window->get_render());


	//Main loop
	Thread* current_thread = Thread::get_current_thread();
	while (framework.do_frame(current_thread) && shouldRun) {

		if ((collidedNodePath == entity.model) && (player.onGround)) {
			player.camera.set_pos(entity.model.get_x(), entity.model.get_y(), player.camera.get_z());
		}

		entity.update();

		// Velocity computing (Z axis)
		if (velocity == 0 && !player.onGround) {
			velocity = 0.01;
		} else {
			if (velocity > 0) {
				if (velocity < 1.25) {
					velocity = velocity * velocityModifier;
				}
			} else if (velocity < 0) {
				double value = (int)(player.camera.get_z() * 100 + 0.5);
				value = (double)value / 100;
				double value2 = (int)((player.camera.get_z() - velocity) * 100 + 0.5);
				value2 = (double)value2 / 100;
				if (value == value2) {
					velocity = 0.01;
				} else {
					velocity = velocity / velocityModifier;
				}
			}
		}
		if (player.onGround) {
			velocity = 0;
		}
		player.camera.set_z(player.camera.get_pos().get_z() - velocity);
		panda.set_z(player.camera.get_pos().get_z() - velocity);

		// Checking if current chunk exists, generate if not.
		if (player.camera.get_x() < 0) {
			chunk_x = std::to_string((int)(player.camera.get_x() - chunksize) / chunksize);
		} else {
			chunk_x = std::to_string((int)player.camera.get_x() / chunksize);
		}
		if (player.camera.get_y() < 0) {
			chunk_y = std::to_string((int)(player.camera.get_y() - chunksize) / chunksize);
		} else {
			chunk_y = std::to_string((int)player.camera.get_y() / chunksize);
		}
		/*chunk_x = std::to_string((int)player.camera.get_x() / 16);
		chunk_y = std::to_string((int)player.camera.get_y() / 16);*/
		
		if (game::chunk::loaded_chunks.find(std::pair<int, int>(std::stoi(chunk_x), std::stoi(chunk_y))) != game::chunk::loaded_chunks.end()) {
			chunk_exists = true;
		} else {
			chunk_exists = false;
		}

		if (!chunk_exists && !keys["f5"]) {
			game::chunk chunk(std::stoi(chunk_x), std::stoi(chunk_y));																//Create new chunk
			chunk.generateChunk(window, framework, perlinNoise);																	//Apply the generateChunk function on the new chunk
			game::chunks.push_back(chunk);																							//Push the chunk to vector game::chunks
			game::chunk::loaded_chunks.insert(std::pair<int, int>(x, y));
			game::chunk::index[std::pair<int, int>(chunk.x, chunk.y)] = game::chunks.size()-1;
		}

		if (mouseInGame) {
			if (handInventoryIndex < 0) {
				handInventoryIndex = 8;
			} else if (handInventoryIndex > 8) {
				handInventoryIndex = 0;
			}

			for (int i = 0; i < 9; i++) {
				if (i == handInventoryIndex) {
					game::setTexture(inventory[i], gamePath + (std::string)"models/textures/png/hand-inventory-highlighted.png");
				} else if (i != handInventoryIndex) {
					game::setTexture(inventory[i], gamePath + (std::string)"models/textures/png/hand-inventory-all.png");
				}
			}
		}

		// check collisions, will call pusher collision handler
		// if a collision is detected
		traverser->traverse(window->get_render());

		myTraverser.traverse(window->get_render());
		if (myHandler->get_num_entries() > 0) {
			myHandler->sort_entries();
			block.hide_bounds();
			CollisionEntry* entry = myHandler->get_entry(0);
			block = myHandler->get_entry(0)->get_into_node_path().get_parent().get_parent();
			LVector3 surface = entry->get_surface_normal(window->get_render());
			block.show_tight_bounds();

			int block_chunk_x;
			int block_chunk_y;
			if (block.get_x() < 0) {
				block_chunk_x = (int)(block.get_x() - chunksize) / chunksize;
			} else {
				block_chunk_x = (int)block.get_x() / chunksize;
			}
			if (block.get_y() < 0) {
				block_chunk_y = (int)(block.get_y() - chunksize) / chunksize;
			} else {
				block_chunk_y = (int)block.get_y() / chunksize;
			}

			if (keys["mouse1"]) {
				if (mouseInGame) {
					game::chunk chunk = game::chunks[game::chunk::index[std::pair<int, int>(block_chunk_x, block_chunk_y)]];		//Get chunk containing the block
					chunk.objects[std::stoull(block.get_tag("chunkObjectId"))].model.remove_node();									//Remove node
					chunk.objects[std::stoull(block.get_tag("chunkObjectId"))] = game::object(false, false);						//Replace game::object with empty game::object
					game::chunks[game::chunk::index[std::pair<int, int>(block_chunk_x, block_chunk_y)]] = chunk;					//Save chunk changes in vector "chunks"
					keys["mouse1"] = false;
				}
			}
			if (keys["mouse2"]) {
				vector_string tagkeys;
				std::string formatted_values;
				block.get_tag_keys(tagkeys);
				if (tagkeys.size() > 0) {
					for (std::string key : tagkeys) {
						formatted_values.append("\n        " + key + ": " + block.get_tag(key));
					}
				} else {
					formatted_values = "none";
				}
				std::string blockInfo =
					"Information about block:\n"
					"    XYZ: " + std::to_string(block.get_x()) + ", " + std::to_string(block.get_y()) + ", " + std::to_string(block.get_z()) + "\n"
					"    Chunk XY: " + std::to_string(block_chunk_x) + ", " + std::to_string(block_chunk_y) + "\n"
					"    Tags: " + formatted_values
				;
				game::logOut(blockInfo);
				keys["mouse2"] = false;
			}
			if (keys["mouse3"]) {
				if (mouseInGame) {
					std::string configPath;

					if (handInventoryIndex == 0) {
						configPath = "data/assets/blockproperties/grass.blockproperties";
					} else if (handInventoryIndex == 1) {
						configPath = "data/assets/blockproperties/rotational-complex.blockproperties";
					} else if (handInventoryIndex == 2) {
						configPath = "data/assets/blockproperties/log.blockproperties";
					} else if (handInventoryIndex == 3) {
						configPath = "data/assets/blockproperties/stone.blockproperties";
					} else if (handInventoryIndex == 4) {
						configPath = "";
					} else if (handInventoryIndex == 5) {
						configPath = "";
					} else if (handInventoryIndex == 6) {
						configPath = "";
					} else if (handInventoryIndex == 7) {
						configPath = "";
					} else if (handInventoryIndex == 8) {
						configPath = "";
					} else if (handInventoryIndex == 9) {
						configPath = "";
					} else if (handInventoryIndex == 10) {
						configPath = "";
					}



					game::object obj(configPath, window, framework, false, false);
					obj.model.set_pos(block.get_x() + surface.get_x() * 2, block.get_y() + surface.get_y() * 2, block.get_z() + surface.get_z() * 2);

					if (obj.model.get_x() < 0) {
						block_chunk_x = (int)(obj.model.get_x() - chunksize) / chunksize;
					} else {
						block_chunk_x = (int)obj.model.get_x() / chunksize;
					}
					if (obj.model.get_y() < 0) {
						block_chunk_y = (int)(obj.model.get_y() - chunksize) / chunksize;
					} else {
						block_chunk_y = (int)obj.model.get_y() / chunksize;
					}

					if (keys["r"]) {

						if (surface.get_z() == -1) {
							pitch = 1;
						} else {
							pitch = 0;
						}

						if (surface.get_x() == -1) {
							pitch = 0.5;
							heading = -0.5;
						} else if (surface.get_x() == 1) {
							pitch = 0.5;
							heading = 0.5;
						} else if (surface.get_y() == -1) {
							pitch = 0.5;
							heading = 0;
						} else if (surface.get_y() == 1) {
							pitch = 0.5;
							heading = 1;
						}
						obj.model.set_hpr(heading * 180, pitch * 180, 0);
					}

					game::chunk chunk = game::chunks[game::chunk::index[std::pair<int, int>(block_chunk_x, block_chunk_y)]];
					
					obj.model.set_tag("chunk", std::to_string(block_chunk_x) + "," + std::to_string(block_chunk_y));
					obj.model.set_tag("id", std::to_string(obj.id));
					obj.model.set_tag("chunkObjectId", std::to_string(chunk.objects.size()));

					obj.model.set_shader_auto();

					chunk.objects.push_back(obj);
					game::chunks[game::chunk::index[std::pair<int, int>(block_chunk_x, block_chunk_y)]] = chunk;

					keys["mouse3"] = false;
				}
			}
		} else {
			// No targeted block
			block.hide_bounds();
		}

		//Set text to the new values
		text->set_text("X: " + std::to_string(player.camera.get_x()) + "\nY: " + std::to_string(player.camera.get_y()) + "\nZ: " + std::to_string(player.camera.get_z()));
		text2->set_text("H: " + std::to_string(player.camera.get_h()) + "\nP: " + std::to_string(player.camera.get_p()) + "\nR: " + std::to_string(player.camera.get_r()));
		text3->set_text("Chunk X: " + chunk_x + "\nChunk Y: " + chunk_y);
		fovText->set_text("VFov: " + std::to_string(window->get_camera(0)->get_lens()->get_vfov()) + "\nHFov: " + std::to_string(window->get_camera(0)->get_lens()->get_hfov()));

		if (mouseInGame) {
			if (window->get_graphics_window()) {
				if (window->get_graphics_window()->get_pointer(0).get_in_window()) {
					center_x = window->get_graphics_window()->get_x_size() / static_cast<double>(2);
					center_y = window->get_graphics_window()->get_y_size() / static_cast<double>(2);

					x = window->get_graphics_window()->get_pointer(0).get_x();
					y = window->get_graphics_window()->get_pointer(0).get_y();

					double move_x = std::floor(center_x - x);
					double move_y = std::floor(center_y - y);

					if (keys["v"]) {
						offset_r += move_x / camera_x_speed;

						if (offset_r < 90 && offset_r > -90) {
							player.camera.set_r(offset_r);
						} else {
							offset_r -= move_x / 5;
						}
					} else {
						offset_h += move_x / camera_x_speed;
						player.camera.set_h(std::fmod(offset_h, 360));

						// Reset rotation
						offset_r = 0;
						player.camera.set_r(offset_r);
					}

					offset_p += move_y / camera_y_speed;
					panda.set_h(std::fmod(offset_h, 360));

					//Adjust the collision box so its pitch doesn't change
					cameraC.set_p(offset_p - offset_p * 2);

					//Adjust the collision box so its rotation doesn't change
					//cameraC.set_r(offset_r - offset_r * 2);
					//Not fixed yet

					if (!keys["v"]) {
						if (offset_p < 90 && offset_p > -90) {
							player.camera.set_p(offset_p);
						} else {
							offset_p -= move_y / 5;
						}
					}

					window->get_graphics_window()->move_pointer(0, center_x, center_y);		//Reset pointer to 0, 0
				}
			}


			if (keys["w"]) {
				player.camera.set_y(panda, 0 + y_speed);
				panda.set_y(panda, 0 + y_speed);
			}
			if (keys["s"]) {
				player.camera.set_y(panda, 0 - y_speed);
				panda.set_y(panda, 0 - y_speed);
			}
			if (keys["a"]) {
				player.camera.set_x(player.camera, 0 - x_speed);
				panda.set_x(player.camera, 0 - x_speed);
			}
			if (keys["d"]) {
				player.camera.set_x(player.camera, 0 + x_speed);
				panda.set_x(player.camera, 0 + x_speed);
			}
			if (keys["lshift"]) {
				if (player.onGround && !player_sneaking) {
					player_sneaking = true;
					cameraC.set_z(cameraC.get_z() + sneak_distance);
					player.camera.set_z(player.camera.get_pos().get_z() - sneak_distance);
				}
			} else if (!keys["lshift"]) {
				if (player.onGround && player_sneaking) {
					player_sneaking = false;
					cameraC.set_z(cameraC.get_z() - sneak_distance);
					player.camera.set_z(player.camera.get_pos().get_z() + sneak_distance);
				}
			}
			if (keys["space"]) {
				if (player.onGround) {
					if (!player_sneaking) {
						velocity = -0.25;
					} else if (player_sneaking) {
						velocity = -0.45;
					}
					player.onGround = false;
				}
			}
			if (keys["q"]) {
				/*game::saveChunk(game::chunks[0]);*/
				for (std::pair<int, int> pair : game::chunk::loaded_chunks) {
					std::cout << pair.first << "	" << pair.second << std::endl;
				}
				entity.update();
				keys["q"] = false;
			}
			if (keys["r"]) {
				player.camera.set_z(30);
				velocity = 0;
			}
			if (keys["f2"]) {
				std::string filename = "screenshots/" + game::getConvertedDateTime() + ".png";
				game::warningOut(filename);
				bool successful = window->get_graphics_window()->save_screenshot(filename, "");
				if (successful) {
					game::logOut("Saved screenshot: '" + filename + "' to the screenshots folder.");
				} else {
					game::errorOut("Screenshot was not saved successfully!");
				}
				keys["f2"] = false;
			}
			if (keys["f3"]) {		//Decrease FOV
				window->get_camera(0)->get_lens()->set_fov(window->get_camera(0)->get_lens()->get_fov()-10);
				keys["f3"] = false;
			}
			if (keys["f4"]) {		//Increase FOV
				window->get_camera(0)->get_lens()->set_fov(window->get_camera(0)->get_lens()->get_fov()+10);
				keys["f4"] = false;
			}

			if (keys["arrow_up"] || keys["arrow_down"] || keys["arrow_left"] || keys["arrow_right"]) {
				double move_y = 0.0;
				double move_x = 0.0;
				if (keys["arrow_up"]) {
					move_y += 3.0;
				}
				if (keys["arrow_down"]) {
					move_y -= 3.0;
				}
				if (keys["arrow_left"]) {
					move_x += 3.0;
				}
				if (keys["arrow_right"]) {
					move_x -= 3.0;
				}

				if (keys["v"]) {
					offset_r += move_x / camera_x_speed;

					if (offset_r < 90 && offset_r > -90) {
						player.camera.set_r(offset_r);
					} else {
						offset_r -= move_x / 5;
					}
				} else {
					offset_h += move_x / camera_x_speed;
					player.camera.set_h(offset_h);

					// Reset rotation
					offset_r = 0;
					player.camera.set_r(offset_r);
				}

				offset_p += move_y / camera_y_speed;
				panda.set_h(offset_h);

				//Adjust the collision box so its pitch doesn't change
				cameraC.set_p(offset_p - offset_p * 2);

				//Adjust the collision box so its rotation doesn't change
				//cameraC.set_r(offset_r - offset_r * 2);
				//Not fixed yet

				if (!keys["v"]) {
					if (offset_p < 90 && offset_p > -90) {
						player.camera.set_p(offset_p);
					} else {
						offset_p -= move_y / 5;
					}
				}
			}
		} else {
			if (keys["q"]) {		//Crash game
				game::importantInfoOut("Crashing game...");
				exit(1);			// Code 1 is used because we crashed the game
			}
		}
		if (keys["e"]) {
			/*game::winds.push_back(game::windObject(window, framework, 0.1, 0.2, 0, 0.1, 1, 1, 1, true));
			game::winds[game::winds.size() - 1].model.set_pos(floor(camera.get_x()), floor(camera.get_y()), floor(camera.get_z()));*/

			if (e_inventory.is_hidden()) {
				e_inventory.show();
				cursor.hide();
				for (NodePath handInventoryNode : inventory) {
					handInventoryNode.hide();
				}
				tool[0].hide();
				text->set_overall_hidden(true);
				text2->set_overall_hidden(true);
				text3->set_overall_hidden(true);
				fovText->set_overall_hidden(true);
				pauseMenu(window);
			} else {
				e_inventory.hide();
				cursor.show();
				for (NodePath handInventoryNode : inventory) {
					handInventoryNode.show();
				}
				tool[0].show();
				text->set_overall_hidden(false);
				text2->set_overall_hidden(false);
				text3->set_overall_hidden(false);
				fovText->set_overall_hidden(false);
				pauseMenu(window);
			}

			keys["e"] = false;
		}

		//Reset mouse clicks
		keys["mouse1"] = false;
		keys["mouse3"] = false;

		//Border checking
		/*if (options["lower_border"] != "none" && options["lower_border"] != "null") {
			if (camera.get_pos().get_z() < std::stof(options["lower_border"])) {
				camera.set_z(std::stof(options["lower_border"]));
			}
		}
		if (options["upper_border"] != "none" && options["upper_border"] != "null") {
			if (camera.get_pos().get_z() > std::stof(options["upper_border"])) {
				camera.set_z(std::stof(options["upper_border"]));
			}
		}*/
	}

	//Saving chunks
	{
		std::ofstream updateIndex(universePath + "index", std::ios::out | std::ios::trunc);
		terrainAnimationShouldRun = true;
		std::thread saving_animation_thread(game::terrainAnimation, "Saving universe");
		for (game::chunk chunk : game::chunks) {
			updateIndex << chunk.x << "." << chunk.y << ".chunk" << std::endl;
			chunk.saveChunk();
		}
		updateIndex.close();
		terrainAnimationShouldRun = false;
		saving_animation_thread.join();
	}
	std::string filename = universePath + "lock";
	std::remove(filename.c_str());

	framework.close_framework();
	//debugInputThread.detach();
	game::logOut("Closing...");

	if (!std::stoi(options["close_console_without_input"])) {
		game::waitForKeypress();
	}

	return 0;
}