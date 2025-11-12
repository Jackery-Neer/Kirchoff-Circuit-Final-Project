#pragma once

#include "Functions.h"

struct Branch{
    int startNode;
    int endNode;
    double resistance;
    double voltage;
    std::string variable;
};