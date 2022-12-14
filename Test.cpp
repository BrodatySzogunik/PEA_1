//
// Created by kmord on 16.10.2022.
//

#include "Test.h"
#include "BruteForce.h"
#include "Timer.h"
#include <iostream>
#include <fstream>
#include <vector>


void Test::runTest() {

    BruteForce bruteForce;
    Timer timer;

    std::string result;
    std::fstream configfile;
    std::fstream outputFile;
    configfile.open("config.ini", std::ios::in);
    outputFile.open("result.csv", std::ios::out);


    if (!configfile.good()) {
        std::cout << "\nNie mozna wczytac pliku!\n";

    }


    std::string fileName;
    int testCount ;
    int instanceSize;
    int optimalPathSize;
    std::vector<int> optimalPath;
    int vertex;

    while(!configfile.eof()){
        optimalPath.clear();
        configfile >> fileName;
        configfile >> testCount;
        configfile >> instanceSize;
        configfile >> optimalPathSize;

        for (int i = 0; i < instanceSize; i++) {
            configfile >> vertex;
            optimalPath.push_back(vertex);
        }
        optimalPath[instanceSize+1] = optimalPath[0];

        outputFile << fileName<<" ";
        outputFile << testCount<<" ";
        outputFile << instanceSize<<" ";
        outputFile << optimalPathSize<<" { ";
        for (int i = 0; i < instanceSize; i++) {
            outputFile << optimalPath[i]<<" ";
        }
        outputFile<<"}";

        outputFile << "\n";
            bruteForce.ReadFromFile(fileName);
        for(int i = 0 ; i < testCount ; i++)
        {
            timer.start();
            result = bruteForce.bruteForceAlgorithm(0);
            timer.stop();
            outputFile << result << ";\t" << timer.getTime(Microseconds);
            outputFile << "\n";
        }

    };


    configfile.close();
    outputFile.close();




}
