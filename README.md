# smart-campus-navigation
Project 2 for CSCE 2110

## Project Description

Menu driven C++ program that loads campus data from four different text files. This program also lets the user find campus facilities, explore building connections, and find optimal routes. Demonstrating hash tables, graphs, breadth-first search (BFS), and general trees. 

## Team Members

- Trenton Maxime (Member A)
- Steven Aguilar (Member B)
- Alison Rojo Hernandez (Member C)

Individual responsibilities and contributions are documented in the
separate Group Contribution Report.

## Project Files

Header files: CampusGraph.h, DepartmentTree.h, Facility.h, FacilityDatabase.h, HashTable.h
Source code: DepartmentTree.cpp, Facility.cpp, FacilityDatabase.cpp, HashTable.cpp, main.cpp
Data files: buildings.txt, connections.txt, facilities.txt, department.txt

## Features

Polynomial string hash implementation
General department tree
Tree traversals and department statistics
Separate chaining collision strategy

## Compile and Run

Run from the project root: g++ -std=c++17 -Wall -Wextra src/*.cpp -o campus
