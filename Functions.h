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

Vector<double> buildRow(std::string equation, Vector<std::string> variables);
bool verifyVal(std::string val, Vector<std::string> variables);
bool isDigits(const std::string& str);