#include "Path.h"

#include <iostream>

PEA::Path::Path()
{
}

PEA::Path::Path(const Path& origin)
        : _totalCost(origin._totalCost), _vertices(origin._vertices)
{
}

PEA::Path::Path(std::vector<int> vertices, int totalCost)
        : _vertices(vertices), _totalCost(totalCost)
{
}

void PEA::Path::display()
{
    std::cout << "Total cost: " << _totalCost << std::endl;
}

void PEA::Path::addVertex(int vertex, int cost)
{
    _vertices.push_back(vertex);
    _totalCost += cost;
}

int PEA::Path::getTotalCost()
{
    return _totalCost;
}

std::vector<int> PEA::Path::getVertices(){
    return _vertices;
}
