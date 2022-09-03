#ifndef TYPES_H
#define TYPES_H

typedef unsigned long long Bitboard;
typedef int Square;

enum Direction {
    NORTH = 8,
    NORTH_EAST = 9,
    NORTH_WEST = 7,

    SOUTH = 8,
    SOUTH_EAST = 7,
    SOUTH_WEST = 9,

    EAST = 1,
    WEST = 1

};

const Bitboard A_FILE = 0x101010101010101;
const Bitboard H_FILE = 0x8080808080808080;
const Bitboard EMPTY = 0x0;

#endif