//
// Created by kmord on 15.10.2022.
//

#include "TravelingSalesman.h"
#include "Timer.h"
#include <iostream>
#include <bits/stdc++.h>

void TSP::ReadFromFile(std::string filename) {
    std::string fileExt = filename.substr(filename.find_last_of(".") + 1);
    if (fileExt != "txt" && fileExt != "tsp" && fileExt != "atsp") //dodanie do nazwy pliku rozszerzenia .txt gdy go nie ma
    {
        filename += ".txt";
    }
    fileExt = filename.substr(filename.find_last_of(".") + 1);

    if (fileExt == "txt") {
        return TSP::LoadFileTXT(filename);
    }

    if (fileExt == "tsp") {
        return TSP::LoadFileTSP(filename);
    }
}
//std::string TSP::PrintInstance(){
//    std::string result = "";
//    for(int i = 0 ; i < *this->vertexNumber; i++){
//        for(int j = 0 ; j < *this->vertexNumber ; j++){
//            std::cout << this -> neighborhoodMatrix[i][j] <<"\t";
//        }
//        std::cout << std::endl;
//    }
//    return result;
//}
//void TSP::GenerateRandomInstance(int vertexNumber){
//    if (*this->vertexNumber != 0) {
//        for (int i = 0; i < *this->vertexNumber; i++)
//            delete this->neighborhoodMatrix[i];
//        delete[] this->neighborhoodMatrix;
//        this->neighborhoodMatrix = NULL;
//        this->vertexNumber = 0;
//    }
//
//
//    this -> vertexNumber = new int(vertexNumber);
//
//    this->neighborhoodMatrix = new int*[vertexNumber];
//
//    for (int i = 0; i < vertexNumber; i++)
//        this -> neighborhoodMatrix[i] = new int[vertexNumber];
//
//    for(int i = 0 ; i < vertexNumber; i++){
//        for(int j = 0 ; j < vertexNumber ; j++){
//            this -> neighborhoodMatrix[i][j] = rand() % 100 + 1;
//        }
//    }
//
//}

//std::string TSP::bruteForceAlgorithm(int start){
//    std::vector<int> vertex;
//    std::vector<int> shortestPathVertex;
//    Timer timer;
//
//
//    for(int i = 0 ; i < *this->vertexNumber ; i++) {    //generowanie wektora zawierającego wszystkie dostępne wierzchołki
//        if (i != start)
//            vertex.push_back(i);
//    }
//
//    int shortestPath = INT_MAX;
//    shortestPathVertex = vertex;
//
//    do{
//        int currentPath = 0;
//
//        int previousVertex = start;
//
//        for(int i = 0 ; i< vertex.size() ; i++){        //obliczenie wartości obecnie testowanej ścieżki
//            currentPath += this->neighborhoodMatrix[previousVertex][vertex[i]];
//            previousVertex = vertex [i];
//            if(currentPath+this->neighborhoodMatrix[previousVertex][start] >= shortestPath){
//                break;
//            }
//        }
//        currentPath += this->neighborhoodMatrix[previousVertex][start];
//
//        if(currentPath < shortestPath){                 //Podmiana wartości shortestPath i shortestPathVerticles jeżeli znaleziono krótszą ścieżkę
//            shortestPath = currentPath;
//            shortestPathVertex = vertex;
//        }
//
//    }while(std::next_permutation(vertex.begin(),vertex.end()));     //Tworzenie kolejnej permutacji do momentu wyczerpania wszystkich możliwości
//
//    std::string result ="";
//    result += std::to_string(shortestPath)+";";
//    result+=std::to_string( 0)+" , ";
//    for(int i  = 0 ; i < *this->vertexNumber-1; i++)
//        result+=std::to_string(shortestPathVertex[i])+" , ";
//    result+=std::to_string( 0);
//    return result;
//}
//


