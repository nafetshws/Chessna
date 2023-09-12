#ifndef MOVE_ORDER_H
#define MOVE_ORDER_H

#include <vector>
#include "../include/types.hpp"
#include "../include/board.hpp"


class MoveOrder {
    public:
        void orderMoves(Board board, std::vector<Move>& moves); 
        void quickSort(std::vector<int>& moveScores, std::vector<Move>& moves, int low, int high); 

    private:
        int partition(std::vector<int>& moveScores, std::vector<Move>& moves, int low, int high);
        void swap(int* a, int* b);
        void swap(Move* a, Move* b);

};

#endif