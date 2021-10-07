
//Specify build options
#define game_console		//Comment this line to prevent console window from being created at startup

//Process build options
#ifndef game_console
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")		//This will remove the console window
#endif


//C++ built-in libraries
#include <map>
#include <string>

//Panda3D libraries
#include "pgButton.h"
#include "mouseButton.h"
#include "rigidBodyCombiner.h"
#include "rigidBodyCombiner.h"
#include "nodePath.h"
#include "pandaSystem.h"
#include "collisionHandlerQueue.h"
#include "collisionHandlerPusher.h"
#include "collisionRay.h"
#include "collisionSphere.h"
#include "ambientLight.h"
#include "mouseWatcher.h"
#include "texturePool.h"
#include "directionalLight.h"

#include "particleSystem.h"			//Particles
#include "sphereVolumeEmitter.h"
#include "pointParticleFactory.h"
#include "pointParticleRenderer.h"
#include "particleSystemManager.h"

int handInventoryIndex;
std::map<std::string, bool> keys;
std::map<std::string, std::string> universeOptions;
bool shouldRun = true;
bool terrainAnimationShouldRun;
bool devMode = false;
bool mouseInGame = true;
std::string gamePath = "./";
std::string universePath = "universes/Test/";
NodePath rbcnp = NodePath("rbcnp");

//My libraries
#include "cppExtension.h"
#include "gameVars.h"
#include "gameFunctions.h"
#include "gameLanguage.h"
#include "constantVars.h"
#include "chunk.h"
#include "gameObject.h"
#include "gameTimer.h"
#include "gameTasks.h"
#include "gameInventory.h"
#include "gameItem.h"
#include "gameGui.h"
#include "gameClasses.h"

game::Player player;

void pauseMenu(WindowFramework* window) {
	
	if (mouseInGame) {
		mouseInGame = false;

		//Set cursor shown
		WindowProperties props;
		props.set_cursor_hidden(false);
		window->get_graphics_window()->request_properties(props);
	} else {
		double center_x = window->get_graphics_window()->get_x_size() / static_cast<double>(2);
		double center_y = window->get_graphics_window()->get_y_size() / static_cast<double>(2);
		window->get_graphics_window()->move_pointer(0, center_x, center_y);
		
		//Set cursor hidden
		WindowProperties props;
		props.set_cursor_hidden(true);
		window->get_graphics_window()->request_properties(props);

		mouseInGame = true;
	}
}
void inventoryMenu(WindowFramework* window) {

	if (mouseInGame) {
		mouseInGame = false;
		
		//Set cursor shown
		WindowProperties props;
		props.set_cursor_hidden(false);
		window->get_graphics_window()->request_properties(props);
	} else {
		double center_x = window->get_graphics_window()->get_x_size() / static_cast<double>(2);
		double center_y = window->get_graphics_window()->get_y_size() / static_cast<double>(2);
		window->get_graphics_window()->move_pointer(0, center_x, center_y);
		
		//Set cursor hidden
		WindowProperties props;
		props.set_cursor_hidden(true);
		window->get_graphics_window()->request_properties(props);

		mouseInGame = true;
	}
}

