#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

Board::Board(std::string fen) {
    initBoard(fen);
}

Board::Board() {
    initBoard(DEFAULT_FEN);
}

void Board::initBoard(std::string fen){

    std::string delimiter = " ";
    int pos = 0;

    //seperates fen string by delimiter
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

    //set bitboards to 0 - somehow necessary
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

    switch (enPassentTargetSquare.at(0)) {
        case 'a':
            this->enPassentTargetSquare = (std::stoi(std::string(1, enPassentTargetSquare.at(1)))-1) * 8 + 0;
            break;
        case 'b':
            this->enPassentTargetSquare = (std::stoi(std::string(1, enPassentTargetSquare.at(1)))-1) * 8 + 1;
            break;
        case 'c':
            this->enPassentTargetSquare = (std::stoi(std::string(1, enPassentTargetSquare.at(1)))-1) * 8 + 2;
            break;
        case 'd':
            this->enPassentTargetSquare = (std::stoi(std::string(1, enPassentTargetSquare.at(1)))-1) * 8 + 3;
            break;
        case 'e':
            this->enPassentTargetSquare = (std::stoi(std::string(1, enPassentTargetSquare.at(1)))-1) * 8 + 4;
            break;
        case 'f':
            this->enPassentTargetSquare = (std::stoi(std::string(1, enPassentTargetSquare.at(1)))-1) * 8 + 5;
            break;
        case 'g':
            this->enPassentTargetSquare = (std::stoi(std::string(1, enPassentTargetSquare.at(1)))-1) * 8 + 6;
            break;
        case 'h':
            this->enPassentTargetSquare = (std::stoi(std::string(1, enPassentTargetSquare.at(1)))-1) * 8 + 7;
            break;
        default:
            //no en passent
            this->enPassentTargetSquare = -1;
            break;
    }

    //init rest of board state
    this->sideToMove = sideToMove;
    this->castlingAbillity = castlingAbillity;
    this->halfMoveClock = stoi(halfMoveClock);
    this->fullMoveCounter = stoi(fullMoveCounter);
}

bool Board::getKingSideCastleAbility(Color color) {
    if(color == WHITE) {
        return this->castlingAbillity.find('K') != std::string::npos;
    } else {
        return this->castlingAbillity.find('k') != std::string::npos;
    }
}
bool Board::getQueenSideCastleAbility(Color color) {
    if(color == WHITE) {
        return this->castlingAbillity.find('Q') != std::string::npos;
    } else {
        return this->castlingAbillity.find('q') != std::string::npos;
    }
}

void Board::castleKingSide(Color color) {
    if(color == WHITE) {
        //check if castling is still available
        if(this->castlingAbillity.find('K') != std::string::npos) return;
        //move king
        this->whiteKing = (this->whiteKing << 2);
        //move rook
        this->whiteRooks -= (1ULL << 7) - (1ULL << 5);
    } else {
        //check if castling is still available
        if(this->castlingAbillity.find('k') != std::string::npos) return;
        //move king
        this->blackKing = (this->blackKing << 2);
        //move rook
        this->blackRooks -= (1ULL << 63) - (1ULL << 61);
    }
}

void Board::castleQueenSide(Color color) {
    if(color == WHITE) {
        //check if castling is still available
        if(this->castlingAbillity.find('Q') != std::string::npos) return;
        //move king
        this->whiteKing = (this->whiteKing >> 2);
        //move rook
        this->whiteRooks += (1ULL << 3)-1;
    } else {
        //check if castling is still available
        if(this->castlingAbillity.find('q') != std::string::npos) return;
        //move king
        this->blackKing = (this->blackKing >> 2);
        //move rook
        this->blackRooks += (1ULL << 59) - (1ULL << 56);
    }
}

void Board::printBoard(Bitboard board) {
    std::string boardAsString = std::to_string(board);
    std::cout << "Board: " << boardAsString << std::endl;
}

void Board::prettyPrintBoard() {
    //initialize 2d vector filled with char 0
    std::vector< std::vector<char> > board (8, std::vector<char>(8, '0'));
    board[0][0] = 'b';

    //convert bitboards to 2d array
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++) {
            int index = row * 8 + col;
            Bitboard b = (1ULL << index);

            if(this->whitePawns & b) board[row][col] = 'P';
            else if(this->whiteRooks & b) board[row][col] = 'R';
            else if(this->whiteKnights & b) board[row][col] = 'N';
            else if(this->whiteBishops & b) board[row][col] = 'B';
            else if(this->whiteQueen & b) board[row][col] = 'Q';
            else if(this->whiteKing & b) board[row][col] = 'K';

            if(this->blackPawns & b) board[row][col] = 'p';
            else if(this->blackRooks & b) board[row][col] = 'r';
            else if(this->blackKnights & b) board[row][col] = 'n';
            else if(this->blackBishops & b) board[row][col] = 'b';
            else if(this->blackQueen & b) board[row][col] = 'q';
            else if(this->blackKing & b) board[row][col] = 'k';
        }
    }

    //print board
    for(int row = 7; row >= 0; row--) {
        std::cout << row+1 << "  ";
        for(int col = 0; col < 8; col++) {
            std::cout << board[row][col] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "   " << "a b c d e f g h" << std::endl;
}

Bitboard Board::getPawns(Color color) {
    return (color == WHITE) ? this->whitePawns : this->blackPawns;
}
Bitboard Board::getRooks(Color color) {
    return (color == WHITE) ? this->whiteRooks : this->blackRooks;
}
Bitboard Board::getKnights(Color color) {
    return (color == WHITE) ? this->whiteKnights : this->blackKnights;
}

Bitboard Board::getBishops(Color color) {
    return (color == WHITE) ? this->whiteBishops : this->blackBishops;
}
Bitboard Board::getQueens(Color color) {
    return (color == WHITE) ? this->whiteQueen : this->blackQueen;
}
Bitboard Board::getKing(Color color) {
    return (color == WHITE) ? this->whiteKing : this->blackKing;
}

Bitboard Board::getOccupiedByWhite() {
    return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueen | whiteKing;
}

Bitboard Board::getOccupiedByBlack() {
    return blackPawns | blackRooks | blackKnights | blackBishops | blackQueen | blackKing;
}

Bitboard Board::getOccupiedBy(Color color) {
    if(color == BLACK) {
        return getOccupiedByBlack();
    } else {
        return getOccupiedByWhite();
    }
}

Bitboard Board::getOccupied() {
    return getOccupiedByWhite() | getOccupiedByBlack(); 
}

Bitboard Board::getOccupiedWithoutWhiteKing() {
    return getOccupiedByBlack() | (getOccupiedByWhite() ^ whiteKing);
}

Bitboard Board::getOccupiedWithoutBlackKing() {
    return getOccupiedByWhite() | (getOccupiedByBlack() ^ blackKing);
}

PieceType Board::getPieceTypeOfSquare(Square square, Color color) {
    Bitboard b = squareToBitboard(square);
    return getPieceTypeOfSquare(b, color);
}

PieceType Board::getPieceTypeOfSquare(Bitboard b, Color color) {
    if((this->getPawns(color) & b) != 0) return PAWN;
    if((this->getBishops(color) & b) != 0) return BISHOP;
    if((this->getKnights(color) & b) != 0) return KNIGHT;
    if((this->getRooks(color) & b) != 0) return ROOK;
    if((this->getQueens(color) & b) != 0) return QUEEN;
    if((this->getKing(color) & b) != 0) return KING;
    return UNOCCUPIED; 
}