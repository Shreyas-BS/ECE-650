#include "optionParser.h"
#include <iostream>
#include <cstdlib>
#include <getopt.h>

Options parseOptions(int argc, char **argv)
{
    Options options;
    int opt;

    options.sValue = 10;
    options.nValue = 5;
    options.lValue = 5;
    options.cValue = 20;

    while ((opt = getopt(argc, argv, "s:n:l:c:")) != -1)
    {
        switch (opt)
        {
        case 's':
            options.sValue = std::atoi(optarg);
            if (options.sValue < 2)
            {
                std::cerr << "Error: s can't be less than 2.\n";
                // exit(0);
            }
            break;
        case 'n':
            options.nValue = std::atoi(optarg);
            if (options.nValue < 1)
            {
                std::cerr << "Error: n can't be less than 1.\n";
                // exit(0);
            }
            break;
        case 'l':
            options.lValue = std::atoi(optarg);
            if (options.lValue < 5)
            {
                std::cerr << "Error: l can't be less than 5.\n";
                // exit(0);
            }
            break;
        case 'c':
            options.cValue = std::atoi(optarg);
            if (options.cValue < 1)
            {
                std::cerr << "Error: K value less than 1.\n";
                // exit(0);
            }
            break;
        case '?':
            if (optopt == 's' || optopt == 'n' || optopt == 'l' || optopt == 'c')
                std::cerr << "Error: Missing argument for option -" << static_cast<char>(optopt) << std::endl;
            else
                std::cerr << "Error: Invalid option.\n";
            // exit(1);
        default:
            exit(0);
        }
    }

    return options;
}