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

std::unordered_map<char, int> FileHandler::getCharacterFrequencies(const std::string& filePath) {

	std::unordered_map<char, int> frequencies;

	// Open the file in binary mode
	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		std::cerr << "ERROR: Failed to open '" << filePath << std::endl;
		return frequencies; // Return the empty map
	}

	char character;
	while (file.get(character)) {
		frequencies[character]++;
	}

	if (file.bad()) {
		std::cerr << "ERROR: An I/O error occured while reading the file" << std::endl;
	}

	file.close();
	return frequencies;
}

void FileHandler::writeCompressedFile(const std::string& inputFile, const std::string& outputFile, const std::unordered_map<char, std::string>& compressedCodes, const std::unordered_map<char, int>& frequencies) {
	// Open input to read text, and output to write the binary
	std::ifstream inFile(inputFile, std::ios::binary);
	std::ofstream outFile(outputFile, std::ios::binary);

	if (!inFile || !outFile) {
		std::cerr << "Error opening files for compresion" << std::endl;
		return;
	}

	// Write the header
	size_t mapSize = frequencies.size();
	outFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

	// Then write each character and it's integer count
	for (const auto& pair : frequencies) {
		outFile.write(&pair.first, sizeof(pair.first));
		outFile.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
	}

	// Then write the compressed bits
	unsigned char bitBuffer = 0;
	int bitCount = 0;
	char character;

	while (inFile.get(character)) {
		// Get the string of '0's and '1's for the character
		std::string code = compressedCodes.at(character);

		// Process each '0' or '1' individually
		for (char bit : code) {
			// Shift the buffer left by 1 and insert a new bit
			bitBuffer = (bitBuffer << 1) | (bit == '1' ? 1 : 0);
			bitCount++;

			// If our buffer is full (8 bits) write the byte to the file
			if (bitCount == 8) {
				outFile.write(reinterpret_cast<const char*>(&bitBuffer), sizeof(bitBuffer));
				bitBuffer = 0;
				bitCount = 0;
			}
		}
	}

	// Handle leftover bits
	if (bitCount > 0) {
		bitBuffer = bitBuffer << (8 - bitCount);
		outFile.write(reinterpret_cast<const char*>(&bitBuffer), sizeof(bitBuffer));
	}

	inFile.close();
	outFile.close();
}