#include "graph.h"

// Constructor
Graph::Graph(int vertixNumber)
{
    this->vertixNumber = vertixNumber;
    neighbourVertexArray.resize(vertixNumber);
    this->edgeCommandGiven = false;
}

// Destructor
Graph::~Graph()
{
    neighbourVertexArray.clear();
    this->edgeCommandGiven = false;
}

int Graph::getVertixNumber()
{
    return this->vertixNumber;
}

bool Graph::getEdgeCommandGiven()
{
    return this->edgeCommandGiven;
}

void Graph::setEdgeCommandGiven(bool edgeCommandGiven)
{
    this->edgeCommandGiven = edgeCommandGiven;
}

void Graph::addEdge(int vertex1, int vertex2)
{
    // Duplicate edge case
    for (std::vector<int>::size_type i = 0; i < neighbourVertexArray[vertex1].size(); i++)
    {
        int adjacentVertex = neighbourVertexArray[vertex1][i];
        if (adjacentVertex == vertex2)
        {
            return;
        }
    }
    // Undirected graph so both vertex is neighbour for each other
    neighbourVertexArray[vertex1].push_back(vertex2);
    neighbourVertexArray[vertex2].push_back(vertex1);
}

std::vector<std::pair<int, int>> Graph::getEdges()
{
    std::vector<std::pair<int, int>> edges;

    for (int i = 0; i < vertixNumber; ++i)
    {
        for (std::vector<int>::size_type j = 0; j < neighbourVertexArray[i].size(); ++j)
        {
            int vertex1 = i;
            int vertex2 = neighbourVertexArray[i][j];

            // To avoid duplicate edges, add only if vertex1 < vertex2
            if (vertex1 < vertex2)
            {
                edges.push_back(std::make_pair(vertex1, vertex2));
            }
        }
    }

    return edges;
}