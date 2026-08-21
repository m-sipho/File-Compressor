#pragma once
#include <string>
#include <unordered_map>

class FileHandler {
public:
	// Function to read and output the file content
	void readAndPrint(const std::string& filePath);

	std::unordered_map<char, int> getCharacterFrequencies(const std::string& filePath);

	void writeCompressedFile(const std::string& inputFile, const std::string& outputFile, const std::unordered_map<char, std::string>& compressedCodes, const std::unordered_map<char, int>& frequencies);

	void decompressFile(const std::string& compressedFilePath, const std::string& outputFile);
};