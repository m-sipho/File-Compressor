#include <iostream>
#include "compressor/fileHandler.h"

using namespace std;

int main(int argc, char* argv[]) {
	// Check if the user provided correct number of arguments
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " <FILENAME>" << endl;
		return -1;
	}

	string filePath = argv[1];

	cout << "--- File Content ---\n";
	FileHandler fileHandler;
	fileHandler.readAndPrint(filePath);
	cout << "\n--------------------------------\n";

	auto frequencyMap = fileHandler.getCharacterFrequencies(filePath);

	cout << "--- Character Frequencies ---\n";
	for (const auto& pair : frequencyMap) {
		
		cout << "'" << pair.first << "' : " << pair.second << endl;
	}
	return 0;
}