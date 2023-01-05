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
    return (float)rand() / (float)RAND_MAX;
}

std::vector<int> TSP::geneticAlgorithm(int populationSize,double pm, double pk, CROSS crossing, MUTATION mutation, SELECT selection, SUCCESS success ,STOP stop, int stopCounter ,double optimalPath, double maxErrorRatio){

    int selectedPopulationSize = ((populationSize / 2) - 1);
    int k = selectedPopulationSize / 3;
    if(k == 0 || k == 1 ) k=2;

    std::vector<std::vector<int>> population = TSP::generateInitialSolution(populationSize,*this->vertexNumber) ;
    std::vector<std::vector<int>> newPopulation ;
    std::vector<std::vector<int>> selectedPopulation ;

    std::vector<int> alphaMale = population[0];
    int i = 0 ; //generation counter
    int noChangeCounter = 0;
    Timer timer ;

    timer.start();


    do{
        int min= calculateCost(alphaMale);
        for(auto & solution: population){
            int cost = calculateCost(solution);
            if(cost < min){
                min = cost;
                alphaMale = solution;
                std::cout<<"generation: "<< i<<"\n";
                std::cout<<"best solution: "<< min<<"\n";
                noChangeCounter = 0;
            }
        }

        if(selection == SELECT::TUR){
            selectedPopulation = tournamentSelection(population, selectedPopulationSize, k);
        }else if(selection == SELECT::DUE){
            selectedPopulation = duelSelection(population, selectedPopulationSize);
        }


        while(newPopulation.size() < populationSize){
            int parent2;
            parent2 = rand() % selectedPopulationSize;

            if(makeDecision(pk)){
                std::vector<int> child1(*this->vertexNumber,-1),child2(*this->vertexNumber,-1);
                if(crossing == CROSS::OX){
                    this->OXCrossing(alphaMale,selectedPopulation[parent2],child1,child2);
                }else if(crossing == CROSS::PMX){
                    this->PMXCrossing(alphaMale,selectedPopulation[parent2],child1,child2);
                }
                newPopulation.push_back(child1);
                newPopulation.push_back(child2);
            }
        }

        for(int j = 0 ; j < newPopulation.size(); j++){
            if(makeDecision(pm)){
                if(mutation == MUTATION::SWP){
                    this->mutateBySwap(newPopulation[j]);
                }else if(mutation == MUTATION::INV){
                    this->mutateByInversion(newPopulation[j]);
                }
            }
        }

        if(success == SUCCESS::ECH){
            int min1=INT32_MAX;
            std::vector<int>NewBestOne;
            for(auto & solution: newPopulation){
                int cost = calculateCost(solution);
                if(cost < min1){
                    min1 = cost;
                    NewBestOne = solution;
                }
            }

            if(min <= min1){
                std::sort(population.begin(), population.end(),
                          [this](std::vector<int>& a, std::vector<int>& b) {
                              return calculateCost(a) < calculateCost(b);
                          });
                std::sort(newPopulation.begin(), newPopulation.end(),
                          [this](std::vector<int>& a, std::vector<int>& b) {
                              return calculateCost(a) < calculateCost(b);
                          });
                for(int i = 0 ; i <= (population.size() * 0.1) ; i++){
                    newPopulation[(newPopulation.size()-2)-i] = population[i];
                }
            }

            population.clear();
            if(min <= min1){
                population.push_back(alphaMale);

            }else{
                population.push_back(NewBestOne);
            }
            for(int i = 0 ; i < populationSize-1 ; i++){
                population.push_back(newPopulation[i]);
            }

        }else if(success == SUCCESS::FCH){
            population.clear();
            for(int i = 0 ; i < populationSize ; i++){
                population.push_back(newPopulation[i]);
            }
        }

        newPopulation.clear();
        selectedPopulation.clear();
        i++;
        noChangeCounter++;

        timer.stop();
        if(timer.getTime(Seconds) > 599){
            break;
        }
    }while((stop == STOP::GEN && i < (*this->vertexNumber * 5))||
    (stop == STOP::NCH && noChangeCounter < stopCounter )&&
    (((calculateCost(alphaMale) / optimalPath -1)*100) > maxErrorRatio));


    alphaMale.push_back(calculateCost(alphaMale));
    return  alphaMale;
}

