#pragma once

#include "Common.h"
#include "Server.h"

#include <map>
#include <sstream>
#include <vector>

class ServerManager {
private:
    vector<Server> servers;
public:
    ServerManager(unsigned int serversQ);
    unsigned long int calcSummaryScore();
    vector<unsigned int> getServersTime();
    void bindFileToServer(compileDataNode& file, unsigned int serverId, map<string, compileDataNode>& compiledData, map<string, vector<string>>& compiledDataDeps);
    long int calculateReplicationTime(const string fileName, unsigned int serverId, map<string, compileDataNode>& compiledData, map<string, vector<string>>& compiledDataDeps);
};

