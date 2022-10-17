//
// Created by kmord on 15.10.2022.
//

#include "BruteForce.h"
#include "Timer.h"
#include <iostream>
#include <bits/stdc++.h>

void BruteForce::ReadFromFile(std::string filename) {
    std::string fileExt = filename.substr(filename.find_last_of(".") + 1);
    if (fileExt != "txt" && fileExt != "tsp" && fileExt != "atsp") //dodanie do nazwy pliku rozszerzenia .txt gdy go nie ma
    {
        filename += ".txt";
    }
    fileExt = filename.substr(filename.find_last_of(".") + 1);

    if (fileExt == "txt") {
        return BruteForce::LoadFileTXT(filename);
    }
}

std::string BruteForce::bruteForceAlgorithm(int start){
    std::vector<int> vertex;
    std::vector<int> shortestPathVertex;
    Timer timer;


    for(int i = 0 ; i < *this->vertexNumber ; i++) {    //generowanie wektora zawierającego wszystkie dostępne wierzchołki
        if (i != start)
            vertex.push_back(i);
    }

    int shortestPath = INT_MAX;
    shortestPathVertex = vertex;

    do{
        int currentPath = 0;

        int previousVertex = start;

        for(int i = 0 ; i< vertex.size() ; i++){        //obliczenie wartości obecnie testowanej ścieżki
            currentPath += this->neighborhoodMatrix[previousVertex][vertex[i]];
            previousVertex = vertex [i];
        }
        currentPath += this->neighborhoodMatrix[previousVertex][start];

        if(currentPath < shortestPath){                 //Podmiana wartości shortestPath i shortestPathVerticles jeżeli znaleziono krótszą ścieżkę
            shortestPath = currentPath;
            shortestPathVertex = vertex;
        }

    }while(std::next_permutation(vertex.begin(),vertex.end()));     //Tworzenie kolejnej permutacji do momentu wyczerpania wszystkich możliwości

    std::string result ="";
    result += std::to_string(shortestPath)+";";
    result+=std::to_string( 0)+" , ";
    for(int i  = 0 ; i < *this->vertexNumber-1; i++)
        result+=std::to_string(shortestPathVertex[i])+" , ";
    result+=std::to_string( 0);
    return result;
}

void BruteForce::LoadFileTXT(std::string & filename)
{
    std::fstream File;
    File.open(filename, std::ios::in);
    if (!File.good()) {
        std::cout << "\nNie mozna wczytac pliku!\n";
    }


    int numberOfVertices;
    File >> numberOfVertices;
    this->vertexNumber = new int(numberOfVertices);
    this->neighborhoodMatrix = new int*[numberOfVertices];
    int line = 0;

    for (int i = 0; i < numberOfVertices; i++)
        this -> neighborhoodMatrix[i] = new int[numberOfVertices];

    while (!File.eof()) {
        for (int i = 0; i < numberOfVertices; i++) {
            for (int j = 0; j < numberOfVertices; j++) {
                File >> this->neighborhoodMatrix[i][j];
            }

            line++;
        }
    }
    File.close();

    if (line != numberOfVertices && line != numberOfVertices * 2) { // zabezpieczenie pir
        std::cout << "-> Zla ilosc danych <-\n";
        for (int i = 0; i < numberOfVertices; i++)
            delete this->neighborhoodMatrix[i];
        delete[] this->neighborhoodMatrix;
        this->neighborhoodMatrix = NULL;
        numberOfVertices = 0;

    }
    std::cout << "\nWczytano plik!\n";

}