std::vector<std::vector<int>> TSP::generateInitialSolution(int populationNumber,int verticesNumber)
{
    std::vector<std::vector<int>> population(populationNumber,std::vector<int>(verticesNumber,1));
    for(int x = 0 ; x < populationNumber ; x++){
        for (int j = 0; j < verticesNumber; j++)
            population[x][j]=j;
        for (int i = 0; i < verticesNumber; i++)
            this->mutateBySwap(population[x]);
    }
    return population;
}



std::vector<std::vector<int>> TSP::tournamentSelection(std::vector<std::vector<int>> & population,int populationSize,int k){
        std::vector<std::vector<int>> newPopulation;
        for(int counter =0 ; counter < populationSize ; counter ++){

            std::vector<int> chosenOnesIndex;
            for(int i = 0 ; i < k; i++){
                int random;
                do{
                    random = rand() % population.size();
                }
                while(std::find(chosenOnesIndex.begin(),chosenOnesIndex.end(),random) != chosenOnesIndex.end());

                chosenOnesIndex.push_back(random);
            }

            std::vector<std::vector<int>> chosenOnes;
            for(auto & index: chosenOnesIndex){
                chosenOnes.push_back(population[index]);
            }

            int min=INT32_MAX;
            std::vector<int>bestOne;
            for(auto & solution: chosenOnes){
                int cost = calculateCost(solution);
                if(cost < min){
                    min = cost;
                    bestOne = solution;
                }
            }
            newPopulation.push_back(bestOne);
            chosenOnesIndex.clear();
            chosenOnes.clear();
            bestOne.clear();
        }
        return newPopulation;
}

std::vector<std::vector<int>> TSP::duelSelection(std::vector<std::vector<int>> & population,int populationSize){
    std::vector<std::vector<int>> newPopulation;
    int random1, random2;
    for(int counter =0 ; counter < populationSize ; counter ++){

        random1 = rand() % population.size();
        random2 = rand() % population.size();
        if(random1 == random2){
            random2++;
            random2 = random2 % population.size();
        }

        if(calculateCost(population[random1]) > calculateCost(population[random2])){
            newPopulation.push_back(population[random2]);
        }else{
            newPopulation.push_back(population[random2]);
        }

    }
    return newPopulation;
}

void TSP::mutateBySwap(std::vector<int>& vertices){
    int verticesNumber = vertices.size();
    int firstIndex = rand() % verticesNumber;
    int secondIndex = rand() % verticesNumber;
    while(firstIndex == secondIndex){
        secondIndex = rand() % verticesNumber;
    }
    std::swap(vertices[firstIndex],vertices[secondIndex]);
}

void TSP::mutateByInversion(std::vector<int>& vertices){
    int verticesNumber = vertices.size();
    int firstIndex ;
    int secondIndex ;
    do{
        firstIndex = rand() % verticesNumber;
        secondIndex = rand() % verticesNumber;
    }while(firstIndex == secondIndex);

    if(firstIndex>secondIndex) std::swap(firstIndex,secondIndex);

    auto diff = secondIndex - firstIndex;

    if(diff == 1){
        std::swap(vertices[firstIndex],vertices[secondIndex]);
        return;
    }

    for(size_t counter = 1; counter < diff; ++firstIndex, --secondIndex, ++counter)
    {
        std::swap(vertices[firstIndex], vertices[secondIndex]);

    }
}

void TSP::mutateByScramble(std::vector<int>& vertices){
    int verticesNumber = vertices.size();

    //    radom number of vercices to swap
    int randomSubsetSize;
    do{
        randomSubsetSize = rand() % verticesNumber;
    }while(randomSubsetSize == 0);

    std::vector<int> *indexesToSwap = new std::vector<int>;

    int temp;
    bool repeat;

    for(int i = 0 ; i < randomSubsetSize; i++){
        temp = rand() % verticesNumber;
        for(auto index: *indexesToSwap){
            if(temp == index){
                repeat = true;
            }
        }
        if(!repeat){
            indexesToSwap->push_back(temp);
        }
        repeat = false;
    }


    for(auto index: *indexesToSwap){
        do{
            temp = rand() % indexesToSwap->size();
        }while(temp == index);

        std::swap(vertices[index], vertices[temp]);
    }
}

