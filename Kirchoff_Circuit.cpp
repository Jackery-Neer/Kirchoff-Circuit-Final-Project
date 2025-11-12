#include "Functions.h"
#include <variant>

int main() {
    std::string str;
    int num_nodes = -1;
    size_t num_branches = 0;
    std::cout << "Welcome to the Kirchoff Complex Circuit Analysis\n";
    while (num_nodes < 0 ) {
        std::cout << "Enter the number of nodes: ";
        std::cin >> str;
        std::cout << std::endl;
        try{
            num_nodes = std::stoi(str);
            if (num_nodes < 0) 
                std::cout << "Please input an integer greater than or equal to 0\n";
        }
        catch(const std::invalid_argument& e) {
            std::cout << "Please enter a valid integer.";
            num_nodes = -1;
        }
    }
    while (num_branches <= 0) {
        std::cout << "Enter the number of branches: ";
        std::cin >> str;
        std::cout << std::endl;
        try{
            num_branches = std::stoul(str);
            if (num_branches <= 0) 
                std::cout << "Please input an integer above 0.\n";
        }
        catch(const std::invalid_argument& e) {
            std::cout << "Please enter a valid integer above 0.\n";
            num_branches = 0;
        }
    }
    std::cout << "Please input the braches with values correspondent to relative concepts:\n";
    std::cout << "(startNode  endNode  total_resistance  total_voltage)\n";
    Vector<Branch> branches;
    for (size_t i = 0; i < num_branches; i++) {
        std::string branch;
        std::cout << "Branch " << i + 1 << ": ";
        //std::cin.ignore();
        std::getline(std::cin >> std::ws, branch);
        Branch b;
        Vector<std::string> tokens;
        std::istringstream iss(branch);
        while (iss >> str)
            tokens.push_back(str);
        try{
            b.startNode = std::stoi(tokens[0]);
            b.endNode = std::stoi(tokens[1]);
            if(b.startNode <= 0 || b.startNode > num_nodes)
                throw std::runtime_error("Node not within range of number of nodes provided.\n");
            if(b.endNode <= 0 || b.endNode > num_nodes) 
                throw std::runtime_error("Node not within range of number of nodes provided.\n");
            b.resistance = std::stod(tokens[2]);
            b.voltage = std::stod(tokens[3]);
            b.variable = "I" + std::to_string(i + 1);
        }
        catch(const std::invalid_argument& e) {
            std::cout << "Invalid input. Please input correct values for relative concepts provided.\n";
            return 0;
        } catch(const std::runtime_error& e) {
            std::cout << "Error: " << e.what();
            return 0;
        }
        branches.push_back(b);
    }
    std::unordered_map<int, Vector<int>> adjacency;
    buildAdjacency(adjacency, branches, num_nodes);
    //Vector<Vector<int>> allLoops = bfsLoops(adjacency);
    Vector<Vector<int>> allLoops = bfsLoops(adjacency, -1);
    for (size_t i = 0; i < allLoops.size(); i++) {
        std::cout << "Loop " << i + 1 << ": ";
        for (int node : allLoops[i])
            std::cout << node << " ";
        std::cout << "\n";
    }
    Vector<std::string> loopeqs = buildLoopEquations(allLoops, branches);
    Vector<int> nodes;

    for(int i = 0; i < num_nodes; i++) {
        nodes.push_back(i + 1);
    }

    Vector<std::string> junctioneqs = buildJunctionEquations(branches, nodes);
    for (const auto& eq : junctioneqs) {
        loopeqs.push_back(eq);
    }
    size_t num_eq = loopeqs.size();

    Matrix<double> kirchoffMatrix(num_eq, num_branches + 1);
    for (size_t i = 0; i < loopeqs.size(); i++) {
        Vector<double> row = buildRow(loopeqs[i], branches);
        kirchoffMatrix.setRow(i, row);
    }
    std::cout << "\n===== Parsed Equations =====\n";
    for (size_t i = 0; i < loopeqs.size(); i++)
        std::cout << "Eq " << i + 1 << ": " << loopeqs[i] << "\n";
    std::cout << "\n===== Constructed Matrix =====\n";
    kirchoffMatrix.printMatrix();
    Vector<std::variant<double, std::string>> x = kirchoffMatrix.solve();
    printSolution(x, branches);
}