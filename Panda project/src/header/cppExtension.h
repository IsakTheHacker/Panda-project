#pragma once

int pass() {
	return 0;
}

char stoc(std::string input) {
	char output = input[0];
	return output;
}

namespace std {
	string to_string(string input) {
		return input;
	}
}