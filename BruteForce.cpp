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

    if (fileExt == "tsp") {
        return BruteForce::LoadFileTSP(filename);
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
            if(currentPath+this->neighborhoodMatrix[previousVertex][start] >= shortestPath){
                break;
            }
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



int BruteForce::heldKarp(int start, int actualVertex, std::vector<bool> remainingVertexes, int actualShortestPath , int actualPath ) {

    std::vector<bool> bufor;
    if(std::find(remainingVertexes.begin(),remainingVertexes.end(),false) == remainingVertexes.end()){
        return actualPath + this -> neighborhoodMatrix[actualVertex][start];
    }else{
        for(int i = 0 ; i < remainingVertexes.size(); i++){
            if(!remainingVertexes[i]){
                if(actualPath + this->neighborhoodMatrix[actualVertex][i] < actualShortestPath){
                    bufor = remainingVertexes;
                    bufor[i] = true;
                    std::next_permutation(bufor.begin(), bufor.end());
                    actualShortestPath = std::min(heldKarp(start, i,bufor,actualShortestPath,actualPath + this->neighborhoodMatrix[actualVertex][i] ), actualShortestPath)   ;
                    bufor.clear();
                }
            }
        }
        return actualShortestPath;
    }
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


void BruteForce::LoadFileTSP(std::string & filename)
{
    std::ifstream File;
    File.open(filename, std::ios::in);
    if (!File.good()) {
        std::cout << "\nNie mozna wczytac pliku!\n";

    }

    if (this->vertexNumber != 0) {
        for (int i = 0; i < *this->vertexNumber; i++)
            delete this->neighborhoodMatrix[i];
        delete[] this->neighborhoodMatrix;
        this->neighborhoodMatrix = NULL;
        this->vertexNumber = 0;
    }

    std::string line;
    while (!File.eof()) {
        getline(File, line);
        {
            if (!line.compare(0, 6, "TYPE: ")) {
                if (!line.compare(7, 3, "TSP")) {
                    std::cout << "\n To nie jest plik TSP\n"; // czy typ pliku to TSP

                }

            }
            else if (!line.compare(0, 7, "TYPE : ")) {
                if (!line.compare(8, 3, "TSP")) {
                    std::cout << "\n To nie jest plik TSP\n"; // czy typ pliku to TSP
                }
            }

            if (!line.compare(0, 11, "DIMENSION: ")) {
                std::string number = line.substr(11, 100);
                this->vertexNumber = new int(stoi(number)); //zapisanie ilo�ci wierzcho�k�w
            }
            else if (!line.compare(0, 12, "DIMENSION : ")) {
                std::string number = line.substr(12, 100);
                *(this->vertexNumber) = stoi(number); //zapisanie ilo�ci wierzcho�k�w
            }


            if (!line.compare(0, 18, "EDGE_WEIGHT_TYPE: ")) {
                if (!line.compare(19, 8, "EXPLICIT")) {
                    std::cout << "\n Nieobslugiwany typ danych\n"; // czy zapisano jako wagi kraw�dzi
                }
            }
            else if (!line.compare(0, 19, "EDGE_WEIGHT_TYPE : ")) {
                if (!line.compare(20, 8, "EXPLICIT")) {
                    std::cout << "\n Nieobslugiwany typ danych\n"; // czy zapisano jako wagi kraw�dzi
                }
            }


            if (!line.compare(0, 20, "EDGE_WEIGHT_FORMAT: ")) { //Obs�uga r�nego typu zapisu macierzy
                if (!line.compare(20, 11, "FULL_MATRIX")) {

                    this->neighborhoodMatrix = new int*[*this->vertexNumber];
                    for (int i = 0; i < *this->vertexNumber; i++)
                        this->neighborhoodMatrix[i] = new int[*this->vertexNumber];

                    std::string temp;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = 0; j < *this->vertexNumber; j++)
                                for (int k = 0; k < *this->vertexNumber; k++)
                                    File >> this->neighborhoodMatrix[j][k];
                        }
                    }
                }
                else if (!line.compare(20, 14, "LOWER_DIAG_ROW")) {

                    this->neighborhoodMatrix = new int*[*this->vertexNumber];
                    for (int i = 0; i < *this->vertexNumber; i++)
                        this->neighborhoodMatrix[i] = new int[*this->vertexNumber];

                    std::string temp;
                    int line = 1;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = 0; j < *this->vertexNumber; j++) {
                                for (int k = 0; k < line; k++) {
                                    File >> this->neighborhoodMatrix[j][k];
                                    if (j != k)
                                        this->neighborhoodMatrix[k][j] = this->neighborhoodMatrix[j][k];
                                }
                                if (line < *this->vertexNumber)
                                    line++;
                            }

                        }
                    }
                }
                else if (!line.compare(20, 14, "UPPER_DIAG_ROW")) {

                    this->neighborhoodMatrix = new int*[*this->vertexNumber];
                    for (int i = 0; i < *this->vertexNumber; i++)
                        this->neighborhoodMatrix[i] = new int[*this->vertexNumber];

                    std::string temp;
                    int line = *this->vertexNumber-1;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = *this->vertexNumber-1; j >= 0; j--) {
                                for (int k = line; k >= 0; k--) {
                                    File >> this->neighborhoodMatrix[j][k];
                                    if (j != k)
                                        this->neighborhoodMatrix[k][j] = this->neighborhoodMatrix[j][k];
                                }
                                if (line >= 0)
                                    line--;
                            }
                        }
                    }
                }
                else {
                    std::cout << "\nNieobs�ugiwany format macierzy\n";
                }
            }

            else if (!line.compare(0, 21, "EDGE_WEIGHT_FORMAT : ")) { //Obs�uga r�nego typu zapisu macierzy
                if (!line.compare(21, 11, "FULL_MATRIX")) {

                    this->neighborhoodMatrix = new int*[*this->vertexNumber];
                    for (int i = 0; i < *this->vertexNumber; i++)
                        this->neighborhoodMatrix[i] = new int[*this->vertexNumber];

                    std::string temp;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = 0; j < *this->vertexNumber; j++)
                                for (int k = 0; k < *this->vertexNumber; k++)
                                    File >> this->neighborhoodMatrix[j][k];
                        }
                    }
                }
                else if (!line.compare(21, 14, "LOWER_DIAG_ROW")) {

                    this->neighborhoodMatrix = new int*[*this->vertexNumber];
                    for (int i = 0; i < *this->vertexNumber; i++)
                        this->neighborhoodMatrix[i] = new int[*this->vertexNumber];

                    std::string temp;
                    int line = 1;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = 0; j < *this->vertexNumber; j++) {
                                for (int k = 0; k < line; k++) {
                                    File >> this->neighborhoodMatrix[j][k];
                                    if (j != k)
                                        this->neighborhoodMatrix[k][j] = this->neighborhoodMatrix[j][k];
                                }
                                if (line < *this->vertexNumber)
                                    line++;
                            }

                        }
                    }
                }
                else if (!line.compare(21, 14, "UPPER_DIAG_ROW")) {

                    this->neighborhoodMatrix = new int*[*this->vertexNumber];
                    for (int i = 0; i < *this->vertexNumber; i++)
                        this->neighborhoodMatrix[i] = new int[*this->vertexNumber];

                    std::string temp;
                    int line = *this->vertexNumber - 1;
                    while (!File.eof()) {
                        File >> temp;
                        if (temp == "EDGE_WEIGHT_SECTION") {
                            for (int j = *this->vertexNumber - 1; j >= 0; j--) {
                                for (int k = line; k >= 0; k--) {
                                    File >> this->neighborhoodMatrix[j][k];
                                    if (j != k)
                                        this->neighborhoodMatrix[k][j] = this->neighborhoodMatrix[j][k];
                                }
                                if (line >= 0)
                                    line--;
                            }
                        }
                    }
                }
                else {
                    std::cout << "\nNieobs�ugiwany format macierzy\n";
                }
            }
        }

    }
    File.close();
    std::cout << "\nWczytano plik!\n";


    for(int i = 0 ; i < *this -> vertexNumber ; i++){
        for(int j = 0 ; j < *this -> vertexNumber ; j++){
            std::cout<<" "<<this->neighborhoodMatrix[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}
//
//void BruteForce::LoadFileATSP(std::string & filename)
//{
//    ifstream File;
//    File.open(filename, ios::in);
//    if (!File.good()) {
//        this->filename = "Brak wczytanego pliku";
//        cout << "\nNie mozna wczytac pliku!\n";
//        return 0;
//    }
//    this->filename = filename;
//
//    if (numberOfVertices != 0) {
//        for (int i = 0; i < numberOfVertices; i++)
//            delete vertices[i];
//        delete[] vertices;
//        vertices = NULL;
//        numberOfVertices = 0;
//    }
//
//    string line;
//    while (!File.eof()) {
//        getline(File, line);
//        {
//            if (!line.compare(0, 6, "TYPE: "))
//                if (!line.compare(7, 4, "ATSP")) {
//                    cout << "\n To nie jest plik ATSP\n"; // czy typ pliku to ATSP
//                    return 1;
//                }
//
//            if (!line.compare(0, 11, "DIMENSION: ")) {
//                string number = line.substr(11, 100);
//                numberOfVertices = stoi(number); //zapisanie ilo�ci wierzcho�k�w
//            }
//            if (!line.compare(0, 18, "EDGE_WEIGHT_TYPE: "))
//                if (!line.compare(19, 8, "EXPLICIT")) {
//                    cout << "\n Nieobslugiwany typ danych\n"; // czy zapisano jako wagi kraw�dzi
//                    return 1;
//                }
//            if (!line.compare(0, 20, "EDGE_WEIGHT_FORMAT: ")) { //Obs�uga r�nego typu zapisu macierzy
//                if (!line.compare(20, 11, "FULL_MATRIX")) {
//
//                    vertices = new int*[numberOfVertices];
//                    for (int i = 0; i < numberOfVertices; i++)
//                        vertices[i] = new int[numberOfVertices];
//
//                    string temp;
//                    while (!File.eof()) {
//                        File >> temp;
//                        if (temp == "EDGE_WEIGHT_SECTION") {
//                            for (int j = 0; j < numberOfVertices; j++)
//                                for (int k = 0; k < numberOfVertices; k++)
//                                    File >> vertices[j][k];
//                        }
//                    }
//                }
//                else {
//                    cout << "\nNieobs�ugiwany format macierzy\n";
//                    return 1;
//                }
//            }
//        }
//
//    }
//    File.close();
//    cout << "\nWczytano plik!\n";
//    return 0;
//}



