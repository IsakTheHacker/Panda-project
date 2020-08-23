//C++ built-in libraries
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <chrono>
//#include <json/json.h>

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

//My libraries
#include "../gameFunctions.h"
#include "../gameClasses.h"

//Preparing for keyboard input

game::key key_w("w", false);
game::key key_a("a", false);
game::key key_s("s", false);
game::key key_d("d", false);
game::key key_space("space", false);
game::key key_lshift("lshift", false);

bool shouldRun = true;

//Key press events
void runPyScript(const Event * theEvent, void * data) {
	game::runPyScript("C:\\dev\\Panda project\\Panda project\\src\\module.py");
}
void exitGame(const Event* theEvent, void* data) {
	shouldRun = false;
}
void key_down(const Event* theEvent, void* data) {
	const char* &keyname = *((const char* *) data);
	if (keyname == "w") {
		key_w.pressed = true;
	} else if (keyname == "a") {
		key_a.pressed = true;
	} else if (keyname == "s") {
		key_s.pressed = true;
	} else if (keyname == "d") {
		key_d.pressed = true;
	} else if (keyname == "space") {
		key_space.pressed = true;
	} else if (keyname == "lshift") {
		key_lshift.pressed = true;
	} else {
		std::cout << "Unknown key";
	}
}
void key_up(const Event* theEvent, void* data) {
	const char*& keyname = *((const char**)data);
	if (keyname == "w") {
		key_w.pressed = false;
	} else if (keyname == "a") {
		key_a.pressed = false;
	} else if (keyname == "s") {
		key_s.pressed = false;
	} else if (keyname == "d") {
		key_d.pressed = false;
	} else if (keyname == "space") {
		key_space.pressed = false;
	} else if (keyname == "lshift") {
		key_lshift.pressed = false;
	} else {
		std::cout << "Unknown key";
	}
}

// Global stuff
PT(AsyncTaskManager) taskMgr = AsyncTaskManager::get_global_ptr();
PT(ClockObject) globalClock = ClockObject::get_global_clock();
PT(TextNode) text = new TextNode("textnode");
NodePath camera;

int main(int argc, char* argv[]) {
	game::logOut("Starting...");

	//Reading options
	std::map<std::string, std::string> options;
	game::readOptions(options);

	game::runPyScript("C:\\dev\\Panda project\\Panda project\\src\\scripts\\createOptionsFile.py");

	// Open a new window framework and set the title
	PandaFramework framework;
	framework.open_framework(argc, argv);
	framework.set_window_title("My Panda3D Window");

	// Open the window
	WindowFramework* window = framework.open_window();
	camera = window->get_camera_group(); // Get the camera and store it



	#include "../gameClasses.h"


	//Gui


	//Keyboard input
	window->enable_keyboard();

	//W-key
	framework.define_key("w", "W-key", key_down, (void*)&key_w.keyname);
	framework.define_key("w-up", "W-key", key_up, (void*)&key_w.keyname);

	//A-key
	framework.define_key("a", "A-key", key_down, (void*)&key_a.keyname);
	framework.define_key("a-up", "A-key", key_up, (void*)&key_a.keyname);

	//S-key
	framework.define_key("s", "S-key", key_down, (void*)&key_s.keyname);
	framework.define_key("s-up", "S-key", key_up, (void*)&key_s.keyname);

	//D-key
	framework.define_key("d", "D-key", key_down, (void*)&key_d.keyname);
	framework.define_key("d-up", "D-key", key_up, (void*)&key_d.keyname);

	//Space-key
	framework.define_key("space", "Space-key", key_down, (void*)&key_space.keyname);
	framework.define_key("space-up", "Space-key", key_up, (void*)&key_space.keyname);

	//LShift-key
	framework.define_key("lshift", "LShift-key", key_down, (void*)&key_lshift.keyname);
	framework.define_key("lshift-up", "LShift-key", key_up, (void*)&key_lshift.keyname);

	//R-key
	framework.define_key("r", "R-key", runPyScript, 0);

	//Esc-key
	framework.define_key("escape", "Esc-key", exitGame, 0);








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

	CollisionNode* cSphere_nodel = new CollisionNode("Sphere");
	cSphere_nodel->add_solid(new CollisionSphere(0, 0, 0, 4));
	NodePath smileyCC = panda.attach_new_node(cSphere_nodel);
	smileyCC.show();


	CollisionHandlerPusher pusher;
	CollisionTraverser* collTrav = new CollisionTraverser();

	collTrav->add_collider(smileyC, &pusher);
	pusher.add_collider(smileyC, blocky);


	//Reading settings from settings map
	int force_x = std::stoi(options["force_x"]);
	int force_y = std::stoi(options["force_y"]);

	int camera_x_speed = std::stoi(options["camera_x_speed"]);
	int camera_y_speed = std::stoi(options["camera_y_speed"]);

	int x_speed = std::stoi(options["x_speed"]);
	int y_speed = std::stoi(options["y_speed"]);
	int z_speed = std::stoi(options["z_speed"]);

	int offset_x;
	int offset_y;

	int current_x;
	int current_y;
	int current_z;

	//Preparing for frame counter
	clock_t current_ticks, delta_ticks;
	clock_t fps = 0;

	Thread* current_thread = Thread::get_current_thread();
	while (framework.do_frame(current_thread) && shouldRun) {
		current_ticks = clock();

		// check collisions, will call pusher collision handler
		// if a collision is detected
		//collTrav->traverse(window->get_render());

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

					if (offset_y < 90 && offset_y > -90) {
						camera.set_p(offset_y);
					} else {
						offset_y -= move_y/5;
					}
					window->get_graphics_window()->move_pointer(0, center_x, center_y);
				}
			}
		}

		if (key_w.pressed) {
			//current_z = camera.get_pos().get_z();
			//camera.set_y(camera, 0 + y_speed);
			//camera.set_z(current_z);

			panda.set_y(panda, 0 - 1);
		}
		if (key_s.pressed) {
			/*current_z = camera.get_pos().get_z();
			camera.set_y(camera, 0 - y_speed);
			camera.set_z(current_z);*/

			panda.set_y(panda, 0 + 1);
		}
		if (key_a.pressed) {
			camera.set_x(camera, 0 - x_speed);
		}
		if (key_d.pressed) {
			camera.set_x(camera, 0 + x_speed);
		}
		if (key_space.pressed) {
			current_x = camera.get_pos().get_x();
			current_y = camera.get_pos().get_y();
			camera.set_z(camera, 0 + z_speed);
			//camera.set_x(current_x);
			//camera.set_y(current_y);
		}
		if (key_lshift.pressed) {
			current_x = camera.get_pos().get_x();
			current_y = camera.get_pos().get_y();
			camera.set_z(camera, 0 - z_speed);
			//camera.set_x(current_x);
			//camera.set_y(current_y);
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