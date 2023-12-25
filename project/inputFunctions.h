#ifndef INPUTFUNCTIONS_H
#define INPUTFUNCTIONS_H

#include "graph.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

void readVertixNumberInput(Graph *&g, const std::string &line);
bool readEdgeInput(Graph *&g, const std::string &line);

#endif