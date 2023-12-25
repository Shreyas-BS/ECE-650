#include "stats.h"

double calculateStandardDeviation(const std::vector<double> &data,
                                  double mean) {
  // double mean = calculateMean(data);
  double variance = 0.0;

  for (double value : data) {
    variance += pow(value - mean, 2);
  }

  double stdDev = sqrt(variance / data.size());
  return stdDev;
}

double calculateMean(const std::vector<double> &data) {
  double sum = 0.0;
  for (double value : data) {
    sum += value;
  }
  return sum / data.size();
}