#pragma once

//C++ built-in libraries
#include <string>
#include <memory>

//Panda3D libraries
#include "nodePath.h"
#include "pandaFramework.h"
#include "collisionNode.h"
#include "collisionBox.h"
#include "collisionEntry.h"
#include "pointLight.h"

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
		std::vector<NodePath> lights;
		double hp = 1;
		double temperature = 0;

		object(WindowFramework*& window, PandaFramework& framework, std::string modelpath, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		object(WindowFramework*& window, PandaFramework& framework, std::vector<NodePath> subobjects, bool collidable = true, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		object(std::string configPath, WindowFramework*& window, PandaFramework& framework, bool shouldLogInConsole = true, bool shouldLogToFile = false, NodePath parent = NodePath("__unspecifiedParent__"));
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
		NodePath firstPerson = NodePath("firstPerson");
		NodePath thirdPerson = NodePath("thirdPerson");
		bool onGround;
		bool sneaking;
		bool flying;
		double velocity = 0.0;
		double velocityModifier = 1.1;
		int chunk_x = 0;
		int chunk_y = 0;
		std::string playerName;

		double offset_h = 0.0;
		double offset_p = 0.0;

		double center_x = 0.0;
		double center_y = 0.0;

		double camera_x_speed;
		double camera_y_speed;

		//Static member variables
		static std::map<std::string, std::string>* options;

		Player(bool shouldLogInConsole = false, bool shouldLogToFile = false);
		Player(std::string configPath, WindowFramework*& window, PandaFramework& framework, bool shouldLogInConsole = true, bool shouldLogToFile = false);
		~Player();

		void doCameraControl(WindowFramework* window);
		void setThirdPersonCamera(WindowFramework* window);
		void setFirstPersonCamera(WindowFramework* window);
	};


	void testIfPlayerOnGround(const Event* theEvent, void* data);
	void getCollidedNodePath(const Event* theEvent, void* data);

	//Creating vectors for the classes
	extern std::vector<entity> entities;
	//extern std::vector<player> players;
}

extern game::Player player;