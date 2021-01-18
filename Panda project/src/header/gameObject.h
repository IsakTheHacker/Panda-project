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
		static std::map<std::string, std::map<std::string, std::string>> knownConfigs;

		void initConfig(WindowFramework*& window, PandaFramework& framework);
	public:
		bool empty = false;
		NodePath model;
		static int current_id;
		static int object_quantity;
		unsigned int id;
		std::string name;
		std::string configPath = "";
		std::map<std::string, std::string> config;
		NodePath collisionNodePath;
		double hp = 1;

		object(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		object(WindowFramework*& window, PandaFramework& framework, std::vector<NodePath> subobjects, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		object(std::string configPath, WindowFramework*& window, PandaFramework& framework, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		object(bool shouldLogInConsole = false, bool shouldLogToFile = false);		//Empty game::object constructor
		~object();

		operator std::string();
		operator NodePath();
	};

	// Entity class
	class entity : public object {
	public:
		entity(bool shouldLogInConsole = false, bool shouldLogToFile = false);
		entity(std::string configPath, WindowFramework*& window, PandaFramework& framework, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		~entity();

		void update();
	};

	// Player class
	class Player : public entity {
	public:
		NodePath collidedNodePath;
		bool onGround;
		bool sneaking;
		bool flying;

		Player(bool shouldLogInConsole = false, bool shouldLogToFile = false);
		Player(std::string configPath, WindowFramework*& window, PandaFramework& framework, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		~Player();
	};


	void testIfPlayerOnGround(const Event* theEvent, void* data);
	void getCollidedNodePath(const Event* theEvent, void* data);

	//Creating vectors for the classes
	extern std::vector<entity> entities;
	//extern std::vector<player> players;
}

extern game::Player player;