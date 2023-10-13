#include <iostream>

#include "../include/board.hpp"
#include "../include/gameInterface.hpp"
#include "../include/types.hpp"
#include "../include/search.hpp"
#include "../include/functions.hpp"
#include "../include/evaluation.hpp"

GameInterface::GameInterface() {
    this->board = Board(DEFAULT_FEN);
    this->isPlaying = false;
    this->lastMinDepth = 0;
}

GameInterface::GameInterface(std::string fen) {
    this->board = Board(fen);
    this->isPlaying = false;
    this->lastMinDepth = 0;
}

void GameInterface::play(Color playerColor) {
    this->isPlaying = true;
    std::string moveNotation;
    MoveGeneration mg;

    while(this->isPlaying) {
        if(this->getGameStatus() != ACTIVE) {
            this->isPlaying = false;
            break;
        }

        //this->board.prettyPrintBoard();

        if(this->board.sideToMove == playerColor) {
            std::cout << "Your move: ";
            std::cin >> moveNotation;
            //make player move
            if(this->moveIsLegal(moveNotation, playerColor)) {
                this->playMove(moveNotation);
            } else {
                std::cout << "\nError: Invalid move. Try again!\n" << std::endl;
                continue;
            }
        } else {
            //make engine move
            Move engineMove = this->getBestEngineMove();
            std::cout << "Engine played (" << this->lastMinDepth << "): " << printableMove(engineMove) << std::endl;
            this->board.makeMove(engineMove);
        }
    }

    GameStatus g = this->getGameStatus();
    if(g == CHECKMATE) {
        std::cout << "\nCheckmate!" << std::endl;
    } else {
        std::cout << "\nDraw!" << std::endl;
    }
}

Move GameInterface::getBestEngineMove() {
    Search search(this->board);
    float time = 30;
    search.iterativeDeepening(time);
    this->lastMinDepth = search.minDepth;
    Move engineMove = search.bestMove;
    return engineMove;
}

GameStatus GameInterface::getGameStatus() {
    MoveGeneration mg;
    std::vector<Move> moves = mg.generateMoves(this->board, this->board.sideToMove);

    //check for draw / stalemate
    if(moves.size() == 0) {
        if(mg.check_info.numberOfChecks != 0) {
            //checkmate
            return CHECKMATE;
        } else {
            //stalemate
            return STALEMATE;
        }
    } else if(this->board.halfMoveClock >= 100) {
        return DRAW_BY_FIFTY_MOVE_RULE;
    } 

    Bitboard minorPiecesWhite = this->board.getBishops(WHITE) | this->board.getKnights(WHITE);
    Bitboard minorPiecesBlack = this->board.getBishops(BLACK) | this->board.getKnights(BLACK);

    if(popcount(minorPiecesWhite) <= 1 && popcount(minorPiecesBlack) <= 1 &&
        (this->board.getKing(WHITE) | minorPiecesWhite) == this->board.getOccupiedBy(WHITE) &&
        (this->board.getKing(BLACK) | minorPiecesBlack) == this->board.getOccupiedBy(BLACK)) {
            return DRAW_BY_INSUFFICIENT_MATERIAL;
    }

   return ACTIVE; 
}

bool GameInterface::moveIsLegal(std::string moveNotation, Color playerColor) {
    MoveGeneration mg;
    std::vector<Move> moves = mg.generateMoves(this->board, playerColor);
    bool moveIsLegal = false;

    for(Move move : moves) {
        std::string coords = (convertSquareToCoordinate(move.origin) + convertSquareToCoordinate(move.destination));
        if(moveNotation.size() == 4 && moveNotation.compare(coords) == 0) {
            moveIsLegal = true;
        } else if(moveNotation.size() == 5) {
            char promPiece = moveNotation.at(4);        
            if(moveNotation.substr(0, 4).compare(coords) == 0
                && promPiece == 'q' || promPiece == 'r'
                || promPiece == 'n' || promPiece == 'b') {
                    moveIsLegal = true;
                }
        }
    }

    return moveIsLegal;
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
    
    //check if move is legal



    Move move(origin, destination, piece.type, this->board.sideToMove, moveType);

    this->board.makeMove(move);
}