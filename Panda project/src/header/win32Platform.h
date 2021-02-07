#pragma once

//C++ built-in libraries
#include <string>

//Include Windows library
#include <Windows.h>

namespace game {

	//Sets the console title
	bool setConsoleTitle(std::wstring title);

	//Changes the text color
	bool setTextColor(HANDLE handle, unsigned short color);

}