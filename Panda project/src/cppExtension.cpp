#include "cppExtension.h"

int pass() {
	return 0;
}
void epass(const Event* theEvent, void* data) {}

char stoc(const std::string& input) {
	char output = input[0];
	return output;
}

namespace std {
	string to_string(const string& input) {
		return input;
	}
}