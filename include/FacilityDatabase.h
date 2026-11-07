// the purpose of this file is to load data from facilities.txt
// the ID search will go through the hash table.

#ifndef FACILITYDATABASE_H
#define FACILITYDATABASE_H

#include "CampusGraph.h" // error until campus graph is implemented.
#include "Facility.h"
#include "HashTable.h"
#include <string>
#include <vector>

class FacilityDatabase {

private:
    HashTable table;  // ID lookups always go through the hash
    vector<Facility> records; // used for name category searches

    // checks one line from the file and returns false with a reason if it's rejected
    bool parseRecord(const string& line, const CampusGraph& graph, string& reason);

    // prints one facility on one line
    void printFacility(const Facility& facility) const;

    // prints the load summary with the hash table numbers
    void printSummary(const string& path, int accepted, int rejected) const;

public:
    static bool isValidId(const string& id);

    // reads facilities.txt and buildings must be loaded in the graph

    void load(const string& path, const CampusGraph& graph);

    void displayAll() const;
    void searchById(const string& input) const;
    void searchByName(const string& input) const;
    void searchByCategory(const string& input) const;
    void categoryReport() const;
};

#endif