//
// Created by kmord on 16.10.2022.
//

#include "Test.h"
#include "TravelingSalesman.h"
#include "Timer.h"
#include <iostream>
#include <fstream>
#include <vector>


void Test::runTest() {
    srand( time( NULL ) );
    TSP TravelingSalesman;
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

    std::vector<bool> vertexes;
    std::vector<int> optimalPath;
    int vertex;

    while(!configfile.eof()){
        optimalPath.clear();
        vertexes.clear();
        TravelingSalesman.shortestPath.clear();
        configfile >> fileName;
        configfile >> testCount;
        configfile >> instanceSize;
        configfile >> optimalPathSize;

        for (int i = 0; i < instanceSize; i++) {
            configfile >> vertex;
            optimalPath.push_back(vertex);
        }
        optimalPath[instanceSize+1] = optimalPath[0];

        for(int i = 1 ; i <= instanceSize ; i++){
            vertexes.push_back(false);
        }
        vertexes[0] = true;

        for(int i = 1 ; i < instanceSize ; i++){
            TravelingSalesman.shortestPath.push_back(i);
        }



        outputFile << fileName<<" ";
        outputFile << testCount<<" ";
        outputFile << instanceSize<<" ";
        outputFile << optimalPathSize<<" { ";
        for (int i = 0; i < instanceSize; i++) {
            outputFile << optimalPath[i]<<" ";
        }
        outputFile<<"}";

        outputFile << "\n";
        TravelingSalesman.ReadFromFile(fileName);

        for(int i = 0 ; i < testCount ; i++)
        {

            timer.start();
            result = std::to_string(TravelingSalesman.heldKarp(0, 0,vertexes ));
            timer.stop();
            outputFile << std::to_string(0) << " , ";
            for(int i = 0 ; i < instanceSize ; i++){
                outputFile << std::to_string(TravelingSalesman.shortestPath[i]) << " , ";
            }
            outputFile <<" ; "<< result << ";\t" << timer.getTime(Microseconds);
            outputFile << "\n";
        }

    };


    configfile.close();
    outputFile.close();




}
