#include <iostream>
#include <string>
#include <vector>
#include "../include/board.hpp"
#include "../include/types.hpp"
#include "../include/functions.hpp"

Board::Board(const std::string &fen) {
    initBoard(fen);
}

Board::Board() {
    initBoard(DEFAULT_FEN);
}

void Board::initBoard(const std::string &fen){

    std::string delimiter = " ";
    int pos = 0;

    //seperates fen string by delimiter
    //position has to be update by the length of the previous token +1 (white space)
    std::string piecePlacement = fen.substr(pos, fen.find(delimiter, pos));
    pos += piecePlacement.length()+1;
    std::string color = fen.substr(pos, fen.find(delimiter, pos)-pos);
    pos += color.length()+1;
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
    this->sideToMove = (color == "w") ? WHITE : BLACK;
    this->castlingAbillity = castlingAbillity;
    this->halfMoveClock = stoi(halfMoveClock);
    this->fullMoveCounter = stoi(fullMoveCounter);

}

void Board::makeMove(Move move) {
    Bitboard *pieces; 
    Bitboard t = move.moveType;

    switch(move.pieceType) {
        case PAWN:
            if(move.color == WHITE) pieces = &this->whitePawns;
            else pieces = &this->blackPawns; 
            break;
        case ROOK:
            if(move.color == WHITE) pieces = &this->whiteRooks;
            else pieces = &this->blackRooks; 
            break;
        case KNIGHT:
            if(move.color == WHITE) pieces = &this->whiteKnights;
            else pieces = &this->blackKnights; 
            break;
        case BISHOP:
            if(move.color == WHITE) pieces = &this->whiteBishops;
            else pieces = &this->blackBishops; 
            break;
        case QUEEN:
            if(move.color == WHITE) pieces = &this->whiteQueen;
            else pieces = &this->blackQueen; 
            break;
        case KING:
            if(move.color == WHITE) pieces = &this->whiteKing;
            else pieces = &this->blackKing; 
            break;
        default:
            break;
    }

    switch(move.moveType) {
        case QUIET: {
            *pieces = (*pieces & (~squareToBitboard(move.origin))) | squareToBitboard(move.destination);
            this->enPassentTargetSquare = -1;
            break;
        }
        case CAPTURE: {
            Piece target = findPiece(move.destination);
            switch(target.type) {
                case PAWN:
                    if(target.color == WHITE) this->whitePawns ^= squareToBitboard(move.destination);
                    else this->blackPawns ^= squareToBitboard(move.destination);
                    break;
                case ROOK:
                    if(target.color == WHITE) this->whiteRooks ^= squareToBitboard(move.destination);
                    else this->blackRooks ^= squareToBitboard(move.destination);
                    break;
                case KNIGHT:
                    if(target.color == WHITE) this->whiteKnights ^= squareToBitboard(move.destination);
                    else this->blackKnights ^= squareToBitboard(move.destination);
                    break;
                case BISHOP:
                    if(target.color == WHITE) this->whiteBishops ^= squareToBitboard(move.destination);
                    else this->blackBishops ^= squareToBitboard(move.destination);
                    break;
                case QUEEN:
                    if(target.color == WHITE) this->whiteQueen ^= squareToBitboard(move.destination);
                    else this->blackQueen ^= squareToBitboard(move.destination);
                    break;
                default:
                    break;
            }

            *pieces = *pieces & (~squareToBitboard(move.origin)) | squareToBitboard(move.destination);
            this->enPassentTargetSquare = -1;
            break;
        }
        case DOUBLE_PAWN_PUSH: {
            Bitboard bDestination = squareToBitboard(move.destination);
            if(move.color == WHITE) {
                Square enPassentSquare = bitboardToSquare(bDestination >> SOUTH);
                this->enPassentTargetSquare = enPassentSquare;
            } else {
                Square enPassentSquare = bitboardToSquare(bDestination << NORTH);
                this->enPassentTargetSquare = enPassentSquare;
            }
            *pieces = *pieces & (~squareToBitboard(move.origin)) | squareToBitboard(move.destination);

            break;
        }
        case KING_CASTLE: {
            this->castleKingSide(move.color);
            this->removeKingSideCastleAbillity(move.color);
            this->enPassentTargetSquare = -1;
            break;
        }
        case QUEEN_CASTLE: {
            this->castleQueenSide(move.color);
            this->removeQueenSideCastleAbillity(move.color);
            this->enPassentTargetSquare = -1;
            break;
        }
        case QUEEN_PROMOTION: {
            this->makeQueenPromotion(move);
            this->enPassentTargetSquare = -1;
            break;
        }
        case EN_PASSENT_CAPTURE: {
            *pieces = *pieces & (~squareToBitboard(move.origin)) | squareToBitboard(move.destination);
            if(move.color == WHITE) {
                Bitboard enPassentSquare = squareToBitboard(this->enPassentTargetSquare);
                Bitboard target = enPassentSquare >> SOUTH;
                this->blackPawns &= ~target;
            } else {
                Bitboard enPassentSquare = squareToBitboard(this->enPassentTargetSquare);
                Bitboard target = enPassentSquare << NORTH;
                this->whitePawns &= ~target;
            }

            this->enPassentTargetSquare = -1;
            break;
        }
        case KNIGHT_PROMOTION: {
            this->makeKnightPromotion(move);
            this->enPassentTargetSquare = -1;
            break;
        }
        case ROOK_PROMOTION: {
            this->makeRookPromotion(move);
            this->enPassentTargetSquare = -1;
            break;
        }
        case BISHOP_PROMOTION: {
            this->makeBishopPromotion(move);
            this->enPassentTargetSquare = -1;
            break;
        }
        case CAPTURE_QUEEN_PROMOTION: CAPTURE_ROOK_PROMOTION: CAPTURE_KNIGHT_PROMOTION: CAPTURE_BISHOP_PROMOTION: {
            Piece target = findPiece(move.destination);
            switch(target.type) {
                case PAWN:
                    if(target.color == WHITE) this->whitePawns ^= squareToBitboard(move.destination);
                    else this->blackPawns ^= squareToBitboard(move.destination);
                    break;
                case ROOK:
                    if(target.color == WHITE) this->whiteRooks ^= squareToBitboard(move.destination);
                    else this->blackRooks ^= squareToBitboard(move.destination);
                    break;
                case KNIGHT:
                    if(target.color == WHITE) this->whiteKnights ^= squareToBitboard(move.destination);
                    else this->blackKnights ^= squareToBitboard(move.destination);
                    break;
                case BISHOP:
                    if(target.color == WHITE) this->whiteBishops ^= squareToBitboard(move.destination);
                    else this->blackBishops ^= squareToBitboard(move.destination);
                    break;
                case QUEEN:
                    if(target.color == WHITE) this->whiteQueen ^= squareToBitboard(move.destination);
                    else this->blackQueen ^= squareToBitboard(move.destination);
                    break;
                default:
                    break;
            }
            this->enPassentTargetSquare = -1;

            switch(move.moveType) {
                case CAPTURE_BISHOP_PROMOTION:
                    this->makeBishopPromotion(move);
                    break;
                case CAPTURE_KNIGHT_PROMOTION:
                    this->makeKnightPromotion(move);
                    break;
                case CAPTURE_ROOK_PROMOTION:
                    this->makeRookPromotion(move);
                    break;
                case CAPTURE_QUEEN_PROMOTION:
                    this->makeQueenPromotion(move);
                    break;
                default:
                    break;
            }

            break;
        }
        default:
            break;
    }

    if(move.pieceType == ROOK) {
        if(move.color == WHITE) {
            if(squareToBitboard(move.origin) == A1) this->removeQueenSideCastleAbillity(move.color);
            else if(squareToBitboard(move.origin) == H1) this->removeKingSideCastleAbillity(move.color);
        } else {
            if(squareToBitboard(move.origin) == A8) this->removeQueenSideCastleAbillity(move.color);
            else if(squareToBitboard(move.origin) == H8) this->removeKingSideCastleAbillity(move.color);
        }
    } else if(move.pieceType == KING) {
        //remove casting ability
        this->removeKingSideCastleAbillity(move.color);
        this->removeQueenSideCastleAbillity(move.color);
    }

    //switch side to move
    this->sideToMove = getOppositeColor(this->sideToMove);
    //set half move counter
    MoveType mt = move.moveType;
    if(move.pieceType == PAWN || mt == CAPTURE || mt == EN_PASSENT_CAPTURE || mt == KING_CASTLE || mt == QUEEN_CASTLE) this->halfMoveClock = 0;
    else halfMoveClock++;
    //set full move counter
    if(move.color == BLACK) fullMoveCounter++;
}

