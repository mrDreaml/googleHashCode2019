#pragma once

#include "Common.h"

vector<string> readFromFile(const string& filePath);

void writeToFile(const string& filePath, vector<string>& data);

vector<string> getStringTokens(string& line);

void parseInputData(const vector<string>& inputFileData, totalFileInfo& info, map<string, compileDataNode>& compiledData, map<string, vector<string>>& compiledDataDeps);

unsigned int parseOutputData(const vector<string>& fileData, vector<submitionResultNode>& submitionResult);;