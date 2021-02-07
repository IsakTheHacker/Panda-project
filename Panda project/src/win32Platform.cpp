#include "win32Platform.h"

namespace game {

	bool setConsoleTitle(std::wstring title) {
		return SetConsoleTitle((LPCWSTR)title.c_str());
	}

	bool setTextColor(HANDLE handle, unsigned short color) {
		return SetConsoleTextAttribute(handle, color);
	}
}