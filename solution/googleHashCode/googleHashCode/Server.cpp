#include "Common.h"

#include <map>
#include <sstream>
#include <vector>

using namespace std;

struct cf {
	string name;
	unsigned int score;
};

class Server {
	private:
		unsigned int serverTime = 0;
		vector<cf> compiledFiles;
		map<string, unsigned int> replicationDeadLine;

		void calculateServerTime(compileDataNode& file, unsigned int replicationTotalTime) {
			serverTime += file.compileTime + replicationTotalTime;
		}

		bool isServerContainFile(string fileName) {
			return replicationDeadLine.find(fileName) != replicationDeadLine.end();
		}
	public:
		unsigned int getServerTime() { return serverTime; }

		int getReplicationTimeForFile(string fileName) {
			if (isServerContainFile(fileName)) {
				unsigned int currentDeadLine = replicationDeadLine[fileName] - serverTime;
				return currentDeadLine < 0 ? 0 : currentDeadLine;
			}
			return -1;
		}

		void bindFile (compileDataNode &file, unsigned int replicationTotalTime) {
			calculateServerTime(file, replicationTotalTime);
			cf s;
			s.name = file.name;
			s.score = 0;
			if (file.deadlineTime != NO_COMPILED_DATA_INFO) {
				int delta = file.deadlineTime - serverTime;
				if (delta > 0) {
					s.score += delta + file.goalPoints;
				}
			}

			compiledFiles.push_back(s);
			replicationDeadLine[file.name] = serverTime + file.replicateTime;
		}

		unsigned int calcSummuryScore() {
			unsigned int res = 0;
			for (cf s : compiledFiles) {
				res += s.score;
			}
			return res;
		}
};