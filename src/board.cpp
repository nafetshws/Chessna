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

    //sepertes fen string by delimiter
    //position has to be update by the length of the previous token +1 (white space)
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

    this->whitePawns = 0ULL; 
    this->whiteRooks = 0ULL; 
    this->whiteKnights = 0ULL; 
    this->whiteBishops = 0ULL; 
    this->whiteQueen = 0ULL; 
    this->whiteKing = 0ULL;

    this->blackPawns = 0ULL; 
    this->blackRooks = 0ULL; 
    this->blackKnights = 0ULL;
    this->blackBishops = 0ULL;
    this->blackQueen = 0ULL; 
    this->blackKing = 0ULL;

    //convert piece placement string to bitboards
    int file = 1;
    int rank = 8;
    for(int i = 0; i < piecePlacement.length(); i++) {
        int currentChar = piecePlacement.at(i);
        Bitboard piece = 1ULL << ((rank-1)*8 + (file-1));

        switch(currentChar) {
            case 'r':
                this->blackRooks = (this->blackRooks ^ piece);
                file++;
                break;
            case 'n':
                this->blackKnights = (this->blackKnights ^ piece);
                file++;
                break;
            case 'b':
                this->blackBishops = (this->blackBishops ^ piece);
                file++;
                break;
            case 'q':
                this->blackQueen = (this->blackQueen ^ piece);
                file++;
                break;
            case 'k':
                this->blackKing = (this->blackKing ^ piece);
                file++;
                break;
            case 'p':
                this->blackPawns = (this->blackPawns ^ piece);
                file++;
                break;

            case 'R':
                this->whiteRooks = (this->whiteRooks ^ piece);
                file++;
                break;
            case 'N':
                this->whiteKnights = (this->whiteKnights ^ piece);
                file++;
                break;
            case 'B':
                this->whiteBishops = (this->whiteBishops ^ piece);
                file++;
                break;
            case 'Q':
                this->whiteQueen = (this->whiteQueen ^ piece);
                file++;
                break;
            case 'K':
                this->whiteKing = (this->whiteKing ^ piece);
                file++;
                break;
            case 'P':
                this->whitePawns = (this->whitePawns ^ piece);
                file++;
                break;

            case '1':
                file += 1;
                break;
            case '2':
                file += 2;
                break;
            case '3':
                file += 3;
                break;
            case '4':
                file += 4;
                break;
            case '5':
                file += 5;
                break;
            case '6':
                file += 6;
                break;
            case '7':
                file += 7;
                break;
            case '8':
                file += 8;
                break;
            
            case '/':
                rank--;
                file = 1;
                break;
            default:
                break;
            
        }
    }

    this->sideToMove = sideToMove;
    this->castlingAbillity = castlingAbillity;
    this->enPassentTargetSquare = enPassentTargetSquare;
    this->halfMoveClock = stoi(halfMoveClock);
    this->fullMoveCounter = stoi(fullMoveCounter);
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
