#include "Functions.h"
#include <variant>

int main() {
    size_t num_eq = 0;
    size_t num_unknowns = 0;
    std::string unknown;
    std::string str;
    Vector<std::string> variables;
    std::cout << "Welcome to the Kirchoff Complex Circuit Analysis\n";
    while (unknown != "r" && unknown != "i") {
        std::cout << "Enter i to solve for currents. Enter r to solve for resistors.\n";
        std::cin >> unknown;
    }
    while (num_unknowns <= 0) {
        std::cout << "How many unknown variables do you have within your equation?\n";
        std::cin >> str;
        try{
            num_unknowns = std::stoul(str);
            if (num_unknowns <= 0) 
                std::cout << "Please input an integer above 0.\n";
        }
        catch(const std::invalid_argument& e){
            std::cout << "Please input an integer above 0.\n";
            num_unknowns = 0;
        }
    }
    for (size_t i = 0; i < num_unknowns; i++) {
        std::string var;
        std::cout << "Enter unknown variable " << i + 1 << ":\n";
        std::cin >> var;
        variables.push_back(var);
    }
    while (num_eq <= 0) {
        std::cout << "How many equations did you get from analyzing the complex circuit using Kirchoff's Rules?\n";
        std::cin >> str;
        try{
            num_eq = std::stoul(str);
            if (num_eq <= 0) 
                std::cout << "Please input an integer above 0.\n";
        }
        catch(const std::invalid_argument& e){
            std::cout << "Please input an integer above 0.\n";
            num_eq = 0;
        }
    }
    Matrix<double> kirchoffMatrix(num_eq, num_unknowns + 1);

    //Utilize stack in order to deal with any form of Kirchoff Equations
    for (size_t i = 0; i < num_eq; i++) {
        std::string equation;
        std::cout << "Enter equation #" << i + 1 << ":\n";
        std::cin.ignore();
        std::getline(std::cin, equation);
        equation.erase(std::remove(equation.begin(), equation.end(), ' '), equation.end());
        Vector<double> row = buildRow(equation, variables);
        //Deals with invalid var entered-(down arrow)
        //Make sure to deal with outside for loop
        //Decide whether you want to break out of program or if you want to restart program 
        if (row.isEmpty()) {
            std::cout << "Invalid variable detected, please enter correct variables into equation.";
            return 0;
            //if decide to break program then do return 0 instead of break
        }
        //If you want to break and then continue if not correctly input vars deal with it here, for now i just return 0
        kirchoffMatrix[i] = row;
    }
    Vector<std::variant<double, std::string>> x = kirchoffMatrix.solve();
    printSolution(x);
    





    





}