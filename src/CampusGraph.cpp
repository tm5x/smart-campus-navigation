#include "../include/CampusGraph.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <cctype>
using namespace std;

void CampusGraph::addBuilding(string buildingName) {
    for (int i = 0; i < buildings.size(); i++) {
        if (buildingName == buildings[i].buildingName) {
            return;
        }
    }

    Building newBuilding;
    newBuilding.buildingName = buildingName;
    buildings.push_back(newBuilding);
}

int CampusGraph::FindBuilding(string buildingName) const {
    for (int i = 0; i < buildings.size(); i++) {
        if (buildingName == buildings[i].buildingName) {
            return i;
        }
    }
    return -1;
}

string CampusGraph::getBuildingName(int index) const{
    if (index < 0 || index >= (int) buildings.size()) {
        return "";
        }
    return buildings[index].buildingName;

}

void CampusGraph::Connect(string buildingName, string neighborName) {
    int index = FindBuilding(buildingName);
    int neighborIndex = FindBuilding(neighborName);

    if (index == -1) {
        cout << "Building not found" << endl;
        return;
    }
    if (neighborIndex == -1) {
        cout << "Building not found" << endl;
        return;
    }

    if (index == neighborIndex) {
        cout << "Error" << endl;
        return;
    }

    for (int j = 0; j < buildings[index].neighbors.size(); j++) {
        if (neighborName == buildings[index].neighbors[j]) {
            return;
        }
    }

    buildings[index].neighbors.push_back(neighborName);
    buildings[neighborIndex].neighbors.push_back(buildingName);
}

void CampusGraph::DisplayGraph() {
    if (buildings.size() == 0) {
        cout << "No buildings" << endl;
        return;
    }
    for (int i = 0; i < buildings.size(); i++) {
        cout << buildings[i].buildingName << endl;
        if (buildings[i].neighbors.size() == 0) {
            cout << "No connections" << endl;
        }
        for (int j = 0; j < buildings[i].neighbors.size(); j++) {
            cout << buildings[i].neighbors[j] << endl;
        }
    }
}

void CampusGraph::DisplayStats() {
    cout << "Total buildings: " << buildings.size() << endl;

    int TotalDegree = 0;

    for (int i = 0; i < buildings.size(); i++) {
        TotalDegree += buildings[i].neighbors.size();
    } 
    int TotalConnections = TotalDegree/2;
    cout << "Total connections: " << TotalConnections << endl;

    for (int i = 0; i < buildings.size(); i++) {
        cout << buildings[i].buildingName << " Degree: " << buildings[i].neighbors.size() << endl;
    }
}

void CampusGraph::bfs(string buildingName) {
    vector<bool>visited(buildings.size(), false);
    queue<int> b;
    int StartIndex = FindBuilding(buildingName);

    if (StartIndex == -1) {
        cout << "Building not found" << endl;
        return;
    }

    visited[StartIndex] = true;
    b.push(StartIndex);

    while (!b.empty()) {
        int current = b.front();
        b.pop();
        cout << buildings[current].buildingName << endl;
        for (int j = 0; j < buildings[current].neighbors.size(); j++) {
            int neighborIndex = FindBuilding(buildings[current].neighbors[j]);
            if (neighborIndex == -1) {
                continue;
            }
            if (visited[neighborIndex] == false) {
                visited[neighborIndex] = true;
                b.push(neighborIndex);
            }
        } 
    }
}

static string trimText(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// error message if loading text doesn't work
// code to go with the header file missing before
void CampusGraph::LoadBuildings(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: could not open " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        line = trimText(line);
        if (line.empty() || line[0] == '#') continue;
        addBuilding(line);
    }
}

void CampusGraph::LoadConnections(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: could not open " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        line = trimText(line);
        if (line.empty() || line[0] == '#') continue;
        size_t bar = line.find('|');
        if (bar == string::npos) continue;
        Connect(trimText(line.substr(0, bar)), trimText(line.substr(bar + 1)));
    }
}
