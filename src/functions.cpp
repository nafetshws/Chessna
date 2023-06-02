#include <string>
#include <iostream>
#include "../include/functions.hpp"

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