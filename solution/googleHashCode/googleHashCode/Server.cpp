#include "Server.h"
#include "Common.h"

#include <map>
#include <sstream>
#include <vector>

using namespace std;

void Server::calculateServerTime(compileDataNode& file, unsigned int replicationTotalTime) {
	serverTime += file.compileTime + replicationTotalTime;
	serverTime;
}

unsigned int Server::getServerTime() { return serverTime; }

bool Server::isServerContainFile(string fileName) {
	return replicationDeadLine.find(fileName) != replicationDeadLine.end();
}

unsigned int Server::getReplicationTimeForFile(string fileName) {
		int currentDeadLine = replicationDeadLine[fileName] - serverTime;
		return currentDeadLine < 0 ? 0 : currentDeadLine;

}

void Server::bindFile (compileDataNode &file, unsigned int replicationTotalTime) {
	calculateServerTime(file, replicationTotalTime);
	serverTime;
	scoreNode s;
	s.name = file.name;
	s.score = 0;
	if (file.deadlineTime != NO_COMPILED_DATA_INFO) {
		int delta = file.deadlineTime - serverTime;
		if (delta >= 0) {
			s.score += delta + file.goalPoints;
		}
	}

	compiledFiles.push_back(s);
	replicationDeadLine[file.name] = serverTime + file.replicateTime;
}

unsigned int Server::calcSummuryScore() {
	unsigned int res = 0;
	for (auto s : compiledFiles) {
		res += s.score;
	}
	return res;
}