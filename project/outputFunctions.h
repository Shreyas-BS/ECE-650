#ifndef OUTPUTFUNCTIONS_H
#define OUTPUTFUNCTIONS_H

#include "stats.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void printMinimumVertexCover(std::string title,
                             std::vector<int> minVertexCover);
void printStats(std::string title, const std::vector<double> &dataArray);

#endif