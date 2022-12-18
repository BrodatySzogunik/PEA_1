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

    PEA::Path *result;

    std::fstream configfile;
    std::fstream outputFile;
    std::fstream averageOutputFile;
    configfile.open("config.ini", std::ios::in);
    outputFile.open("resultAll.csv", std::ios::out);
    averageOutputFile.open("resultAvg.csv", std::ios::out);



    if (!configfile.good()) {
        std::cout << "\nNie mozna wczytac pliku!\n";

    }


    std::string fileName;
    int testCount  = 0;
    int instanceSize = 0;
    double optimalPathSize = 0;
    double errorRatio = 0;

    int timeSum = 0;
    double errorRatioSum = 0;
    int shortestPathSum = 0;

    double coolingRatioValues[] = {0.9,0.99,0.999,0.9999};

    std::vector<bool> vertexes;
    std::vector<int> optimalPath;

    bool isOptimalPath = true;
    int vertex = 0;

    while(!configfile.eof()){
        optimalPath.clear();
        vertexes.clear();
        isOptimalPath = true;
        TravelingSalesman.shortestPath.clear();
        configfile >> fileName;
        configfile >> testCount;
        configfile >> instanceSize;
        configfile >> optimalPathSize;

        for (int i = 0; i < instanceSize; i++) {
            configfile >> vertex;
            if (vertex == -1){
                isOptimalPath = false;
                break;
            }
            optimalPath.push_back(vertex);
        }
        if(isOptimalPath){
            optimalPath[instanceSize+1] = optimalPath[0];
        }


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

        outputFile << optimalPathSize << " ";
        if(isOptimalPath){
            outputFile <<" { ";
            for (int i = 0; i < instanceSize; i++) {
                outputFile << optimalPath[i]<<" ";
            }
            outputFile<<"}";
        }else{
            outputFile<<"No optimal path";
        }

        outputFile << "\n";

        outputFile <<"totalCost;error;time;InitialTemperature;coolingRatio;epochSize\n";

        averageOutputFile << fileName<<" ";
        averageOutputFile << testCount<<" ";
        averageOutputFile << instanceSize<<" ";

        averageOutputFile << optimalPathSize << " ";
        if(isOptimalPath){
            averageOutputFile <<" { ";
            for (int i = 0; i < instanceSize; i++) {
                averageOutputFile << optimalPath[i]<<" ";
            }
            averageOutputFile<<"}";
        }else{
            averageOutputFile<<"No optimal path";
        }

        averageOutputFile << "\n";

        averageOutputFile <<"totalCost;error;time;InitialTemperature;coolingRatio;epochSize\n";


        TravelingSalesman.ReadFromFile(fileName);


        for(int inTemp = 10; inTemp <= 100000; inTemp *= 10){
            for(double coolingRatio: coolingRatioValues){
                for(int epochSize = 10; epochSize <= 100000; epochSize *=10 ){
                    for(int i = 0 ; i < testCount ; i++)
                    {
                        timer.start();
                        result = TravelingSalesman.SimulatedAnnealing(inTemp,coolingRatio,epochSize) ;
                        timer.stop();

                        errorRatio = (result->getTotalCost()/optimalPathSize -1) * 100;
                        shortestPathSum +=result ->getTotalCost();
                        errorRatioSum += errorRatio;
                        timeSum += timer.getTime(Microseconds);
                        outputFile << result ->getTotalCost() << ";\t" << errorRatio << ";\t" << timer.getTime(Microseconds)<< ";" << inTemp<<";" << coolingRatio << ";"<<epochSize << "\n";
                    }
                    averageOutputFile << shortestPathSum/testCount <<";"<< errorRatioSum/testCount <<";"<< timeSum/testCount<< ";" << inTemp<<";" << coolingRatio << ";"<<epochSize << "\n";
                    shortestPathSum = 0;
                    errorRatioSum = 0;
                    timeSum = 0;
                }
            }
        }

    };


    configfile.close();
    outputFile.close();




}
