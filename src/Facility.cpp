//this file holds the constructors and the getters for the facility class

#include "../include/Facility.h"

Facility::Facility() {

}

Facility::Facility(const string& inc_facility_ID, const string& inc_facility_Name, const string& inc_building_Name, const string& inc_category_Name){

    facility_ID = inc_facility_ID;
    facility_Name = inc_facility_Name;
    building_Name = inc_building_Name;
    category_Name = inc_category_Name;
}

// each getter needs to return the right data

string Facility::getFacilityID() const {
    return facility_ID;
}

string Facility::getFacilityName() const {
    return facility_Name;
}

string Facility::getBuildingName() const {
    return building_Name;
}

string Facility::getCategory() const {
    return category_Name;
}
