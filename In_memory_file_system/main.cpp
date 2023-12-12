#include "FileSystem.h"
#include <iostream>
#include <sstream>

void printLS(std::vector<std::string> list) {
    for (auto child : list) {
        std::cout << child << std::endl;
    }
}

bool startsWith(const std::string& str, const std::string& prefix) {
    return (str.substr(0, prefix.length()) == prefix);
}

std::vector<std::string> splitString(const std::string& input) {
    std::istringstream iss(input);
    std::vector<std::string> words;

    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }

    return words;
}

std::string removeQuotes(const std::string& input) {
    if (input.length() >= 2) {
        return input.substr(1, input.length() - 2);
    }
    else {
        return "";
    }
}

int main() {
    FileSystem fs;
    std::cout << "Do you want to look for any previous state and load it ?(y for yes, anything else for no)" << std::endl;
    std::string doLoadState;
    getline(std::cin, doLoadState);
    if(doLoadState == "y")
        fs.loadStateFromFile("filesystem_state.json");

    std::cout << "Hello World!!" << std::endl;

    while (true) {
        std::cout << fs.currentDirectory->fullPath + ">";
        std::string input;
        getline(std::cin, input);
        std::vector<std::string> splitInput = splitString(input);
        
        // change directory : accepts 1 parameter
        if (splitInput[0] =="cd") {
            std::vector<std::string> splitInput = splitString(input);
            if (splitInput.size() == 2) {
                fs.cd(splitInput[1]);
            }
            else {
                
                std::cout << "Invalid command expected 2 arguments got "<<splitInput.size()<< std::endl;
            }
        }

        // ls : accepts 0 or 1 parameters
        else if (splitInput[0] == "ls") {
            if (splitInput.size() == 1) {
                printLS(fs.ls());
            }
            else if (splitInput.size() == 2) {
                printLS(fs.ls(splitInput[1]));
            }
            else {
                std::cout << "Invalid command expected 2 arguments got " << splitInput.size() << std::endl;
            }
        }

        // mkdir : accepts 1 parameter
        else if (splitInput[0] == "mkdir") {
            if (splitInput.size() == 2) {
                fs.mkdir(splitInput[1]);
            }
            else {
                std::cout << "Invalid command expected 2 arguments got " << splitInput.size() << std::endl;
            }
        }

        // touch : accepts 1 parameter
        else if (splitInput[0] == "touch") {
            if (splitInput.size() == 2) {
                fs.touch(splitInput[1]);
            }
            else {
                std::cout << "Invalid command expected 2 arguments got " << splitInput.size() << std::endl;
            }
        }

        // cat : accepts 1 parameter
        else if (splitInput[0] == "cat") {
            if (splitInput.size() == 2) {
                std::cout << fs.cat(splitInput[1]) << std::endl;
            }
            else {
                std::cout << "Invalid command expected 2 arguments got " << splitInput.size() << std::endl;
            }
        }

        // echo : accepts 2 parameter
        else if (splitInput[0] == "echo") {
            std::string content;
            int i = 1;
            while (splitInput[i] != ">" && i < splitInput.size()) {
                content += splitInput[i];
                i++;
            }
            if (i == splitInput.size()) {
                std::cout << "Invalid command !!" << std::endl;
                continue;
            }
            i++;
            if (splitInput.size() == i+1 && content.size() >= 2 && (content[0]=='\'' || content[0]=='"') && (content[content.size()-1] == '\'' || content[content.size() - 1] == '"')) {
                content = removeQuotes(content);
                fs.echo(content, splitInput[i]);
            }
            else {
                std::cout << "Invalid command expected 2 arguments got " << splitInput.size() << std::endl;
            }
        }

        // cp : accepts 3 parameter
        else if (splitInput[0] == "cp") {
            if (splitInput.size() == 3) {
                fs.cp(splitInput[1], splitInput[2]);
            }
            else {
                std::cout << "Invalid command expected 2 arguments got " << splitInput.size() << std::endl;
            }
        }

        // mv : accepts 3 parameter
        else if (splitInput[0] == "mv") {
            if (splitInput.size() == 3) {
                fs.mv(splitInput[1], splitInput[2]);
            }
            else {
                std::cout << "Invalid command expected 2 arguments got " << splitInput.size() << std::endl;
            }
        }

        // rm : accepts 3 parameter
        else if (splitInput[0] == "rm") {
            if (splitInput.size() == 2) {
                fs.rm(splitInput[1]);
            }
            else {
                std::cout << "Invalid command expected 2 arguments got " << splitInput.size() << std::endl;
            }
        }
        else if (input == "exit") {
            break;
        }

        else {
            std::cout << "Invalid command !!"<< std::endl;
        }
    }

    fs.saveStateToFile("filesystem_state.json");

    return 0;
}
