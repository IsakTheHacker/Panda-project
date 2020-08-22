#include <iostream>
#include <conio.h>

int main(int argc, char* argv[]) {
	std::cout << "\nPlease select one of the available conversions:\n";
	std::cout << "1. blend2bam\n";
	std::cout << "2. bam2egg\n";
	int pickedConversion;
	std::string file;
	pickedConversion = getch();
	if (pickedConversion == 49) {
		std::cout << "\nblend2bam: Pick the file you want to convert! Without file extension.\n";
		std::cin >> file;
		std::cout << "\n";
		system(("py -m blend2bam ./blend/" + file + ".blend ./bam/" + file + ".bam").c_str());
		std::cout << "\nblend2bam: Succesfully converted " << file << ".blend to " << file << ".bam!";
	} else if (pickedConversion == 50) {
		std::cout << "\nbam2egg: Pick the file you want to convert! Without file extension.\n";
		std::cin >> file;
		system(("bam2egg ./bam/" + file + ".bam -o ./egg/" + file + ".egg").c_str());
		std::cout << "\nbam2egg: Succesfully converted " << file << ".bam to " << file << ".egg!";
	}
	std::cout << "\nPress any key to exit...";
	getch();
	return 0;
}