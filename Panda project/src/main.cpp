
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
		WindowProperties props = window->get_graphics_window()->get_properties();
		props.set_cursor_hidden(false);
		window->get_graphics_window()->request_properties(props);
	} else {
		double center_x = window->get_graphics_window()->get_x_size() / static_cast<double>(2);
		double center_y = window->get_graphics_window()->get_y_size() / static_cast<double>(2);
		window->get_graphics_window()->move_pointer(0, center_x, center_y);
		
		//Set cursor hidden
		WindowProperties props = window->get_graphics_window()->get_properties();
		props.set_cursor_hidden(true);
		window->get_graphics_window()->request_properties(props);

		mouseInGame = true;
	}
}
void inventoryMenu(WindowFramework* window) {

	if (mouseInGame) {
		mouseInGame = false;
		
		//Set cursor shown
		WindowProperties props = window->get_graphics_window()->get_properties();
		props.set_cursor_hidden(false);
		window->get_graphics_window()->request_properties(props);
	} else {
		double center_x = window->get_graphics_window()->get_x_size() / static_cast<double>(2);
		double center_y = window->get_graphics_window()->get_y_size() / static_cast<double>(2);
		window->get_graphics_window()->move_pointer(0, center_x, center_y);
		
		//Set cursor hidden
		WindowProperties props = window->get_graphics_window()->get_properties();
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

	//Testing the color of log functions
	game::errorOut("This is an error message.");
	game::importantInfoOut("This is an important message.");
	game::logOut("This is a log message.");
	game::timingInfoOut("This is a timing info message.");
	game::userConfigOut("This is a user info message.");
	game::warningOut("This is a warning message.");

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

	//Picker Ray
	PT(CollisionHandlerQueue) myHandler = new CollisionHandlerQueue();			//Create Handler
	CollisionTraverser myTraverser;												//Create Traverser
	PT(CollisionNode) pickerNode = new CollisionNode("mouseRay");				//Create CollisionNode
	PT(CollisionRay) pickerRay = new CollisionRay();							//Create CollisionRay
	NodePath pickerNP = player.model.attach_new_node(pickerNode);				//Create NodePath for the attached new node
	pickerNode->set_from_collide_mask(GeomNode::get_default_collide_mask());	//Set from collide mask to use
	pickerNode->add_solid(pickerRay);											//Add solid to CollisionNode
	pickerNode->set_into_collide_mask(0);										//Disable into-collisions
	myTraverser.add_collider(pickerNP, myHandler);								//Add collider to traverser
	pickerRay->set_from_lens(window->get_camera(0), 0, 0);						//Adjust pickerRay with set_from_lens method

	//Experimental GUI
	game::button returnToGameButton(framework, game::unpause, 0, 0.15, 0, 0.15, "Return to game");
	returnToGameButton.hide();

	game::button quitSaveButton(framework, game::exitGame, 0, -0.15, 0, -0.15, "Quit and save");
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
				std::endl;
			createProfile.close();
			playerHandInventory.resize(6);
			playerHandInventory.setItem(0, game::item("data/assets/blockproperties/grass.blockproperties", 1));
			playerHandInventory.setItem(1, game::item("data/assets/blockproperties/rotational-complex.blockproperties", 1));
			playerHandInventory.setItem(2, game::item("data/assets/blockproperties/log.blockproperties", 1));
			playerHandInventory.setItem(3, game::item("data/assets/blockproperties/stone.blockproperties", 1));
			playerHandInventory.setItem(4, game::item("data/assets/blockproperties/leaves.blockproperties", 1));
			playerHandInventory.setItem(5, game::item("data/assets/blockproperties/lightblock.blockproperties", 1));
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

		NodePath something = window->load_model(framework.get_models(), "models/egg/block.egg");
		something.set_pos_hpr(0 + something.get_sx() / 2, 0, 0 + something.get_sz() / 2, -112.5, -45, 45);
		something.set_scale(0.3);

		Texture* texture = TexturePool::get_global_ptr()->load_cube_map("models/textures/png/grass-#.png");
		texture->set_minfilter(SamplerState::FilterType::FT_nearest);
		texture->set_magfilter(SamplerState::FilterType::FT_nearest);
		something.set_texture(texture);
		something.set_tex_gen(TextureStage::get_default(), RenderAttrib::M_world_position);
		something.set_tex_projector(TextureStage::get_default(), window->get_render_2d(), something);

		something.reparent_to(hand_inventoryNode);
		place.push_back(something);

		if (i == -5) {
			i++;
			tool.push_back(card);
		} else {
			inventory.push_back(card);
		}
	}


	//Particles
	PT(PointParticleFactory) pt_particle_factory = new PointParticleFactory();
	pt_particle_factory->set_lifespan_base(0.5);
	pt_particle_factory->set_lifespan_spread(0);
	pt_particle_factory->set_mass_base(1.0);
	pt_particle_factory->set_mass_spread(0);
	pt_particle_factory->set_terminal_velocity_base(400);
	pt_particle_factory->set_terminal_velocity_spread(1);

	PT(SphereVolumeEmitter) sphere_emitter = new SphereVolumeEmitter;
	sphere_emitter->set_emission_type(SphereVolumeEmitter::ET_RADIATE);
	sphere_emitter->set_radius(3.0);
	// negative values emit the particles toward the sphere center
	sphere_emitter->set_amplitude(1);
	sphere_emitter->set_amplitude_spread(0);
	sphere_emitter->set_offset_force(LVector3(0, 0, 0));
	sphere_emitter->set_explicit_launch_vector(LVector3(1, 0, 0));
	sphere_emitter->set_radiate_origin(LPoint3(0, 0, 0));

	PT(PointParticleRenderer) pt_particle_rend = new PointParticleRenderer();
	pt_particle_rend->set_alpha_mode(BaseParticleRenderer::PR_ALPHA_OUT);
	pt_particle_rend->set_user_alpha(1);
	pt_particle_rend->set_point_size(2.0);
	pt_particle_rend->set_start_color(LColor(1, 0, 0, 1)); // alpha value is ignored
	pt_particle_rend->set_end_color(LColor(1, 1, 0, 1));
	pt_particle_rend->set_blend_type(PointParticleRenderer::PointParticleBlendType::PP_BLEND_LIFE);
	pt_particle_rend->set_blend_method(BaseParticleRenderer::ParticleRendererBlendMethod::PP_BLEND_LINEAR);
	//pt_particle_rend->set_color_blend_mode(ColorBlendAttrib::Mode::M_inv_subtract);
	//pt_particle_rend->set_ignore_scale(false);

	PT(ParticleSystem) particle_sys = new ParticleSystem();
	particle_sys->set_pool_size(1024);
	particle_sys->set_birth_rate(0.01);
	particle_sys->set_litter_size(10);
	particle_sys->set_litter_spread(2);
	particle_sys->set_local_velocity_flag(true);
	//particle_sys->set_spawn_on_death_flag(true); // this caused an exception!!
	particle_sys->set_system_grows_older_flag(true);
	particle_sys->set_system_lifespan(3.0);
	particle_sys->set_active_system_flag(true);
	// use it to advance system age, or start at some age
	//particle_sys->set_system_age(5.0);
	// system_age is updated only when set_system_grows_older_flag(true);
	// get_system_age() returns 0 unless system_grows_older_flag is set

	particle_sys->set_factory(pt_particle_factory);
	particle_sys->set_renderer(pt_particle_rend);
	particle_sys->set_emitter(sphere_emitter);
	// if spawn and render parents should be different
	//particle_sys->set_spawn_render_node_path(window->get_render());
	particle_sys->set_render_parent(window->get_render());

	ParticleSystemManager particle_sys_mgr;
	particle_sys_mgr.attach_particlesystem(particle_sys);


	NodePath blocky = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/blocky.egg");
	blocky.set_scale(0.5);
	blocky.set_pos(0, 0, 20);
	blocky.reparent_to(window->get_render());

	CollisionNode* cSphere_node2 = new CollisionNode("Sphere");
	cSphere_node2->add_solid(new CollisionSphere(0, 0, 0, 4));
	NodePath blockyC = blocky.attach_new_node(cSphere_node2);
	
	NodePath panda("panda");
	panda.set_scale(0.5);
	panda.set_pos(0, 0, 0);
	panda.reparent_to(window->get_render());
	panda.hide();

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
		myTraverser.show_collisions(window->get_render());
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
	PT(GenericAsyncTask) computePlayerZVelocity = new GenericAsyncTask("calculatePlayerZVelocity", task::computePlayerZVelocity, (void*)&panda);
	AsyncTaskManager::get_global_ptr()->add(computePlayerZVelocity);

	PT(GenericAsyncTask) setPlayerChunkPos = new GenericAsyncTask("setPlayerChunkPos", task::setPlayerChunkPos, NULL);
	AsyncTaskManager::get_global_ptr()->add(setPlayerChunkPos);

	std::tuple<WindowFramework*, PandaFramework*, PerlinNoise3*> tuple = { window, &framework, &perlinNoise };
	PT(GenericAsyncTask) generateChunks = new GenericAsyncTask("generateChunks", task::generateChunks, (void*)&tuple);
	generateChunks->set_task_chain("generateChunksChain");
	AsyncTaskManager::get_global_ptr()->add(generateChunks);

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

	//Main loop
	while (framework.do_frame(Thread::get_current_thread()) && shouldRun) {

		particle_sys_mgr.do_particles(ClockObject::get_global_clock()->get_dt(), particle_sys);

		//plnp.set_pos(cos(light_X)*10, sin(light_X)*10, 5);
		blocky.set_pos(cos(light_X)*10, sin(light_X)*10, 5);

		light_X += ClockObject::get_global_clock()->get_dt();

		if ((player.collidedNodePath == entity.model) && (player.onGround)) {
			player.model.set_pos(entity.model.get_x(), entity.model.get_y(), player.model.get_z());
		}
		entity.update();

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

		traverser->traverse(window->get_render());		//Check collisions and call pusher if a collision is detected

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
				block_chunk_x = (int)(block.get_x() - game::chunk::chunksize) / game::chunk::chunksize;
			} else {
				block_chunk_x = (int)block.get_x() / game::chunk::chunksize;
			}
			if (block.get_y() < 0) {
				block_chunk_y = (int)(block.get_y() - game::chunk::chunksize) / game::chunk::chunksize;
			} else {
				block_chunk_y = (int)block.get_y() / game::chunk::chunksize;
			}

			if (keys["mouse1"]) {
				if (mouseInGame) {
					game::chunk chunk = game::chunks[game::chunk::index[std::pair<int, int>(block_chunk_x, block_chunk_y)]];		//Get chunk containing the block
					chunk.objects[std::stoull(block.get_tag("chunkObjectId"))].model.remove_node();									//Remove node
					for (NodePath value : chunk.objects[std::stoull(block.get_tag("chunkObjectId"))].lights) {						//Remove lights
						window->get_render().clear_light(value);
					}
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
					std::string configPath = playerHandInventory.getItem(handInventoryIndex).configPath;

					game::object object(configPath, window, framework, false, false);
					object.model.set_pos(block.get_x() + surface.get_x() * 2, block.get_y() + surface.get_y() * 2, block.get_z() + surface.get_z() * 2);

					if (object.model.get_x() < 0) {
						block_chunk_x = (int)(object.model.get_x() - game::chunk::chunksize) / game::chunk::chunksize;
					} else {
						block_chunk_x = (int)object.model.get_x() / game::chunk::chunksize;
					}
					if (object.model.get_y() < 0) {
						block_chunk_y = (int)(object.model.get_y() - game::chunk::chunksize) / game::chunk::chunksize;
					} else {
						block_chunk_y = (int)object.model.get_y() / game::chunk::chunksize;
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
						object.model.set_hpr(heading * 180, pitch * 180, 0);
					}

					game::chunk chunk = game::chunks[game::chunk::index[std::pair<int, int>(block_chunk_x, block_chunk_y)]];
					
					object.model.set_tag("chunk", std::to_string(block_chunk_x) + "," + std::to_string(block_chunk_y));
					object.model.set_tag("id", std::to_string(object.id));
					object.model.set_tag("chunkObjectId", std::to_string(chunk.objects.size()));

					object.model.set_shader_auto();

					object.model.ls();

					chunk.objects.push_back(object);
					game::chunks[game::chunk::index[std::pair<int, int>(block_chunk_x, block_chunk_y)]] = chunk;

					keys["mouse3"] = false;
				}
			}
		} else {
			// No targeted block
			block.hide_bounds();
		}

		//Set text to the new values
		text->set_text("X: " + std::to_string(player.model.get_x()) + "\nY: " + std::to_string(player.model.get_y()) + "\nZ: " + std::to_string(player.model.get_z()));
		text2->set_text("H: " + std::to_string(player.model.get_h()) + "\nP: " + std::to_string(player.model.get_p()) + "\nR: " + std::to_string(player.model.get_r()));
		text3->set_text("Chunk X: " + std::to_string(player.chunk_x) + "\nChunk Y: " + std::to_string(player.chunk_y));
		fovText->set_text("VFov: " + std::to_string(DCAST(Camera, player.firstPerson.node())->get_lens()->get_vfov()) + "\nHFov: " + std::to_string(DCAST(Camera, player.firstPerson.node())->get_lens()->get_hfov()));

		if (mouseInGame) {
			if (window->get_graphics_window()) {
				if (window->get_graphics_window()->get_pointer(0).get_in_window()) {
					center_x = window->get_graphics_window()->get_x_size() / static_cast<double>(2);
					center_y = window->get_graphics_window()->get_y_size() / static_cast<double>(2);

					double move_x = std::floor(center_x - window->get_graphics_window()->get_pointer(0).get_x());
					double move_y = std::floor(center_y - window->get_graphics_window()->get_pointer(0).get_y());

					if (keys["v"]) {
						offset_r += move_x / camera_x_speed;

						if (offset_r < 90 && offset_r > -90) {
							player.model.set_r(offset_r);
						} else {
							offset_r -= move_x / 5;
						}
					} else {
						offset_h += move_x / camera_x_speed;
						player.model.set_h(std::fmod(offset_h, 360));

						// Reset rotation
						offset_r = 0;
						player.model.set_r(offset_r);
					}

					offset_p += move_y / camera_y_speed;
					panda.set_h(std::fmod(offset_h, 360));

					//Adjust the collision box so its pitch doesn't change
					player.collisionNodePath.set_p(offset_p - offset_p * 2);

					//Adjust the collision box so its rotation doesn't change
					//cameraC.set_r(offset_r - offset_r * 2);
					//Not fixed yet

					if (!keys["v"]) {
						if (offset_p < 90 && offset_p > -90) {
							player.model.set_p(offset_p);
						} else {
							offset_p -= move_y / 5;
						}
					}

					window->get_graphics_window()->move_pointer(0, center_x, center_y);		//Reset pointer to 0, 0
				}
			}


			if (keys["w"]) {
				player.model.set_y(panda, y_speed * ClockObject::get_global_clock()->get_dt());
				panda.set_y(panda, y_speed * ClockObject::get_global_clock()->get_dt());
			}
			if (keys["s"]) {
				player.model.set_y(panda, - y_speed * ClockObject::get_global_clock()->get_dt());
				panda.set_y(panda, - y_speed * ClockObject::get_global_clock()->get_dt());
			}
			if (keys["a"]) {
				player.model.set_x(player.model, - x_speed * ClockObject::get_global_clock()->get_dt());
				panda.set_x(player.model, - x_speed * ClockObject::get_global_clock()->get_dt());
			}
			if (keys["d"]) {
				player.model.set_x(player.model, x_speed * ClockObject::get_global_clock()->get_dt());
				panda.set_x(player.model, x_speed * ClockObject::get_global_clock()->get_dt());
			}
			if (keys["lshift"]) {
				if (player.flying) {
					player.model.set_z(player.model.get_pos().get_z() - z_speed * ClockObject::get_global_clock()->get_dt());
				} else if (player.onGround && !player.sneaking) {
					player.sneaking = true;
					player.collisionNodePath.set_z(player.collisionNodePath.get_z() + sneak_distance * ClockObject::get_global_clock()->get_dt());
					player.model.set_z(player.model.get_pos().get_z() - sneak_distance * ClockObject::get_global_clock()->get_dt());
				}
			} else if (!keys["lshift"]) {
				if (player.onGround && player.sneaking) {
					player.sneaking = false;
					player.collisionNodePath.set_z(player.collisionNodePath.get_z() - sneak_distance * ClockObject::get_global_clock()->get_dt());
					player.model.set_z(player.model.get_pos().get_z() + sneak_distance * ClockObject::get_global_clock()->get_dt());
				}
			}
			if (keys["space"]) {
				std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - timepoint;
				if (player.flying && duration.count() > 0.05 && duration.count() < 0.275) {
					player.flying = false;
					player.velocity = 0;
				} else if (player.flying) {
					player.model.set_z(player.model.get_pos().get_z() + z_speed * ClockObject::get_global_clock()->get_dt());
				} else if (!player.onGround && duration.count() > 0.05 && !player.flying) {
					player.flying = true;
				} else if (player.onGround) {
					if (!player.sneaking) {
						player.velocity = -0.25;
					} else if (player.sneaking) {
						player.velocity = -0.45;
					}
					player.onGround = false;
				}
				timepoint = std::chrono::high_resolution_clock::now();
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
				keys["f3"] = false;
			}
			if (keys["f4"]) {		//Increase FOV
				window->get_camera(0)->get_lens()->set_fov(window->get_camera(0)->get_lens()->get_fov()+10);
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
						player.model.set_r(offset_r);
					} else {
						offset_r -= move_x / 5;
					}
				} else {
					offset_h += move_x / camera_x_speed;
					player.model.set_h(offset_h);

					// Reset rotation
					offset_r = 0;
					player.model.set_r(offset_r);
				}

				offset_p += move_y / camera_y_speed;
				panda.set_h(offset_h);

				//Adjust the collision box so its pitch doesn't change
				player.collisionNodePath.set_p(offset_p - offset_p * 2);

				//Adjust the collision box so its rotation doesn't change
				//cameraC.set_r(offset_r - offset_r * 2);
				//Not fixed yet

				if (!keys["v"]) {
					if (offset_p < 90 && offset_p > -90) {
						player.model.set_p(offset_p);
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
		if (keys["escape"]) {
			pauseMenu(window);
			if (mouseInGame) {
				quitSaveButton.hide();
				returnToGameButton.hide();
			} else {
				quitSaveButton.show();
				returnToGameButton.show();
			}
			keys["escape"] = false;
		}
		if (keys["e"]) {
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
				inventoryMenu(window);
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
				inventoryMenu(window);
			}

			keys["e"] = false;
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
			chunk.saveChunk();
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