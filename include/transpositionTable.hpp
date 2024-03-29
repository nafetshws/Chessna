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

const unsigned int TT_SIZE_MB = 1000;
const unsigned int TT_SIZE = (TT_SIZE_MB * 1024 * 1024) / sizeof(HashEntry);

const int POS_NOT_FOUND = -100000;
const int POS_NOT_DEEP_ENOUGH = -200000;

class TranspositionTable {
    public:

        //static std::vector<HashEntry> entries;
        static HashEntry testEntry;

        static HashEntry entries[];

        static long numberOfCurrentEntries;
        static long numberOfMaxEntries; 

        static void init();

        static int getTtEvaluation(u64 zobristKey, int depth, int alpha, int beta, int depthFromRoot);
        static void storeTtEvaluation(u64 zobristKey, int depth, int eval, EvalFlag flag, Move move, int depthFromRoot);

        static int convertEvalToTT(int eval, int depthFromRoot);
        static int convertEvalFromTT(int eval, int depthFromRoot);

        static HashEntry* fetchEntry(u64 zobristKey);

    private:
};

#endif
