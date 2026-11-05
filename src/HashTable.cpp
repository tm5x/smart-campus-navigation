// our own hash table for facilities

#include "../include/HashTable.h"
#include <cctype>

using namespace std;

static string toUpper(string text) {
    for (size_t i = 0; i < text.size(); i++){
        text[i] = (char) toupper((unsigned char) text[i]);

    }
    return text;
}

HashTable::HashTable(int capacity) {
    if (capacity <= 0){
        capacity = DEFAULT_CAPACITY;
    }
    buckets.resize(capacity);
    entryCount = 0;
    collisionCount = 0;
}

// our hash function will start with h = 0 then for every character
// h = (h * 31 + character) % capacity
// the most important part of this function is the modulos because it keeps the number small
// to prevent overflowing

int HashTable::hash(const string& key) const{
    long cap = (long) buckets.size();
    long h = 0;
    for(size_t i = 0; i < key.size(); i++){
         h = (h * 31 + (unsigned char) key[i]) % cap;
    }
    return (int) h;
}

bool HashTable::insert(const Facility& facility) {
    string key = toUpper(facility.getFacilityID());
    int index = hash(key);

    // checking for duplicates
    for (size_t i = 0; i < buckets[index].size(); i++){
        if (buckets[index][i].getFacilityID() == key){
            return false; //do not count rejected dupes
        }
    }
    // collision definition saying something inserterd that already
    // has something in it
    if (!buckets[index].empty()){
        collisionCount++;
    }

    Facility stored(key, facility.getFacilityName(), facility.getBuildingName(), facility.getCategory());
    buckets[index].push_back(stored);
    entryCount++;
    return true;

}

// looks up an ID, only the one bucket it hashes to is searched
bool HashTable::find(const string& id, Facility& outputFacility) const {
    string key = toUpper(id);
    int index = hash(key);

    for (size_t i = 0; i < buckets[index].size(); i++) {
        if (buckets[index][i].getFacilityID() == key) {
            outputFacility = buckets[index][i];
            return true;
        }
    }
    return false;
}

int HashTable::capacity() const {
    return (int) buckets.size();
}

int HashTable::size() const {
    return entryCount;
}

int HashTable::collisions() const {
    return collisionCount;
}