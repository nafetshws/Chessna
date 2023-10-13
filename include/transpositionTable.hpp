#ifndef TRANSPOSITION_TABLE_H
#define TRANSPOSITION_TABLE_H

#include <vector> 
#include "../include/types.hpp"

struct HashEntry {
    u64 zobristKey;
    int depth;
    int eval;
    EvalFlag flag;
    Move move;

    HashEntry(u64 _zobristKey, int _depth, int _eval, EvalFlag _flag, Move _move) :
    zobristKey(_zobristKey), depth(_depth), eval(_eval), flag(_flag), move(_move) {}

    HashEntry() : zobristKey(0), depth(0), eval(0), flag(HASH_EXACT), move(Move()) {}
};

const int POS_NOT_FOUND = -100000;
const int POS_NOT_DEEP_ENOUGH = -200000;

class TranspositionTable {
    public:

        //static std::vector<HashEntry> entries;
        static HashEntry testEntry;

        static HashEntry entries[];

        static long numberOfCurrentEntries;
        static long numberOfMaxEntries; 

        static void init(int sizeMB);

        static int getTtEvaluation(u64 zobristKey, int depth, int alpha, int beta);
        static void storeTtEvaluation(u64 zobristKey, int depth, int eval, EvalFlag flag, Move move);

        static HashEntry* fetchEntry(u64 zobristKey);

    private:
};

#endif
