#pragma once

#include "Functions.h"
//Struct for each branch that holds data and relative variable
struct Branch{
    int startNode;
    int endNode;
    double resistance;
    double voltage;
    std::string variable;
};