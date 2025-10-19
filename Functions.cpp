#include "Functions.h"

bool verifyVal(std::string var, Vector<std::string> variables) {
    for (size_t i = 0; i < variables.size(); i++) {
        if (var == variables[i]) 
            return true;
    }
    return false;
}

Vector<double> buildRow(std::string equation, Vector<std::string> variables) {
    Stack<std::string> eq_stack;
    for (size_t i = 0; i < equation.size(); i++) {
        char c = equation[i];
        if(isdigit(c)) {
            std::string num;
            while (i < equation.size() && (isdigit(equation[i]) || equation[i] == '.')) {
                num += equation[i++];
            }
            if (i < equation.size() && isalpha(equation[i])) {
                eq_stack.push(num);
                eq_stack.push("*");
                std::string var;
                while (i < equation.size() && isalnum(equation[i])) {
                    var += equation[i++];
                }
                if (!verifyVal(var, variables)) {
                    Vector<double> empty_vec;
                    return empty_vec;
                }
                eq_stack.push(var);
                i--;
            } else {
                eq_stack.push(num);
                i--;
            }
        } else if (isalpha(c)) {
            std::string var;
            while (i < equation.size() && isalnum(equation[i])) {
                var += equation[i++];
            }
            if (!verifyVal(var, variables)) {
                Vector<double> empty_vec;
                return empty_vec;
            }
            eq_stack.push(var);
            i--;
        } else {
            eq_stack.push(std::string(1, c));
        }
    }
    //Code before reads string onto stack, next bit is manipulating it into the form for a proper matrix
}
