//
// Created by kmord on 15.10.2022.
//

#ifndef BRUTEFORCE_TRAVELINGSALESMAN_H
#define BRUTEFORCE_TRAVELINGSALESMAN_H
#include <iostream>
#include <vector>

class TSP {

public:
    TSP()=default;
    void ReadFromFile(std::string fileName);
    void LoadFileTXT(std::string &filename);
    void LoadFileTSP(std::string &filename);
//    void LoadFileATSP(std::string &filename);
    std::string bruteForceAlgorithm(int start);
    int heldKarp(int start, int actualVertex, std::vector<bool> remainingVertexes, int actualShortestPath = INT32_MAX, int actualPath = 0, int recLevel =0  );
    std::vector<int> shortestPath;
    void GenerateRandomInstance(int VertexNumber);
    std::string PrintInstance();
private:
    int** neighborhoodMatrix;
    int* vertexNumber = new int(0) ;
    std::string *fileName;



};


#endif //BRUTEFORCE_TRAVELINGSALESMAN_H
