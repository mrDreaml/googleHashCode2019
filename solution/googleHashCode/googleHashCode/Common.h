#include <sstream>
#pragma once

const int NO_COMPILED_DATA_INFO = -1;

struct compileDataNode {
    std::string name;
    unsigned int compileTime;
    unsigned int replicateTime;
    int deadlineTime = NO_COMPILED_DATA_INFO;
    int goalPoints = NO_COMPILED_DATA_INFO;
    unsigned int depsQ;
};

struct totalFileInfo {
    unsigned int filesQ;
    unsigned int targetsQ;
    unsigned int serversQ;
};