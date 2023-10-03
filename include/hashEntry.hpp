#ifndef HASH_ENTRY_H
#define HASH_ENTRY_H

#include "../include/types.hpp"

class hashEntry {
    public:
        u64 zobristKey;
        int depth;
        int eval;
        Move move; 

    private:
};

#endif