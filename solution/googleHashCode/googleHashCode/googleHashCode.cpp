#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Common.h"
#include "Server.cpp"

using namespace std;

const string SELECTED_INPUT_FILE_PATH = "..//inputData//a_example.in";
const string SELECTED_OUTPUT_FILE_PATH = "..//outputData/output.out";

vector<string> readFromFile(const string& filePath) {
    ifstream file(filePath);
    vector<string> result;
    for (string line; getline(file, line);) {
        result.push_back(line);
    }
    return result;
}

void writeToFile(const string& filePath, vector<string>& data) {
    ofstream file;
    file.open(filePath);
    for (string line : data) {
        file << line << endl;
    }
    file.close();
}

vector<string> getStringTokens(string& line) {
    vector<string> tokens;
    size_t pos;
    do {
        pos = line.find(' ');
        tokens.push_back(line.substr(0, pos));
        line.erase(0, pos + 1);
    } while (pos != string::npos);
    return tokens;
}

void parseData(const vector<string> &inputFileData, totalFileInfo &info, map<string, compileDataNode> &compiledData, map<string, vector<string>> &compiledDataDeps) {
    unsigned int i = 0;
    compileDataNode node;
    string fileName;
    for (string line : inputFileData) {
        cout << i << ": " << line << endl;
        auto tokens = getStringTokens(line);
        if (i == 0) {
            info.filesQ = stoi(tokens[0]);
            info.targetsQ = stoi(tokens[1]);
            info.serversQ = stoi(tokens[2]);
        }
        else if (i <= info.filesQ * 2) {
            if (i % 2 == 1) {
                fileName = tokens[0];
                node.name = fileName;
                node.compileTime = stoi(tokens[1]);
                node.replicateTime = stoi(tokens[2]);
            }
            else {
                node.depsQ = stoi(tokens[0]);
                if (tokens.size() > 1) {
                    compiledDataDeps[fileName] = std::vector<string>(tokens.begin() + 1, tokens.end());
                }
            }
            compiledData[fileName] = node;
        }
        else {
            compileDataNode* currentNode = &compiledData[tokens[0]];
            currentNode->deadlineTime = stoi(tokens[1]);
            currentNode->goalPoints = stoi(tokens[2]);
        }
        i++;
    }
}

bool isCanBeCompiled(string targetName, map<string, vector<string>> &deps) {
    vector<string> depsV = deps[targetName];
    if (!depsV.empty()) {
        for (string depName : depsV) {
            if (!isCanBeCompiled(depName, deps)) {
                return false;
            }
        }
    }
    return true;
}


void main() {
    totalFileInfo info;
    map<string, compileDataNode> compiledData;
    map<string, vector<string>> compiledDataDeps;
    vector<string> inputFileData = readFromFile(SELECTED_INPUT_FILE_PATH);
    parseData(inputFileData, info, compiledData, compiledDataDeps);

    Server s;
    Server s2;
    s2.bindFile(compiledData["c0"], 10);

    cout << endl << s.getServerTime() << " " << s2.getServerTime();

    // vector<string> d = { "7", "c0 1" };
    //writeToFile(SELECTED_OUTPUT_FILE_PATH, d);
}

