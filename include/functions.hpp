#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include "types.hpp"

std::string convertSquareToCoordinate(Square square); 
Square convertCoordinateToSquare(std::string coordinate);
Color getOppositeColor(Color color);
void printMoves(std::vector<Move> moves, int max = 40);
std::string printableMove(Move move);
Bitboard squareToBitboard(Square square);
Square bitboardToSquare(Bitboard b);
std::vector<Square> convertBitboardToSquares(Bitboard bitboard);
void convertBitbaordToMoves(Bitboard intersect, Bitboard destination, PieceType pieceType, Color color, Bitboard occupied, int &numberOfAttacks, Attack_Info &attack_info);

int getPieceTypeValue(PieceType pieceType);
int popcount(Bitboard bitboard);

u64 getCurrentTime();
float getTimeDifference(u64 before, u64 after);




#endif