int main(int argc, char* argv[]) {

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

	//Create folders and files
	game::mkdir("data");
	game::mkdir("screenshots");
	if (!game::fileExists("data/options.txt")) {
		std::ofstream file("data/options.txt");
		std::string newLine = "\n";
		for (size_t i = 0; i < game::optionLines.size(); i++) {
			if (i == game::optionLines.size() - 1) {
				newLine = "";
			}
			file << game::optionLines[i] << newLine;
		}
		file.close();
	}

	//Read options
	std::map<std::string, std::string> options;
	game::readOptions(options, "data/options.txt");
	game::chunk::options = &options;
	game::Player::options = &options;
	std::map<std::string, std::string> scripting_options;
	game::readOptions(scripting_options, "data/scripting_options.txt");

	//Connect to PStats server if user wants to
	if (std::stoi(options["enable_pstats"])) {
		game::connectToPStats(options["pstat-host"]);
	}

	game::setHeading(options["console-heading"]);
	game::setConsoleTitle(L"The Panda Project: Console window");
	game::logOut("Starting...");
	game::userConfigOut("Panda3D version: " + PandaSystem::get_version_string());
	game::userConfigOut("It was built on " + PandaSystem::get_build_date());
	game::userConfigOut("Running on " + PandaSystem::get_platform());
	game::listOptions(options);
	game::listOptions(scripting_options, "Scripting options:");

	// Open a new window framework and set the title
	PandaFramework framework;
	framework.open_framework(argc, argv);
	framework.set_window_title("The Panda Project: Prealpha 0.1.5");

	// Open the window
	WindowFramework* window = framework.open_window();
	if (window == nullptr) {
		game::errorOut("Error while opening window. Quitting!");
		game::waitForKeypress();
		return 1;
	}
	window->get_render().set_shader_auto();

	//Create RigidBodyCombiner
	PT(RigidBodyCombiner) rbc = new RigidBodyCombiner("rbc");
	rbcnp = NodePath(rbc);

	player = game::Player("data/assets/playerproperties/standard.playerproperties", window, framework, false, false);
	DCAST(Camera, player.firstPerson.node())->get_lens()->set_fov(std::stod(options["fov"]));
	window->get_display_region_3d()->set_camera(player.firstPerson);

	//Set default window instance to use for chunk class
	game::chunk::setDefaultWindow(window);
	game::chunk::setDefaultFramework(framework);

	//Experimental GUI
	game::button returnToGameButton(framework, game::unpause, 0, 0.30, 0, 0.30, "Return to game");
	returnToGameButton.hide();

	game::button generateUniverse(framework, epass, 0, 0, 0, 0, "Generate universe");
	generateUniverse.hide();

	game::button quitSaveButton(framework, game::exitGame, 0, -0.30, 0, -0.30, "Quit and save");
	quitSaveButton.hide();

	player.model.ls();

	//Set up frame rate meter
	if (!std::stoi(options["hide_fps"])) {
		PT(FrameRateMeter) meter;
		meter = new FrameRateMeter("frame_rate_meter");
		meter->setup_window(window->get_graphics_window());
	}

	//Initialize fog
	PT(Fog) fog = new Fog("Fog");
	fog->set_color(25, 25, 25);
	fog->set_exp_density(0.0001);
	window->get_render().set_fog(fog);

	//Mouse input
	PT(MouseWatcher) mouseWatcher = DCAST(MouseWatcher, window->get_mouse().node());
	WindowProperties props = window->get_graphics_window()->get_properties();
	props.set_cursor_hidden(std::stoi(options["hidden_cursor"]));
	props.set_mouse_mode(WindowProperties::M_relative);
	window->get_graphics_window()->request_properties(props);

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

	//Loading chunks
	player.chunk_x = 0;
	player.chunk_y = 0;
	game::inventory playerHandInventory;
	game::readOptions(universeOptions, universePath + "universe");
	game::chunk::chunksize = std::stoi(universeOptions["chunksize"]);
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
	if (game::isOdd(game::chunk::chunksize)) {
		game::errorOut("Chunksize must be even, not odd!");
		exit(1);
	}
	{

		//Load universe
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

		//Load profiles
		std::ifstream profile(universePath + "profiles/" + player.playerName + ".prof");
		if (profile.fail()) {
			game::warningOut("Could not find player profile. Creating...");
			game::mkdir(universePath + "profiles");
			std::ofstream createProfile(universePath + "profiles/" + player.playerName + ".prof");
			createProfile << "x=" << player.model.get_x() << std::endl;
			createProfile << "y=" << player.model.get_y() << std::endl;
			createProfile << "z=" << player.model.get_z() << std::endl;
			createProfile << "handInventory=" <<
				"data/assets/blockproperties/grass.blockproperties|" <<
				"data/assets/blockproperties/rotational-complex.blockproperties|" <<
				"data/assets/blockproperties/log.blockproperties|" <<
				"data/assets/blockproperties/stone.blockproperties|" <<
				"data/assets/blockproperties/leaves.blockproperties|" <<
				"data/assets/blockproperties/lightblock.blockproperties|" <<
				"data/assets/blockproperties/sand.blockproperties|" <<
				"data/assets/blockproperties/water.blockproperties|" <<
				std::endl;
			createProfile.close();
			playerHandInventory.appendItem(game::item("data/assets/blockproperties/grass.blockproperties", 1));
			playerHandInventory.appendItem(game::item("data/assets/blockproperties/rotational-complex.blockproperties", 1));
			playerHandInventory.appendItem(game::item("data/assets/blockproperties/log.blockproperties", 1));
			playerHandInventory.appendItem(game::item("data/assets/blockproperties/stone.blockproperties", 1));
			playerHandInventory.appendItem(game::item("data/assets/blockproperties/leaves.blockproperties", 1));
			playerHandInventory.appendItem(game::item("data/assets/blockproperties/lightblock.blockproperties", 1));
			playerHandInventory.appendItem(game::item("data/assets/blockproperties/sand.blockproperties", 1));
			playerHandInventory.appendItem(game::item("data/assets/blockproperties/water.blockproperties", 1));
		} else {
			std::map<std::string, std::string> vector;
			std::string line;
			while (std::getline(profile, line)) {
				vector[game::split(line, "=")[0]] = game::split(line, "=")[1];
			}
			player.model.set_x(std::stod(vector["x"]));
			player.model.set_y(std::stod(vector["y"]));
			player.model.set_z(std::stod(vector["z"]));

			std::vector<std::string> stringItems = game::split(vector["handInventory"], "|");
			playerHandInventory.resize(stringItems.size());
			for (size_t i = 0; i < stringItems.size(); i++) {
				game::item item(stringItems[i], 1);
				playerHandInventory.setItem(i, item);
			}
		}
		profile.close();
	}

	//Hotbar
	std::vector<NodePath> inventory;
	std::vector<NodePath> tool;
	std::vector<NodePath> place;
	CardMaker hand_inventory("hand_inventory");
	for (int i = -5; i < 6; i++) {
		NodePath hand_inventoryNode = NodePath("slot");
		hand_inventoryNode.set_sx(0.2);
		hand_inventoryNode.set_sz(0.2);
		hand_inventoryNode.set_pos(i / static_cast<float>(5) - hand_inventoryNode.get_sx() / 2, 0, -0.85 - hand_inventoryNode.get_sz() / 2);
		hand_inventoryNode.reparent_to(window->get_aspect_2d());

		NodePath card = NodePath(hand_inventory.generate());
		game::setTexture(card, gamePath + (std::string)"models/textures/png/hand-inventory-all.png");
		card.set_transparency(TransparencyAttrib::M_alpha);
		card.reparent_to(hand_inventoryNode);

		game::object something(playerHandInventory.getItem(i + 3).configPath, window, framework, false, false, hand_inventoryNode);
		something.model.set_pos_hpr(0 + something.model.get_sx() / 2, 0, 0 + something.model.get_sz() / 2, 0, 0, 0);
		something.model.set_scale(0.3);

		Texture* texture = TexturePool::get_global_ptr()->load_cube_map("models/textures/png/grass-#.png");
		texture->set_minfilter(SamplerState::FilterType::FT_nearest);
		texture->set_magfilter(SamplerState::FilterType::FT_nearest);
		something.model.set_texture(texture);
		something.model.set_tex_gen(TextureStage::get_default(), RenderAttrib::M_world_position);
		something.model.set_tex_projector(TextureStage::get_default(), window->get_render_2d(), something);

		place.push_back(something);

		if (i == -5) {
			i++;
			tool.push_back(card);
		} else {
			inventory.push_back(card);
		}
	}

	game::entity blocky("data/assets/entityproperties/test.entityproperties", window, framework, false);
	blocky.model = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/blocky.egg");
	blocky.model.set_scale(0.5);
	blocky.model.set_pos(0, 0, 20);
	blocky.model.reparent_to(window->get_render());

	CollisionNode* cSphere_node2 = new CollisionNode("Sphere");
	cSphere_node2->add_solid(new CollisionSphere(0, 0, 0, 4));
	NodePath blockyC = blocky.model.attach_new_node(cSphere_node2);

	CollisionHandlerPusher pusher;
	pusher.add_in_pattern("Something");
	pusher.add_out_pattern("Something2");

	framework.define_key("Something", "", game::testIfPlayerOnGround, 0);
	framework.define_key("Something2", "", game::testIfPlayerOnGround, (void*)1);

	framework.define_key("Something", "", game::getCollidedNodePath, 0);
	framework.define_key("Something2", "", game::getCollidedNodePath, (void*)1);

	CollisionTraverser* traverser = new CollisionTraverser();

	//Apply show collision settings
	if (std::stoi(options["show_ray-collisions"])) {
		player.pickerTraverser.show_collisions(window->get_render());
	}
	if (std::stoi(options["show_block-collisions"])) {
		traverser->show_collisions(window->get_render());
	}

	traverser->add_collider(player.collisionNodePath, &pusher);
	pusher.add_collider(player.collisionNodePath, player.model);

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

	PerlinNoise3 perlinNoise(128, 128, 128, 256, seed);

	//Add task chains
	AsyncTaskChain* generateChunksChain = AsyncTaskManager::get_global_ptr()->make_task_chain("generateChunksChain");
	generateChunksChain->set_num_threads(1);

	//Add tasks
	PT(GenericAsyncTask) computePlayerZVelocity = new GenericAsyncTask("calculatePlayerZVelocity", task::computePlayerZVelocity, NULL);
	AsyncTaskManager::get_global_ptr()->add(computePlayerZVelocity);

	PT(GenericAsyncTask) setPlayerChunkPos = new GenericAsyncTask("setPlayerChunkPos", task::setPlayerChunkPos, NULL);
	AsyncTaskManager::get_global_ptr()->add(setPlayerChunkPos);

	std::tuple<WindowFramework*, PandaFramework*, PerlinNoise3*> tuple = { window, &framework, &perlinNoise };
	PT(GenericAsyncTask) generateChunks = new GenericAsyncTask("generateChunks", task::generateChunks, (void*)&tuple);
	generateChunks->set_task_chain("generateChunksChain");
	AsyncTaskManager::get_global_ptr()->add(generateChunks);

	PT(GenericAsyncTask) updateHotbar = new GenericAsyncTask("updateHotbar", task::updateHotbar, (void*)&inventory);
	AsyncTaskManager::get_global_ptr()->add(updateHotbar);

	//Reading settings from settings map

	double x_speed = std::stof(options["x_speed"]);
	double y_speed = std::stof(options["y_speed"]);
	double z_speed = std::stof(options["z_speed"]);

	double sneak_distance = std::stod(options["sneak-distance"]);
	bool chunk_exists = false;

	NodePath block("Block");

	double heading;
	double pitch;

	//Testing entities
	game::entity entity("data/assets/entityproperties/test.entityproperties", window, framework, false);
	entity.model.set_pos(0, 0, 15);
	entity.model.reparent_to(window->get_render());

	std::chrono::time_point<std::chrono::steady_clock> timepoint;

	double light_X = 0;


	//Note: it isn't particularly efficient to make every face as a separate Geom.
	//instead, it would be better to create one Geom holding all of the faces.
	PT(Geom) square0 = game::makeSquare(-1, -1, -1, 1, -1, 1);
	PT(Geom) square1 = game::makeSquare(-1, 1, -1, 1, 1, 1);
	PT(Geom) square2 = game::makeSquare(-1, 1, 1, 1, -1, 1);
	PT(Geom) square3 = game::makeSquare(-1, 1, -1, 1, -1, -1);
	PT(Geom) square4 = game::makeSquare(-1, -1, -1, -1, 1, 1);
	PT(Geom) square5 = game::makeSquare(1, -1, -1, 1, 1, 1);
	GeomNode snode("square");
	snode.add_geom(square0);
	snode.add_geom(square1);
	snode.add_geom(square2);
	snode.add_geom(square3);
	snode.add_geom(square4);
	snode.add_geom(square5);
	NodePath cube = window->get_render().attach_new_node(DCAST(PandaNode, &snode));
	cube.set_two_sided(true);
	cube.set_pos(5, 5, 10);

	//Note: it isn't particularly efficient to make every face as a separate Geom.
	//instead, it would be better to create one Geom holding all of the faces.
	PT(Geom) square01 = game::makeSquare(-1, -1, -1, 1, -1, 1);
	PT(Geom) square11 = game::makeSquare(-1, 1, -1, 1, 1, 1);
	PT(Geom) square21 = game::makeSquare(-1, 1, 1, 1, -1, 1);
	PT(Geom) square31 = game::makeSquare(-1, 1, -1, 1, -1, -1);
	PT(Geom) square41 = game::makeSquare(-1, -1, -1, -1, 1, 1);
	PT(Geom) square51 = game::makeSquare(1, -1, -1, 1, 1, 1);
	GeomNode snode1("square");
	snode1.add_geom(square01);
	snode1.add_geom(square11);
	snode1.add_geom(square21);
	snode1.add_geom(square31);
	snode1.add_geom(square41);
	snode1.add_geom(square51);
	NodePath cube1 = window->get_render().attach_new_node(DCAST(PandaNode, &snode1));
	cube1.set_pos(-5, 5, 10);

	//Main loop
	while (framework.do_frame(Thread::get_current_thread()) && shouldRun) {

		player.doCameraControl(window);

		//game::chunk chunk(player.chunk_x, player.chunk_y);		//Create new chunk
		//chunk.generateChunk(window, framework, perlinNoise);	//Apply the generateChunk function on the new chunk

		if (mouseInGame) {

			if (keys["w"]) {
				player.model.set_y(player.model, y_speed * ClockObject::get_global_clock()->get_dt());
			}
			if (keys["s"]) {
				player.model.set_y(player.model, - y_speed * ClockObject::get_global_clock()->get_dt());
			}
			if (keys["a"]) {
				player.model.set_x(player.model, - x_speed * ClockObject::get_global_clock()->get_dt());
			}
			if (keys["d"]) {
				player.model.set_x(player.model, x_speed * ClockObject::get_global_clock()->get_dt());
			}
			if (keys["lshift"]) {
				if (player.onGround && !player.sneaking) {
					player.sneaking = true;
					y_speed /= 2;
					x_speed /= 2;
					player.firstPerson.set_z(player.firstPerson.get_z() - sneak_distance * ClockObject::get_global_clock()->get_dt());
				}
			} else if (!keys["lshift"]) {
				if (player.onGround && player.sneaking) {
					player.sneaking = false;
					y_speed *= 2;
					x_speed *= 2;
					player.firstPerson.set_z(player.firstPerson.get_z() + sneak_distance * ClockObject::get_global_clock()->get_dt());
				}
			}
			if (keys["space"]) {
				if (player.onGround) {
					if (!player.sneaking) {
						player.velocity = -0.25;
					} else if (player.sneaking) {
						player.velocity = -0.45;
					}
					player.onGround = false;
				}
			}
			if (keys["q"]) {
				for (std::pair<int, int> pair : game::chunk::loaded_chunks) {
					std::cout << pair.first << "	" << pair.second << std::endl;
				}
				entity.update();
				keys["q"] = false;
			}
			if (keys["r"]) {
				player.model.set_z(30);
				player.velocity = 0;
			}
			if (keys["f2"]) {
				std::string filename = "screenshots/" + game::getConvertedDateTime() + ".png";
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
				player.setFirstPersonCamera(window);
				keys["f3"] = false;
			}
			if (keys["f4"]) {		//Increase FOV
				window->get_camera(0)->get_lens()->set_fov(window->get_camera(0)->get_lens()->get_fov()+10);
				player.setThirdPersonCamera(window);
				keys["f4"] = false;
			}
			if (keys["f6"]) {
				SceneGraphAnalyzer sga;
				sga.add_node(window->get_render().node());
				game::setTextColor(color::FG_LIGHTCYAN);
				sga.write(std::cerr);
				game::setTextColor(color::FG_WHITE);
				keys["f6"] = false;
			}

			
		} else {
			if (keys["q"]) {		//Crash game
				game::importantInfoOut("Crashing game...");
				exit(1);			// Code 1 is used because we crashed the game
			}
		}
		if (keys["escape"]) {
			pauseMenu(window);
			if (mouseInGame) {
				quitSaveButton.hide();
				generateUniverse.hide();
				returnToGameButton.hide();
				cursor.show();
			} else {
				quitSaveButton.show();
				generateUniverse.show();
				returnToGameButton.show();
				cursor.hide();
			}
			keys["escape"] = false;
		}

		//Reset mouse clicks
		keys["mouse1"] = false;
		keys["mouse3"] = false;
	}

	//Saving chunks
	{
		//Save profiles
		game::mkdir(universePath + "profiles");
		std::ofstream profile(universePath + "profiles/" + player.playerName + ".prof", std::ios::out | std::ios::trunc);
		if (profile.fail()) {
			game::errorOut("Failed to create player profile. The path was: " + universePath + "profiles/" + player.playerName + ".prof" + "!");
		} else {
			profile << "x=" << player.model.get_x() << std::endl;
			profile << "y=" << player.model.get_y() << std::endl;
			profile << "z=" << player.model.get_z() << std::endl;
			profile << "handInventory=";
			for (size_t i = 0; i < playerHandInventory.slots; i++) {
				profile << playerHandInventory.getItem(i).configPath << "|";
			}
			profile << std::endl;
		}
		profile.close();

		//Save universe
		std::ofstream updateIndex(universePath + "index", std::ios::out | std::ios::trunc);
		terrainAnimationShouldRun = true;
		std::thread saving_animation_thread(game::terrainAnimation, "Saving universe");
		for (game::chunk chunk : game::chunks) {
			updateIndex << chunk.x << "." << chunk.y << ".chunk" << std::endl;
			if (chunk.saveChunk()) {
				break;					//Chunk failed to save correctly!
			}
		}
		updateIndex.close();

		terrainAnimationShouldRun = false;
		saving_animation_thread.join();
	}
	std::string filename = universePath + "lock";
	std::remove(filename.c_str());

	framework.close_framework();
	game::logOut("Closing...");

	if (!std::stoi(options["close_console_without_input"])) {
		game::waitForKeypress();
	}

	return 0;
}