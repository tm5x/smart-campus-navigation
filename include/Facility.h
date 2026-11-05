// the purpose of this file is to describe one campus facility record
// this includes ID, name, building, category


#ifndef FACILITY_H
#define FACILITY_H

#include <string>
using std::string; 

// a single facility with ID always being uppercase!
class Facility{
private:
    string facility_ID;
    string facility_Name;
    string building_Name;
    string category_Name;


public:
    Facility();
    // making the constructor
    Facility(const string& inc_facility_ID,
             const string& inc_facility_Name,
             const string& inc_building_Name,
             const string& inc_category_Name);

    string getFacilityID() const;
    string getFacilityName() const;
    string getBuildingName() const;
    string getCategory() const;

    };
#endif