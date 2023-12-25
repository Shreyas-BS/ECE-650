#ifndef INPUTFUNCTIONS_H
#define INPUTFUNCTIONS_H

#include <string>
#include "graph.h"

void readVertixNumberInput(Graph *&g, const std::string &line);
void readEdgeInput(Graph *&g, const std::string &line);
void readShortestPathInput(Graph *&g, const std::string &line);

#endif
