#include "compressor/huffmanTree.h"

Node* HuffmanTree::createNode(char ch, int freq, Node* left, Node* right) {
	Node* newNode = new Node();
	newNode->character = ch;
	newNode->frequency = freq;
	newNode->left = left;
	newNode->right = right;

	return newNode;
}

void HuffmanTree::buildTree(const std::unordered_map<char, int>& frequencies) {
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