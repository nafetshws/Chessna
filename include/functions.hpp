#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include "types.hpp"

std::string convertSquareToCoordinate(Square square); 
Color getOppositeColor(Color color);
void printMoves(std::vector<Move> moves, int max = 20);
Bitboard squareToBitboard(Square square);

#endif