#pragma once
#include <string>
#include <unordered_map>

class FileHandler {
public:
	// Function to read and output the file content
	void readAndPrint(const std::string& filePath);

	std::unordered_map<char, int> getCharacterFrequencies(const std::string& filePath);
};