void TSP::PMXCrossing(std::vector<int>  & parent1, std::vector<int>  & parent2,std::vector<int> & child1, std::vector<int> & child2){
    std::vector<int> child1buff(parent1.size(), -1);
    std::vector<int> child2buff(parent1.size(), -1);

    int verticesNumber = parent1.size();
    int firstIndex ,secondIndex ;
    do{
        firstIndex = rand() % verticesNumber;
        secondIndex = rand() % verticesNumber;
    }while(firstIndex == secondIndex);

    if(firstIndex > secondIndex)
        std::swap(firstIndex, secondIndex);

    // copy genomes into new chromosomes

    copyGenomes(parent2, child1buff, firstIndex, secondIndex);
    copyGenomes(parent1, child2buff, firstIndex, secondIndex);

    // inserting matching genomes
    for(int i = 0; i < child1buff.size(); ++i)
    {
        if(i >= firstIndex && i <= secondIndex)
            continue;

        auto found = std::find(child1buff.begin(), child1buff.end(), parent1[i]);
        if(found != child1buff.end())
            continue;

        child1buff[i] = parent1[i];
    }

    for(int i = 0; i < child2buff.size(); ++i)
    {
        if(i >= firstIndex && i <= secondIndex)
            continue;

        auto found = std::find(child2buff.begin(), child2buff.end(), parent2[i]);
        if(found != child2buff.end())
            continue;

        child2buff[i] = parent2[i];
    }

    // filling chromosomes
    for(int i = 0, j = 0; i < parent1.size() && j < parent1.size() ; ++i)
    {
        if(child1buff[i] != -1)
            continue;

        while(j < parent1.size())
        {
            auto found = std::find(child1buff.begin(), child1buff.end(), j);
            if(found != child1buff.end())
            {
                j++;
                continue;
            }

            break;
        }

        child1buff[i] = j;
        j++;

    }

    for(int i = 0, j = 0; i < parent2.size() && j < parent2.size() ; ++i)
    {
        if(child2buff[i] != -1)
            continue;

        while(j < parent1.size())
        {
            auto found = std::find(child2buff.begin(), child2buff.end(), j);
            if(found != child2buff.end())
            {
                j++;
                continue;
            }

            break;
        }

        child2buff[i] = j;
        j++;
    }

    child1 = child1buff;
    child2 = child2buff;

};

void TSP::OXCrossing(std::vector<int> & parent1, std::vector<int> & parent2,std::vector<int> & child1, std::vector<int> & child2){


    int verticesNumber = parent1.size();
    int firstIndex ,secondIndex ;
    do{
        firstIndex = rand() % verticesNumber;
        secondIndex = rand() %  verticesNumber;
    }while(firstIndex == secondIndex);

    std::vector<int> child1buff(parent1.size(), -1);
    std::vector<int> child2buff(parent1.size(), -1);

    if(firstIndex > secondIndex)
        std::swap(firstIndex, secondIndex);

    std::vector<int> copyBufor1;
    std::vector<int> copyBufor2;

    for(int i = firstIndex ; i <= secondIndex ; i++ ){
        copyBufor2.push_back(parent2[i]);
        copyBufor1.push_back(parent1[i]);
    }

    copyGenomes(parent2, child1buff, firstIndex, secondIndex);
    copyGenomes(parent1, child2buff, firstIndex, secondIndex);

    int child2iterator = secondIndex, child1iterator = secondIndex;
    for(int i = secondIndex+1 ; i < verticesNumber ; i++){
        if(std::find(copyBufor2.begin(), copyBufor2.end(), parent1[i]) == copyBufor2.end()){
            child1iterator++;
            if(child1iterator >= verticesNumber){
                child1iterator = 0 ;
            }
            child1buff[child1iterator] = parent1[i];

        }
        if(std::find(copyBufor1.begin(), copyBufor1.end(), parent2[i]) == copyBufor1.end()){
            child2iterator++;
            if(child2iterator >= verticesNumber){
                child2iterator = 0 ;
            }
            child2buff[child2iterator] = parent2[i];

        }
    }


    for(int i = 0 ; i <= secondIndex ; i++){
        if(std::find(copyBufor2.begin(), copyBufor2.end(), parent1[i]) == copyBufor2.end()){
            child1iterator++;
            if(child1iterator >= verticesNumber){
                child1iterator = 0 ;
            }
            child1buff[child1iterator] = parent1[i];


        }
        if(std::find(copyBufor1.begin(), copyBufor1.end(), parent2[i]) == copyBufor1.end()){
            child2iterator++;
            if(child2iterator >= verticesNumber){
                child2iterator = 0 ;
            }
            child2buff[child2iterator] = parent2[i];

        }
    }


    child1 = child1buff;
    child2 = child2buff;

    child2buff.clear();
    child1buff.clear();
    copyBufor1.clear();
    copyBufor2.clear();
};

