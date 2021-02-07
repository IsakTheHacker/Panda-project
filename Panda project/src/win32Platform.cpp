#include "win32Platform.h"

namespace game {

	bool setConsoleTitle(std::string title) {
		return SetConsoleTitle((LPCWSTR)title.c_str());
	}

}