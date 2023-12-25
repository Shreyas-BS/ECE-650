#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include <vector>
#include <signal.h>
#include "optionParser.h"
/*
 *
 */
struct Point
{
    int xCoordinate;
    int yCoordinate;
};

struct Line
{
    Point sourcePoint;
    Point destinationPoint;
};

struct Street
{
    std::vector<Line> linesList;
};

struct StreetsList
{
    std::vector<Street> streetsList;
};

// Standard raandom generation algorithm
int getRandomNumber(int min, int max)
{
    std::ifstream urandom("/dev/urandom", std::ios::binary);
    if (!urandom)
    {
        std::cerr << "Error: Unable to open /dev/urandom" << std::endl;
        exit(1);
    }

    unsigned int randomValue;

    // Read random data from /dev/urandom
    urandom.read(reinterpret_cast<char *>(&randomValue), sizeof(randomValue));

    if (!urandom)
    {
        std::cerr << "Error reading from /dev/urandom" << std::endl;
        exit(1);
    }

    urandom.close();

    // Convert random bytes to a value within the specified range [min, max]
    int range = max - min + 1;
    int randomInt = min + (randomValue % range);

    return randomInt;
}

bool doesLineOverlapLine(Line lineSegment1, Line lineSegment2)
{
    int x1 = lineSegment1.sourcePoint.xCoordinate;
    int y1 = lineSegment1.sourcePoint.yCoordinate;
    int x2 = lineSegment1.destinationPoint.xCoordinate;
    int y2 = lineSegment1.destinationPoint.yCoordinate;
    int x3 = lineSegment2.sourcePoint.xCoordinate;
    int y3 = lineSegment2.sourcePoint.yCoordinate;
    int x4 = lineSegment2.destinationPoint.xCoordinate;
    int y4 = lineSegment2.destinationPoint.yCoordinate;

    double m1 = (x2 - x1) != 0 ? (y2 - y1) / (x2 - x1) : INFINITY;
    double m2 = (x4 - x3) != 0 ? (y4 - y3) / (x4 - x3) : INFINITY;

    // Check if the lines are parallel (slopes are equal)
    if (m1 == m2)
    {
        int c1 = y1 - (m1 * x1);
        int c2 = y3 - (m2 * x3);

        // Check if the y intercept of lines are same to meet overlap condition
        if (c1 == c2)
        {
            return true;
        }
    }
    return false;
}

// bool doesLinesIntersect(Line lineSegment1, Line lineSegment2)
//     {
//         int x1 = lineSegment1.sourcePoint.xCoordinate;
//         int y1 = lineSegment1.sourcePoint.yCoordinate;
//         int x2 = lineSegment1.destinationPoint.xCoordinate;
//         int y2 = lineSegment1.destinationPoint.yCoordinate;
//         int x3 = lineSegment2.sourcePoint.xCoordinate;
//         int y3 = lineSegment2.sourcePoint.yCoordinate;
//         int x4 = lineSegment2.destinationPoint.xCoordinate;
//         int y4 = lineSegment2.destinationPoint.yCoordinate;

//         double m1 = (x2 - x1) != 0 ? (y2 - y1) / (x2 - x1) : INFINITY;
//         double m2 = (x4 - x3) != 0 ? (y4 - y3) / (x4 - x3) : INFINITY;

//         // Check if the lines are parallel (slopes are equal)
//         if (m1 == m2)
//         {
//             int c1 = y1 - (m1 * x1);
//             int c2 = y3 - (m2 * x3);

//             // Check if the y intercept of lines are same to meet overlap condition
//             if (c1 == c2)
//             {
//                 return true;
//             }
//         }

//         // Calculate the intersection point
//         double x_int, y_int;
//         if (m1 == INFINITY)
//         {
//             // Line 1 is vertical, so x1 is the x-coordinate of the intersection
//             x_int = x1;
//             y_int = m2 * (x1 - x3) + y3;
//         }
//         else if (m2 == INFINITY)
//         {
//             // Line 2 is vertical, so x3 is the x-coordinate of the intersection
//             x_int = x3;
//             y_int = m1 * (x3 - x1) + y1;
//         }
//         else
//         {
//             // Calculate the intersection point using the equation of lines
//             x_int = (y3 - y1 + m1 * x1 - m2 * x3) / (m1 - m2);
//             y_int = m1 * (x_int - x1) + y1;
//         }

