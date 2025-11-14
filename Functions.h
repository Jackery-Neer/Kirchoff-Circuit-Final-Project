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

Vector<double> buildRow(std::string equation, Vector<Branch> branches);
bool isDigits(const std::string& str);
void printSolution(const Vector<std::variant<double, std::string>>& x, Vector<Branch>& branches);
void buildAdjacency(std::unordered_map<int, Vector<int>>& adjacency, const Vector<Branch>& branches, int num_nodes);
Vector<Vector<int>> bfsLoops(const std::unordered_map<int, Vector<int>>& adjacency, int start_hint);
Vector<int> buildCycle(int a, int b, const std::unordered_map<int,int>& parent);
Vector<std::string> buildLoopEquations(const Vector<Vector<int>>& loops, const Vector<Branch>& branches);
Vector<std::string> buildJunctionEquations(const Vector<Branch>& branches, const Vector<int>& nodes);
