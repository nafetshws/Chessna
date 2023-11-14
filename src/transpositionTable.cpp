#include <iostream>
#include "../include/transpositionTable.hpp"
#include "../include/types.hpp"
#include "../include/evaluation.hpp"
#include <vector>

long TranspositionTable::numberOfCurrentEntries;
long TranspositionTable::numberOfMaxEntries;
HashEntry TranspositionTable::entries[TT_SIZE];
HashEntry TranspositionTable::testEntry = HashEntry(0, 0, 0, HASH_EXACT, Move());


void TranspositionTable::init() {
    numberOfMaxEntries = TT_SIZE;
    numberOfCurrentEntries = 0;
}

void TranspositionTable::storeTtEvaluation(u64 zobristKey, int depth, int eval, EvalFlag flag, Move move, int depthFromRoot) {
    //Always replace
    HashEntry entry(zobristKey, depth, convertEvalToTT(eval, depthFromRoot), flag, move);
    entries[zobristKey % numberOfMaxEntries] = entry;
}

int TranspositionTable::getTtEvaluation(u64 zobristKey, int depth, int alpha, int beta, int depthFromRoot) {
    HashEntry entry = entries[zobristKey % numberOfMaxEntries];
    if(entry.zobristKey != zobristKey) return POS_NOT_FOUND;
    entry.eval = convertEvalFromTT(entry.eval, depthFromRoot);

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
    return POS_NOT_DEEP_ENOUGH;
}


HashEntry* TranspositionTable::fetchEntry(u64 zobristKey) {
    HashEntry* entry = &entries[zobristKey % numberOfMaxEntries];
    if(entry->zobristKey == zobristKey) return entry;
    return NULL;
}

int TranspositionTable::convertEvalToTT(int eval, int depthFromRoot) {
    if(eval > 99000) {
        return (eval + depthFromRoot);
    } else if(eval < -99000) {
        return (eval - depthFromRoot);
    } else {
        return eval;
    }
}
int TranspositionTable::convertEvalFromTT(int eval, int depthFromRoot) {
    if(eval > 99000) {
        return (eval - depthFromRoot); 
    } else if(eval < -99000) {
        return (eval + depthFromRoot);
    } else {
        return eval;
    }
}

