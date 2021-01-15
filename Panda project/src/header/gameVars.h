#pragma once
#include "nodePath.h"
#include "gameObject.h"

extern int handInventoryIndex;
extern std::map<std::string, bool> keys;
extern std::map<std::string, std::string> universeOptions;
extern bool shouldRun;
extern bool terrainAnimationShouldRun;
extern bool devMode;
extern bool mouseInGame;
extern bool playerOnGround;
extern NodePath collidedNodePath;
extern game::player player;
extern std::string gamePath;
extern bool player_sneaking;
extern std::string universePath;