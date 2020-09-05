#pragma once

//Disable strange warnings
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

//C++ built-in libraries
#include <iostream>
#include <fstream>
#include <ctime>
#include <array>
#include <algorithm>

//Including necessary Windows libraries
#include <Windows.h>
#include <conio.h>

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

namespace game {
	void waitForKeypress() {
		std::cout << "\nPress any key to exit...\n";
		getch();
	}

	int setConsoleTitle(const char* title) {
		return 0;
	}

	int setHeading(std::string heading) {
		std::cout << "\n\n" << heading << std::endl;
		for (int i = 0; i < heading.length(); i++) {
			std::cout << "-";
		}
		std::cout << "\n" << std::endl;
		return 0;
	}

	int findReplaceFirst(std::string& str, std::string oldSubstring, std::string newSubstring) {
		str.replace(str.find(oldSubstring), oldSubstring.length(), newSubstring);
		return 0;
	}

	int findReplaceAll(std::string& str, std::string oldSubstring, std::string newSubstring) {
		while (str.find(oldSubstring) != std::string::npos) {
			str.replace(str.find(oldSubstring), oldSubstring.length(), newSubstring);
		}
		return 0;
	}

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
		std::cout << str << std::endl;
		return 0;
	}

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
		std::cout << str << std::endl;
		return 0;
	}

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

	int logToFile(std::string path, std::string text, bool datePresent = true, bool timePresent = true) {
		std::string convertedText = getConvertedDateTime(datePresent, timePresent) + " " + text;
		std::ofstream logFile(path, std::ios_base::app);
		logFile << convertedText << "\n";
		logFile.close();
		return 0;
	}

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

	int listOptions(std::map<std::string, std::string>& options) {
		game::logOut("Options:");
		for (std::pair<std::string, std::string> option : options)
		{
			std::cout << option.first << "=" << option.second << std::endl;
		}
		return 0;
	}

	//Events
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
}