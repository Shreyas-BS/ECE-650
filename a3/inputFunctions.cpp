#include "inputFunctions.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <vector>

// Function to read vertix input command
void readVertixNumberInput(Graph *&g, const std::string &line) {
    int vertixNumber = std::stoi(line.substr(2));
    // Check if vertixNumber is greater than 1
    if (vertixNumber < 2) {
        std::cerr << "Error: Number of vertices should be greater than 1.\n";
    }
    else {
        if (g != nullptr) {
            delete g;
            g = nullptr;
        }
        g = new Graph(vertixNumber);
    }
}

// Function to read edge input command
void readEdgeInput(Graph *&g, const std::string &line) {
    g->setEdgeCommandGiven(true);
    std::regex edgeRegex("<(\\d+),(\\d+)>");
    std::sregex_iterator edgeIterator(line.begin(), line.end(), edgeRegex);
    std::sregex_iterator endIterator;
    std::vector<std::pair<int, int>> edgeList;
    bool validEdges = true;

    // Check all edges within vertixNumber range
    while (edgeIterator != endIterator) {
        std::smatch match = *edgeIterator;
        int source = std::stoi(match[1]);
        int destination = std::stoi(match[2]);
        int vertixNumber = g->getVertixNumber();

        if (source >= 1 && source <= vertixNumber && destination >= 1 && destination <= vertixNumber) {
            edgeList.push_back({source, destination});
        }
        else {
            validEdges = false;
            break;
        }
        edgeIterator++;
    }

    // Add edges if all are valid
    if (validEdges == true) {
        for (size_t i = 0; i < edgeList.size(); i++)
        {
            const auto &edge = edgeList[i];
            int source = edge.first;
            int destination = edge.second;
            g->addEdge(source - 1, destination - 1);
        }
    }
    else {
        std::cerr << "Error: Input vertex is out of Graph vertices range.\n";
    }
   
}

// Function to read shortest path input command
void readShortestPathInput(Graph *&g, const std::string &line) {
    int source, destination;
    std::istringstream iss(line);
    char command;

    // Check if graph exists
    if (g != nullptr) {
        if (iss >> command >> source >> destination && command == 's') {
            int vertixNumber = g->getVertixNumber();
            // Check all source and destination vertix within vertixNumber range
            if (source >= 1 && source <= vertixNumber && destination >= 1 && destination <= vertixNumber) {
                // When source and destination are same
                if( source == destination) {
                    std::cout << source << std::endl;
                }
                else {
                    std::vector<int> path = g->getShortestPath(source - 1, destination - 1);
                    // Print the path
                    if (!path.empty()) {
                        for (size_t i = 0; i < path.size(); i++) {
                            std::cout << path[i] + 1;
                            if (i < path.size() - 1) {
                                std::cout << "-";
                            }
                        }
                        std::cout << std::endl;
                    }
                    // No path exists
                    else {
                        std::cerr << "Error: No path exists.\n";
                    }
                }
            }
            else {
                std::cerr << "Error: Input vertex is out of Graph vertices range.\n";
            }
        }
    }
    else {
        std::cerr << "Error: Graph not yet defined.\n";
    }
}