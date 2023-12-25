#include "graph.h"

double Graph::approx_1_size = 0;
double Graph::approx_2_size = 0;

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

int Graph::getVertixNumber() { return this->vertixNumber; }

bool Graph::getEdgeCommandGiven() { return this->edgeCommandGiven; }

void Graph::setEdgeCommandGiven(bool edgeCommandGiven) {
  this->edgeCommandGiven = edgeCommandGiven;
}

void Graph::addEdge(int vertex1, int vertex2) {
  // Duplicate edge case
  for (std::vector<int>::size_type i = 0;
       i < neighbourVertexArray[vertex1].size(); i++) {
    int adjacentVertex = neighbourVertexArray[vertex1][i];
    if (adjacentVertex == vertex2) {
      return;
    }
  }
  // Undirected graph so both vertex is neighbour for each other
  neighbourVertexArray[vertex1].push_back(vertex2);
  neighbourVertexArray[vertex2].push_back(vertex1);
}

std::vector<std::pair<int, int>> Graph::getEdges() {
  std::vector<std::pair<int, int>> edges;

  for (int i = 0; i < vertixNumber; ++i) {
    for (std::vector<int>::size_type j = 0; j < neighbourVertexArray[i].size();
         ++j) {
      int vertex1 = i;
      int vertex2 = neighbourVertexArray[i][j];

      // To avoid duplicate edges, add only if vertex1 < vertex2
      if (vertex1 < vertex2) {
        edges.push_back(std::make_pair(vertex1, vertex2));
      }
    }
  }

  return edges;
}

std::vector<int> Graph::approximateMinimumVertexCover1() {
  std::vector<int> vertexCover;

  std::vector<int> degrees(vertixNumber, 0);

  for (int i = 0; i < vertixNumber; ++i) {
    degrees[i] = neighbourVertexArray[i].size();
  }

  while (true) {
    auto maxDegreeVertex = std::max_element(degrees.begin(), degrees.end());

    if (*maxDegreeVertex == 0) {
      break;
    }

    vertexCover.push_back(maxDegreeVertex - degrees.begin());
    degrees[maxDegreeVertex - degrees.begin()] = 0;

    for (int neighbor :
         neighbourVertexArray[maxDegreeVertex - degrees.begin()]) {

      degrees[neighbor]--;
    }
    *maxDegreeVertex = 0;
  }

  approx_1_size = vertexCover.size();

  return vertexCover;
}

std::vector<int> Graph::approximateMinimumVertexCover2() {
  std::vector<bool> visited;
  std::vector<int> vertexCover;

  for (int i = 0; i < vertixNumber; i++) {
    visited.push_back(false);
  }

  // Consider all edges one by one
  for (int u = 0; u < vertixNumber; u++) {
    if (visited[u] == false) {
      for (int v : neighbourVertexArray[u]) {
        if (visited[v] == false) {
          visited[v] = true;
          visited[u] = true;
          break;
        }
      }
    }
  }

  for (int i = 0; i < vertixNumber; i++) {
    if (visited[i]) {
      vertexCover.push_back(i);
    }
  }

  approx_2_size = vertexCover.size();
  return vertexCover;
}