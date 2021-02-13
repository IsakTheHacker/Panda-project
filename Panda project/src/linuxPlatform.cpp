#include "linuxPlatform.h"

namespace game {

	bool setConsoleTitle(std::string title) {
		std::cout << "\033]0;" << title << "\007";
		return 0;
	}

}