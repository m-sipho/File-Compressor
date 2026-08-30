#include <iostream>
#include <fstream>
#include "compressor/fileHandler.h"
#include "compressor/huffmanTree.h"
#include <vector>
#include <algorithm>

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

std::vector<std::pair<char, int>> FileHandler::getCharacterFrequencies(const std::string& filePath) {

	std::unordered_map<char, int> frequencies;

	// Open the file in binary mode
	std::ifstream file(filePath, std::ios::binary);

	if (!file) {
		std::cerr << "ERROR: Failed to open '" << filePath << std::endl;
		return {}; // Return the empty vector
	}

	char character;
	while (file.get(character)) {
		frequencies[character]++;
	}

	if (file.bad()) {
		std::cerr << "ERROR: An I/O error occured while reading the file" << std::endl;
	}

	std::vector<std::pair<char, int>> sortedFrequencies(frequencies.begin(), frequencies.end());
	std::sort(sortedFrequencies.begin(), sortedFrequencies.end(), [](const auto& a, const auto& b) {
		return a.first < b.first;
		});

	file.close();
	return sortedFrequencies;
}

void FileHandler::writeCompressedFile(const std::string& inputFile, const std::string& outputFile, const std::unordered_map<char, std::string>& compressedCodes, const std::unordered_map<char, int>& frequencies) {
	// Open input to read text, and output to write the binary
	std::ifstream inFile(inputFile, std::ios::binary);
	std::ofstream outFile(outputFile, std::ios::binary);

	if (!inFile || !outFile) {
		std::cerr << "Error opening files for compression" << std::endl;
		return;
	}

	// Write the header
	size_t mapSize = frequencies.size();
	outFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));

	std::vector<std::pair<char, int>> sortedFrequencies(frequencies.begin(), frequencies.end());
	std::sort(sortedFrequencies.begin(), sortedFrequencies.end(), [](const auto& a, const auto& b) {
		return a.first < b.first;
		});

	// Then write each character and it's integer count
	for (const auto& pair : sortedFrequencies) {
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

void FileHandler::decompressFile(const std::string& compressedFilePath, const std::string& outputFile) {
	std::ifstream inFile(compressedFilePath, std::ios::binary);
	std::ofstream outFile(outputFile, std::ios::binary);

	if (!inFile || !outFile) {
		std::cerr << "Error opening files for decompression." << std::endl;
		return;
	}

	// Read the header
	size_t mapSize = 0;
	inFile.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

	// std::unordered_map<char, int> frequencies;
	std::vector<std::pair<char, int>> sortedFrequencies;
	long long totalCharacters = 0;

	for (size_t i = 0; i < mapSize; i++) {
		char character;
		int frequency;

		inFile.read(reinterpret_cast<char*>(&character), sizeof(character));
		inFile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));

		// frequencies[character] = frequency;
		sortedFrequencies.push_back({ character, frequency });
		totalCharacters += frequency;
	}

	std::sort(sortedFrequencies.begin(), sortedFrequencies.end(), [](const auto& a, const auto& b) {
		return a.first < b.first;
		});

	// Rebuild the Huffman Tree
	HuffmanTree tree;
	tree.buildTree(sortedFrequencies);

	tree.generateCompressedBinaryCodes();
	auto compressedBinaryCodes = tree.getCompressedBinaryCodes();

	//std::cout << "===== Huffman Tree Codes on Compression =====" << std::endl;
	//for (const auto& pair : compressedBinaryCodes) {
	//	std::cout << "'" << pair.first << "' : " << pair.second << std::endl;
	//}

	Node* currentNode = tree.getRootNode();
	if (!currentNode) {
		std::cerr << "Error: Huffman tree is empty" << std::endl;
		return;
	}

	long long decodedCount = 0;
	unsigned char buffer;

	while (inFile.read(reinterpret_cast<char*>(&buffer), sizeof(buffer))) {
		for (int i = 7; i >= 0; i--) {
			if (decodedCount >= totalCharacters) {
				break;
			}

			int bit = (buffer >> i) & 1;

			if (bit == 0) {
				currentNode = currentNode->left;
			}
			else {
				currentNode = currentNode->right;
			}

			if (currentNode->left == nullptr && currentNode->right == nullptr) {
				outFile.write(&currentNode->character, sizeof(currentNode->character));

				currentNode = tree.getRootNode();
			}
		}

		if (decodedCount >= totalCharacters) {
			break;
		}
	}

	inFile.close();
	outFile.close();

	std::cout << "File successfully decompressed at " << outputFile << std::endl;
}