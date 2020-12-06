#include <iostream>

#include "GeneticAlgorithm.h"

populationT GeneticAlgorithm::generatePopulation(unsigned int populationQ) {
		populationT population;
		vector<string> fileNames;
		int size = _compiledData.size();
		for (auto it = _compiledData.begin(); it != _compiledData.end(); it++) {
			fileNames.push_back(it->first);
		}

		for (unsigned int p = 0; p < populationQ; p++) {
			for (unsigned int i = 0; i < size; i++) {
				swap(fileNames[rand() % size], fileNames[rand() % size]);
			}
			chromosomeT chromosome;
			for (unsigned int i = 0; i < size; i++) {
				genT gen;
				gen.fileName = fileNames[i];
				gen.serverId = rand() % _serversQ;
				chromosome.push_back(gen);
			}
			int weight = evaluationFunction(chromosome);
			if (weight != EMPTY) {
				chromosomeWithWeight cw;
				cw.chromosome = chromosome;
				cw.weight = weight;
				population.push_back(cw);
			}
		}
		return population;
}

int GeneticAlgorithm::evaluationFunction(chromosomeT& chromosome) {
	ServerManager sManager(_serversQ);
	for (genT gen : chromosome) {
		bool isBinded = sManager.bindFileToServer(_compiledData[gen.fileName], gen.serverId, _compiledData, _compiledDataDeps);
		if (!isBinded) {
			return EMPTY;
		}
	}
	return sManager.calcSummaryScore();
}

void GeneticAlgorithm::recalcWeight(populationT& population, unsigned int id) {
	chromosomeWithWeight* selected = &population[id];
	int weight = evaluationFunction(selected->chromosome);
	if (weight != EMPTY) {
		selected->weight = weight;
	} else {
		swap(*selected, population.back());
		population.pop_back();
	}
}

void GeneticAlgorithm::mutateSingle(populationT& population) {
	unsigned int id = rand() % population.size();
	chromosomeWithWeight* selectedChromosome = &population[id];
	chromosomeT* chromosome = &selectedChromosome->chromosome;
	for (genT& gen : *chromosome) {
		if (rand() % 2 == 0) {
			gen.serverId = rand() % _serversQ;
		}
	}
	unsigned int swapId1 = rand() % chromosome->size();
	unsigned int swapId2 = rand() % chromosome->size();
	swap((*chromosome)[swapId1], (*chromosome)[swapId2]);
	recalcWeight(population, id);
}

chromosomeWithWeight GeneticAlgorithm::getBestChromosome(populationT& population) {
	chromosomeWithWeight bestNode = population[0];
	for (auto node : population) {
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
	unsigned int stagnationPeriod = 0;
	if (population.size() == 0) {
		return;
	}
	chromosomeWithWeight bestNode = getBestChromosome(population);
	cout << "\nq:" << population.size() << endl;
	while (stagnationPeriod < STARGANTION_PERIOD) {
		for (unsigned int i = 0; i < population.size(); i++) {
			if (rand() % 2 == 0) {
				mutateSingle(population);
			}
		}
		if (population.size() == 0) {
			break;
		}
		chromosomeWithWeight currentNode = getBestChromosome(population);
		if (bestNode.weight >= currentNode.weight) {
			stagnationPeriod++;
		} else {
			cout << "\n+++\n";
			bestNode = currentNode;
			stagnationPeriod = 0;
		}
	}
	bestNode;
	std::cout << "\nGeneteic result:\n";
	cout << "final population: " << population.size() << endl;
	cout << "best score: " << bestNode.weight << endl;
}
