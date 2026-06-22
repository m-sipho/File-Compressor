#include <iostream>
#include "compressor/fileHandler.h"

using namespace std;

int main() {
	cout << "--- File Content ---\n";
	FileHandler fileHandler;
	fileHandler.readAndPrint("data.txt");
	cout << "\n--------------------------------\n";
	return 0;
}