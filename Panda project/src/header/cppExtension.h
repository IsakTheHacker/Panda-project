#pragma once

//C++ built-in libraries
#include <string>

//Panda3D libraries
#include "event.h"

int pass();
void epass(const Event* theEvent, void* data);

char stoc(const std::string& input);

namespace std {
	string to_string(const string& input);
}