//         // Check if the intersection point falls within the line segments
//         if (std::min(x1, x2) < x_int && x_int < std::max(x1, x2) &&
//             std::min(y1, y2) < y_int && y_int < std::max(y1, y2) &&
//             std::min(x3, x4) < x_int && x_int < std::max(x3, x4) &&
//             std::min(y3, y4) < y_int && y_int < std::max(y3, y4))
//         {
//             return true;
//         }
//         else
//         {
//             return false; // Intersection point is outside the line segments
//         }
//     }

    bool doesLineOverlapStreet(Line st, Street s)
    {
        for (Line inner_line : s.linesList)
        {
            if (doesLineOverlapLine(st, inner_line))
            {
                return true;
            }
        }
        return false;
    }

    // bool doesLinesInteresectStreet(Line st, Street s)
    // {
    //     for (Line inner_line : s.linesList)
    //     {
    //         if (doesLinesIntersect(st, inner_line))
    //         {
    //             return true;
    //         }
    //     }
    //     return false;
    // }

    void create_streets(Options options)
    {
        int maxStreetsCount = options.sValue;
        int maxLineSegmentNumber = options.nValue;
        int coordinateRange = options.cValue;

        int A = 25; // Number of attempts

        StreetsList street_list;
        int streetCount = getRandomNumber(2, maxStreetsCount);

        for (int street_num = 0; street_num < streetCount; street_num++)
        {
            int num_of_lines = getRandomNumber(1, maxLineSegmentNumber);
            Street current_street;
            Point point1;
            Point point2;
            // int limit = 0;
            for (int lines_generated = 0; lines_generated < num_of_lines; lines_generated++)
            {
                int limit = 0;
                if (lines_generated == 0)
                {
                    point1.xCoordinate = getRandomNumber(-coordinateRange, coordinateRange);
                    point1.yCoordinate = getRandomNumber(-coordinateRange, coordinateRange);
                }
                Line current_line;
                bool validNumGenerated;

                do
                {
                    validNumGenerated = true;
                    point2.xCoordinate = getRandomNumber(-coordinateRange, coordinateRange);
                    point2.yCoordinate = getRandomNumber(-coordinateRange, coordinateRange);

                    // Zero length line segment case
                    if (point1.xCoordinate == point2.xCoordinate && point1.yCoordinate == point2.yCoordinate)
                    {
                        limit++;
                        validNumGenerated = false;
                        continue;
                    }

                    current_line.sourcePoint = point1;
                    current_line.destinationPoint = point2;

                    if (lines_generated > 0)
                    {
                        // Current Street Overlap check Self intersection not tested
                        // if (doesLinesInteresectStreet(current_line, current_street))
                        if (doesLineOverlapStreet(current_line, current_street))
                        {
                            limit++;
                            validNumGenerated = false;
                        }

                        // Other Street Overlap check
                        for (Street street : street_list.streetsList)
                        {
                            if (doesLineOverlapStreet(current_line, street))
                            {
                                limit++;
                                validNumGenerated = false;
                                break;
                            }
                        }
                    }

                    if (limit > A)
                    {
                        std::cerr << "Error: Maximum limit " << A << " reached.\n";
                        kill(getppid(), SIGTERM);
                        exit(0);
                        return;
                    }

                } while (!validNumGenerated);
                point1 = point2;
                current_street.linesList.push_back(current_line);
            }

            street_list.streetsList.push_back(current_street);

            std::cout << "add"
                      << "  "
                      << "\"" << street_num + 1 << "\""
                      << "  ";

            bool firstSourcePoint = true;

            for (Line single_line : current_street.linesList)
            {
                Point pointS = single_line.sourcePoint;
                Point pointD = single_line.destinationPoint;
                if (firstSourcePoint)
                {
                    std::cout << "(" << pointS.xCoordinate << "," << pointS.yCoordinate << ") ";
                }
                std::cout << "(" << pointD.xCoordinate << "," << pointD.yCoordinate << ") ";
                firstSourcePoint = false;
            }
            std::cout << std::endl;
        }
        std::cout << "gg\n";

        for (size_t street_num = 0; street_num < street_list.streetsList.size(); ++street_num)
        {
            std::cout << "rm"
                      << " "
                      << "\"" << street_num + 1 << "\"";
            std::cout << std::endl;
        }
    }

    int main(int argc, char **argv)
    {
        Options options = parseOptions(argc, argv);
        int maxWaitTime = options.lValue;
        while (true)
        {
            create_streets(options);
            sleep(getRandomNumber(5, maxWaitTime));
        }
        return 0;
    }
