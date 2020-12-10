#include "GeneticAlgorithm.h"

vector<unsigned int> generateRandRange(unsigned int size) {
	vector<unsigned int> res;
	for (unsigned int i = 0; i < size; i++) {
		res.push_back(i);
	}
	for (unsigned int i = 0; i < size; i++) {
		swap(res[rand() % size], res[rand() % size]);
	}
	return res;
}

void generateGroupOfDeth(dethList& root, groupOfFileNames& currentDethFileNames, map<string, vector<string>>& deps) {
	root.push_back(currentDethFileNames);
	groupOfFileNames nextDeth;
	for (string fileName : currentDethFileNames) {
		for (string depFileName : deps[fileName]) {
			nextDeth.push_back(depFileName);
		}
	}
	if (nextDeth.size() != 0) {
		return generateGroupOfDeth(root, nextDeth, deps);
	}
}

populationT GeneticAlgorithm::generatePopulation(unsigned int populationQ) {
		populationT population;
		groupOfDeths groupedSolutions;

		for (string targetFileName : _targetFiles) {
			dethList deths;
			groupOfFileNames rootNode = { targetFileName };
			generateGroupOfDeth(deths, rootNode, _compiledDataDeps);
			reverse(deths.begin(), deths.end());
			groupedSolutions.push_back(deths);
		}
		
		for (unsigned int p = 0; p < populationQ; p++) {
			chromosomeT chromosome;
			map<string, bool> compiled;
			for (auto solutionId : generateRandRange(groupedSolutions.size())) {
				for (groupOfFileNames fileNames : groupedSolutions[solutionId]) {
					string targetFileName;
					auto fileNamesSize = fileNames.size();
					if (fileNamesSize != 0) {
						for (auto id : generateRandRange(fileNamesSize)) {
							targetFileName = fileNames[id];
							if (!compiled[targetFileName]) {
								genT gen;
								compiled[targetFileName] = true;
								gen.fileName = targetFileName;
								gen.serverId = rand() % _serversQ;
								chromosome.push_back(gen);
							}
						}
					} else {
						targetFileName = fileNames[0];
						if (!compiled[targetFileName]) {
							genT gen;
							compiled[targetFileName] = true;
							gen.fileName = targetFileName;
							gen.serverId = rand() % _serversQ;
							chromosome.push_back(gen);
						}
					}
				}
			}
			unsigned long int weight;
			try {
				weight = evaluationFunction(chromosome);
			} catch (...) {
				continue;
			}
			cout << weight << endl;
			chromosomeWithWeight cw;
			cw.chromosome = chromosome;
			cw.weight = weight;
			population.push_back(cw);
		}
		return population;
}

unsigned long int GeneticAlgorithm::evaluationFunction(chromosomeT& chromosome) {
	ServerManager sManager(_serversQ);
	for (genT gen : chromosome) {
		sManager.bindFileToServer(_compiledData[gen.fileName], gen.serverId, _compiledData, _compiledDataDeps);
	}
	return sManager.calcSummaryScore();
}

void GeneticAlgorithm::selection(populationT& population, unsigned int selectionQ) {
	population;
	sort(population.begin(), population.end(), weightComparator);
	auto size = population.size();
	int d = size - (rand() % selectionQ);
	if (d > 0) {
		population.resize(d);
	}
}

void GeneticAlgorithm::crossover(populationT& population) {
	chromosomeWithWeight* parent1 = &population[rand() % population.size()];
	chromosomeWithWeight* parent2 = &population[rand() % population.size()];
	chromosomeT childChromosome;
	for (unsigned int i = 0; i < parent1->chromosome.size(); i++) {
		if (rand() % 2 == 0) {
			childChromosome.push_back(parent1->chromosome[i]); // mb clone?
		} else {
			childChromosome.push_back(parent2->chromosome[i]);
		}
	}
	unsigned long int weight;
	try {
		weight = evaluationFunction(childChromosome);
	}
	catch (...) { return;  }
	chromosomeWithWeight child;
	child.weight = weight;
	child.chromosome = childChromosome;
	cout << "+ crossover, weight:" << weight << endl;
	population.push_back(child);
}

void GeneticAlgorithm::mutateSingle(populationT& population) {
	unsigned int id = 0;
	chromosomeWithWeight* selectedChromosome = &population[id];
	chromosomeT chromosome = selectedChromosome->chromosome;
	for (genT& gen : chromosome) {
		if (rand() % 2 == 0) {
			gen.serverId = rand() % _serversQ;
		}
	}
	unsigned int swapId1 = rand() % chromosome.size();
	unsigned int swapId2 = rand() % chromosome.size();
	swap(chromosome[swapId1], chromosome[swapId2]);
	unsigned long int weight;
	try {
		weight = evaluationFunction(chromosome);
	}
	catch (...) { return; }

	cout << "+ mutation, weight: " << weight << endl;
	population[id];
	selectedChromosome;
	selectedChromosome->weight = weight;
	selectedChromosome->chromosome = chromosome;
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

chromosomeT GeneticAlgorithm::start(const unsigned int populationQ, const unsigned int STARGANTION_PERIOD, const unsigned int SELECTION_Q, const unsigned int SELECTION_PERIOD) {
	populationT population = generatePopulation(populationQ);
	unsigned int stagnationPeriod = 0;
	if (population.size() == 0) {
		chromosomeT empty;
		return empty;
	}
	chromosomeWithWeight bestNode = getBestChromosome(population);
	cout << "\nq:" << population.size() << endl;
	cout << "best score: " << bestNode.weight << endl;
	unsigned int p = 0;
	while (stagnationPeriod < STARGANTION_PERIOD && population.size() != 0) {
		if (p % SELECTION_PERIOD == 0) {
			selection(population, SELECTION_Q);
		}
		for (unsigned int i = 0; i < population.size(); i++) {
			if (rand() % 2 == 0) {
				mutateSingle(population);
			}
		}
		crossover(population);
		if (population.size() == 0) {
			break;
		}
		chromosomeWithWeight currentNode = getBestChromosome(population);
		if (bestNode.weight >= currentNode.weight) {
			stagnationPeriod++;
		} else {
			bestNode = currentNode;
			cout << "\nBetter: " << bestNode.weight << endl;
			stagnationPeriod = 0;
		}
		p++;
	}
	std::cout << "\nGeneteic result:\n";
	cout << "final population: " << population.size() << endl;
	cout << "best score: " << bestNode.weight << endl;
	return bestNode.chromosome;
}

GeneticAlgorithm::GeneticAlgorithm(map<string, compileDataNode>& compiledData, map<string, vector<string>>& compiledDataDeps, vector<string>& targetFiles, unsigned int serversQ) {
	_compiledData = compiledData;
	_compiledDataDeps = compiledDataDeps;
	_targetFiles = targetFiles;
	_serversQ = serversQ;
}