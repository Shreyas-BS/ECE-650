#pragma once

struct Options
{
    int sValue;
    int nValue;
    int lValue;
    int cValue;
};

Options parseOptions(int argc, char **argv);