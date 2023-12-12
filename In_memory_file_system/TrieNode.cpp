#include "TrieNode.h"

TrieNode::TrieNode(bool isDir) : isDirectory(isDir) {}

TrieNode::~TrieNode() {
    for (auto& entry : children) {
        delete entry.second;
    }
}
