#include <iostream>
#include <fstream>
#include <sstream>

#include "Common.h"
#include "InputOutput.h"
#include "Server.h"
#include "ServerManager.h"
#include "GeneticAlgorithm.h"

const string SELECTED_INPUT_FILE_PATH = "..//inputData//a_example.in";
const string SELECTED_OUTPUT_FILE_PATH = "..//outputData/output.out";
const string SUBMITION_FILE_PATH = "..//outputData//submitionExample.out";


void startServersBySubmitionData (unsigned int serversQ, vector<submitionResultNode> submitionResults, map<string, compileDataNode> compiledData, map<string, vector<string>> compiledDataDeps) {
    ServerManager sManager(serversQ);
    for (submitionResultNode submitionResult : submitionResults) {
        sManager.bindFileToServer(compiledData[submitionResult.name], submitionResult.serverId, compiledData, compiledDataDeps);
    }
    cout << "Score: " << sManager.calcSummaryScore() << endl;
    int serverId = 0;
    for (auto t : sManager.getServersTime()) {
        cout << "Server Time " << serverId++ << ": " << t << endl;
    }
    sManager;
}


int main() {
    srand(time(NULL)); // allows to generate new random values
    totalFileInfo info;
    map<string, compileDataNode> compiledData;
    map<string, vector<string>> compiledDataDeps;
    parseInputData(readFromFile(SELECTED_INPUT_FILE_PATH), info, compiledData, compiledDataDeps);

    //vector<submitionResultNode> submitionResults;
    //parseOutputData(readFromFile(SUBMITION_FILE_PATH /*SELECTED_OUTPUT_FILE_PATH*/), submitionResults);
    //startServersBySubmitionData(info.serversQ, submitionResults, compiledData, compiledDataDeps);

    GeneticAlgorithm ga = GeneticAlgorithm(compiledData, compiledDataDeps, info.serversQ);
    ga.start(100, 30);

    // vector<string> d = { "7", "c0 1" };
    //writeToFile(SELECTED_OUTPUT_FILE_PATH, d);
}

