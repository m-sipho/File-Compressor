#include "compressor/huffmanTree.h"

Node* HuffmanTree::createNode(char ch, int freq, Node* left, Node* right) {
	Node* newNode = new Node();
	newNode->character = ch;
	newNode->frequency = freq;
	newNode->left = left;
	newNode->right = right;

	return newNode;
}

void HuffmanTree::buildTree(const std::vector<std::pair<char, int>>& frequencies) {
	std::priority_queue<Node*, std::vector<Node*>, CompareNodes> frequencyQueue;

	for (const auto& pair : frequencies) {
		frequencyQueue.push(createNode(pair.first, pair.second));
	}

	while (frequencyQueue.size() > 1) {
		Node* leftChild = frequencyQueue.top();
		frequencyQueue.pop();

		Node* rightChild = frequencyQueue.top();
		frequencyQueue.pop();

		int combinedFrequencies = leftChild->frequency + rightChild->frequency;

		Node* parentNode = createNode('\0', combinedFrequencies, leftChild, rightChild);

		frequencyQueue.push(parentNode);
	}

	if (frequencyQueue.empty()) {
		root = nullptr;
	}
	else {
		root = frequencyQueue.top();
	}
}

Node* HuffmanTree::getRootNode() {
	return root;
}

void HuffmanTree::generateCompressedBinaryCodes() {
	compressedBinaryCodes.clear();

	if (root != nullptr) {
		generateCompressedBinaryCodesHelper(root, "");
	}
}

void HuffmanTree::generateCompressedBinaryCodesHelper(Node* node, std::string currentCode) {
	// Check if we fall out of the tree
	if (node == nullptr) {
		return;
	}

	// Base case
	if (node->character != '\0') {
		compressedBinaryCodes[node->character] = currentCode;
		return;
	}

	// Recursive case
	generateCompressedBinaryCodesHelper(node->left, currentCode + "0"); // Add "0" when going to the left
	generateCompressedBinaryCodesHelper(node->right, currentCode + "1"); // Add "1" when going to the right
}

std::unordered_map<char, std::string> HuffmanTree::getCompressedBinaryCodes() {
	return compressedBinaryCodes;
}