#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "Stack.h"
#include <iostream>
#include <stdexcept>
#include <cstddef>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <utility>
#include <variant>
#include "Branch.h"
#include <queue>
#include <unordered_map>

//Builds row for matrix
Vector<double> buildRow(std::string equation, Vector<Branch> branches);
//Checks if string is a number or not
bool isDigits(const std::string& str);
//Prints final solution to matrix
void printSolution(const Vector<std::variant<double, std::string>>& x, Vector<Branch>& branches);
//Builds adjacency map for circuit graph
void buildAdjacency(std::unordered_map<int, Vector<int>>& adjacency, const Vector<Branch>& branches, int num_nodes);
//Utilizes bfs to loop through graph finding loops
Vector<Vector<int>> bfsLoops(const std::unordered_map<int, Vector<int>>& adjacency, int start_hint);
//Finds loops in circuit graph
Vector<int> buildCycle(int a, int b, const std::unordered_map<int,int>& parent);
//Builds loop equations
Vector<std::string> buildLoopEquations(const Vector<Vector<int>>& loops, const Vector<Branch>& branches);
//Builds junction equations
Vector<std::string> buildJunctionEquations(const Vector<Branch>& branches, const Vector<int>& nodes);
