#pragma once

#include "Array.h"

namespace PEA
{
    class Path
    {
    public:
        Path();
        Path(const Path& origin);
        Path(SDIZO::Array<int> vertices, int totalCost);
        void display();
        void addVertex(int vertex, int cost);
        int getTotalCost();
        SDIZO::Array<int> getVertices();

    private:
        int _totalCost{ 0 };
        SDIZO::Array<int> _vertices;
    };
}