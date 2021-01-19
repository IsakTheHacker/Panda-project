#include "gameTimer.h"

namespace game {
	timer::timer() {
		start = std::chrono::high_resolution_clock::now();
	}

	timer::~timer() {
		end = std::chrono::high_resolution_clock::now();
		duration = end - start;

		double convertedDuration = duration.count();
		game::timingInfoOut("Timer took " + std::to_string(convertedDuration) + " seconds!");
	}
}