#include "gameFunctions.h"

namespace game {

	void terrainAnimation(const std::string& message) {
		std::cout << message;
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

	void waitForKeypress() {
		std::cout << "\nPress any key to exit...\n";
		getch();
	}

	int setHeading(const std::string& heading) {
		std::cout << "\n\n" << heading << std::endl;
		for (int i = 0; i < heading.length(); i++) {
			std::cout << "-";
		}
		std::cout << "\n" << std::endl;
		return 0;
	}

	bool findReplaceFirst(std::string& str, const std::string& oldSubstring, const std::string& newSubstring) {
		if (str.find(oldSubstring) != std::string::npos) {
			str.replace(str.find(oldSubstring), oldSubstring.length(), newSubstring);
			return true;
		} else {
			return false;
		}
	}

	bool findReplaceAll(std::string& str, const std::string& oldSubstring, const std::string& newSubstring) {
		if (str.find(oldSubstring) != std::string::npos) {
			while (str.find(oldSubstring) != std::string::npos) {
				str.replace(str.find(oldSubstring), oldSubstring.length(), newSubstring);
			}
			return true;
		} else {
			return false;
		}
	}

	std::vector<std::string> split(std::string str, const std::string& delimiter) {
		std::vector<std::string> tokens;
		size_t pos = 0;
		std::string token;
		while ((pos = str.find(delimiter)) != std::string::npos) {
			token = str.substr(0, pos);
			str.erase(0, pos + delimiter.length());
			if (token != "") {
				tokens.push_back(token);
			}
		}
		if (str != "") {
			tokens.push_back(str);
		}
		return tokens;
	}

	int encrypt(std::string& str, const std::string& separator) {
		std::string newStr;
		for (int i = 0; i < str.size(); i++) {
			char newChar = str[i];
			newStr += std::to_string(int(newChar));

			//Append separator
			if (i != str.size() - 1) {
				newStr += separator;
			}
		}
		str = newStr;
		return 0;
	}

	int decrypt(std::string& str, const std::string& separator) {
		std::array<int, 11> validChars = { 1,2,3,4,5,6,7,8,9,0,int(stoc(separator)) };
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

	std::string getConvertedDateTime() {
		std::time_t current_time = time(0);
		std::tm* tm_local = std::localtime(&current_time);
		std::string convertedDateTime =
			std::to_string(tm_local->tm_year + 1900) +
			"-" + std::to_string(tm_local->tm_mon + 1) +
			"-" + std::to_string(tm_local->tm_mday) +
			" " + std::to_string(tm_local->tm_hour) +
			"." + std::to_string(tm_local->tm_min) +
			"." + std::to_string(tm_local->tm_sec);
		return convertedDateTime;
	}

	std::tm getTmLocal() {
		std::time_t current_time = time(0);
		std::tm* tm_local = std::localtime(&current_time);
		tm_local->tm_year += 1900;
		tm_local->tm_mon += 1;
		return *tm_local;
	}

	int runPyScript(const std::string& path) {
		std::ifstream pyFile(path);
		if (pyFile.fail()) {
			game::errorOut("Could not find Python file: " + path);
			return 1;
		}
		const char* filename = path.c_str();
		FILE* fp;
		Py_Initialize();
		PyObject* m_pMainModule = PyImport_AddModule("__main__");
		PyObject* m_pGlobalDict = PyModule_GetDict(m_pMainModule);
		fp = _Py_fopen(filename, "r");
		PyObject* result = PyRun_File(fp, filename, Py_file_input, m_pGlobalDict, m_pGlobalDict);
		Py_Finalize();
		return 0;
	}

	int readOptions(std::map<std::string, std::string>& options, const std::string& path) {

		//Constant vars
		std::string delimiter = "=";
		const char commentChar = '#';

		std::ifstream optionsFile(path);
		std::string line;
		while (std::getline(optionsFile, line)) {
			if (line[0] == commentChar) {
				pass();
			} else {
				size_t pos = 0;
				std::string token;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					token = line.substr(0, pos);
					line.erase(0, pos + delimiter.length());
				}
				options[token] = line;
			}
		}
		optionsFile.close();
		return 0;
	}

	int listOptions(std::map<std::string, std::string>& options, const std::string& title) {
		game::logOut(title);
		for (std::pair<std::string, std::string> option : options) {
			std::cout << "    " << option.first << "=" << option.second << std::endl;
		}
		return 0;
	}

	int setTexture(NodePath& object, const std::string& path) {
		PT(Texture) texture = TexturePool::load_texture(path);
		texture->set_minfilter(SamplerState::FilterType::FT_nearest);
		texture->set_magfilter(SamplerState::FilterType::FT_nearest);
		object.set_texture(texture);
		return 0;
	}

	int setTextureScale(NodePath& object, const float& scale) {
		TextureStage* textureStage = object.find_texture_stage("textureStage");
		object.set_tex_scale(textureStage, scale);
		return 0;
	}

	void takeDebugInput() {
		while (shouldRun) {
			std::cin.get();
			importantInfoOut("Debug!!! :)");
		}
	}

	bool isNegative(const int& value) {
		if (value < 0) {
			return true;
		} else {
			return false;
		}
	}

	bool isPositive(const int& value) {
		if (value > 0) {
			return true;
		} else {
			return false;
		}
	}

	int parseNegative(int value) {
		if (isNegative(value)) {
			return value;
		} else {
			return value - value * 2;
		}
	}

	int parsePositive(int value) {
		if (isPositive(value)) {
			return value;
		} else {
			return value - value * 2;
		}
	}

	bool isOdd(const int& value) {
		if (value % 2 != 0) {
			return true;
		} else {
			return false;
		}
	}

	bool isEven(const int& value) {
		if (value % 2 == 0) {
			return true;
		} else {
			return false;
		}
	}

	bool fileExists(const std::string& path) {
		std::ifstream file(path);
		if (file.fail()) {
			file.close();
			return false;
		} else {
			file.close();
			return true;
		}
	}

	bool connectToPStats(std::string host, int port) {
		if (PStatClient::is_connected()) {
			PStatClient::disconnect();
		}

		if (!PStatClient::connect(host, port)) {
			game::warningOut("Could not connect to PStat server.");
			return false;
		} else {
			game::importantInfoOut("Successfully connected to PStat server!");
			return true;
		}
	}

	bool mkdir(std::string path) {
		Filename dir;
		dir.set_dirname(path);
		return dir.mkdir();
	}

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
	void key_swap(const Event* theEvent, void* data) {
		std::string& key = *((std::string*)data);
		keys[key] = !keys[key];
	}
	void wheel_roll(const Event* theEvent, void* data) {
		if (mouseInGame) {
			int& indexModification = *((int*)data);
			handInventoryIndex += indexModification;
		}
	}
	void unpause(const Event* theEvent, void* data) {
		key_swap(0, (void*)&game::esc);
	}
}