Piece Board::findPiece(Square s) {
    Bitboard position = squareToBitboard(s);
    Color color;
    PieceType pieceType = UNOCCUPIED;
    
    if((position & this->getOccupiedBy(WHITE)) != 0) color = WHITE;
    else color = BLACK;

    if((this->getPawns(color) & position) != 0) pieceType = PAWN;
    if((this->getRooks(color) & position) != 0) pieceType = ROOK;
    if((this->getKnights(color) & position) != 0) pieceType = KNIGHT;
    if((this->getBishops(color) & position) != 0) pieceType = BISHOP;
    if((this->getQueens(color) & position) != 0) pieceType = QUEEN;
    if((this->getKing(color) & position) != 0) pieceType = KING;

    return Piece(s, pieceType, color);
}

bool Board::getKingSideCastleAbility(Color color) {
    if(color == WHITE) {
        return (this->castlingAbillity.find('K') != std::string::npos) && (this->whiteRooks & H1) != 0;
    } else {
        return (this->castlingAbillity.find('k') != std::string::npos) && (this->blackRooks & H8) != 0;
    }
}
bool Board::getQueenSideCastleAbility(Color color) {
    if(color == WHITE) {
        return (this->castlingAbillity.find('Q') != std::string::npos) && (this->whiteRooks & A1) != 0;
    } else {
        return (this->castlingAbillity.find('q') != std::string::npos) && (this->blackRooks & A8) != 0;
    }
}

