#include <string>
#include <iostream>
#include <chrono>
#include "../include/functions.hpp"
#include "../include/types.hpp"
#include "../include/evaluation.hpp"


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

Square convertCoordinateToSquare(std::string coordinate) {
    char file = coordinate.at(0);
    int row = std::stoi(std::string(1, coordinate.at(1)));

    int square = (row - 1) * 8;

    switch(file) {
        case 'a':
            square += 0;
            break;
        case 'b':
            square += 1;
            break;
        case 'c':
            square += 2;
            break;
        case 'd':
            square += 3;
            break;
        case 'e':
            square += 4;
            break;
        case 'f':
            square += 5;
            break;
        case 'g':
            square += 6;
            break;
        case 'h':
            square += 7;
            break;
        default:
            return -1;
            break;
    }
    return square;
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

std::string printableMove(Move move) {
    std::string stringMove =  convertSquareToCoordinate(move.origin) + convertSquareToCoordinate(move.destination);
    MoveType t = move.moveType;
    if(t == QUEEN_PROMOTION || t == CAPTURE_QUEEN_PROMOTION) {
        stringMove += "q";
    } else if(t == ROOK_PROMOTION || t == CAPTURE_ROOK_PROMOTION) {
        stringMove += "r";
    } else if(t == KNIGHT_PROMOTION || t == CAPTURE_KNIGHT_PROMOTION) {
        stringMove += "n";
    } else if(t == BISHOP_PROMOTION || t == CAPTURE_BISHOP_PROMOTION) {
        stringMove += "b";
    }
    return stringMove;
}

Bitboard squareToBitboard(Square square) {
    return 1ULL << square;
}

Square bitboardToSquare(Bitboard b) {
    return __builtin_ctzll(b);
}

std::vector<Square> convertBitboardToSquares(Bitboard bitboard) {
    std::vector<Square> squares;
    while(bitboard != 0){
        Square index = __builtin_ctzll(bitboard);
        squares.push_back(index);
        //remove index from bitboard
        bitboard = (~squareToBitboard(index)) & bitboard;
    }
    return squares;
}

void convertBitbaordToMoves(Bitboard intersect, Bitboard destination, PieceType pieceType, Color color, Bitboard occupied, int &numberOfAttacks, Attack_Info &attack_info){
    if(intersect != 0) {
        std::vector<Square> origins = convertBitboardToSquares(intersect);
        for(int i = 0; i < origins.size(); i++) {
            Bitboard promotionRank = (color == WHITE) ? RANK_8 : RANK_1;
            if(pieceType == PieceType::PAWN && (destination & promotionRank) != 0) {
                if((destination & occupied) != 0) {
                    //capture promotion
                    attack_info.moves.push_back(Move(origins.at(i), (Square)__builtin_ctzll(destination), pieceType, color, MoveType::CAPTURE_BISHOP_PROMOTION));
                    attack_info.moves.push_back(Move(origins.at(i), (Square)__builtin_ctzll(destination), pieceType, color, MoveType::CAPTURE_KNIGHT_PROMOTION));
                    attack_info.moves.push_back(Move(origins.at(i), (Square)__builtin_ctzll(destination), pieceType, color, MoveType::CAPTURE_ROOK_PROMOTION));
                    attack_info.moves.push_back(Move(origins.at(i), (Square)__builtin_ctzll(destination), pieceType, color, MoveType::CAPTURE_QUEEN_PROMOTION));
                } else {
                    //normal promotion
                    attack_info.moves.push_back(Move(origins.at(i), (Square)__builtin_ctzll(destination), pieceType, color, MoveType::BISHOP_PROMOTION));
                    attack_info.moves.push_back(Move(origins.at(i), (Square)__builtin_ctzll(destination), pieceType, color, MoveType::KNIGHT_PROMOTION));
                    attack_info.moves.push_back(Move(origins.at(i), (Square)__builtin_ctzll(destination), pieceType, color, MoveType::ROOK_PROMOTION));
                    attack_info.moves.push_back(Move(origins.at(i), (Square)__builtin_ctzll(destination), pieceType, color, MoveType::QUEEN_PROMOTION));
                }
                //+4 malfunctions -> probably because 4 attacks mean the king has to move
                numberOfAttacks++; 
            } else {
                MoveType moveType = (destination & occupied) != 0 ? MoveType::CAPTURE : MoveType::QUIET;
                Move move(origins.at(i), (Square)__builtin_ctzll(destination), pieceType, color, moveType);
                numberOfAttacks++;
                attack_info.moves.push_back(move);
            }
        }
    }
}

int getPieceTypeValue(PieceType pieceType) {
    switch(pieceType) {
        case KING:
            return KING_VALUE;
        case QUEEN:
            return QUEEN_VALUE;
        case ROOK:
            return ROOK_VALUE;
        case BISHOP:
            return BISHOP_VAUE;
        case KNIGHT:
            return KNIGHT_VALUE;
        case PAWN:
            return PAWN_VALUE;
        default:
            return 0;
    }
}

u64 getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

float getTimeDifference(u64 before, u64 after) {
    return ((after - before)/(float)1000000000);
}

int popcount(Bitboard bitboard) {
    return std::__libcpp_popcount(bitboard);
}
