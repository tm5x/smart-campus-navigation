// this file is meant to load facilities.txt, search the facilities, and prints the category report
// this file will also feature various error handling and input checks

#include "../include/FacilityDatabase.h"
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

// helper functions only used in this file
// (every .cpp keeps its own copies, there is no shared utils file)

// removes spaces, tabs, \r and \n from both ends (spaces in the middle stay)
static string trim(const string& s) {
    string whitespace = " \t\r\n";
    size_t start = s.find_first_not_of(whitespace);
    if (start == string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(whitespace);
    return s.substr(start, end - start + 1);
}

// makes a copy of the text in uppercase (also used to compare ignoring case)
static string toUpper(string text) {
    for (size_t i = 0; i < text.size(); i++) {
        text[i] = (char) toupper((unsigned char) text[i]);
    }
    return text;
}

// splits a line at each delimiter, empty fields are kept
// so F005||Library|Lab gives 4 fields and is rejected for the empty one
static vector<string> split(const string& line, char delimiter) {
    vector<string> fields;
    string current = "";
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == delimiter) {
            fields.push_back(current);
            current = "";
        }
        else {
            current += line[i];
        }
    }
    fields.push_back(current);
    return fields;
}

// blank lines and comment lines are skipped
// a comment only counts if # is the first character that is not whitespace
static bool shouldSkipLine(const string& line) {
    string t = trim(line);
    return t.empty() || t[0] == '#';
}

// F followed by exactly three digits
bool FacilityDatabase::isValidId(const string& id) {
    if (id.size() != 4 || id[0] != 'F') {
        return false;
    }
    for (size_t i = 1; i < id.size(); i++) {
        if (!isdigit((unsigned char) id[i])) {
            return false;
        }
    }
    return true;
}

// reads the whole file one line at a time
void FacilityDatabase::load(const string& path, const CampusGraph& graph) {
    ifstream file;
    file.open(path);

    // missing file, print an error and treat it as empty
    if (!file.is_open()) {
        cout << "Error: could not open " << path << "! This file will be treated as empty." << endl;
        printSummary(path, 0, 0);
        return;
    }

    string line;
    int lineNumber = 0;
    int accepted = 0;
    int rejected = 0;

    // reading files and incrementing the variables above
    while (getline(file, line)) {
        lineNumber++;

        if (shouldSkipLine(line)) {
            continue;
        }

        string reason = "";
        if (parseRecord(line, graph, reason)) {
            accepted++;
        }
        else {
            cout << "Warning: " << path << " line " << lineNumber << ": " << reason << endl;
            rejected++;
        }
    }
    file.close();

    printSummary(path, accepted, rejected);
}

// checks one record in this order: field count, empty fields, ID format,
// unknown building, duplicate ID
bool FacilityDatabase::parseRecord(const string& line, const CampusGraph& graph, string& reason) {
    vector<string> fields = split(line, '|');

    // checking if the user entered the right amount of fields.
    if (fields.size() != 4) {
        reason = "expected 4 fields, found " + to_string(fields.size());
        return false;
    }

    for (size_t i = 0; i < fields.size(); i++) {
        fields[i] = trim(fields[i]);
    }

    if (fields[0].empty() || fields[1].empty() || fields[2].empty() || fields[3].empty()) {
        reason = "empty required field";
        return false;
    }

    // uppercase the ID first, then check the format
    string id = toUpper(fields[0]);
    if (!isValidId(id)) {
        reason = "invalid ID \"" + fields[0] + "\" (must be F followed by exactly three digits)";
        return false;
    }

    // code depends on campus graph file
    int buildingIndex = graph.findBuilding(fields[2]);
    if (buildingIndex == -1) {
        reason = "unknown building \"" + fields[2] + "\"";
        return false;
    }

    // use the building spelling from buildings.txt
    Facility facility(id, fields[1], graph.getBuildingName(buildingIndex), fields[3]);

    // the table rejects duplicate IDs (ignoring case) and keeps the first one
    if (!table.insert(facility)) {
        reason = "duplicate facility ID " + id;
        return false;
    }

    // only accepted records get added to the list
    records.push_back(facility);
    return true;
}

