#pragma once	//Only include this header file once

//C++ built-in libraries
#include <string>
#include <map>
#include <vector>
#include <fstream>

//My libraries
#include "gameFunctions.h"
#include "pandaIncludes.h"

namespace game {

	// Item class
	class item {
		private:
		public:
			std::string configPath;
			unsigned int stackedItems;
			std::map<std::string, std::string> options;

			item(const std::string& configPath, const unsigned int& stackedItems = 1);
	};

	extern item emptyItem;

	//Class for pauseMenu event parameters
	class pauseMenuEventParameters {
		public:
			WindowFramework* window;
			MouseWatcher* mouseWatcher;
	};

	//Class for collisionRay event parameters
	class collisionRayEventParameters {
		public:
			WindowFramework* window;
			CollisionRay* pickerRay;
			MouseWatcher* mouseWatcher;
	};

	// Class for computer specs
	class computerSpecs {
		public:
			std::string platform;
	};

	// Wind Object class
	class windObject {
		protected:
			bool shouldLogInConsoleIntern;
			bool shouldLogToFileIntern;
		public:
			NodePath model;
			static int current_id;
			static int object_quantity;
			unsigned int id;
			std::string name;
			LPoint3 direction;
			double velocity;

			windObject(WindowFramework*& window, PandaFramework& framework, const double& mx, const double& my, const double& mz, const double& velocity, const PN_stdfloat& sx = 1, const PN_stdfloat& sy = 1, const PN_stdfloat& sz = 1, bool shouldLogInConsole = false, bool shouldLogToFile = false);
			~windObject();
	};

	//Creating vectors for the classes
	extern std::vector<windObject> winds;
}