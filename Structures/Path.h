#pragma once

#include <vector>

namespace PEA
{
    class Path
    {
    public:
        Path();
        Path(const Path& origin);
        Path(std::vector<int> vertices, int totalCost);
        void display();
        void addVertex(int vertex, int cost);
        int getTotalCost();
        std::vector<int> getVertices();

    private:
        int _totalCost{ 0 };
        std::vector<int> _vertices;
    };
}