// shows every facility in file order
void FacilityDatabase::displayAll() const {
    if (records.empty()) {
        cout << "No facilities are loaded." << endl;
        return;
    }

    for (size_t i = 0; i < records.size(); i++) {
        printFacility(records[i]);
    }
    cout << "Total facilities: " << records.size() << endl;
}

// search by ID using the hash table (no scanning the list)
void FacilityDatabase::searchById(const string& input) const {
    // trim and uppercase before anything else
    string id = toUpper(trim(input));

    // outcome 1: bad format
    if (!isValidId(id)) {
        cout << "Invalid format: an ID is F followed by exactly three digits." << endl;
        return;
    }

    Facility found;
    if (table.find(id, found)) {
        // outcome 3: found
        cout << "Found: ";
        printFacility(found);
    }
    else {
        // outcome 2: valid format but not in the table
        cout << "Not found: no facility with ID " << id << "." << endl;
    }
}

// search by the complete facility name, ignoring case names can repeat, so every match is shown

void FacilityDatabase::searchByName(const string& input) const {
    string target = toUpper(trim(input));
    int matches = 0;

    for (size_t i = 0; i < records.size(); i++) {
        if (toUpper(records[i].getFacilityName()) == target) {
            printFacility(records[i]);
            matches++;
        }
    }

    // let the user know there are no matches for their input
    if (matches == 0) {
        cout << "No facilities named \"" << trim(input) << "\"." << endl;
    }
    cout << "Matches: " << matches << endl;
}

// search by the complete category, ignoring upper or lower case
void FacilityDatabase::searchByCategory(const string& input) const {
    string target = toUpper(trim(input));
    int matches = 0;

    for (size_t i = 0; i < records.size(); i++) {
        if (toUpper(records[i].getCategory()) == target) {
            printFacility(records[i]);
            matches++;
        }
    }

    if (matches == 0) {
        cout << "No facilities in category \"" << trim(input) << "\"." << endl;
    }
    cout << "Matches: " << matches << endl;
}

// count and percentage for each category
// categories that only differ by case are grouped, the first spelling is kept
void FacilityDatabase::categoryReport() const {
    // return early when empty so we never divide by zero
    if (records.empty()) {
        cout << "No facilities are available." << endl;
        cout << "Total: 0" << endl;
        return;
    }

    // two lists that line up, counts[i] belongs to categories[i]
    vector<string> categories;
    vector<int> counts;

    for (size_t i = 0; i < records.size(); i++) {
        bool found = false;
        for (size_t j = 0; j < categories.size(); j++) {
            if (toUpper(categories[j]) == toUpper(records[i].getCategory())) {
                counts[j]++;
                found = true;
                break;
            }
        }
        if (!found) { // "not false"
            categories.push_back(records[i].getCategory());
            counts.push_back(1);
        }
    }

    double total = (double) records.size();

    // one decimal place for the percentages
    cout << fixed << setprecision(1);
    for (size_t i = 0; i < categories.size(); i++) {
        cout << categories[i] << ": " << counts[i]
             << " (" << (counts[i] * 100.0 / total) << "%)" << endl;
    }
    cout << "Total: " << records.size() << " (100.0%)" << endl;

    // put the number formatting back to normal
    cout << defaultfloat << setprecision(6);
}

// ID | name | building | category
void FacilityDatabase::printFacility(const Facility& facility) const {
    cout << facility.getFacilityID() << " | "
         << facility.getFacilityName() << " | "
         << facility.getBuildingName() << " | "
         << facility.getCategory() << endl;
}

// load summary plus the hash table numbers on the same line
void FacilityDatabase::printSummary(const string& path, int accepted, int rejected) const {
    cout << path << ": " << accepted << " accepted, " << rejected << " rejected"
         << " (hash capacity " << table.capacity()
         << ", entries " << table.size()
         << ", collisions " << table.collisions() << ")" << endl;
}