#include "Path.h"

#include <iostream>

PEA::Path::Path()
{
}

PEA::Path::Path(const Path& origin)
        : _totalCost(origin._totalCost), _vertices(origin._vertices)
{
}

PEA::Path::Path(SDIZO::Array<int> vertices, int totalCost)
        : _vertices(vertices), _totalCost(totalCost)
{
}

void PEA::Path::display()
{
    std::cout << "Total cost: " << _totalCost << std::endl;
    _vertices.print();
}

void PEA::Path::addVertex(int vertex, int cost)
{
    _vertices.addFront(vertex);
    _totalCost += cost;
}

int PEA::Path::getTotalCost()
{
    return _totalCost;
}

SDIZO::Array<int> PEA::Path::getVertices(){
    return _vertices;
}
