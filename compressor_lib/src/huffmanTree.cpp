#include "compressor/huffmanTree.h"

Node* HuffmanTree::createNode(char ch, int freq, Node* left, Node* right) {
	Node* newNode = new Node();
	newNode->character = ch;
	newNode->frequency = freq;
	newNode->left = left;
	newNode->right = right;

	return newNode;
}