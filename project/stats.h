#ifndef STATS_H
#define STATS_H

#include <cmath>
#include <vector>

double calculateStandardDeviation(const std::vector<double> &data, double mean);
double calculateMean(const std::vector<double> &data);

#endif