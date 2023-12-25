#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph {
    public:
        Graph(int vertixNumber);
        ~Graph();
        int getVertixNumber();
        bool getEdgeCommandGiven();
        void setEdgeCommandGiven(bool edgeCommandGiven);
        void addEdge(int v, int w);
        std::vector<int> getShortestPath(int sourceVertex, int destinationVertex);

    private:
        int vertixNumber;
        bool edgeCommandGiven;
        std::vector<std::vector<int>> neighbourVertexArray;
};

#endif