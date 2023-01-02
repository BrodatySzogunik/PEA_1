//
// Created by kmord on 15.10.2022.
//

#ifndef BRUTEFORCE_TRAVELINGSALESMAN_H
#define BRUTEFORCE_TRAVELINGSALESMAN_H
#include <iostream>
#include <vector>
#include <unordered_map>

class TSP {

public:
    enum MUTATION{SWP, INV};
    enum CROSS{PMX,OX};
    enum STOP {GEN, NCH};
    enum SELECT{TUR, DUE};
    enum SUCCESS{FCH , ECH};
    std::unordered_map<std::string,MUTATION> const mutations = { {"SWP",MUTATION::SWP},{"INV",MUTATION::INV}};
    std::unordered_map<std::string,CROSS> const crossing = { {"PMX", CROSS::PMX}, {"OX", CROSS::OX} };
    std::unordered_map<std::string,STOP> const stop = { {"GEN", STOP::GEN}, {"NCH", STOP::NCH} };
    std::unordered_map<std::string,SELECT> const select = { {"TUR", SELECT::TUR}, {"DUE", SELECT::DUE} };
    std::unordered_map<std::string,SUCCESS> const success = { {"FCH", SUCCESS::FCH}, {"ECH", SUCCESS::ECH} };





    TSP()=default;
    void ReadFromFile(std::string fileName);
    void LoadFileTXT(std::string &filename);
    void LoadFileTSP(std::string &filename);
    std::vector<int> shortestPath;
//    std::vector<int> SimulatedAnnealing(int startingTemperature, double coolingRatio, int epochLength,bool isGeoCoolingType);
    int calculateCost( std::vector<int>& vertices);
    double getRandom();
//    double calculateProbability(const int delta, const double temperature);
    bool makeDecision(double probability);
    void mutateBySwap(std::vector<int>& vertices);
    void mutateByInversion(std::vector<int>& vertices);
    void mutateByScramble(std::vector<int>& vertices);
    std::vector<std::vector<int>>  generateInitialSolution(int populationNumber,int verticesNumber);
    void copyGenomes(const std::vector<int> & source_, std::vector<int> & destination_, int firstIndex_, int secondIndex_);
    void PMXCrossing(std::vector<int> & parent1, std::vector<int> & parent2,std::vector<int> & child1, std::vector<int> & child2);
    void OXCrossing(std::vector<int> & parent1, std::vector<int> & parent2,std::vector<int> & child1, std::vector<int> & child2);
    std::vector<int> geneticAlgorithm(int populationSize = 100,
                                      double pm = 0.1,
                                      double pk = 0.9,
                                      CROSS crossing = CROSS::OX ,
                                      MUTATION mutation = MUTATION::INV,
                                      SELECT selection = SELECT::TUR,
                                      SUCCESS success = SUCCESS::ECH ,
                                      STOP stop = STOP::NCH,
                                      int stopCounter = 300,
                                      double optimalPath = 100,
                                      double maxErrorRatio = 150
    );
    std::vector<std::vector<int>> tournamentSelection(std::vector<std::vector<int>> & population, int populationSize,int k);
    std::vector<std::vector<int>> duelSelection(std::vector<std::vector<int>> & population, int populationSize);


private:
    double _coolingRatio = 0.999;
    double _startTemperature = 1000;

    int** neighborhoodMatrix;
    int* vertexNumber = new int(0) ;
    std::string *fileName;



};


#endif //BRUTEFORCE_TRAVELINGSALESMAN_H
