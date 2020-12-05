#include "GeneticAlgorithm.h"

populationT GeneticAlgorithm::generatePopulation(unsigned int populationQ) {
		int size = _compiledData.size();
		populationT population;
		for (unsigned int p = 0; p < populationQ; p++) {
			chromosomeT chromosome;
			vector<string> fileNames;
			for (auto it = _compiledData.begin(); it != _compiledData.end(); it++) {
				fileNames.push_back(it->first);
			}
			for (unsigned int i = 0; i < size; i++) {
				auto fileNameId = rand() % fileNames.size();
				genT gen;
				gen.fileName = fileNames[fileNameId];
				gen.serverId = rand() % _serversQ;
				chromosome.push_back(gen);
				swap(fileNames[fileNameId], fileNames.back());
				fileNames.pop_back();
			}
			population.push_back(chromosome);
		}
		return population;
}

int GeneticAlgorithm::evaluationFunction(chromosomeT& chromosome) {
	ServerManager sManager(_serversQ);
	try {
		for (genT gen : chromosome) {
			sManager.bindFileToServer(_compiledData[gen.fileName], gen.serverId, _compiledData, _compiledDataDeps);
		}
	}
	catch (...) {
		return EMPTY;
	}
	return sManager.calcSummaryScore();
}

void GeneticAlgorithm::recalcWeight(weighedPopulationT& weighedPopulation, unsigned int id) {
	chromosomeWithWeight* selected = &weighedPopulation[id];
	int weight = evaluationFunction(selected->chromosome);
	if (weight != EMPTY) {
		selected->weight = weight;
	} else {
		swap(*selected, weighedPopulation.back());
		weighedPopulation.pop_back();
	}
}

void GeneticAlgorithm::mutateSingle(weighedPopulationT& weighedPopulation) {
	unsigned int id = rand() % weighedPopulation.size();
	chromosomeWithWeight* selectedChromosome = &weighedPopulation[id];
	chromosomeT* chromosome = &selectedChromosome->chromosome;
	for (genT gen : *chromosome) {
		if (rand() % 2 == 0) {
			gen.serverId = rand() % _serversQ;
		}
	}
	swap(chromosome[rand() % weighedPopulation.size()], chromosome[rand() % weighedPopulation.size()]);
	recalcWeight(weighedPopulation, id);
}

chromosomeWithWeight GeneticAlgorithm::getBestChromosome(weighedPopulationT& weighedPopulation) {
	chromosomeWithWeight bestNode = weighedPopulation[0];
	for (auto node : weighedPopulation) {
		if (node.weight > bestNode.weight) {
			bestNode = node;
		}
	}
	return bestNode;
}

GeneticAlgorithm::GeneticAlgorithm(map<string, compileDataNode> compiledData, map<string, vector<string>> compiledDataDeps, unsigned int serversQ) {
		_compiledData = compiledData;
		_compiledDataDeps = compiledDataDeps;
		_serversQ = serversQ;
}

void GeneticAlgorithm::start(unsigned int populationQ = 10, const unsigned int STARGANTION_PERIOD = 10) {
	populationT population = generatePopulation(populationQ);
	weighedPopulationT weighedPopulation;
	for (auto chromosome : population) {
		int weight = evaluationFunction(chromosome);
		if (weight != EMPTY) {
			chromosomeWithWeight cw;
			cw.chromosome = chromosome;
			cw.weight = weight;
			weighedPopulation.push_back(cw);
		}
	}
	unsigned int stagnationPeriod = 0;
	chromosomeWithWeight bestNode = getBestChromosome(weighedPopulation);
	while (stagnationPeriod < STARGANTION_PERIOD) {
		mutateSingle(weighedPopulation);
		chromosomeWithWeight currentNode = getBestChromosome(weighedPopulation);
		if (bestNode.weight >= currentNode.weight) {
			stagnationPeriod++;
		} else {
			bestNode = currentNode;
			stagnationPeriod = 0;
		}
	}
}
