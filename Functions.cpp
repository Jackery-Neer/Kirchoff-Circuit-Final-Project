#include "Functions.h"

bool verifyVal(std::string var, Vector<std::string> variables) {
    for (size_t i = 0; i < variables.size(); i++) {
        if (var == variables[i]) 
            return true;
    }
    return false;
}

bool isDigits(const std::string& str) {
    return str.find_first_not_of("0123456789.") == std::string::npos;
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
    Vector<double> row(variables.size() + 1);
    Stack<std::string> reverse_stack;
    double constant = 0;
    double sign = 1;
    double aug = 0;
    while(!eq_stack.isEmpty()) {
        reverse_stack.push(eq_stack.top());
        eq_stack.pop();
    }

    while (!reverse_stack.isEmpty()) {
        std::string token = reverse_stack.top();
        reverse_stack.pop();
        if(token == "+") {
            sign = 1;
        } else if (token == "-") {
            sign = -1;
        } else if (token == "*") {
            continue;
        } else if (isDigits(token)){
            constant = std::stod(token);
            std::string peek_item = reverse_stack.peek(1);
            bool test_var = false;
            for (size_t i = 0; i < variables.size(); i++) {
                if (peek_item == variables[i]) {
                    test_var = true;
                    break;
                }
            }
            if (!test_var) {
                aug -= constant * sign;
                constant = 0;
                sign = 1;
            }

        } else if (token == "=") {
            token = reverse_stack.top();
            reverse_stack.pop();
            if (token == "-") {
                token = reverse_stack.top();
                reverse_stack.pop();
                aug -= std::stod(token);
            } else if (isDigits(token)) {
                aug += std::stod(token);
            }
        } else if(token == "(") {
            double distributer = (constant == 0 ? 1 : constant);
            constant = 0;
            token = reverse_stack.top();
            reverse_stack.pop();
            while(token != ")") {
                if (token == "-") {
                    sign = -1;
                } else if (token == "+") {
                    sign = 1;
                } else {
                    for (size_t i = 0; i < variables.size(); i++) {
                        if (variables[i] == token) {
                            row[i] += distributer * sign;
                            break;
                        }
                    }
                }
                token = reverse_stack.top();
                reverse_stack.pop();
            }
            sign = 1;
        } else {
            for (size_t i = 0; i < variables.size(); i++) {
                if (variables[i] == token) {
                    //+= instead of = so if two variables are the same they add together
                    if (constant == 0) {
                        row[i] += sign;
                    } else {
                        row[i] += constant * sign;
                    }
                    constant = 0;
                    sign = 1;
                    break;
                }
            }
        }
    }
    row[variables.size()] = aug;
    return row;
}
/*currently does not work if:
    there is a varible past "=" 
    if the structure is reversed 
        *might be able to use if i check orignal stack and set conditions to make reverse stack
    if missing ")", this will cause infinite loop 
    Nested parenthesis 
    if variable is not alphanumeric (ex: I_1 or I[1])
    Nested Distribution, kind of goes with nested parenthesis 
*/
//Could create lambda fn to deal with applying terms
//Double pop method could be utilized to deal with "+" and "-"

