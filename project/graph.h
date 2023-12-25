#ifndef GRAPH_H
#define GRAPH_H

#include "outputFunctions.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

class Graph {
public:
  Graph(int vertixNumber);
  ~Graph();
  int getVertixNumber();
  bool getEdgeCommandGiven();
  void setEdgeCommandGiven(bool edgeCommandGiven);
  void addEdge(int v, int w);
  std::vector<std::pair<int, int>> getEdges();
  std::vector<int> approximateMinimumVertexCover1();
  std::vector<int> approximateMinimumVertexCover2();
  static double approx_1_size;
  static double approx_2_size;

private:
  int vertixNumber;
  bool edgeCommandGiven;
  std::vector<std::vector<int>> neighbourVertexArray;
};

#endif