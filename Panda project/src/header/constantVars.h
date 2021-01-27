#pragma once

namespace game {
	const std::string w = "w";
	const std::string a = "a";
	const std::string s = "s";
	const std::string d = "d";
	const std::string space = "space";
	const std::string lshift = "lshift";
	const std::string arrow_up = "arrow_up";
	const std::string arrow_down = "arrow_down";
	const std::string arrow_left = "arrow_left";
	const std::string arrow_right = "arrow_right";
	const std::string q = "q";
	const std::string e = "e";
	const std::string v = "v";
	const std::string f2 = "f2";
	const std::string f3 = "f3";
	const std::string f4 = "f4";
	const std::string f5 = "f5";
	const std::string r = "r";
	const std::string mouse1 = "mouse1";
	const std::string mouse2 = "mouse2";
	const std::string mouse3 = "mouse3";

	const int wheel_up = -1;
	const int wheel_down = 1;

	const std::array<std::string, 2> allowed_parameters = {"-dev", "-vs"};

	const std::array<std::string, 26> optionLines = {
		"force_x=4",
		"force_y=4",
		"camera_x_speed=5",
		"camera_y_speed=5",
		"x_speed=0.2",
		"y_speed=0.4",
		"z_speed=0.2",
		"hidden_cursor=1",
		"close_console_without_input=1",
		"lower_border=0",
		"upper_border=none",
		"startscript=scripts/standard_startscript.txt",
		"console-title=The Panda Project",
		"console-heading=The Panda Project",
		"window-title=The Panda Project",
		"model-format=egg",
		"hide_fps=0",
		"pitch-behavior_while_rotating=none",
		"sneak-distance=0.5",
		"fov=70",
		"show_ray-collisions=0",
		"show_block-collisions=0",
		"enable_pstats=0",
		"pstat-host=localhost",
		"save_newly_created_chunks=1",
		"player-name=Samurai"
	};
}