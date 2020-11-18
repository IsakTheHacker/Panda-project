#pragma once

//Disable strange warnings
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

//C++ built-in libraries
#include <iostream>
#include <fstream>
#include <ctime>
#include <array>
#include <algorithm>
#include <thread>
#include <stdio.h>
#include <chrono>
#include <vector>
#include <filesystem>

//JSON library
#include "src/external/json.hpp"
using json = nlohmann::json;

//Python library
#define PY_SSIZE_T_CLEAN
#include <Python.h>

//Including necessary Windows libraries
#include <Windows.h>
#include <conio.h>

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

namespace game {

	// Loading terrain animation
	void terrainAnimation() {
		std::cout << "Loading terrain";
		while (terrainAnimationShouldRun) {
			for (int i = 0; i < 3; i++) {
				std::cout << ".";
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
			}
			std::cout << "\b\b\b   \b\b\b";
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		}
		std::cout << "\n";
	}

	// Waits for input
	void waitForKeypress() {
		std::cout << "\nPress any key to exit...\n";
		getch();
	}

	// Sets the console title
	int setConsoleTitle(const char* title) {
		return 0;
	}

	// Sets the console heading
	int setHeading(std::string heading) {
		std::cout << "\n\n" << heading << std::endl;
		for (int i = 0; i < heading.length(); i++) {
			std::cout << "-";
		}
		std::cout << "\n" << std::endl;
		return 0;
	}

	// Replaces the first found substring in a string
	bool findReplaceFirst(std::string& str, std::string oldSubstring, std::string newSubstring) {
		if (str.find(oldSubstring) != std::string::npos) {
			str.replace(str.find(oldSubstring), oldSubstring.length(), newSubstring);
			return true;
		} else {
			return false;
		}
	}

	// Replaces all found substrings in a string
	bool findReplaceAll(std::string& str, std::string oldSubstring, std::string newSubstring) {
		if (str.find(oldSubstring) != std::string::npos) {
			while (str.find(oldSubstring) != std::string::npos) {
				str.replace(str.find(oldSubstring), oldSubstring.length(), newSubstring);
			}
			return true;
		} else {
			return false;
		}
	}

	std::vector<std::string> split(std::string str, std::string delimiter) {
		std::vector<std::string> vector;
		size_t pos = 0;
		std::string token;
		while ((pos = str.find(delimiter)) != std::string::npos) {
			token = str.substr(0, pos);
			str.erase(0, pos + delimiter.length());
			vector.push_back(token);
		}
		return vector;
	}

	// Encrypts a given string with a basic encryption
	int encrypt(std::string& str, std::string separator = "|") {
		std::string newStr;
		for (int i = 0; i < str.size(); i++) {
			char newChar = str[i];
			newStr += std::to_string(int(newChar));

			//Append separator
			if (i != str.size()-1) {
				newStr += separator;
			}
		}
		str = newStr;
		return 0;
	}

	// Decrypts a given string
	int decrypt(std::string& str, std::string separator = "|") {
		std::array<int, 11> validChars = {1,2,3,4,5,6,7,8,9,0,int(stoc(separator)) };
		std::string newStr;
		std::string newCharA;
		char newChar;
		for (int i = 0; i < str.size(); i++) {
			newCharA = "";
			while (true) {
				newCharA += str[i];
				i++;

				//Checking if next character is a separator
				if (int(str[i]) == int(stoc(separator)) || std::find(validChars.begin(), validChars.end(), int(str[i])) != validChars.end()) {
					break;
				}
			}
			newChar = (char)std::stoi(newCharA);
			newStr += newChar;
		}
		str = newStr;
		return 0;
	}

	// Gets the current date and time and returns a string ready for logging
	std::string getConvertedDateTime(bool datePresent, bool timePresent) {
		std::time_t current_time = time(0);
		std::tm* tm_local = std::localtime(&current_time);
		std::string convertedDateTime = "[";
		if (datePresent) {
			std::string convertedDate =
				std::to_string(tm_local->tm_year + 1900) +
				"-" +
				std::to_string(tm_local->tm_mon + 1) +
				"-" +
				std::to_string(tm_local->tm_mday);
			convertedDateTime += convertedDate;
		}
		if (timePresent) {
			std::string convertedTime =
				std::to_string(tm_local->tm_hour) +
				":" +
				std::to_string(tm_local->tm_min) +
				":" +
				std::to_string(tm_local->tm_sec);
			if (datePresent) {
				convertedDateTime += " " + convertedTime;
			} else {
				convertedDateTime += convertedTime;
			}
		}
		convertedDateTime += "]";
		return convertedDateTime;
	}


