#ifndef TRIENODE_H
#define TRIENODE_H

#include <unordered_map>
#include <string>

class TrieNode {
public:
    std::unordered_map<std::string, TrieNode*> children;
    bool isDirectory;
    std::string content;
    std::string fullPath;
    std::string name;

    TrieNode(bool isDir = false);
    ~TrieNode();
};

#endif // TRIENODE_H
