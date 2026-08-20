#include <iostream>
#include "compressor/fileHandler.h"
#include "compressor/huffmanTree.h"

using namespace std;

int main(int argc, char* argv[]) {
	// Check if the user provided correct number of arguments
	if (argc != 2) {
		cerr << "Usage: " << argv[0] << " <FILENAME>" << endl;
		return -1;
	}

	string filePath = argv[1];

	// Generate the output file
	string outputFile;
	// Find where the final dot is
	size_t dotPosition = filePath.find_last_of('.');

	if (dotPosition != string::npos) {
		outputFile = filePath.substr(0, dotPosition) + "_compressed.bin";
	}
	else {
		outputFile = filePath + "_compressed.bin";
	}

	cout << "--- File Content ---\n";
	FileHandler fileHandler;
	fileHandler.readAndPrint(filePath);
	cout << "\n--------------------------------\n";

	auto frequencyMap = fileHandler.getCharacterFrequencies(filePath);

	cout << "--- Character Frequencies ---\n";
	for (const auto& pair : frequencyMap) {
		
		cout << "'" << pair.first << "' : " << pair.second << endl;
	}

	HuffmanTree tree;
	tree.buildTree(frequencyMap);
	cout << "Root node frequency: " << tree.getRootNode()->frequency << endl;

	tree.generateCompressedBinaryCodes();
	auto compressedBinaryCodes = tree.getCompressedBinaryCodes();

	cout << "--- Compressed Binary Codes ---\n";
	for (const auto& pair : compressedBinaryCodes) {

		cout << "'" << pair.first << "' : " << pair.second << endl;
	}

	fileHandler.writeCompressedFile(filePath, outputFile, compressedBinaryCodes, frequencyMap);
	return 0;
}