	// Logs a string to a file
	int logToFile(std::string path, std::string text, bool datePresent = true, bool timePresent = true) {
		std::string convertedText = getConvertedDateTime(datePresent, timePresent) + " " + text;
		std::ofstream logFile(path, std::ios_base::app);
		logFile << convertedText << "\n";
		logFile.close();
		return 0;
	}

	// Prints an error to the console (red color)
	int errorOut(std::string text, bool shouldLogToFile = true, bool includeTime = true) {
		SetConsoleTextAttribute(h, 4 | FOREGROUND_INTENSITY);
		if (!includeTime) {
			std::cout << "Error: " << text << "\n";
		} else {
			std::cout << getConvertedDateTime(false, true) << " Error: " << text << "\n";
		}
		SetConsoleTextAttribute(h, 7 | FOREGROUND_INTENSITY);
		if (shouldLogToFile) {
			logToFile("game.log", "Error: " + text);
		}
		return 0;
	}

	// Prints a warning to the console (yellow color)
	int warningOut(std::string text, bool shouldLogToFile = true, bool includeTime = true) {
		SetConsoleTextAttribute(h, 6 | FOREGROUND_INTENSITY);
		if (!includeTime) {
			std::cout << "Warning: " << text << "\n";
		} else {
			std::cout << getConvertedDateTime(false, true) << " Warning: " << text << "\n";
		}
		SetConsoleTextAttribute(h, 7 | FOREGROUND_INTENSITY);
		if (shouldLogToFile) {
			logToFile("game.log", "Warning: " + text);
		}
		return 0;
	}

	// Prints important info to the console (green color)
	int importantInfoOut(std::string text, bool shouldLogToFile = true, bool includeTime = true) {
		SetConsoleTextAttribute(h, 2 | FOREGROUND_INTENSITY);
		if (!includeTime) {
			std::cout << "Important info: " << text << "\n";
		} else {
			std::cout << getConvertedDateTime(false, true) << " Important info: " << text << "\n";
		}
		SetConsoleTextAttribute(h, 7 | FOREGROUND_INTENSITY);
		if (shouldLogToFile) {
			logToFile("game.log", "Important info: " + text);
		}
		return 0;
	}

	// Prints unimportant logs to the console (white color)
	int logOut(std::string text, bool shouldLogToFile = false, bool includeTime = true) {
		if (!includeTime) {
			std::cout << "Log: " << text << "\n";
		} else {
			std::cout << getConvertedDateTime(false, true) << " Log: " << text << "\n";
		}
		if (shouldLogToFile) {
			logToFile("game.log", "Log: " + text);
		}
		return 0;
	}

	// Executes a Python script
	int runPyScript(const char* path) {
		const char* filename = path;
		FILE* fp;
		Py_Initialize();
		PyObject* m_pMainModule = PyImport_AddModule("__main__");
		PyObject* m_pGlobalDict = PyModule_GetDict(m_pMainModule);
		fp = _Py_fopen(filename, "r");
		PyObject* result = PyRun_File(fp, filename, Py_file_input, m_pGlobalDict, m_pGlobalDict);
		/*Py_INCREF(result);
		PyObject* list;
		if (PyArg_Parse(result, "O", &list)) {
			int count = PyList_Size(list);
			for (int i = 0; i < count; i++)
			{
				PyObject* item = PyList_GetItem(list, i);
				const char* str = PyBytes_AsString(item);
				std::cout << str;
			}
		}*/
		Py_Finalize();
		return 0;
	}

	// Reads options
	int readOptions(std::map<std::string, std::string>& options, std::string path = "data/options.txt") {
		std::ifstream optionsFile(path);
		std::string line;
		while (std::getline(optionsFile, line)) {
			std::string delimiter = "=";
			size_t pos = 0;
			std::string token;
			while ((pos = line.find(delimiter)) != std::string::npos) {
				token = line.substr(0, pos);
				line.erase(0, pos + delimiter.length());
			}

			options[token] = line;
		}
		optionsFile.close();
		return 0;
	}

	// Lists a std::map of options to the console
	int listOptions(std::map<std::string, std::string>& options, std::string title = "Options:") {
		game::logOut(title);
		for (std::pair<std::string, std::string> option : options)
		{
			std::cout << "    " << option.first << "=" << option.second << std::endl;
		}
		return 0;
	}

