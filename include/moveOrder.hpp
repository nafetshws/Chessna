#ifndef MOVE_ORDER_H
#define MOVE_ORDER_H

#include <vector>
#include <map>
#include <array>
#include "../include/types.hpp"
#include "../include/board.hpp"

const int KILLER_SCORE_1 = 150;
const int KILLER_SCORE_2 = 145;

class MoveOrder {
    public:
        //std::array<std::array<Move, 256>, 2> killerMoves;
        std::array<std::map<int, Killermove>, 2> killerMoves;

        void orderMoves(Board board, std::vector<Move>& moves, Move prevBestMove, int depthFromRoot); 
        void quickSort(std::vector<int>& moveScores, std::vector<Move>& moves, int low, int high); 

        void insertKillerMove(Killermove killerMove, int depthFromRoot);
        int getKillerMoveScore(Move &move, Killermove &k1, Killermove &k2);

    private:
        int partition(std::vector<int>& moveScores, std::vector<Move>& moves, int low, int high);
        void swap(int* a, int* b);
        void swap(Move* a, Move* b);

};

#endif