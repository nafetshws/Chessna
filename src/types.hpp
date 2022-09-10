#ifndef TYPES_H
#define TYPES_H

#include <string>

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
    WEST = 1,
    
    //Knight
    NORTH_NORTH_EAST = 17,
    NORTH_EAST_EAST = 10,
    SOUTH_EAST_EAST = 6,
    SOUTH_SOUTH_EAST = 15,
    SOUTH_SOUTH_WEST = 17,
    SOUTH_WEST_WEST = 10,
    NORTH_WEST_WEST = 6,
    NORTH_NORTH_WEST = 15

};

const Bitboard FILE_A = 0x101010101010101;
const Bitboard FILE_B = 0x202020202020202;
const Bitboard FILE_G = 0x4040404040404040;
const Bitboard FILE_H = 0x8080808080808080;
const Bitboard EMPTY = 0x0;
const Bitboard RANK_1 = 0xff;
const Bitboard RANK_2 = 0xff00;
const Bitboard RANK_5 = 0xff00000000;
const Bitboard RANK_7 = 0xff000000000000;
const Bitboard RANK_8 = 0xff00000000000000;

const std::string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

#endif