void Board::castleKingSide(Color color) {
    if(color == WHITE) {
        //check if castling is still available
        //if(this->castlingAbillity.find('K') != std::string::npos) return;
        //move king
        this->whiteKing = (this->whiteKing << 2);
        //move rook
        this->whiteRooks -= (squareToBitboard(7)) - (squareToBitboard(5));
        //this->whiteRooks = (this->whiteRooks - squareToBitboard(7)) + squareToBitboard(5);
    } else {
        //check if castling is still available
        //if(this->castlingAbillity.find('k') != std::string::npos) return;
        //move king
        this->blackKing = (this->blackKing << 2);
        //move rook
        this->blackRooks -= (squareToBitboard(63)) - (squareToBitboard(61));
    }
}

void Board::castleQueenSide(Color color) {
    if(color == WHITE) {
        //check if castling is still available
        //if(this->castlingAbillity.find('Q') != std::string::npos) return;
        //move king
        this->whiteKing = (this->whiteKing >> 2);
        //move rook
        this->whiteRooks += (1ULL << 3)-1;
    } else {
        //check if castling is still available
        //if(this->castlingAbillity.find('q') != std::string::npos) return;
        //move king
        this->blackKing = (this->blackKing >> 2);
        //move rook
        this->blackRooks += (1ULL << 59) - (1ULL << 56);
    }
}

void Board::removeKingSideCastleAbillity(Color color) {
    char charToremove = (color == WHITE) ? 'K' : 'k';
    this->castlingAbillity.erase(std::remove(this->castlingAbillity.begin(), this->castlingAbillity.end(), charToremove));
}

void Board::removeQueenSideCastleAbillity(Color color) {
    char charToremove = (color == WHITE) ? 'Q' : 'q';
    this->castlingAbillity.erase(std::remove(this->castlingAbillity.begin(), this->castlingAbillity.end(), charToremove));
}


void Board::makeRookPromotion(Move move) {
    if(move.color == WHITE) {
        this->whitePawns &= ~(squareToBitboard(move.origin));
        this->whiteRooks |= squareToBitboard(move.destination);
    } else {
        this->blackPawns &= ~(squareToBitboard(move.origin));
        this->blackRooks |= squareToBitboard(move.destination);
    }
}

void Board::makeBishopPromotion(Move move) {
    if(move.color == WHITE) {
        this->whitePawns &= ~(squareToBitboard(move.origin));
        this->whiteBishops |= squareToBitboard(move.destination);
    } else {
        this->blackPawns &= ~(squareToBitboard(move.origin));
        this->blackBishops |= squareToBitboard(move.destination);
    }
}

void Board::makeKnightPromotion(Move move) {
    if(move.color == WHITE) {
        this->whitePawns &= ~(squareToBitboard(move.origin));
        this->whiteKnights |= squareToBitboard(move.destination);
    } else {
        this->blackPawns &= ~(squareToBitboard(move.origin));
        this->blackKnights |= squareToBitboard(move.destination);
    }
}

void Board::makeQueenPromotion(Move move) {
    if(move.color == WHITE) {
        this->whitePawns &= ~(squareToBitboard(move.origin));
        this->whiteQueen |= squareToBitboard(move.destination);
    } else {
        this->blackPawns &= ~(squareToBitboard(move.origin));
        this->blackQueen |= squareToBitboard(move.destination);
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