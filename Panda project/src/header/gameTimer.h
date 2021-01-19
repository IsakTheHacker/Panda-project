#pragma once	//Only include this header file once

//C++ built-in libraries
#include <chrono>

//My libraries
#include "gameFunctions.h"

namespace game {
	class timer {
	private:
		bool printDuration;
	public:
		std::chrono::time_point<std::chrono::steady_clock> start, end;
		std::chrono::duration<double> duration;

		timer(bool printDuration = true);
		~timer();
	};
}