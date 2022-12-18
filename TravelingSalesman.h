//
// Created by kmord on 15.10.2022.
//

#ifndef BRUTEFORCE_TRAVELINGSALESMAN_H
#define BRUTEFORCE_TRAVELINGSALESMAN_H
#include <iostream>
#include <vector>
#include "./Structures/Array.h"
#include "./Structures/Path.h"

class TSP {

public:
    TSP()=default;
    void ReadFromFile(std::string fileName);
    void LoadFileTXT(std::string &filename);
    void LoadFileTSP(std::string &filename);
//    void LoadFileATSP(std::string &filename);
    std::string bruteForceAlgorithm(int start);
    int branchAndBound(int start, int actualVertex, std::vector<bool> remainingVertexes, int actualShortestPath = INT32_MAX, int actualPath = 0, int recLevel =0  );
    std::vector<int> shortestPath;
    void GenerateRandomInstance(int VertexNumber);
    std::string PrintInstance();
    PEA::Path* SimulatedAnnealing(int startingTemperature, double coolingRatio, int epochLength);
    int calculateCost( SDIZO::Array<int>& vertices);
    double getRandom();
    double calculateProbability(const int delta, const double temperature);
    bool makeDecision(const int delta, const double temperature);
    void switchVertex(SDIZO::Array<int>& vertices);
    SDIZO::Array<int> getInitialSolution(int verticesNumber);

private:
    double _coolingRatio = 0.999;
    double _startTemperature = 1000;

    int** neighborhoodMatrix;
    int* vertexNumber = new int(0) ;
    std::string *fileName;



};


#endif //BRUTEFORCE_TRAVELINGSALESMAN_H
