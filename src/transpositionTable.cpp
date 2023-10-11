#include <iostream>
#include "../include/transpositionTable.hpp"
#include "../include/types.hpp"
#include <vector>

long TranspositionTable::numberOfCurrentEntries;
long TranspositionTable::numberOfMaxEntries;
std::vector<HashEntry> TranspositionTable::entries;


void TranspositionTable::init(int sizeMB) {
    int entrySize = sizeof(HashEntry);
    numberOfMaxEntries = (sizeMB * 1024 * 1024) / entrySize; 
    std::cout << numberOfMaxEntries << std::endl;
    entries = std::vector<HashEntry>(numberOfMaxEntries); 
    numberOfCurrentEntries = 0;
}

void TranspositionTable::storeTtEvaluation(u64 zobristKey, int depth, int eval, EvalFlag flag, Move move) {
    //Always replace
    HashEntry entry(zobristKey, depth, eval, flag, move);
    entries.insert(entries.begin() + (zobristKey % numberOfMaxEntries), entry);
}

int TranspositionTable::getTtEvaluation(u64 zobristKey, int depth, int alpha, int beta) {
    HashEntry entry = entries.at(zobristKey % numberOfMaxEntries);
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
    HashEntry* entry = &entries.at(zobristKey % numberOfMaxEntries);
    if(entry->zobristKey == zobristKey) return entry;
    return NULL;
}

