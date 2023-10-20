#include <iostream>
#include <vector>
#include "../include/moveOrder.hpp"
#include "../include/board.hpp"
#include "../include/functions.hpp"

void MoveOrder::orderMoves(Board board, std::vector<Move>& moves, Move prevBestMove) {
    std::vector<int> moveScores;
    for(int i = 0; i < moves.size(); i++) {
        Move move = moves.at(i);
        int score = 0;

        if(move.checkIfEqual(prevBestMove)) {
            score = BEST_MOVE_SCORE;
            moveScores.push_back(score); 
            continue;
        }

        if(move.moveType == QUIET) {
            moveScores.push_back(score);
            continue;
        };

        //MVV-LVA (Most valuable victim - Least valuable aggressor) - Captures
        if(move.moveType == CAPTURE) {
            PieceType attackedPieceType = board.findPiece(move.destination).type;
            PieceType attackerPieceType = move.pieceType;
            int materialDifference = getPieceTypeValue(attackedPieceType) - getPieceTypeValue(attackerPieceType);

            score += materialDifference;
        }

        //Other adventagous moves
        if(move.moveType == QUEEN_PROMOTION) {
            score += 800; 
        } else if(move.moveType == KNIGHT_PROMOTION) {
            score += 220; 
        } else if(move.moveType == KING_CASTLE) {
            score += 80;
        } else if(move.moveType == QUEEN_CASTLE) {
            score += 75;
        } else if(move.moveType == CAPTURE_QUEEN_PROMOTION) {
            score += 1000;
        } else if(move.moveType == CAPTURE_KNIGHT_PROMOTION) {
            score += 300;
        } 

        moveScores.push_back(score); 
    }

    this->quickSort(moveScores, moves, 0, moves.size()-1);
}

void MoveOrder::quickSort(std::vector<int>& moveScores, std::vector<Move>& moves, int low, int high) {
    if(low < high) {
        int partitionIndex = this->partition(moveScores, moves, low, high);
        this->quickSort(moveScores, moves, low, partitionIndex - 1);
        this->quickSort(moveScores, moves, partitionIndex+1, high);
    }

}

int MoveOrder::partition(std::vector<int>& moveScores, std::vector<Move>& moves, int low, int high) {
    int i = (low - 1);
    int pivot = moveScores.at(high);;

    for(int j = low; j < high; j++) {
        if(moveScores.at(j) > pivot) {
            i++;
            this->swap(&moveScores.at(i), &moveScores.at(j));
            this->swap(&moves.at(i), &moves.at(j));
        }
    }
    //swap pivot with i+1
    this->swap(&moveScores.at(i+1), &moveScores.at(high));
    this->swap(&moves.at(i+1), &moves.at(high));
    return i+1;
}

void MoveOrder::swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
} 

void MoveOrder::swap(Move* a, Move* b) {
    Move t = *a;
    *a = *b;
    *b = t;
} 

