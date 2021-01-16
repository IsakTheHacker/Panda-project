#pragma once
#include "gameObject.h"


//Forward declares
namespace game {
	class Player;
}
class NodePath;

extern int handInventoryIndex;
extern std::map<std::string, bool> keys;
extern std::map<std::string, std::string> universeOptions;
extern bool shouldRun;
extern bool terrainAnimationShouldRun;
extern bool devMode;
extern bool mouseInGame;
extern NodePath collidedNodePath;
extern game::Player player;
extern std::string gamePath;
extern std::string universePath;