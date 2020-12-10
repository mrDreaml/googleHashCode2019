#pragma once

#include <iostream>

#include "Common.h"
#include "ServerManager.h"

#include <map>
#include <sstream>
#include <vector>
#include <algorithm>

typedef submitionResultNode genT;
typedef vector<genT> chromosomeT;
struct chromosomeWithWeight { chromosomeT chromosome; unsigned long int weight; };
typedef vector<chromosomeWithWeight> populationT;

typedef vector<string> groupOfFileNames;
typedef vector<groupOfFileNames> dethList;
typedef vector<dethList> groupOfDeths;

struct {
	bool operator() (chromosomeWithWeight a, chromosomeWithWeight b) { return (a.weight > b.weight); }
} weightComparator;


vector<unsigned int> generateRandRange(unsigned int size);
void generateGroupOfDeth(dethList& root, groupOfFileNames& currentDethFileNames, map<string, vector<string>>& deps);

class GeneticAlgorithm
{
	vector<string> _targetFiles;
	map<string, compileDataNode> _compiledData;
	map<string, vector<string>> _compiledDataDeps;
	unsigned int _serversQ;

private:
	populationT generatePopulation(unsigned int populationQ);
	unsigned long int evaluationFunction(chromosomeT& chromosome);
	void selection(populationT&, unsigned int);
	void crossover(populationT& weighedPopulation);
	void mutateSingle(populationT& weightPopulation);
	chromosomeWithWeight getBestChromosome(populationT& weightPopulation);
public:
	chromosomeT start(const unsigned int populationQ, const unsigned int stagnationPeriod, const unsigned int SELECTION_Q, const unsigned int SELECTION_PERIOD);
	GeneticAlgorithm(map<string, compileDataNode>& compiledData, map<string, vector<string>>& compiledDataDeps, vector<string>& targetFiles, unsigned int serversQ);
};

