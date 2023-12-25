#include <iostream>
#include <string>
#include "graph.h"
#include "inputFunctions.h"

int main()
{
    while (!std::cin.eof())
    {
        std::string line;
        Graph *g = nullptr;

        // std::cout << "Hello 1\n";

        while (std::getline(std::cin, line))
        {
            // if cmd length is less than 2 characters
            if (line.length() <= 2)
            {
                std::cerr << "Error: Command length should be minimum 3 charcters\n";
            }
            try
            {
                // Check cmd cases
                if (line[0] == 'V')
                {
                    std::cout << line << std::endl;
                    readVertixNumberInput(g, line);
                }
                else if (line[0] == 'E')
                {
                    std::cout << line << std::endl;
                    if (g == nullptr)
                    {
                        std::cerr << "Error: Graph not yet defined.\n";
                    }
                    else if (!(g->getEdgeCommandGiven()))
                    {
                        readEdgeInput(g, line);
                    }
                }
                else if (line[0] == 's')
                {
                    readShortestPathInput(g, line);
                }
                else
                {
                    std::cerr << "Error: unknown command\n";
                }
            }
            catch (...)
            {
                std::cerr << "Error: Some error occurred during the parsing of the given command.\n";
            }
        }
    }
    return 0;
}