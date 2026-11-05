

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Facility.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

const int DEFAULT_CAPACITY = 11;


class HashTable{
    private:
        //list of facilities that hash to index i
        vector<vector<Facility> > buckets;
        int entryCount;
        int collisionCount;


        // turns an uppercase ID into an index starting from 0 to capacity - 1
        int hash(const string& key) const;

    public:
        // constructor
        HashTable(int capacity = DEFAULT_CAPACITY);

        // adds a facility and will return false if that ID exists
        bool insert(const Facility& facility);

        // looks up an ID and returns false if it isn't in the table
        bool find(const string& id, Facility& outputFacility) const;

        int capacity() const;
        int size() const;
        int collisions() const;
};

#endif