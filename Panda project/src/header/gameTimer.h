#pragma once	//Only include this header file once

//C++ built-in libraries
#include <chrono>
#include <iostream>

namespace game {
	class timer {
	public:
		std::chrono::time_point<std::chrono::steady_clock> start, end;
		std::chrono::duration<double> duration;

		timer();
		~timer();
	};
}