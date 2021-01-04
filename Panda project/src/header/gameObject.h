#pragma once

//C++ built-in libraries
#include <string>

//Panda3D libraries
#include <nodePath.h>
#include <pandaFramework.h>

//My libraries
#include "gameFunctions.h"

namespace game {
	// Object class
	class object {
	protected:
		std::string modelpathIntern;
		std::string oldModelpath;
		bool shouldLogInConsoleIntern;
		bool shouldLogToFileIntern;
		bool modelNotFound;
	public:
		bool empty = false;
		NodePath model;
		static int current_id;
		static int object_quantity;
		unsigned int id;
		std::string name;

		object(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		object(WindowFramework*& window, PandaFramework& framework, std::vector<NodePath> subobjects, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		object(bool shouldLogInConsole = false, bool shouldLogToFile = false);		//Empty game::object constructor
		~object();

		operator std::string();
	};

	// Entity class
	class entity : public object {
	public:
		entity(WindowFramework*& window, PandaFramework& framework, const std::string& modelpath, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		~entity();
	};

	// Player class
	class player : public entity {
	public:
		NodePath collisionNodePath;

		player(WindowFramework*& window, PandaFramework& framework, const std::string& modelpath, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		~player();
	};

	//Creating vectors for the classes
	extern std::vector<entity> entities;
	extern std::vector<player> players;
}