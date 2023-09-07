#include <iostream>

#include "../include/board.hpp"
#include "../include/gameInterface.hpp"
#include "../include/types.hpp"
#include "../include/search.hpp"
#include "../include/functions.hpp"

GameInterface::GameInterface() {
    this->board = Board(DEFAULT_FEN);
    this->isPlaying = false;
}

GameInterface::GameInterface(std::string fen) {
    this->board = Board(fen);
    this->isPlaying = false;
}

void GameInterface::play(Color playerColor) {
    this->isPlaying = true;
    std::string moveNotation;

    while(this->isPlaying) {
        this->board.prettyPrintBoard();
        std::cout << "\nYour move: ";
        std::cin >> moveNotation;
        //make player move
        this->playMove(moveNotation);
        //make engine move
        Move engineMove = this->getBestEngineMove();
        std::cout << "Engine played: " << printableMove(engineMove) << std::endl << std::endl;
        this->board.makeMove(engineMove);
    }
}

Move GameInterface::getBestEngineMove() {
    Search search(this->board);
    int depth = 6;
    int eval = search.alphaBeta(negativeInfinity, positiveInfinity, 6, 0);
    Move engineMove = search.bestMove;
    return engineMove;
}

void GameInterface::playMove(std::string moveNotation) {
    Square origin = convertCoordinateToSquare(moveNotation.substr(0, 2));
    Square destination = convertCoordinateToSquare(moveNotation.substr(2, 2));
    Piece piece = this->board.findPiece(origin);

    MoveType moveType;
    Bitboard capture = this->board.getOccupied() & squareToBitboard(destination);

    //check for promotion
    if(moveNotation.size() == 5) {
        switch(moveNotation.at(4)) {
            case 'q':
                moveType = (capture == EMPTY) ? QUEEN_PROMOTION : CAPTURE_QUEEN_PROMOTION;
                break;
            case 'r':
                moveType = (capture == EMPTY) ? ROOK_PROMOTION : CAPTURE_ROOK_PROMOTION;
                break;
            case 'n':
                moveType = (capture == EMPTY) ? KNIGHT_PROMOTION : CAPTURE_KNIGHT_PROMOTION;
                break;
            case 'b':
                moveType = (capture == EMPTY) ? BISHOP_PROMOTION : CAPTURE_BISHOP_PROMOTION;
                break;
            default:
                break;
        }
    } else {
        if(capture != EMPTY) moveType = CAPTURE;
        else if(piece.type == KING && (destination - origin) == 2) moveType = KING_CASTLE; 
        else if(piece.type == KING && (destination - origin) == -2) moveType = QUEEN_CASTLE; 
        else if(piece.type == PAWN && std::abs(destination - origin) == 16) moveType = DOUBLE_PAWN_PUSH;
        else if(piece.type == PAWN && std::abs(destination - origin) != 8) moveType = EN_PASSENT_CAPTURE;
        else moveType = QUIET;
    }

    Move move(origin, destination, piece.type, this->board.sideToMove, moveType);

    this->board.makeMove(move);
}