#include <iostream>
#include "../include/transpositionTable.hpp"
#include "../include/types.hpp"
#include <vector>

long TranspositionTable::numberOfCurrentEntries;
long TranspositionTable::numberOfMaxEntries;
HashEntry TranspositionTable::entries[TT_SIZE];
HashEntry TranspositionTable::testEntry = HashEntry(0, 0, 0, HASH_EXACT, Move());


void TranspositionTable::init(int sizeMB) {
    int entrySize = sizeof(HashEntry);
    numberOfMaxEntries = (sizeMB * 1024 * 1024) / entrySize; 
    numberOfCurrentEntries = 0;
}

void TranspositionTable::storeTtEvaluation(u64 zobristKey, int depth, int eval, EvalFlag flag, Move move) {
    //Always replace
    HashEntry entry(zobristKey, depth, eval, flag, move);
    entries[zobristKey % numberOfMaxEntries] = entry;
}

int TranspositionTable::getTtEvaluation(u64 zobristKey, int depth, int alpha, int beta) {
    HashEntry entry = entries[zobristKey % numberOfMaxEntries];
    if(entry.zobristKey != zobristKey) return POS_NOT_FOUND;

    if(entry.depth >= depth){
        switch (entry.flag)
        {
        case HASH_EXACT:
            return entry.eval;
        case HASH_ALPHA:
            if(entry.eval <= alpha) {
                return alpha;
            }
            return entry.eval;
        case HASH_BETA:
            if(entry.eval >= beta) {
                return beta;
            }
            break;
        default:
            break;
        }
    }
    return POS_NOT_FOUND;
}


HashEntry* TranspositionTable::fetchEntry(u64 zobristKey) {
    HashEntry* entry = &entries[zobristKey % numberOfMaxEntries];
    if(entry->zobristKey == zobristKey) return entry;
    return NULL;
}

