#include "ServerManager.h"

#include <map>
#include <sstream>
#include <vector>



ServerManager::ServerManager(unsigned int serversQ) {
    for (unsigned int i = 0; i < serversQ; i++) {
        Server s;
        servers.push_back(s);
    }
};

unsigned int ServerManager::calcSummaryScore() {
    unsigned int summaryScore = 0;
    for (Server server : servers) {
        summaryScore += server.calcSummuryScore();
    }
    return summaryScore;
}

vector<unsigned int> ServerManager::getServersTime() {
    vector<unsigned int> result;
    for (Server server : servers) {
        result.push_back(server.getServerTime());
    }
    return result;
}

bool ServerManager::bindFileToServer (compileDataNode& file, unsigned int serverId, map<string, compileDataNode>& compiledData, map<string, vector<string>>& compiledDataDeps) {
    unsigned int replicationTotalTime = calculateReplicationTime(file.name, serverId, compiledData, compiledDataDeps);
    if (replicationTotalTime != EMPTY) {
        servers[serverId].bindFile(file, replicationTotalTime);
        return true;
    }
    return false;
}

int ServerManager::calculateReplicationTime(const string fileName, unsigned int serverId, map<string, compileDataNode>& compiledData, map<string, vector<string>>& compiledDataDeps) {
    vector<string>* deps = &compiledDataDeps[fileName];
    unsigned int replicationTime = 0;
    for (string depFileName : *deps) {
        int currentFileReplicationTime = EMPTY;
        for (unsigned int i = 0; i < servers.size(); i++) {
            if (servers[i].isServerContainFile(depFileName)) {
                if (serverId != i) {
                    unsigned int tmp = servers[i].getReplicationTimeForFile(depFileName, servers[serverId].getServerTime());
                    if (currentFileReplicationTime != EMPTY) {
                        currentFileReplicationTime = currentFileReplicationTime > tmp ? tmp : currentFileReplicationTime;
                    } else {
                        currentFileReplicationTime = tmp;
                    }
                } else {
                    currentFileReplicationTime = 0;
                    break;
                }
            }
        }
        if (currentFileReplicationTime == EMPTY) {       
            return EMPTY;
        }
        replicationTime += currentFileReplicationTime;
    }
    return replicationTime;
}