	TexturePool* texturePool = TexturePool::get_global_ptr();

	// Sets a texture
	int setTexture(NodePath object, std::string path) {
		Texture* texture = texturePool->load_texture(path);
		texture->set_minfilter(SamplerState::FilterType::FT_nearest);
		texture->set_magfilter(SamplerState::FilterType::FT_nearest);
		object.set_texture(texture);
		return 0;
	}

	//Changes texture scale
	int setTextureScale(NodePath object, float scale) {
		TextureStage* textureStage = object.find_texture_stage("textureStage");
		object.set_tex_scale(textureStage, scale);
		return 0;
	}

	// Thread functions
	void takeDebugInput() {
		while (shouldRun) {
			std::cin.get();
			importantInfoOut("Debug!!! :)");
		}
	}

	std::vector<NodePath> blocks;

	//Reads a chunk
	int readChunk(WindowFramework*& window, PandaFramework& framework, std::string path, int x, int y) {
		//Initalize variables
		std::ifstream file(path);
		std::string line;

		int x_level = x;
		int y_level = y;
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

		NodePath object;

		while (std::getline(file, line)) {
			if (line.find("z") != std::string::npos) {
				findReplaceFirst(line, "z", "");
				z_level = std::stoi(line);
				x_level = x;
			} else {
				x_level += 2;
				y_level = y;
				block_list = split(line, ",");
				for (std::string block : block_list) {
					y_level += 2;
					findReplaceFirst(block, "{", "");
					findReplaceFirst(block, "}", "");
					block_attributes = split(block, "|");
					for (std::string attribute : block_attributes) {
						attribute_array = split(attribute, ":");
						if (placeholder.find(attribute_array[0]) != placeholder.end()) {
							placeholder[attribute_array[0]] = attribute_array[1];
						}
					}
					NodePath object = window->load_model(framework.get_models(), "/c/dev/Panda project/Panda project/models/egg/" + (std::string)"block.egg");
					object.reparent_to(window->get_render());
					object.set_pos(x_level, y_level, z_level);
					object.set_tag("myObjectTag", "");
					if (placeholder["texture"] != "") {
						game::setTexture(object, placeholder["texture"]);
					}
					if (placeholder["texture-scale"] != "") {
						game::setTextureScale(object, std::stoi(placeholder["texture-scale"]));
					}

					object.clear_texture();


					TexturePool* texturePool = TexturePool::get_global_ptr();
					TextureStage* textureStage = new TextureStage("textureStage2");
					textureStage->set_sort(0);
					textureStage->set_mode(TextureStage::M_replace);

					object.set_tex_gen(textureStage->get_default(), RenderAttrib::M_world_position);
					object.set_tex_projector(textureStage->get_default(), window->get_render(), object);

					Texture* texture = texturePool->load_cube_map("/c/dev/Panda project/Panda project/models/textures/png/grass-#.png");
					texture->set_minfilter(SamplerState::FilterType::FT_nearest);
					texture->set_magfilter(SamplerState::FilterType::FT_nearest);
					/*texture->set_x_size(32);
					texture->set_y_size(32);*/
					/*std::cout << "Pages: " << texture->get_num_pages() << std::endl;
					std::cout << "OrigFileXSize: " << texture->get_orig_file_x_size() << std::endl;
					std::cout << "XSize: " << texture->get_x_size() << std::endl;
					std::cout << "TexScale: " << texture->get_tex_scale() << std::endl;*/
					object.set_texture(texture, 1);

					blocks.push_back(object);

					framework.all_windows_closed();
				}
			}
		}
		file.close();
		return 0;
	}

	// Events
	void runPyScript(const Event* theEvent, void* data) {
		game::runPyScript("C:\\dev\\Panda project\\Panda project\\src\\module.py");
	}
	void exitGame(const Event* theEvent, void* data) {
		shouldRun = false;
	}
	void key_down(const Event* theEvent, void* data) {
		std::string& key = *((std::string*)data);
		keys[key] = true;
	}
	void key_up(const Event* theEvent, void* data) {
		std::string& key = *((std::string*)data);
		keys[key] = false;
	}
	void wheel_roll(const Event* theEvent, void* data) {
		int& indexModification = *((int*)data);
		handInventoryIndex += indexModification;
	}
}