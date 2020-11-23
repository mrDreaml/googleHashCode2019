#include "Common.h"

#include <map>
#include <sstream>
#include <vector>

using namespace std;

class Server {
	private:
		unsigned int serverTime = 0;
		vector<string> compiledFiles;
		map<string, unsigned int> replicationDeadLine;

		void calculateServerTime(compileDataNode& file, unsigned int replicationTotalTime) {
			serverTime += file.compileTime + replicationTotalTime;
		}

	public:
		unsigned int getServerTime() { return serverTime; }

		bool isServerContainFile(string fileName) {
			return replicationDeadLine.find(fileName) != replicationDeadLine.end();
		}

		int getReplicationTimeForFile(string fileName) {
			if (isServerContainFile(fileName)) {
				unsigned int currentDeadLine = replicationDeadLine[fileName] - getServerTime();
				return currentDeadLine < 0 ? 0 : currentDeadLine;
			}
			return -1;
		}

		void bindFile (compileDataNode &file, unsigned int replicationTotalTime) {
			calculateServerTime(file, replicationTotalTime);
			compiledFiles.push_back(file.name);
			replicationDeadLine[file.name] = getServerTime() + file.replicateTime;
		}


};