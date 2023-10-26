#ifndef REPETITION_TABLE_H
#define REPETITION_TABLE_H

#include <map>
#include "../include/types.hpp"

const unsigned int RT_SIZE = 1000;

class RepetitionTable {
    public:
        static void init();
        static bool checkForDrawByRepetition(u64 zobristKey);
        static void incrementPositionOccurence(u64 zobristKey);
        static int numberOfEntries;
    private:
        static u64 repetitions[RT_SIZE];



};

#endif