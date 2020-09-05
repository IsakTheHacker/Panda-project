//C++ built-in libraries
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <chrono>
#include <vector>

//Python library
#define PY_SSIZE_T_CLEAN
#include <Python.h>

//Panda3d libraries
#include "pgButton.h"
#include "pandaFramework.h"
#include "pandaSystem.h"
#include "genericAsyncTask.h"
#include "asyncTaskManager.h"
#include <mouseWatcher.h>

#include "collisionTraverser.h"
#include "collisionHandlerPusher.h"
#include "collisionNode.h"
#include "collisionHandlerFluidPusher.h"
#include "collisionSphere.h"

std::map<std::string, bool> keys;
bool shouldRun = true;

//My libraries
#include "../cppExtension.h"
#include "../gameFunctions.h"
#include "../gameClasses.h"
#include "../gameLanguage.h"
#include "../constantVars.h"

// Global stuff
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject) globalClock = ClockObject::get_global_clock();
PT(TextNode) text = new TextNode("textnode");
NodePath camera;

int main(int argc, char* argv[]) {
	game::logOut("Starting...");

	game::runPyScript("C:\\dev\\Panda project\\Panda project\\src\\scripts\\makeDirectories.py");
	game::runPyScript("C:\\dev\\Panda project\\Panda project\\src\\scripts\\createOptionsFile.py");

	//Reading options
	std::map<std::string, std::string> options;
	game::readOptions(options);

	// Open a new window framework and set the title
	PandaFramework framework;
	framework.open_framework(argc, argv);
	framework.set_window_title("My Panda3D Window");

	// Open the window
	WindowFramework* window = framework.open_window();
	camera = window->get_camera_group(); // Get the camera and store it


	//Keyboard input
	window->enable_keyboard();

	//W-key
	framework.define_key("w", "W-key", game::key_down, (void*)&game::w);
	framework.define_key("w-up", "W-key", game::key_up, (void*)&game::w);

	//A-key
	framework.define_key("a", "A-key", game::key_down, (void*)&game::a);
	framework.define_key("a-up", "A-key", game::key_up, (void*)&game::a);

	//S-key
	framework.define_key("s", "S-key", game::key_down, (void*)&game::s);
	framework.define_key("s-up", "S-key", game::key_up, (void*)&game::s);

	//D-key
	framework.define_key("d", "D-key", game::key_down, (void*)&game::d);
	framework.define_key("d-up", "D-key", game::key_up, (void*)&game::d);

	//Space-key
	framework.define_key("space", "Space-key", game::key_down, (void*)&game::space);
	framework.define_key("space-up", "Space-key", game::key_up, (void*)&game::space);

	//LShift-key
	framework.define_key("lshift", "LShift-key", game::key_down, (void*)&game::lshift);
	framework.define_key("lshift-up", "LShift-key", game::key_up, (void*)&game::lshift);

	//Arrow up-key
	framework.define_key("arrow_up", "Arrow up-key", game::key_down, (void*)&game::arrow_up);
	framework.define_key("arrow_up-up", "Arrow up-key", game::key_up, (void*)&game::arrow_up);

	//Arrow down-key
	framework.define_key("arrow_down", "Arrow down-key", game::key_down, (void*)&game::arrow_down);
	framework.define_key("arrow_down-up", "Arrow down-key", game::key_up, (void*)&game::arrow_down);

	//R-key
	framework.define_key("r", "R-key", game::runPyScript, 0);

	//Esc-key
	framework.define_key("escape", "Esc-key", game::exitGame, 0);


	//PT(PGButton) my_button;
	//my_button = new PGButton("MyButton");
	//my_button->setup("Button", 0.1);
	//PT(Texture) button_ready = TexturePool::load_texture("button.png");
	//PT(Texture) button_rollover = TexturePool::load_texture("button_active.png");
	//PT(Texture) button_pressed = TexturePool::load_texture("button_pressed.png");
	//PT(Texture) button_inactive = TexturePool::load_texture("button_inactive.png");

	//// PGFrameStyle is a powerful way to change the appearance of the button:
	//PGFrameStyle MyStyle = my_button->get_frame_style(0); // frame_style(0): ready state
	//MyStyle.set_type(PGFrameStyle::T_flat);

	//MyStyle.set_texture(button_ready);    my_button->set_frame_style(0, MyStyle);
	//MyStyle.set_texture(button_rollover); my_button->set_frame_style(1, MyStyle);
	//MyStyle.set_texture(button_pressed);  my_button->set_frame_style(2, MyStyle);
	//MyStyle.set_texture(button_inactive); my_button->set_frame_style(3, MyStyle);

	//NodePath defbutNP = window->get_aspect_2d().attach_new_node(my_button);
	//defbutNP.set_scale(0.1);

	//// Setup callback function
	//framework.define_key(my_button->get_click_event(MouseButton::one()), "button press", &GUI_Callback_Button_Clicked, my_button);





	//Mouse input
	MouseWatcher* mouseWatcher = DCAST(MouseWatcher, window->get_mouse().node());
	WindowProperties props = window->get_graphics_window()->get_properties();
	props.set_cursor_hidden(std::stoi(options["hidden_cursor"]));
	props.set_mouse_mode(WindowProperties::M_relative);
	window->get_graphics_window()->request_properties(props);

	// Load the environment model
	NodePath scene = window->load_model(framework.get_models(), "models/environment");
	scene.reparent_to(window->get_render());
	scene.set_scale(0.25, 0.25, 0.25);
	scene.set_pos(-8, 42, 0);

	// Load our characters

	game::object asd(window, framework, "/c/dev/Panda project/Panda project/models/egg/simple_house.egg");


	NodePath blocky = window->load_model(framework.get_models(), "/c/dev/Panda project/Panda project/models/egg/blocky.egg");
	blocky.set_scale(0.5);
	blocky.set_pos(0, 0, 0);
	blocky.reparent_to(window->get_render());

	CollisionNode* cSphere_node = new CollisionNode("Sphere");
	cSphere_node->add_solid(new CollisionSphere(0, 0, 0, 4));
	NodePath smileyC = blocky.attach_new_node(cSphere_node);
	smileyC.show();


	NodePath panda = window->load_model(framework.get_models(), "panda.egg");
	panda.set_scale(0.5);
	panda.set_pos(0, 0, 0);
	panda.reparent_to(window->get_render());
	panda.hide();


	CollisionHandlerPusher pusher;
	CollisionTraverser* collTrav = new CollisionTraverser();

	collTrav->add_collider(smileyC, &pusher);
	pusher.add_collider(smileyC, blocky);


	//Reading settings from settings map
	double force_x = std::stof(options["force_x"]);
	double force_y = std::stof(options["force_y"]);

	double camera_x_speed = std::stof(options["camera_x_speed"]);
	double camera_y_speed = std::stof(options["camera_y_speed"]);

	double x_speed = std::stof(options["x_speed"]);
	double y_speed = std::stof(options["y_speed"]);
	double z_speed = std::stof(options["z_speed"]);

	int offset_x;
	int offset_y;

	std::string sad = "Hello World";
	game::encrypt(sad);
	game::decrypt(sad);

	//Preparing for frame counter
	clock_t current_ticks, delta_ticks;
	clock_t fps = 0;

	Thread* current_thread = Thread::get_current_thread();
	while (framework.do_frame(current_thread) && shouldRun) {
		current_ticks = clock();

		// check collisions, will call pusher collision handler
		// if a collision is detected
		collTrav->traverse(window->get_render());

		if (mouseWatcher->has_mouse()) {
			if (window->get_graphics_window()) {
				int center_x = window->get_graphics_window()->get_x_size() / 2;
				int center_y = window->get_graphics_window()->get_y_size() / 2;

				int x = window->get_graphics_window()->get_pointer(0).get_x();
				int y = window->get_graphics_window()->get_pointer(0).get_y();

				int move_x = center_x - x;
				int move_y = center_y - y;

				if (move_x > force_x || move_x < force_x-force_x*2 || move_y > force_y || move_y < force_y-force_y*2) {
					offset_x += move_x/camera_x_speed;
					offset_y += move_y/camera_y_speed;
					
					camera.set_h(offset_x);
					panda.set_h(offset_x);


					if (offset_y < 90 && offset_y > -90) {
						camera.set_p(offset_y);
					} else {
						offset_y -= move_y/5;
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

			game::objects.push_back(game::object(window, framework, "/c/dev/Panda project/Panda project/models/egg/simple_house.egg"));
		}
		if (keys["d"]) {
			camera.set_x(camera, 0 + x_speed);
			panda.set_x(camera, 0 + x_speed);
		}
		if (keys["space"]) {
			camera.set_z(camera.get_pos().get_z() + z_speed);
			panda.set_z(camera.get_pos().get_z() + z_speed);
		}
		if (keys["lshift"]) {
			camera.set_z(camera.get_pos().get_z() - z_speed);
			panda.set_z(camera.get_pos().get_z() - z_speed);
		}


		//Border checking
		if (std::stof(options["lower_border"]) != 0) {
			if (camera.get_pos().get_z() < std::stof(options["lower_border"])) {
				camera.set_z(std::stof(options["lower_border"]));
			}
		}
		if (std::stof(options["upper_border"]) != 0) {
			if (camera.get_pos().get_z() < std::stof(options["upper_border"])) {
				camera.set_z(std::stof(options["upper_border"]));
			}
		}

		delta_ticks = clock() - current_ticks;
		if (delta_ticks > 0) {
			fps = CLOCKS_PER_SEC / delta_ticks;
		}
		if (fps == 999) {
			std::cout << "FPS: " << fps << "\n";
		}
	}
	framework.close_framework();

	game::logOut("Closing...");

	if (!std::stoi(options["close_console_without_input"])) {
		game::waitForKeypress();
	}

	return 0;
}