//int TSP::branchAndBound(int start, int actualVertex, std::vector<bool> remainingVertexes, int actualShortestPath , int actualPath , int recLevel) {
//    int cost;
//
//    if(std::find(remainingVertexes.begin(),remainingVertexes.end(),false) == remainingVertexes.end()){
//        this -> shortestPath[recLevel] = start;
//        return actualPath + this -> neighborhoodMatrix[actualVertex][start];
//
//    }else{
//        for(int i = 0 ; i < remainingVertexes.size(); i++){
//            if(remainingVertexes[i])
//               continue;
//
//            cost = this->neighborhoodMatrix[actualVertex][i];
//
//            if(actualPath + cost < actualShortestPath){
//                remainingVertexes[i] = true;
//                int heldKarpResult = branchAndBound(start, i, remainingVertexes, actualShortestPath, actualPath + cost,
//                                                    recLevel + 1);
//                if(heldKarpResult < actualShortestPath){
//                    actualShortestPath = heldKarpResult;
//                    this -> shortestPath[recLevel] = i;
//                }
//                remainingVertexes[i] = false;
//            }
//        }
//        return actualShortestPath;
//    }
//}

std::vector<int> TSP::SimulatedAnnealing(int startingTemperature, double coolingRatio, int epochLength, bool isGeoCoolingType)
{
    int verticesNumber = * this -> vertexNumber;

    std::vector<int> vertices = this->generateInitialSolution(verticesNumber);

    int cost = this->calculateCost(vertices);
    double temperature = startingTemperature;

    std::vector<int> bestSolution = vertices;
    int bestCost = cost;
    int epochCount = 0 ;

    while (temperature > 1)
    {
        for(int i = 0 ; i <= epochLength ; i++ ) {
            std::vector<int> newVertices(vertices);
            this->switchVertex(newVertices);
            int newCost = this->calculateCost(newVertices);
            int delta = cost - newCost;

            if (delta < 0 && !this->makeDecision(delta, temperature))
                continue;

            cost = newCost;
            vertices = newVertices;;

            if (cost < bestCost) {
                bestSolution = vertices;
                bestCost = cost;
            }
        }


        epochCount++ ;
        if(isGeoCoolingType)
            temperature *= pow(coolingRatio,epochCount);
        else
            temperature = temperature/(1+log(1+epochCount));


    }

    std::vector<int> solution = bestSolution;
    solution.push_back(bestCost);
    return solution;
}

int TSP::calculateCost( std::vector<int>& vertices)
{
    int result = 0;
    int iterationNumber = *this -> vertexNumber - 1;

    for (int i = 0; i < iterationNumber; i++)
    {
        result += this -> neighborhoodMatrix [vertices[i]] [vertices[i + 1]];
    }
    result += this -> neighborhoodMatrix [vertices[iterationNumber]] [vertices[0]];

    return result;
}

double TSP::getRandom()
{
    return rand() % 101 / 100;
}

double TSP::calculateProbability(const int delta, const double temperature)
{
    return exp(delta / temperature);
}

bool TSP::makeDecision(const int delta, const double temperature)
{
    return this->getRandom() < this->calculateProbability(delta, temperature);
}


void TSP::switchVertex(std::vector<int>& vertices){
    int verticesNumber = vertices.size();
    int firstIndex = rand() % verticesNumber;
    int secondIndex = rand() % verticesNumber;
    while(firstIndex == secondIndex){
        secondIndex = rand() % verticesNumber;
    }
    std::swap(vertices[firstIndex],vertices[secondIndex]);
}

std::vector<int> TSP::generateInitialSolution(int verticesNumber)
{
    std::vector<int> vertices;
    for (int i = 0; i < verticesNumber; i++)
        vertices.push_back(i);
    for (int i = 0; i < verticesNumber; i++)
        this->switchVertex(vertices);

    return vertices;
}

void TSP::LoadFileTXT(std::string & filename)
{
    std::fstream File;
    File.open(filename, std::ios::in);
    if (!File.good()) {
        std::cout << "\nNie mozna wczytac pliku!\n";
    }
    if (*this->vertexNumber != 0) {
        for (int i = 0; i < *this->vertexNumber; i++)
            delete this->neighborhoodMatrix[i];
        delete[] this->neighborhoodMatrix;
        this->neighborhoodMatrix = NULL;
        this->vertexNumber = 0;
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


void TSP::LoadFileTSP(std::string & filename)
{
    std::ifstream File;
    File.open(filename, std::ios::in);
    if (!File.good()) {
        std::cout << "\nNie mozna wczytac pliku!\n";

    }

    if (*this->vertexNumber != 0) {
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
                this->vertexNumber = new int(stoi(number)); //zapisanie ilo�ci wierzcho�k�w
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
}
//
//void TSP::LoadFileATSP(std::string & filename)
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



