#include "Functions.h"

bool isDigits(const std::string& str) {
    return str.find_first_not_of("0123456789.") == std::string::npos;
}

Vector<double> buildRow(std::string equation, Vector<Branch> branches) {
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
            eq_stack.push(var);
            i--;
        } else {
            eq_stack.push(std::string(1, c));
        }
    }
    //Code before reads string onto stack, next bit is manipulating it into the form for a proper matrix
    Vector<double> row(branches.size() + 1);
    Stack<std::string> reverse_stack;
    double constant = 0;
    double sign = 1;
    double aug = 0;
    bool have_explicit_coeff = false;
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
            have_explicit_coeff = true;
            std::string peek_item = reverse_stack.peek(1);
            bool test_var = false;
            for (size_t i = 0; i < branches.size(); i++) {
                if (peek_item == branches[i].variable) {
                    test_var = true;
                    break;
                }
            }
            if (!test_var) {
                aug -= constant * sign;
                constant = 0;
                sign = 1;
                have_explicit_coeff = false;
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
                } else if (isDigits(token)) {
                    constant = std::stod(token);
                } else {
                    for (size_t i = 0; i < branches.size(); i++) {
                        if (branches[i].variable == token) {
                            double coeff = (constant == 0 ? 1 : constant);
                            row[i] += distributer * sign * coeff;
                            break;
                        }
                    }
                }
                if (reverse_stack.isEmpty()) 
                    throw std::runtime_error("Missing ')' after '(' was used in equation.");
                token = reverse_stack.top();
                reverse_stack.pop();
            }
            sign = 1;
        } else {
            for (size_t i = 0; i < branches.size(); i++) {
                if (branches[i].variable == token) {
                    //+= instead of = so if two variables are the same they add together
                    double coeff = have_explicit_coeff ? constant : 1.0;
                    row[i] += coeff * sign;
                    constant = 0;
                    have_explicit_coeff = false;
                    sign = 1;
                    break;
                }
            }
        }
    }
    row[branches.size()] = aug;
    return row;
}

void printSolution(const Vector<std::variant<double, std::string>>& x, Vector<Branch>& branches) {
    for (size_t i = 0; i < x.size(); i++) {
        std::cout << branches[i].variable << " = ";
        if (std::holds_alternative<double>(x[i]))
            std::cout << std::get<double>(x[i]);
        else
            std::cout << std::get<std::string>(x[i]);
        std::cout << std::endl;
    }
}

void buildAdjacency(std::unordered_map<int, Vector<int>>& adjacency, const Vector<Branch>& branches, int num_nodes) {
    for (int i = 1; i <= num_nodes; i++) {
        adjacency.emplace(i, Vector<int>());

    }
    for (const auto& b : branches) {
        adjacency[b.startNode].push_back(b.endNode);
        adjacency[b.endNode].push_back(b.startNode);
    }
}

Vector<int> buildCycle(int x, int y, const std::unordered_map<int,int>& parent) {
    std::unordered_map<int, bool> seen;
    Vector<int> pathA;
    while (x != -1 && parent.find(x) != parent.end()) {
        pathA.push_back(x);
        seen[x] = true;
        int next = parent.at(x); 
        x = next;
    }
    Vector<int> pathB;
    while (y != -1 && parent.find(y) != parent.end()) {
        if (seen.find(y) != seen.end()) break;
        pathB.push_back(y);
        int next = parent.at(y);
        y = next;
    }
    int LCA = y; 

    Vector<int> loop;
    for (size_t i = 0; i < pathA.size(); ++i) {
        loop.push_back(pathA[i]);
        if (pathA[i] == LCA) 
            break; 
    }
    for (int i = static_cast<int>(pathB.size()) - 1; i >= 0; --i) {
        loop.push_back(pathB[static_cast<size_t>(i)]);
    }
    loop.push_back(loop.at(0));
    return loop;
}

Vector<Vector<int>> bfsLoops(const std::unordered_map<int, Vector<int>>& adjacency, int start_hint = -1 ) {
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, int>  parent;

    Vector<Vector<int>> loops;
    std::queue<int> q;

    auto try_start = [&](int s) {
        if (visited[s]) return;
        visited[s] = true;
        parent[s] = -1;
        q.push(s);
        while (!q.empty()) {
            int cur = q.front(); 
            q.pop();
            auto it = adjacency.find(cur);
            if (it == adjacency.end()) 
                continue;

            for (int nbr : it->second) {
                if(nbr == 0) continue;
                if (!visited[nbr]) {
                    visited[nbr] = true;
                    parent[nbr] = cur;
                    q.push(nbr);
                } else if (parent[cur] != nbr) {
                    if (cur < nbr) { 
                        Vector<int> cyc = buildCycle(cur, nbr, parent);
                        for(int i = static_cast<int>(cyc.size()) - 1; i >= 0; i--) {
                            if (cyc[i] == 0) {
                                cyc.erase(i);
                            }
                        }
                        cyc.push_back(cyc[0]);
                        if (!cyc.isEmpty() && cyc.size() > 2) {
                            loops.push_back(cyc);
                        }
                    }
                }
            }
        }
    };
    for (size_t i = 0; i < loops.size(); i++) {
        std::cout << "Loop " << i + 1 << ": ";
        for (int node : loops[i])
            std::cout << node << " ";
        std::cout << "\n";
    }
    if (start_hint != -1 && adjacency.find(start_hint) != adjacency.end()) {
        try_start(start_hint);
    }
    for (const auto& kv : adjacency) {
        int node = kv.first;
        if (!visited[node]) 
            try_start(node);
    }

    return loops;
}

Vector<std::string> buildLoopEquations(const Vector<Vector<int>>& loops, const Vector<Branch>& branches) {
    Vector<std::string> equations;
    for (size_t i = 0; i < loops.size(); i++) {
        if (loops[i].size() < 2) continue;
        std::ostringstream eq;
        const auto& loop = loops[i];
        for (size_t j = 0; j + 1 < loop.size(); j++) {
            int a = loop[j];
            int b = loop[j + 1];
            for (const auto& br : branches) {
                if ((br.startNode == a && br.endNode == b) || (br.startNode == b && br.endNode == a)) {
                    int direction = (br.startNode == a && br.endNode == b) ? 1 : -1;
                    eq << (direction == 1 ? "+" : "-");
                    eq << br.resistance << "*" << br.variable;
                    if (br.voltage != 0) {
                        double effectiveVoltage = (direction == 1 ? -br.voltage : br.voltage);
                        if (effectiveVoltage >= 0) eq << "+";
                        eq << effectiveVoltage;
                    }
                    break;
                }
            }

        }
        eq << "=0";
        equations.push_back(eq.str());
    }
    return equations;
}

Vector<std::string> buildJunctionEquations(const Vector<Branch>& branches, const Vector<int>& nodes) {
    Vector<std::string> equations;
    for (int node : nodes) {
        std::ostringstream eq;
        for (const auto& b : branches) {
            if(b.startNode == node) {
                eq << "-" << b.variable;
            } else if (b.endNode == node) {
                eq << "+" << b.variable;
            }
        }
        eq << "=0";
        equations.push_back(eq.str());
    }
    return equations;
}