#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <vector> 
#include "../include/hashEntry.hpp"

class TranspositionTable {
    public:
        TranspositionTable(int sizeMB);
        std::vector<hashEntry> entries;

        long numberOfCurrentEntries;
        long numberOfMaxEntries; 

    private:
};

#endif
