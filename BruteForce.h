//
// Created by kmord on 15.10.2022.
//

#ifndef BRUTEFORCE_BRUTEFORCE_H
#define BRUTEFORCE_BRUTEFORCE_H
#include <iostream>

class BruteForce {

public:
    BruteForce()=default;
    void ReadFromFile(std::string fileName);
    void LoadFileTXT(std::string &filename);
    void LoadFileTSP(std::string &filename);
//    void LoadFileATSP(std::string &filename);
    std::string bruteForceAlgorithm(int start);
private:
    int** neighborhoodMatrix;
    int* vertexNumber = 0 ;
    std::string *fileName;

};


#endif //BRUTEFORCE_BRUTEFORCE_H
