#pragma once
#include <unordered_map>
#include <queue>
#include <vector>

struct Node {
	char character;
	int frequency;
	Node* left;
	Node* right;
};

struct CompareNodes {
	bool operator()(Node* leftNode, Node* rightNode) {
		return leftNode->frequency > rightNode->frequency;
	}
};

class HuffmanTree {
public:
	Node* createNode(char ch, int freq, Node* left = nullptr, Node* right = nullptr);
	void buildTree(const std::unordered_map<char, int>& frequencies);
	Node* getRootNode();

private:
	Node* root;
};