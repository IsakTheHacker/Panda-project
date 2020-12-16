#include "./header/pandaIncludes.h"

int handInventoryIndex;
std::map<std::string, bool> keys;
bool shouldRun = true;
bool terrainAnimationShouldRun;
bool devMode = false;
bool mouseInGame = true;
bool playerOnGround = false;
std::string gamePath = "./";

//My libraries
#include "./header/cppExtension.h"
#include "./header/gameFunctions.h"
#include "./header/gameLanguage.h"
#include "./header/constantVars.h"

// Global stuff
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject) globalClock = ClockObject::get_global_clock();
PT(TextNode) text = new TextNode("textnode");
NodePath camera;
WindowFramework* window;



// Cool stuff
PT(MouseWatcher) mouseWatcher;
PT(CollisionRay) pickerRay;
CollisionTraverser myTraverser = CollisionTraverser("ctraverser");
PT(CollisionHandlerQueue) myHandler;
PT(CollisionNode) pickerNode;
NodePath pickerNP;

#include "./header/gameClasses.h"

// Set up the GeoMipTerrain
GeoMipTerrain* terrain;



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



// And the task, outside main:
AsyncTask::DoneStatus UpdateTerrain(GenericAsyncTask* task, void* data) {
	terrain->update();
	return AsyncTask::DS_cont;
}


