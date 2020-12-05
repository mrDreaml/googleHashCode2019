#pragma once

#include "Common.h"
#include "ServerManager.h"

#include <map>
#include <sstream>
#include <vector>

struct genT { unsigned int serverId = 0; string fileName; };
typedef vector<genT> chromosomeT;
struct chromosomeWithWeight { chromosomeT chromosome; int weight; };
typedef vector<chromosomeT> populationT;
typedef vector<chromosomeWithWeight> weighedPopulationT;

class GeneticAlgorithm
{
	map<string, compileDataNode> _compiledData;
	map<string, vector<string>> _compiledDataDeps;
	unsigned int _serversQ;

private:
	populationT generatePopulation(unsigned int populationQ);
	int evaluationFunction(chromosomeT& chromosome);
	void recalcWeight(weighedPopulationT& weighedPopulation, unsigned int);
	void crossover(weighedPopulationT& weighedPopulation);
	void mutateSingle(weighedPopulationT& weightPopulation);
	chromosomeWithWeight getBestChromosome(weighedPopulationT& weightPopulation);
public:
	GeneticAlgorithm(map<string, compileDataNode> compiledData, map<string, vector<string>> compiledDataDeps, unsigned int serversQ);
	void start(unsigned int populationQ, unsigned int stagnationPeriod);
};
