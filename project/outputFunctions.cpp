#include "outputFunctions.h"

void printMinimumVertexCover(std::string title,
                             std::vector<int> minVertexCover) {
  int size = minVertexCover.size();
  std::sort(minVertexCover.begin(), minVertexCover.end());

  std::cout << title;
  for (int i = 0; i < size; ++i) {
    std::cout << minVertexCover[i] + 1;
    if (i < size - 1) {
      std::cout << ",";
    }
  }
  std::cout << std::endl;
}

void printStats(std::string title, const std::vector<double> &dataArray) {
  double mean = calculateMean(dataArray);
  std::cout << "Mean of " << title << std::fixed << std::setprecision(2) << mean
            << std::endl;
  std::cout << "Std Deviation " << title << std::fixed << std::setprecision(2)
            << calculateStandardDeviation(dataArray, mean) << std::endl;
}