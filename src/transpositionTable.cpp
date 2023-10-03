#include <iostream>
#include "../include/transpositionTable.hpp"

TranspositionTable::TranspositionTable(int sizeMB) {
    int entrySize = sizeof(hashEntry);
    this->numberOfMaxEntries = (sizeMB * 1024 * 1024) / entrySize; 
    this->numberOfCurrentEntries = 0;
}

