#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using namespace std;

struct Building {
  string buildingName;
  vector <string> neighbors;
};

class CampusGraph {
  private:
    vector <Building> buildings;
  public:
    void addBuilding(string buildingName);
    void Connect(string buildingName, string neighborName);
    void DisplayGraph();
    void DisplayStats();
    void FindBuilding();
    void bfs();
};

#endif
