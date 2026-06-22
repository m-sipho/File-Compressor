#include <iostream>
#include <fstream>
#include "compressor/fileHandler.h"

void FileHandler::readAndPrint(const std::string& filePath) {
	std::ifstream file(filePath);

	// Check if the file opened successfully
	if (!file) {
		std::cerr << "ERROR: Failed to open '" << filePath << "'" << std::endl;
		return;
	}

	char character;

	// Read file character by character and output it
	while (file.get(character)) {
		std::cout << character;
	}
	std::cout << std::endl;

	// Check for read error
	if (file.bad()) {
		std::cerr << "ERROR: An I/O error occured while reading the file" << std::endl;
		return;
	}

	file.close();
}