//
// Created by kmord on 16.10.2022.
//

#include "Test.h"
#include "TravelingSalesman.h"
#include "Timer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>



void Test::runTest() {
    srand( time( NULL ) );
    TSP TravelingSalesman;
    Timer timer;

    std::vector<int> result;

    std::fstream configfile;
    std::fstream outputFile;
    std::fstream averageOutputFile;
    configfile.open("config.ini", std::ios::in);



    if (!configfile.good()) {
        std::cout << "\nNie mozna wczytac pliku!\n";

    }


    std::string fileName;
    int testCount  = 0;
    int instanceSize = 0;
    double optimalPathSize = 0;
    int populationNumber = 0;
    double pm = 0 ;
    double pk = 0 ;
    std::string crossing="";
    std::string mutation="";
    std::string selection="";
    std::string success="";
    std::string stop="";
    TSP::CROSS crossingEnum;
    TSP::MUTATION mutationEnum;
    TSP::SELECT selectionEnum;
    TSP::SUCCESS successEnum;
    TSP::STOP stopEnum;
    int stopCounter = 0;
    double maxErrorRatio = 0;
    double errorRatio = 0;

    int timeSum = 0;
    double errorRatioSum = 0;
    int shortestPathSum = 0;

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
        configfile >> populationNumber;
        configfile >> pm ;
        configfile >> pk  ;
        configfile >> crossing;
        configfile >> mutation;
        configfile >> selection;
        configfile >> success;
        configfile >> stop;
        configfile >> stopCounter;
        configfile >> maxErrorRatio;

        outputFile.open(fileName+"resultAll.csv", std::ios::out);
        averageOutputFile.open(fileName+"resultAvg.csv", std::ios::out);


        crossingEnum = TravelingSalesman.crossing.find(crossing)->second;
        mutationEnum = TravelingSalesman.mutations.find(mutation)->second;;
        selectionEnum = TravelingSalesman.select.find(selection)->second;;
        successEnum = TravelingSalesman.success.find(success)->second;;
        stopEnum = TravelingSalesman.stop.find(stop)->second;;



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

        outputFile << populationNumber <<" ";
        outputFile << pm  <<" ";
        outputFile << pk   <<" ";
        outputFile << crossing <<" ";
        outputFile << mutation <<" ";
        outputFile << selection <<" ";
        outputFile << success <<" ";
        outputFile << stop <<" ";
        outputFile << stopCounter <<" ";
        outputFile << maxErrorRatio <<" ";

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

        outputFile <<"totalCost;error;time;PopulationNumber;pm;pk;cross;mutate;select;success;stop;stopCounter;optimalPathSize;maxErrorRatio\n";

        averageOutputFile << fileName<<" ";
        averageOutputFile << testCount<<" ";
        averageOutputFile << instanceSize<<" ";

        averageOutputFile << populationNumber <<" ";
        averageOutputFile << pm  <<" ";
        averageOutputFile << pk   <<" ";
        averageOutputFile << crossing <<" ";
        averageOutputFile << mutation <<" ";
        averageOutputFile << selection <<" ";
        averageOutputFile << success <<" ";
        averageOutputFile << stop <<" ";
        averageOutputFile << stopCounter <<" ";
        averageOutputFile << maxErrorRatio <<" ";

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

        averageOutputFile <<"totalCost;error;time;PopulationNumber;pm;pk;cross;mutate;select;success;stop;stopCounter;optimalPathSize;maxErrorRatio\n";


        TravelingSalesman.ReadFromFile(fileName);
        int populationNumbers[] = { 200};
        double pms [] = {0.3};
        double pks [] = { 0.9,};
        TSP::CROSS crossIt [] = {TSP::CROSS::OX};
        TSP::MUTATION mutateIt [] = {TSP::MUTATION::SWP ,TSP::MUTATION::INV};
        TSP::SELECT selectIt [] = { TSP::SELECT::DUE};
        TSP::SUCCESS successIt [] = { TSP::SUCCESS::FCH};
        TSP::STOP stopIt [] = { TSP::STOP::NCH};



        for(auto populationNumber1: populationNumbers){
            for( auto pm1 : pms){
                for( auto pk1 : pks){
                    for(auto mutate : mutateIt){
                        for(auto select : selectIt){
                            for(auto successs : successIt){
                                for(auto stopp : stopIt){
                                    for(auto cross : crossIt){
                                        crossing=cross ? "OX" : "PMX";
                                        mutation=mutate ? "INV" : "SWP";
                                        selection=select ? "DUE" : "TUR";
                                        success=successs ? "ECH" : "FCH";
                                        stop=stopp ? "NCH" : "GEN";
                                        for(int i = 0 ; i < testCount ; i++)
                                        {

                                            timer.start();
                                            result = TravelingSalesman.geneticAlgorithm(populationNumber1,pm1,pk1, cross,mutate,select,successs,stopp,stopCounter, optimalPathSize, maxErrorRatio) ;
                                            timer.stop();

                                            errorRatio = (result[result.size()-1] / optimalPathSize -1) * 100;

                                            shortestPathSum +=result[result.size()-1];
                                            errorRatioSum += errorRatio;
                                            timeSum += timer.getTime(Seconds);
                                            outputFile << result[result.size()-1] << ";\t" << errorRatio << ";\t" << timer.getTime(Seconds) << ";" <<populationNumber1<< ";" << pm1 << ";" << pk1 << ";"<< crossing<< ";"<< mutation << ";" << selection << ";" << success << ";" << stop << ";" <<stopCounter<<";"<< optimalPathSize << ";" << maxErrorRatio<<"\n";
                                            outputFile.flush();
                                        }


                                        averageOutputFile << shortestPathSum/testCount <<";"<< errorRatioSum/testCount <<";"<< timeSum/testCount << ";" <<populationNumber1<< ";" << pm1 << ";" << pk1 << ";"<< crossing<< ";"<< mutation << ";" << selection << ";" << success << ";" << stop << ";" <<stopCounter<<";"<< optimalPathSize << ";" << maxErrorRatio<<"\n";
                                        shortestPathSum = 0;
                                        errorRatioSum = 0 ;
                                        timeSum = 0;
                                        averageOutputFile.flush();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }





//
//        for(int i = 0 ; i < testCount ; i++)
//        {
//            timer.start();
//            result = TravelingSalesman.geneticAlgorithm(populationNumber,pm,pk, crossingEnum,mutationEnum,selectionEnum,successEnum,stopEnum,stopCounter, optimalPathSize, maxErrorRatio) ;
//            timer.stop();
//
//            errorRatio = (result[result.size()-1] / optimalPathSize -1) * 100;
//
//            shortestPathSum +=result[result.size()-1];
//            errorRatioSum += errorRatio;
//            timeSum += timer.getTime(Seconds);
//            outputFile << result[result.size()-1] << ";\t" << errorRatio << ";\t" << timer.getTime(Seconds) << ";" <<populationNumber<< ";" << pm << ";" << pk << ";"<< crossing<< ";"<< mutation << ";" << selection << ";" << success << ";" << stop << ";" <<stopCounter<<";"<< optimalPathSize << ";" << maxErrorRatio<<"\n";
//            outputFile.flush();
//        }
//        averageOutputFile << shortestPathSum/testCount <<";"<< errorRatioSum/testCount <<";"<< timeSum/testCount << ";" <<populationNumber<< ";" << pm << ";" << pk << ";"<< crossing<< ";"<< mutation << ";" << selection << ";" << success << ";" << stop << ";" <<stopCounter<<";"<< optimalPathSize << ";" << maxErrorRatio<<"\n";
//        shortestPathSum = 0;
//        errorRatioSum = 0 ;
//        timeSum = 0;
//        averageOutputFile.flush();
//
//        outputFile.close();
//        averageOutputFile.close();
//
//        outputFile.clear();
//        averageOutputFile.clear();
    };


    configfile.close();
    outputFile.close();
    averageOutputFile.close();




}