void TSP::copyGenomes(const std::vector<int> & source_, std::vector<int> & destination_, int firstIndex_, int secondIndex_)
{
    for(int i = firstIndex_; i <= secondIndex_; ++i)
    {
        destination_[i] = source_[i];
    }
}

bool TSP::makeDecision(double probability){
    return this->getRandom() < probability;
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



//
//void TSP::PMXCrossover(std::vector<int> & parent1, std::vector<int> & parent2)
//{
//    int verticesNumber = parent1.size();
//    int firstIndex{0}, secondIndex{0};
//
//    while(firstIndex == secondIndex)
//    {
//        firstIndex = rand() % verticesNumber;
//        secondIndex =  rand() % verticesNumber;
//    }
//
//    if(firstIndex > secondIndex)
//        std::swap(firstIndex, secondIndex);
//
//
//    std::vector<int> buffer1(parent1.size(), -1);
//    std::vector<int> buffer2(parent2.size(), -1);
//
//    // copy genomes into new chromosomes
//
//    copyGenomes(parent1, buffer1, firstIndex, secondIndex);
//    copyGenomes(parent2, buffer2, firstIndex, secondIndex);
//
//    // inserting matching genomes
//    for(int i = 0; i < buffer1.size(); ++i)
//    {
//        if(i >= firstIndex && i <= secondIndex)
//            continue;
//
//        auto found = std::find(buffer1.begin(), buffer1.end(), parent1[i]);
//        if(found != buffer1.end())
//            continue;
//
//        buffer1[i] = parent1[i];
//    }
//
//    for(int i = 0; i < buffer2.size(); ++i)
//    {
//        if(i >= firstIndex && i <= secondIndex)
//            continue;
//
//        auto found = std::find(buffer2.begin(), buffer2.end(), parent2[i]);
//        if(found != buffer2.end())
//            continue;
//
//        buffer2[i] = parent2[i];
//    }
//
//    // filling chromosomes
//    for(int i = 0, j = 1; i < parent1.size() && j < parent1.size() + 1; ++i)
//    {
//        if(buffer1[i] != -1)
//            continue;
//
//        while(j < parent1.size())
//        {
//            auto found = std::find(buffer1.begin(), buffer1.end(), j);
//            if(found != buffer1.end())
//            {
//                j++;
//                continue;
//            }
//
//            break;
//        }
//
//        buffer1[i] = j;
//        j++;
//
//    }
//
//    for(int i = 0, j = 1; i < parent2.size() && j < parent2.size() + 1; ++i)
//    {
//        if(buffer2[i] != -1)
//            continue;
//
//        while(j < parent1.size())
//        {
//            auto found = std::find(buffer2.begin(), buffer2.end(), j);
//            if(found != buffer2.end())
//            {
//                j++;
//                continue;
//            }
//
//            break;
//        }
//
//        buffer2[i] = j;
//        j++;
//    }
//
//    parent1 = buffer1;
//    parent2 = buffer2;
//}
//