int main(int argc, char* argv[]) {

	//Checking if any arguments was given at startup
	if (argc > 2) {
		game::warningOut("Too many arguments was given, only one is allowed!");
	} else if (argc == 2) {
		if (std::find(std::begin(game::allowed_parameters), std::end(game::allowed_parameters), argv[1]) != std::end(game::allowed_parameters)) {
			if (argv[1] == game::allowed_parameters[0]) {
				devMode = true;
				game::importantInfoOut("Game was started in devmode!");
			} else if (argv[1] == game::allowed_parameters[1]) {
				gamePath = "../../../Panda Project/";
				devMode = true;
				game::importantInfoOut("Game was started in Visual Studio devmode!");
			}
		} else {
			game::warningOut("An unknown argument was given!");
		}
	}

	//Starting debug input thread if game was started in devmode!
	//std::thread debugInputThread(game::takeDebugInput);

	//Changing window size
	load_prc_file_data("", "win-size 1280 720");

	//Creating folders and files
	game::runPyScript("data/scripts/makeDirectories.py");
	game::runPyScript("data/scripts/createOptionsFile.py");

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
	framework.set_window_title("The Panda Project: Alpha 0.1.0");

	// Open the window
	WindowFramework* window = framework.open_window();
	camera = window->get_camera_group();

	// Cool stuff
	pickerNode = new CollisionNode("mouseRay");
	pickerNP = camera.attach_new_node(pickerNode);
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


	LPoint2 mpos = mouseWatcher->get_mouse();

	// This makes the ray's origin the camera and makes the ray point
	// to the screen coordinates of the mouse.

	pickerRay->set_from_lens(window->get_camera(0), mpos.get_x(), mpos.get_y());

	// Change background color to black
	window->get_graphics_window()->get_active_display_region(0)->set_clear_color(LColorf(0, 0, 0, 1));

	//Keyboard input
	window->enable_keyboard();
	#include "./header/keyDefinitions.h"

	PT(TextNode) text;
	text = new TextNode("node name");
	NodePath textNodePath = window->get_aspect_2d().attach_new_node(text);
	textNodePath.set_scale(0.07);
	textNodePath.set_pos(-1.25,0,0.9);

	PT(TextNode) text2;
	text2 = new TextNode("node name2");
	NodePath textNodePath2 = window->get_aspect_2d().attach_new_node(text2);
	textNodePath2.set_scale(0.07);
	textNodePath2.set_pos(-1.25, 0, 0.65);

	// Crosshair
	CardMaker cardmaker("crosshair");
	NodePath cursor(cardmaker.generate());
	game::setTexture(cursor, gamePath + (std::string)"models/textures/png/crosshair.png");
	cursor.set_sx(0.1);
	cursor.set_sz(0.1);
	cursor.set_pos(0 - cursor.get_sx() / 2, 0, 0 - cursor.get_sz() / 2);
	cursor.set_transparency(TransparencyAttrib::M_alpha);
	cursor.reparent_to(window->get_aspect_2d());

	std::vector<NodePath> inventory;

	for (int i = -5; i < 6; i++) {
		std::cout << "Run..." << std::endl;
		CardMaker hand_inventory("hand_inventory" + i);
		NodePath hand_inventoryNode(hand_inventory.generate());
		game::setTexture(hand_inventoryNode, gamePath + (std::string)"models/textures/png/hand-inventory-all.png");
		hand_inventoryNode.set_sx(0.2);
		hand_inventoryNode.set_sz(0.2);
		hand_inventoryNode.set_pos(i/static_cast<float>(5) - hand_inventoryNode.get_sx() / 2, 0, -0.85 - hand_inventoryNode.get_sz() / 2);
		hand_inventoryNode.set_transparency(TransparencyAttrib::M_alpha);
		hand_inventoryNode.reparent_to(window->get_aspect_2d());
		inventory.push_back(hand_inventoryNode);
	}

	std::cout << inventory[5] << std::endl;

	terrainAnimationShouldRun = true;
	std::thread terrain_animation_thread(game::terrainAnimation, "Loading terrain");

	{
		std::ifstream index("universes/Test/index");
		std::string line;
		while (std::getline(index, line)) {
			game::readChunk(window, framework, "universes/Test/" + line, std::stoi(game::split(line, ".")[0]), std::stoi(game::split(line, ".")[1]));
		}
		index.close();
	}
	
	terrainAnimationShouldRun = false;
	terrain_animation_thread.join();


	PT(PGButton) my_button;
	my_button = new PGButton("MyButton");
	my_button->setup("Button");
	PT(Texture) button_ready = TexturePool::load_texture("button.png");
	PT(Texture) button_rollover = TexturePool::load_texture("button_active.png");
	PT(Texture) button_pressed = TexturePool::load_texture("button_pressed.png");
	PT(Texture) button_inactive = TexturePool::load_texture("button_inactive.png");

	// PGFrameStyle is a powerful way to change the appearance of the button:
	PGFrameStyle MyStyle = my_button->get_frame_style(0); // frame_style(0): ready state
	MyStyle.set_type(PGFrameStyle::T_flat);

	MyStyle.set_texture(button_ready);    my_button->set_frame_style(0, MyStyle);
	MyStyle.set_texture(button_rollover); my_button->set_frame_style(1, MyStyle);
	MyStyle.set_texture(button_pressed);  my_button->set_frame_style(2, MyStyle);
	MyStyle.set_texture(button_inactive); my_button->set_frame_style(3, MyStyle);

	NodePath defbutNP = window->get_aspect_2d().attach_new_node(my_button);
	defbutNP.set_scale(0.1);
	//defbutNP.set_pos(0, 0, 0);
	std::cout << defbutNP.get_pos() << std::endl;
	defbutNP.set_pos(0 - defbutNP.get_sx()/2, 0, 0 - defbutNP.get_sz()/2);
	//defbutNP.show_bounds();
	defbutNP.hide();
	//defbutNP.set_pos(defbutNP.get_sx(), 3, 0);

	ButtonHandle button_handle{};

	// Setup callback function
	framework.define_key(my_button->get_click_event(button_handle), "button press", game::key_down, 0);


	NodePath blocky = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/blocky.egg");
	//NodePath blocky = window->load_model(framework.get_models(), "/c/dev/Panda project/Panda project/models/egg/blocky.egg");
	blocky.set_scale(0.5);
	blocky.set_pos(0, 0, 100);
	blocky.reparent_to(window->get_render());

	CollisionNode* cSphere_node2 = new CollisionNode("Sphere");
	cSphere_node2->add_solid(new CollisionSphere(0, 0, 0, 4));
	NodePath blockyC = blocky.attach_new_node(cSphere_node2);
	blockyC.show();
	
	camera.set_z(30);

	NodePath panda = window->load_model(framework.get_models(), "panda.egg");
	panda.set_scale(0.5);
	panda.set_pos(0, 0, 0);
	panda.reparent_to(window->get_render());
	panda.hide();


	CollisionNode* cSphere_node = new CollisionNode("Sphere");
	cSphere_node->add_solid(new CollisionBox(0, 0.8, 0.8, 3));
	NodePath cameraC = camera.attach_new_node(cSphere_node);

	CollisionHandlerPusher pusher;
	pusher.add_in_pattern("Something");
	pusher.add_out_pattern("Something2");
	//pusher.add_again_pattern("%fn-into-%in");

	//framework.define_key("render/camera_group/Sphere-into-render/object/Box", "", game::testIfPlayerOnGround, 0);
	framework.define_key("Something", "", game::testIfPlayerOnGround, 0);
	framework.define_key("Something2", "", game::testIfPlayerOnGround, (void*)1);

	//window->get_render().ls();

	//CollisionHandlerQueue queue;
	CollisionTraverser* traverser = new CollisionTraverser();

	traverser->add_collider(cameraC, &pusher);
	pusher.add_collider(cameraC, camera);

	traverser->traverse(window->get_render());
	//traverser->show_collisions(window->get_render());

	PT(AmbientLight) alight = new AmbientLight("alight");
	alight->set_color(0.2);
	NodePath alnp = window->get_render().attach_new_node(alight);
	window->get_render().set_light(alnp);

	PT(DirectionalLight) d_light = new DirectionalLight("my d_light");
	d_light->set_color(LColor(0.8, 0.8, 0.5, 1));
	NodePath dlnp = window->get_render().attach_new_node(d_light);
	dlnp.set_hpr(0, -90, 0);
	dlnp.set_pos(5, 5, 10);
	window->get_render().set_light(dlnp);



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

	std::string sad = "Hello World";
	/*myTraverser.show_collisions(window->get_render());

	framework.show_collision_solids(window->get_render());*/

	NodePath block;

	double heading;
	double pitch;

	double velocity = 0.0;

	Thread* current_thread = Thread::get_current_thread();
	while (framework.do_frame(current_thread) && shouldRun) {
		if (velocity == 0 && !playerOnGround) {
			velocity = 0.01;
		} else {
			velocity = velocity * 1.05;
		}

		if (playerOnGround) {
			velocity = 0;
		}

		camera.set_z(camera.get_pos().get_z() - velocity);
		panda.set_z(camera.get_pos().get_z() - velocity);

		if (handInventoryIndex < 0) {
			handInventoryIndex = 10;
		} else if (handInventoryIndex > 10) {
			handInventoryIndex = 0;
		}

		for (int i = 0; i < 11; i++) {
			if (i == handInventoryIndex) {
				game::setTexture(inventory[i], gamePath + (std::string)"models/textures/png/hand-inventory-highlighted.png");
			} else if (i != handInventoryIndex) {
				game::setTexture(inventory[i], gamePath + (std::string)"models/textures/png/hand-inventory-all.png");
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
			if (keys["mouse1"]) {
				block.remove_node();
				keys["mouse1"] = false;
			}
			if (keys["mouse3"]) {
				std::vector<NodePath> vec;
				LVector3 notRotatedSurface = surface;
				notRotatedSurface.set_x(0);
				notRotatedSurface.set_y(0);
				notRotatedSurface.set_z(0);

				std::cout << surface << std::endl;
				std::cout << notRotatedSurface << std::endl;

				std::string path;

				NodePath object;
				TexturePool* texturePool = TexturePool::get_global_ptr();
				TextureStage* textureStage = new TextureStage("textureStage2");
				textureStage->set_sort(0);
				textureStage->set_mode(TextureStage::M_replace);
				Texture* texture;
				Texture* texture2;

				NodePath block2;

				if (handInventoryIndex == 0) {
					path = "wedge.egg";
					object = NodePath("object");
					NodePath incline = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/half_block_wedge_incline");
					incline.reparent_to(object);
					NodePath base = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/half_block_wedge_base");
					base.reparent_to(object);

					texture = texturePool->load_cube_map(gamePath + (std::string)"models/textures/png/grass-#.png");
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);
					base.set_texture(texture, 1);

					texture2 = texturePool->load_texture(gamePath + (std::string)"models/textures/png/grass-4.png");
					texture2->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture2->set_magfilter(SamplerState::FilterType::FT_nearest);
					incline.set_texture(texture2, 1);

					object.set_tex_gen(textureStage->get_default(), RenderAttrib::M_world_position);
					object.set_tex_projector(textureStage->get_default(), window->get_render(), object);
				} else if (handInventoryIndex == 1) {
					path = "block.egg";
					object = NodePath("object");
					NodePath block2 = window->load_model(framework.get_models(), gamePath + (std::string)"/models/egg/" + (std::string)path);
					block2.reparent_to(object);

					texture = texturePool->load_cube_map(gamePath + (std::string)"models/textures/png/rotational-complex-#.png");
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);
					block2.set_texture(texture, 1);
				} else {
					path = "block.egg";
					object = NodePath("object");
					NodePath block2 = window->load_model(framework.get_models(), gamePath + (std::string)"models/egg/" + (std::string)path);
					block2.reparent_to(object);

					texture = texturePool->load_cube_map(gamePath + (std::string)"models/textures/png/grass-#.png");
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);
					block2.set_texture(texture, 1);
				}


				object.set_pos(block.get_x() + surface.get_x()*2, block.get_y() + surface.get_y()*2, block.get_z() + surface.get_z()*2);
				object.reparent_to(window->get_render());

				if (keys["r"]) {

					if (surface.get_z() == -1) {
						pitch = 1;
					} else{
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

					std::cout << heading << std::endl;
					std::cout << pitch << std::endl;
					object.set_hpr(heading * 180, pitch * 180, 0);
				}

				
				object.set_tex_gen(textureStage->get_default(), RenderAttrib::M_world_position);
				object.set_tex_projector(textureStage->get_default(), window->get_render(), object);

				if (handInventoryIndex == 0) {
					
				} else if (handInventoryIndex == 1) {
					texture = texturePool->load_cube_map(gamePath + (std::string)"models/textures/png/rotational-complex-#.png");
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);
					block2.set_texture(texture, 1);
				} else {
					texture = texturePool->load_cube_map(gamePath + (std::string)"models/textures/png/grass-#.png");
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);
					block2.set_texture(texture, 1);
				}

				CollisionNode* collisionNode = new CollisionNode("Box");
				collisionNode->add_solid(new CollisionBox(0, 1, 1, 1));
				NodePath collisionNodePath = object.attach_new_node(collisionNode);

				game::object obj(window, framework, vec);

				game::chunk chunk = game::chunks[std::stoi(block.get_tag("chunk"))];
				chunk.objects.push_back(obj);

				keys["mouse3"] = false;
			}
		} else {
			// No targeted block
			block.hide_bounds();
		}

		text->set_text("X: " + std::to_string(floor(camera.get_x())) + "\nY: " + std::to_string(floor(camera.get_y())) + "\nZ: " + std::to_string(floor(camera.get_z())));
		text2->set_text("H: " + std::to_string(floor(camera.get_h())) + "\nP: " + std::to_string(floor(camera.get_p())) + "\nR: " + std::to_string(floor(camera.get_r())));

		if (mouseInGame) {
			if (mouseWatcher->has_mouse()) {
				if (window->get_graphics_window()) {
					center_x = window->get_graphics_window()->get_x_size() / static_cast<double>(2);
					center_y = window->get_graphics_window()->get_y_size() / static_cast<double>(2);

					x = window->get_graphics_window()->get_pointer(0).get_x();
					y = window->get_graphics_window()->get_pointer(0).get_y();

					double move_x = std::floor(center_x - x);
					double move_y = std::floor(center_y - y);

					if (keys["v"]) {
						offset_r += move_x / camera_x_speed;

						if (offset_r < 90 && offset_r > -90) {
							camera.set_r(offset_r);
						} else {
							offset_r -= move_x / 5;
						}
					} else {
						offset_h += move_x / camera_x_speed;
						camera.set_h(offset_h);

						// Reset rotation
						offset_r = 0;
						camera.set_r(offset_r);
					}

					offset_p += move_y / camera_y_speed;
					panda.set_h(offset_h);

					//Adjust the collision box so its pitch doesn't change
					cameraC.set_p(offset_p - offset_p*2);

					if (!keys["v"]) {
						if (offset_p < 90 && offset_p > -90) {
							camera.set_p(offset_p);
						} else {
							offset_p -= move_y / 5;
						}
					}

					window->get_graphics_window()->move_pointer(0, center_x, center_y);
				}
			}
		}

		if (keys["w"]) {
			camera.set_y(panda, 0 + y_speed);
			panda.set_y(panda, 0 + y_speed);
		}
		if (keys["s"]) {
			camera.set_y(panda, 0 - y_speed);
			panda.set_y(panda, 0 - y_speed);
		}
		if (keys["a"]) {
			camera.set_x(camera, 0 - x_speed);
			panda.set_x(camera, 0 - x_speed);
		}
		if (keys["d"]) {
			camera.set_x(camera, 0 + x_speed);
			panda.set_x(camera, 0 + x_speed);
		}
		if (keys["space"]) {
			if (playerOnGround) {
				camera.set_z(camera.get_pos().get_z() + z_speed*17);
				panda.set_z(camera.get_pos().get_z() + z_speed*17);
			}
			keys["space"] = false;
			playerOnGround = false;
			velocity = 0;
		}
		if (keys["lshift"]) {
			camera.set_z(camera.get_pos().get_z() - z_speed);
			panda.set_z(camera.get_pos().get_z() - z_speed);
		}
		if (keys["q"]) {
			/*game::objects.push_back(game::object(window, framework, gamePath + (std::string)"models/egg/simple_house.egg"));
			game::errorOut("Object quantity: " + std::to_string(game::object::object_quantity));
			keys["q"] = false;*/
			game::saveChunk(game::chunks[0]);
			keys["q"] = false;
		}
		if (keys["e"]) {
			game::winds.push_back(game::windObject(window, framework, 0.1, 0.2, 0, 0.1, 1, 1, 1, true));
			//game::objects.push_back(game::object(window, framework, "/c/dev/Panda project/Panda project/models/egg/block.egg"));
			game::winds[game::winds.size()-1].model.set_pos(floor(camera.get_x()), floor(camera.get_y()), floor(camera.get_z()));
			//game::setTexture(game::objects[game::objects.size() - 1].model, "/c/dev/Panda project/Panda project/envir-rock1.jpg");
		}

		//Border checking
		if (options["lower_border"] != "none" && options["lower_border"] != "null") {
			if (camera.get_pos().get_z() < std::stof(options["lower_border"])) {
				camera.set_z(std::stof(options["lower_border"]));
			}
		}
		if (options["upper_border"] != "none" && options["upper_border"] != "null") {
			if (camera.get_pos().get_z() > std::stof(options["upper_border"])) {
				camera.set_z(std::stof(options["upper_border"]));
			}
		}
	}

	//Saving chunks
	terrainAnimationShouldRun = true;
	std::thread saving_animation_thread(game::terrainAnimation, "Saving world");
	{
		for (game::chunk chunk : game::chunks) {
			game::importantInfoOut("chunk");
			game::saveChunk(chunk);
		}
	}
	terrainAnimationShouldRun = false;
	saving_animation_thread.join();

	framework.close_framework();
	//debugInputThread.detach();
	game::logOut("Closing...");

	if (!std::stoi(options["close_console_without_input"])) {
		game::waitForKeypress();
	}

	return 0;
}