# Kirchoff-Circuit-Final-Project
Title:
Kirchhoff Circuit Simulation and Analysis Program

Description:
This program simulates and then analyzes a complex circuit involving batteries and resistors. The user is asked to describe the circuit with nodes and branches. A node is any junction point in the circuit. A branch is any line connecting two nodes together. The user is then asked to give properties within the node including the total resistance, total voltage, start node, and end node. These are important in analyzing the circuit. The program will then analyze the circuit and be able to output the currents throughout the circuit.

UML Diagrams:
Vector
-----------
-data: T*
-num_items: size_t
-capacity: size_t
-resize(size_t new_capacity): void
------------
+Vector()
+Vector(size_t _size)
+Vector(size_t _size, const T& val)
+Vector(const Vector& other)
+Vector& operator=(const Vector& other)
+Vector(Vector&& other)
+Vector& operator=(Vector&& other) noexcept
+~Vector()
+push_back(const T& val): void
+pop_back(): void
+erase(size_t index): void
erase(T* it): void
+back(): T&
+back() const: const T&
+front(): T&
+front() const: const T&
+operator[](size_t index): T&
+operator[](size_t index) const: const T&
+at(size_t index): T&
+at(size_t index) const: const T&
+size() const: size_t
+capacity() const: size_t
+clear(): void
+isEmpty(): bool
+begin(): T*
+end(): T*
+begin() const: const T*
+end() const: const T*


Branch
--------
-startNode: int
-endNode: int
-resistance: double
-voltage: double
-variable: std::string


Stack
--------
-data: Vector<T>
---------
+push(const T& val): void
+pop(): void
+top(): T&
+isEmpty(): bool
+size(): size_t
+peek(size_t depth = 0): T&
+clear(): void
+operator<<(std::ostream& os, const Stack<T>& s): friend std::ostream&


Matrix
----------
-mat_data: Vector<Vector<T>>
-row: size_t
-col: size_t
----------
+Matrix()
+Matrix(size_t r, size_t c)
+Matrix(const Matrix& other)
+operator=(const Matrix& other): Matrix&
+Matrix(Matrix&& other) noexcept
+operator=(Matrix&& other): Matrix&
+setRow(size_t r, const Vector<T>& rowVec): void
+printMatrix() const: void
+operator()(size_t i, size_t j): T&
+operator()(size_t i, size_t j) const: const T&
+operator[](size_t i): Vector<T>&
+operator[](size_t i) const: const Vector<T>&
+numRow(): size_t
+numCol(): size_t
+setRow(size_t r): void
+setCol(size_t c): void
+partialPivot(size_t k): void
+gaussianElimination(): void
+backSubstitution(): Vector<std::variant<double,std::string>>
+solve(): Vector<std::variant<double, std::string>>


Efficiency of Algorithms:
Vector Class:
void resize(size_t new_capacity) efficiency: O(n)
Default Cnstructor is O(1) but all other constructors and copy functions are O(n)
Move constructor and assignemnt is O(1)
Destructor is O(1)
Erase function is O(n) but all other functions are O(1)

Stack Class:
Clear and overload operator << are O(n)
All other functions are O(1)

Matrix Class: 
Constructor is O(n)
Copy, assignment, and move are all O(1)
Set row is O(1)
Print matrix is O(n)
Partial pivot function is O(n)
Gaussian elimination funnction is O(n^3) because it is O(n^2) and then calls the partial pivot function
Back substitution is O(n^2)
The solve combines back substitution and gaussian elimination together making it O(n^3)

Functions:
isDigits is O(1)
buildRow function is O(n^2). However after looking at it and learning more about hash maps you can improve this runtime to O(n). 
Printing the solution is O(n)
Building the adjacency is O(n)
Building the cycle is O(n)
The bfsLoops equation depends on how dense the graph is. If the graph is really dense, meaning it has a lot of elements it acts more like a O(n^3) runtime but with a sparse amount of elements it is more like O(n^2)
The build loop equations function is O(n^3) however if I utilized hash maps, which I am started to consider going back and implementing, it would become O(1) to look up the the branches making the runtime O(1). 
Building the junction equations is O(n^2)

References: 
I used ChatGPT. The main thing I used it for was debugging my code. It helped a lot more when I found where the problem was in my code, like the specific function or even a few lines that held the error and then asking it to look and giving me possible fixes to my code. While coming up with my project I would bounce a lot of my ideas off of ChatGPT to see what it thought or be able to look at a loose logic of what I was trying to accomplish. I would then take that loose logic and implement it the way that I wanted to. If ChatGPT utilized something that I wasn't very familiar with I would research it until I understood exactly how it worked. 
For example, I used GeeksForGeeks, a popular website that helps people learn coding, to learn about hash maps before I was able to learn about them in class. I didn't utilize them for everything as I mentioned above, but I did use it in my BFS function. I also used GeeksForGeeks to research variant types in C++. 

Notes:
This is not the project I was supposed to do for my final extra credit project however it implements a LOT of what I have learned throughout the course of the semester. The most obvious are the Vector and Stack class that I used throughout my code. I am going to be completely honest, making my own vector class made this project a lot harder, but it was also a lot more fun. Using a vector class helped me to really see how it works and how many different types of functions play into the regular vector class. The stack class was arguably necessary to use for my project. It utilizes a function called peek() for sparsing the equation strings to see if there is a variable after a number is detected. This is important because it determines what to do with the number before removing it from the stack, whether thats add it to the variable so that in the matrix the number appears in its respective spot or whether that is indicative of the voltage and should be added to the augment of the matrix. My function utilizes breadth-first search (BFS) to search through the circuit graph and find loops inside of the circuit graph. This is important for coming up with the loop equations that describe the circuit graph. I utilize the C++ queue, but eventually want to create my own queue and implement that into the BFS function, kind of just for fun. 

Running the program: 
To run the code run command line: clang++ --std=c++17 Kirchhoff_Circuit.cpp Functions.cpp
Then run: ./a.out


