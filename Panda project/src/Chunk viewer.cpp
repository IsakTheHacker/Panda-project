//Disable strange warnings
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

//C++ libraries
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>

//Panda3d libraries
#include <pandaFramework.h>

//My libraries
#include "gameFunctions.h"

//Global stuff
bool shouldRun = true;
NodePath camera;
void exitProgram(const Event* theEvent, void* data) {
	shouldRun = false;
}


int main(int argc, char* argv[]) {

	//Getting path variable
	std::string path;
	if (argc > 2) {
		std::cout << "Too many arguments was given, only one is allowed!" << std::endl;
	} else if (argc == 2) {
		path = argv[1];
	} else if (argc == 1) {
		std::cout << "No argument was given!" << std::endl;
		std::cout << "Enter the path: " << std::endl;
		std::getline(std::cin, path);
	}
	std::cout << "Path: " << path << std::endl;

	//Initalize the scene
	PandaFramework framework;
	framework.open_framework(argc, argv);
	framework.set_window_title("Chunk viewer");

	WindowFramework* window = framework.open_window();
	camera = window->get_camera_group();

	window->get_graphics_window()->get_active_display_region(0)->set_clear_color(LColorf(0, 0, 0, 1));

	PT(FrameRateMeter) meter;
	meter = new FrameRateMeter("frame_rate_meter");
	meter->setup_window(window->get_graphics_window());

	//Keyboard & mouse input
	framework.define_key("escape", "Esc-key", exitProgram, 0);
	window->setup_trackball();

	//Initalize variables
	std::ifstream file(path);
	std::string line;

	int x_level = 0;
	int y_level = 0;
	int z_level;
	std::vector<std::string> block_list;
	std::vector<std::string> block_attributes;
	std::vector<std::string> attribute_array;
	std::string block_model;
	std::string block_texture;
	
	std::vector<NodePath> objects;
	NodePath object;

	while (std::getline(file, line)) {
		if (line.find("z") != std::string::npos) {
			game::findReplaceFirst(line, "z", "");
			z_level = std::stoi(line);
			x_level = 0;
		} else {
			x_level += 2;
			y_level = 0;
			block_list = game::split(line, ",");
			for (std::string block : block_list) {
				y_level += 2;
				game::findReplaceFirst(block, "{", "");
				game::findReplaceFirst(block, "}", "");
				block_attributes = game::split(block, "|");
				for (std::string attribute : block_attributes) {
					attribute_array = game::split(attribute, ":");
					if (attribute_array[0].find("model") != std::string::npos) {
						block_model = attribute_array[1];
					} else if (attribute_array[0].find("texture") != std::string::npos) {
						block_texture = attribute_array[1];
					}
				}
				NodePath object = window->load_model(framework.get_models(), "/c/dev/Panda project/Panda project/models/egg/" + (std::string)"block.egg");
				object.reparent_to(window->get_render());
				object.set_pos(x_level, y_level, z_level);
				objects.push_back(object);
			}
		}
	}
	file.close();

	//Main loop
	while (framework.do_frame(Thread::get_current_thread()) && shouldRun) {

	}

	framework.close_framework();

	std::cout << "\nPress any key to exit...";
	getch();
	return 0;
}