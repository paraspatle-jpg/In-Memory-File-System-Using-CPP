#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "TrieNode.h"
#include "json.hpp"

using json = nlohmann::json;

class FileSystem {
private:
    TrieNode* root;

public:
    TrieNode* currentDirectory;
    FileSystem();

    void mkdir(const std::string& name);
    void cd(const std::string& path);
    std::vector<std::string> ls(const std::string& path = "");
    void touch(const std::string& filename);
    void echo(const std::string& text, const std::string& filename);
    std::string cat(const std::string& filename);
    void mv(const std::string& sourcePath, const std::string& destPath);
    void cp(const std::string& sourcePath, const std::string& destPath);
    void rm(const std::string& path);

    TrieNode* traverseFilePath(const std::string& path);
    TrieNode* traversePath(const std::string& path);


    json serialize() const;

    void deserialize(const json& data);

    json serializeNode(const TrieNode* node) const;
    TrieNode* deserializeNode(const json& data, TrieNode* parent);

    void saveStateToFile(const std::string& filename) const;
    void loadStateFromFile(const std::string& filename);
};

#endif // FILESYSTEM_H
