#include <iostream>
#include <algorithm>
#include <conio.h>

int findReplace(std::string& str, std::string oldSubstring, std::string newSubstring) {
	str.replace(str.find(oldSubstring), oldSubstring.length(), newSubstring);
	return 0;
}

int main(int argc, char* argv[]) {
	/*std::string str2 = "Sweden";
	std::string str = "Hello Sweden";
	std::cout << str << "\n";

	str.replace(str.find(str2), str2.length(), "World");

	std::cout << str << "\n";*/

	std::string string = "Hello Sweden";
	findReplace(string, "Sweden", "World");
	std::cout << string;

	//Do not change the code under this comment!
	std::cout << "\nPress any key to exit...";
	getch();
	return 0;
}