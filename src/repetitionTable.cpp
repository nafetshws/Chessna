#include "../include/repetitionTable.hpp"

#include <map>
#include <iostream>
#include "../include/types.hpp"

u64 RepetitionTable::repetitions[RT_SIZE];
int RepetitionTable::numberOfEntries;

void RepetitionTable::init() {
    std::fill_n(repetitions, numberOfEntries, 0);
    numberOfEntries = 0;
}

bool RepetitionTable::checkForDrawByRepetition(u64 zobristKey) {
    for(int i = 0; i < numberOfEntries; i += 2) {
        if(repetitions[i] == zobristKey) {
            return (repetitions[i+1]+1 >= 3);
        } 
    }

    return false;
}

void RepetitionTable::incrementPositionOccurence(u64 zobristKey) {
    for(int i = 0; i < numberOfEntries; i+=2) {
        if(repetitions[i] == zobristKey) {
            repetitions[i+1] += 1;
            return;
        }
    }

    if(numberOfEntries < RT_SIZE) {
        repetitions[numberOfEntries] = zobristKey;
        repetitions[numberOfEntries+1] = 1;
        numberOfEntries += 2;
    }
}

