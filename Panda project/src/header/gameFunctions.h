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
#include <string>

//JSON library
#include "external/json.hpp"
using json = nlohmann::json;

//Python library
#define PY_SSIZE_T_CLEAN
#ifdef _DEBUG
	#undef _DEBUG
	#include <python.h>
	#define _DEBUG
#else
	#include <python.h>
#endif

//Panda3D libraries
#include "pStatClient.h"
#include "texturePool.h"

//My libraries
#include "cppExtension.h"
#include "constantVars.h"
#include "gameVars.h"
#include "platformPicker.h"

//Including necessary Windows libraries
#include <conio.h>

namespace game {

	// Loading animation
	void terrainAnimation(const std::string& message);

	// Waits for input
	void waitForKeypress();

	// Sets the console heading
	int setHeading(const std::string& heading);

	// Replaces the first found substring in a string
	bool findReplaceFirst(std::string& str, const std::string& oldSubstring, const std::string& newSubstring);

	// Replaces all found substrings in a string
	bool findReplaceAll(std::string& str, const std::string& oldSubstring, const std::string& newSubstring);

	/// <summary> Splits a string with the specified delimiter. </summary>
	/// <param name="str">- your string</param>
	/// <param name="delimiter">- the delimter to use. " " is standard</param>
	/// <returns> A vector containing the tokens. </returns>
	std::vector<std::string> split(std::string str, const std::string& delimiter = " ");

	// Encrypts a given string with a basic encryption
	int encrypt(std::string& str, const std::string& separator = "|");

	// Decrypts a given string
	int decrypt(std::string& str, const std::string& separator = "|");

	// Gets the current date and time and returns a string ready for logging
	std::string getConvertedDateTime(bool datePresent, bool timePresent);

	// Gets the current date and time and returns a valid filename string
	std::string getConvertedDateTime();

	// Gets the current date and time and returns an std::tm
	std::tm getTmLocal();

	// Logs a string to a file
	template<typename T>
	int logToFile(const std::string& path, T input, bool datePresent = true, bool timePresent = true) {
		std::string text = std::to_string(input);
		std::string convertedText = getConvertedDateTime(datePresent, timePresent) + " " + text;
		std::ofstream logFile(path, std::ios_base::app);
		logFile << convertedText << "\n";
		logFile.close();
		return 0;
	}

	// Prints an error to the console (red color)
	template<typename T>
	int errorOut(T input, bool shouldLogToFile = true, bool includeTime = true) {
		std::string text = std::to_string(input);
		setTextColor(color::FG_LIGHTRED);
		if (!includeTime) {
			std::cout << "Error: " << text << "\n";
		} else {
			std::cout << getConvertedDateTime(false, true) << " Error: " << text << "\n";
		}
		setTextColor(color::FG_WHITE);
		if (shouldLogToFile) {
			logToFile("game.log", "Error: " + text);
		}
		return 0;
	}

	// Prints a warning to the console (yellow color)
	template<typename T>
	int warningOut(T input, bool shouldLogToFile = true, bool includeTime = true) {
		std::string text = std::to_string(input);
		setTextColor(color::FG_YELLOW);
		if (!includeTime) {
			std::cout << "Warning: " << text << "\n";
		} else {
			std::cout << getConvertedDateTime(false, true) << " Warning: " << text << "\n";
		}
		setTextColor(color::FG_WHITE);
		if (shouldLogToFile) {
			logToFile("game.log", "Warning: " + text);
		}
		return 0;
	}

	// Prints important info to the console (green color)
	template<typename T>
	int importantInfoOut(T input, bool shouldLogToFile = true, bool includeTime = true) {
		std::string text = std::to_string(input);
		setTextColor(color::FG_LIGHTGREEN);
		if (!includeTime) {
			std::cout << "Important info: " << text << "\n";
		} else {
			std::cout << getConvertedDateTime(false, true) << " Important info: " << text << "\n";
		}
		setTextColor(color::FG_WHITE);
		if (shouldLogToFile) {
			logToFile("game.log", "Important info: " + text);
		}
		return 0;
	}

