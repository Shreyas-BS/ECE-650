#pragma once

#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include <minisat/core/Solver.h>
#include "graph.h"
#include "inputFunctions.h"

class VertexCoverSolver
{
public:
    VertexCoverSolver(Graph *g);
    ~VertexCoverSolver();
    void printMinVertexCover();

private:
    void findMinVertexCover();
    void setClause1();
    void setClause2();
    void setClause3();
    void setClause4();
    std::vector<int> getVertexCoverForCurrentVNum();
    void setLiteral();

    int currentVertexCoverNumber;
    std::unique_ptr<Minisat::Solver> solver;
    std::vector<std::vector<Minisat::Lit>> literalTable;
    Graph *g;
    std::vector<int> minVertexCover;
};