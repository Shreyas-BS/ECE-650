#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>

class Graph
{
public:
    Graph(int vertixNumber);
    ~Graph();
    int getVertixNumber();
    bool getEdgeCommandGiven();
    void setEdgeCommandGiven(bool edgeCommandGiven);
    void addEdge(int v, int w);
    std::vector<std::pair<int, int>> getEdges();

private:
    int vertixNumber;
    bool edgeCommandGiven;
    std::vector<std::vector<int>> neighbourVertexArray;
};

#endif