
#include "FileSystem.h"
#include <fstream>
#include <iomanip>
#include <sstream>

#include <iostream>

FileSystem::FileSystem() {
    root = new TrieNode(true);
    currentDirectory = root;
    currentDirectory->fullPath = "";
    currentDirectory->name = "";
}





void FileSystem::mkdir(const std::string& name) {
    if (!currentDirectory->children.count(name)) {
        TrieNode* newDir = new TrieNode(true);
        newDir->children[".."] = currentDirectory;
        newDir->fullPath = currentDirectory->fullPath + "/" + name;
        newDir->name = name;
        currentDirectory->children[name] = newDir;
        std::cout << "Successfully created "+name+" !!" << std::endl;
    }
}

void FileSystem::touch(const std::string& filename) {
    if (!currentDirectory->children.count(filename)) {
        TrieNode* newFile = new TrieNode(false);
        newFile->children[".."] = currentDirectory;
        newFile->fullPath = currentDirectory->fullPath + "/" + filename;
        newFile->name = filename;
        currentDirectory->children[filename] = newFile;
        std::cout << "Successfully created " + filename + " !!" << std::endl;
    }
}
void FileSystem::cd(const std::string& path) {
    if (path == "/") {
        currentDirectory = root;
    }
    else if (path == ".." && currentDirectory->isDirectory && currentDirectory->children.count("..")) {
        currentDirectory = currentDirectory->children[".."];
    }
    else {
        TrieNode* targetDir = traversePath(path);
        if (targetDir) {
            currentDirectory = targetDir;
        }
    }
}

std::vector<std::string> FileSystem::ls(const std::string& path) {
    TrieNode* targetDir = (path.size() == 0) ? currentDirectory: traversePath(path);

    if (targetDir && targetDir->isDirectory) {
        std::vector<std::string> result;
        for (const auto& entry : targetDir->children) {
            result.push_back(entry.first);
        }
        return result;
    }
    else {
        return {};
    }
}


void FileSystem::echo(const std::string& text, const std::string& filename) {
    TrieNode* targetFile = traverseFilePath(filename);
    if (targetFile && !targetFile->isDirectory) {
        targetFile->content = text;
    }
    else {
        touch(filename);
        echo(text, filename);
    }
}

std::string FileSystem::cat(const std::string& filename) {
    TrieNode* targetFile = traverseFilePath(filename);
    if (targetFile && !targetFile->isDirectory) {
        return targetFile->content;
    }
    else {
        return "File not found.";
    }
}

void FileSystem::mv(const std::string& sourcePath, const std::string& destPath) {
    TrieNode* sourceEntry = traverseFilePath(sourcePath);
    TrieNode* destDirectory = traversePath(destPath);

    if (sourceEntry && destDirectory && destDirectory->isDirectory) {
        TrieNode* parentOfSourceEntry = sourceEntry->children[".."];
        sourceEntry->fullPath = destDirectory->fullPath + '/' + sourceEntry->name;
        sourceEntry->children[".."] = destDirectory;
        destDirectory->children[sourceEntry->name] = sourceEntry;
        parentOfSourceEntry->children.erase(sourceEntry->name);
    }
}

void FileSystem::cp(const std::string& sourcePath, const std::string& destPath) {
    TrieNode* sourceEntry = traverseFilePath(sourcePath);
    TrieNode* destDirectory = traversePath(destPath);
    if (sourceEntry && destDirectory && destDirectory->isDirectory) {
        TrieNode* newEntry = new TrieNode(sourceEntry->isDirectory);
        newEntry->content = sourceEntry->content;
        newEntry->fullPath = destDirectory->fullPath + '/' + sourceEntry->name;
        newEntry->children[".."] = destDirectory;
        destDirectory->children[sourceEntry->name] = newEntry;

    }
}

void FileSystem::rm(const std::string& path) {
    TrieNode* entry = traverseFilePath(path);
    if (entry && entry != currentDirectory) {
        TrieNode* parentOfSourceEntry = entry->children[".."];
        parentOfSourceEntry->children.erase(entry->name);

    }
    else {
        if (entry == currentDirectory)
            std::cout << "Cannot remove current directory" << std::endl;
    }
}



void FileSystem::saveStateToFile(const std::string& filename) const {
    json serializedState = serialize();

    std::ofstream file(filename);
    file << std::setw(4) << serializedState << std::endl;
    file.close();
}

void FileSystem::loadStateFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    json recoveredState;
    inputFile >> recoveredState;
    deserialize(recoveredState);
}

json FileSystem::serialize() const {
    return serializeNode(root);
}

void FileSystem::deserialize(const json& data) {
    delete root;
    root = deserializeNode(data, nullptr);
    currentDirectory = root;
}


json FileSystem::serializeNode(const TrieNode* node) const {
    std::cout << "In Savestate" << std::endl;

    json result;
    result["isDirectory"] = node->isDirectory;
    result["content"] = node->content;
    result["fullPath"] = node->fullPath;
    result["name"] = node->name;

    for (const auto& entry : node->children) {
        result["children"][entry.first] = (entry.first == "..") ? "prev" : serializeNode(entry.second);
    }
    std::cout << "Out Savestate" << std::endl;


    return result;
}

TrieNode* FileSystem::deserializeNode(const json& data, TrieNode* parent) {
    TrieNode* node = new TrieNode(data["isDirectory"]);

    if (data.count("content")) {
        node->content = data["content"];
    }
    if (data.count("name")) {
        node->name = data["name"];
    }
    if (data.count("fullPath")) {
        node->fullPath = data["fullPath"];
    }

    if (data.count("children")) {
        for (const auto& entry : data["children"].items()) {
            node->children[entry.key()] = entry.key() == ".." ? parent : deserializeNode(entry.value(), node);
        }
    }

    return node;
}


TrieNode* FileSystem::traversePath(const std::string& path) {
    std::istringstream iss(path);
    std::string component;
    TrieNode* current = (path[0] == '/') ? root : currentDirectory;

    while (std::getline(iss, component, '/')) {
        if (component == ".." && current->children.count("..")) {
            current = current->children[".."];
        }
        else if (current->children.count(component)) {
            if (current->children[component]->isDirectory)
                current = current->children[component];
            else {
                std::cout << "Please specify valid path!!" << std::endl;
                return nullptr;
            }
        }
        else {
            std::cout << path + " does not exist." << std::endl;
            return nullptr;
        }
    }

    return current;
}

TrieNode* FileSystem::traverseFilePath(const std::string& path) {
    std::istringstream iss(path);
    std::string component;
    TrieNode* current = (path[0] == '/') ? root : currentDirectory;

    while (std::getline(iss, component, '/')) {
        if (component == ".." && current->children.count("..")) {
            current = current->children[".."];
        }
        else if (current->children.count(component)) {
            current = current->children[component];
        }
        else {
            std::cout << path + " does not exist." << std::endl;
            return nullptr;
        }
    }

    return current;
}

