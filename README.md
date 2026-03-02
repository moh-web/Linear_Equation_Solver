Linear System Solver (Gaussian Elimination in C++)
📌 Overview

This project is a C++ implementation of an n × n system of linear equations solver using:

Dynamic memory allocation (new / delete)

Manual equation parsing from user input

Gaussian Elimination with Partial Pivoting

Back Substitution

The program reads equations in human-readable algebraic form and computes the numerical solution.

🧸 Features

Accepts dynamic system size (n × n)

Parses equations like:

3x1 + 55 + x2 = 60
2x1 - 4x2 + x3 = 10

Handles:

Positive and negative coefficients

Floating-point numbers

Constants on both sides

Detects:

Missing variable indices

Out-of-range variables

No unique solution (singular matrix)

Uses Partial Pivoting for numerical stability

🧠 Algorithm Used
1️⃣ Parsing Phase

The parseEquation() function:

Removes whitespace

Detects equation side (= handling)

Extracts:

Coefficients

Variable indices (x1, x2, ...)

Constants

Builds:

Coefficient matrix A

Constant vector b

2️⃣ Gaussian Elimination

Steps performed:

Partial pivoting (row swapping)

Forward elimination

Back substitution

Mathematically solving:

A x = b
⚙️ Technologies Used

C++

Standard Library:

<iostream>

<string>

<cctype>

<stdexcept>

<cmath>

No external libraries are required.

📂 Project Structure

Single file implementation:

main.cpp

Core components:

struct Equation

parseEquation()

Gaussian elimination logic

Back substitution

Memory cleanup

▶ How to Compile and Run
Using g++
g++ main.cpp -o solver
./solver
Using Visual Studio

Create new C++ Console Project

Replace main.cpp with this code

Build and run

🏃‍♂️‍➡️ Example Run

Input:

Enter number of equations (n x n system): 2

Equation 1: 2x1 + 3x2 = 13
Equation 2: x1 - x2 = 1

Output:

Solution:
x1 = 4
x2 = 3
⚠ Error Handling

The program throws runtime errors if:

Variable index is missing (x)

Variable index is out of range

Matrix is singular (no unique solution)

💾 Memory Management

Dynamic allocations used:

Equation* eq = new Equation[n];

eq[i].a = new double[n];

double* x = new double[n];

All memory is properly deallocated using:

delete[]

No memory leaks.

⌛️ Time Complexity

Gaussian Elimination:

O(n³)

Back Substitution:

O(n²)

Total Complexity:

O(n³)
🌚 Limitations

Solves only square systems (n × n)

Assumes unique solution exists

No support for infinite solutions case classification

No matrix input mode (equation text only)

📚 Educational Purpose

This project demonstrates:

Manual string parsing

Pointer-based dynamic arrays

Numerical linear algebra fundamentals

Error handling in C++

Clean memory management

Suitable for:

Data Structures courses

Numerical Methods courses

Linear Algebra implementation practice
