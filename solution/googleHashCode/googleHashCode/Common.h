#pragma once

#include <map>
#include <sstream>
#include <vector>
#include <map>

const int NO_COMPILED_DATA_INFO = -1;
const int EMPTY = -1;

using namespace std;

struct compileDataNode {
    string name;
    unsigned int compileTime;
    unsigned int replicateTime;
    int deadlineTime = NO_COMPILED_DATA_INFO;
    int goalPoints = NO_COMPILED_DATA_INFO;
    unsigned int depsQ;
};

struct submitionResultNode {
    string fileName;
    unsigned int serverId;
};

struct totalFileInfo {
    unsigned int filesQ;
    unsigned int targetsQ;
    unsigned int serversQ;
};