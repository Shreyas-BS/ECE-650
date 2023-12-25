#include "graph.h"
#include <iostream>
#include <queue>
#include <algorithm>

// Constructor
Graph::Graph(int vertixNumber) {
    this->vertixNumber = vertixNumber;
    neighbourVertexArray.resize(vertixNumber);
    this->edgeCommandGiven = false;
}

// Destructor
Graph::~Graph() {
    neighbourVertexArray.clear();
    this->edgeCommandGiven = false;
}

int Graph::getVertixNumber() {
    return this->vertixNumber;
}

bool Graph::getEdgeCommandGiven() {
    return this->edgeCommandGiven;
}

void Graph::setEdgeCommandGiven(bool edgeCommandGiven) {
    this->edgeCommandGiven = edgeCommandGiven;
}

void Graph::addEdge(int vertex1, int vertex2) {
    // Duplicate edge case
    for (std::vector<int>::size_type i = 0; i < neighbourVertexArray[vertex1].size(); i++)
    {
        int adjacentVertex = neighbourVertexArray[vertex1][i];
        if (adjacentVertex == vertex2) {
            return;
        }
    }
    // Undirected graph so both vertex is neighbour for each other
    neighbourVertexArray[vertex1].push_back(vertex2);
    neighbourVertexArray[vertex2].push_back(vertex1);
}

// Standard BFS algorithm
std::vector<int> Graph::getShortestPath(int sourceVertex, int destinationVertex) {
    std::vector<int> ansector(vertixNumber, -1);
    std::queue<int> bfsQueue;
    std::vector<int> visitedMap(vertixNumber, 0);
    int currentVertex;

    bfsQueue.push(sourceVertex);
    visitedMap[sourceVertex] = 1;

    while (!bfsQueue.empty()) {
        currentVertex = bfsQueue.front();
        bfsQueue.pop();

        for (std::vector<int>::size_type i = 0; i < neighbourVertexArray[currentVertex].size(); i++) {
                int adjacentVertex = neighbourVertexArray[currentVertex][i];
                // Move to next node only if adjacentVertex is not visited before
                if (visitedMap[adjacentVertex] == 0)
                {
                    ansector[adjacentVertex] = currentVertex;
                    visitedMap[adjacentVertex] = 1;
                    bfsQueue.push(adjacentVertex);

                    if (adjacentVertex == destinationVertex)
                    {
                        std::vector<int> path;
                        while (adjacentVertex != -1)
                        {
                            path.push_back(adjacentVertex);
                            adjacentVertex = ansector[adjacentVertex];
                        }
                        std::reverse(path.begin(), path.end());
                        return path;
                    }
                }
            }
    }
    return std::vector<int>();
}