#include <string>
#include <iostream>
#include "../include/functions.hpp"
#include "../include/types.hpp"


std::string convertSquareToCoordinate(Square square) {
    int file = square % 8; //0-7
    int row = square / 8; //0-7

    std::string coordinate = "";
    switch(file) {
        case 0:
            coordinate += 'a';
            break;
        case 1:
            coordinate += 'b';
            break;
        case 2:
            coordinate += 'c';
            break;
        case 3:
            coordinate += 'd';
            break;
        case 4:
            coordinate += 'e';
            break;
        case 5:
            coordinate += 'f';
            break;
        case 6:
            coordinate += 'g';
            break;
        case 7:
            coordinate += 'h';
            break;
        default:
            return "error";
    }
    coordinate += std::to_string(row+1);
    return coordinate;
}

Color getOppositeColor(Color color) {
    if(color == WHITE) return BLACK;
    return WHITE;
}

void printMoves(std::vector<Move> moves, int max){
    int i = 0;
    while(i < moves.size() && i < max) {
        std::cout << convertSquareToCoordinate(moves.at(i).origin) << convertSquareToCoordinate(moves.at(i).destination) << std::endl;
        i++;
    }
}

Bitboard squareToBitboard(Square square) {
    return 1ULL << square;
}