	// Prints timer information to the console (blue color)
	template<typename T>
	int timingInfoOut(T input, bool shouldLogToFile = true, bool includeTime = true) {
		std::string text = std::to_string(input);
		setTextColor(color::FG_LIGHTBLUE);
		if (!includeTime) {
			std::cout << "Timing info: " << text << "\n";
		} else {
			std::cout << getConvertedDateTime(false, true) << " Timing info: " << text << "\n";
		}
		setTextColor(color::FG_WHITE);
		if (shouldLogToFile) {
			logToFile("game.log", "Timing info: " + text);
		}
		return 0;
	}

	// Prints user configuration information to the console (purple)
	template<typename T>
	int userConfigOut(T input, bool shouldLogToFile = true, bool includeTime = true) {
		std::string text = std::to_string(input);
		setTextColor(color::FG_LIGHTMAGENTA);
		if (!includeTime) {
			std::cout << "User config info: " << text << "\n";
		} else {
			std::cout << getConvertedDateTime(false, true) << " User config info: " << text << "\n";
		}
		setTextColor(color::FG_WHITE);
		if (shouldLogToFile) {
			logToFile("game.log", "User config info: " + text);
		}
		return 0;
	}

	// Prints unimportant logs to the console (white color)
	template<typename T>
	int logOut(T input, bool shouldLogToFile = false, bool includeTime = true) {
		std::string text = std::to_string(input);
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

	// For each function to easily loop through arrays
	template<typename listType, typename funcType>
	void forEach(const listType& list, funcType function) {
		for (auto value : list) {
			function(value);
		}
	}
	template<typename listType>			//Overload to be used if no function was specified
	void forEach(const listType& list) {
		for (auto value : list) {
			logOut(value);
		}
	}

	/// <summary> Checks if a value is negative. </summary>
	/// <param name="value">- Value to check</param>
	/// <returns> True, if negative. False, if not! </returns>
	bool isNegative(const int& value);

	/// <summary> Checks if a value is positive. </summary>
	/// <param name="value">- Value to check</param>
	/// <returns> True, if positive. False, if not! </returns>
	bool isPositive(const int& value);

	/// <summary> Makes a value negative. Does nothing if already negative! </summary>
	/// <param name="value">- Value to parse</param>
	/// <returns> Parsed value </returns>
	int parseNegative(int value);

	/// <summary> Makes a value positive. Does nothing if already positive! </summary>
	/// <param name="value">- Value to parse</param>
	/// <returns> Parsed value </returns>
	int parsePositive(int value);

	/// <summary> Checks if a value is odd or not. </summary>
	/// <param name="value">- Input integer</param>
	/// <returns> True if odd, false if not. </returns>
	bool isOdd(const int& value);

	/// <summary> Checks if a value is even or not. </summary>
	/// <param name="value">- Input integer</param>
	/// <returns> True if even, false if not. </returns>
	bool isEven(const int& value);

	/// <summary> Checks if a file exists. </summary>
	/// <param name="path">- The path to your file</param>
	/// <returns> True if file exists, false if it doesn't. </returns>
	bool fileExists(const std::string& path);

	bool connectToPStats(std::string host = "localhost", int port = -1);

	bool mkdir(std::string path);

	// Executes a Python script
	int runPyScript(const std::string& path);

	// Reads options
	int readOptions(std::map<std::string, std::string>& options, const std::string& path);

	// Lists a std::map of options to the console
	int listOptions(std::map<std::string, std::string>& options, const std::string& title = "Options:");

	// Sets a texture
	int setTexture(NodePath& object, const std::string& path);

	//Changes texture scale
	int setTextureScale(NodePath& object, const float& scale);

	// Thread functions
	void takeDebugInput();

	// Events
	void runPyScript(const Event* theEvent, void* data);
	void exitGame(const Event* theEvent, void* data);
	void key_down(const Event* theEvent, void* data);
	void key_up(const Event* theEvent, void* data);
	void key_swap(const Event* theEvent, void* data);
	void wheel_roll(const Event* theEvent, void* data);
	void unpause(const Event* theEvent, void* data);
}