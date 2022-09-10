#include <iostream>
#include <string>
#include "board.hpp"

void Board::initBoard(){
    whitePawns = 0xff00;
    whiteRooks = 0x81;
    whiteKnights = 0x42;
    whiteBishops = 0x24;
    whiteQueen = 0x8;
    whiteKing = 0x10;

    blackPawns = 0xff000000000000;
    blackRooks = 0x8100000000000000;
    blackKnights = 0x4200000000000000;
    blackBishops = 0x2400000000000000;
    blackQueen = 0x800000000000000;
    blackKing = 0x1000000000000000;
}

void Board::initBoard(std::string fen){
    //initBoard();

    std::string delimiter = " ";
    int pos = 0;

    std::string piecePlacement = fen.substr(pos, fen.find(delimiter, pos));
    pos += piecePlacement.length()+1;
    std::string sideToMove = fen.substr(pos, fen.find(delimiter, pos)-pos);
    pos += sideToMove.length()+1;
    std::string castlingAbillity = fen.substr(pos, fen.find(delimiter, pos)-pos);
    pos += castlingAbillity.length()+1;
    std::string enPassentTargetSquare = fen.substr(pos, fen.find(delimiter, pos)-pos);
    pos += enPassentTargetSquare.length()+1;
    std::string halfMoveClock = fen.substr(pos, fen.find(delimiter, pos)-pos);
    pos += halfMoveClock.length()+1;
    std::string fullMoveCounter = fen.substr(pos, fen.find(delimiter, pos)-pos);

    std::cout << "PiecePlacement: " << piecePlacement << std::endl;
    std::cout << "sideToMove    : " << sideToMove << std::endl;
    std::cout << "castlingAbillity: " << castlingAbillity << std::endl;
    std::cout << "en passent: " << enPassentTargetSquare << std::endl;
    std::cout << "halfMoveClock: " << halfMoveClock << std::endl;
    std::cout << "fullMoveCounter: " << fullMoveCounter << std::endl;

    initBoard();
}

void Board::printBoard(Bitboard board) {
    std::string boardAsString = std::to_string(board);
    std::cout << "Board: " << boardAsString << std::endl;
}

Bitboard Board::getOccupiedByWhite() {
    return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueen | whiteKing;
}

Bitboard Board::getOccupiedByBlack() {
    return blackPawns | blackRooks | blackKnights | blackBishops | blackQueen | blackKing;
}

Bitboard Board::getOccupied() {
    return getOccupiedByWhite() | getOccupiedByBlack(); 
}
