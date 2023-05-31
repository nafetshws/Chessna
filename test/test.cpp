#include <iostream>
#include <string>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

int counter = 1;

#define IS_TRUE(value) { \
    if(!(value)) { \
        std::cout << "Test " << counter << ": " << __FUNCTION__ << RED << " FAILED on line " << __LINE__ << RESET << "." << std::endl; \
    } else { \
        std::cout << "Test " << counter << ": " << __FUNCTION__ << GREEN << " PASSED" << RESET << "." << std::endl; \
    } \
    counter++; \
}

#define IS_EQUAL(test_value, correct_value) { \
    if (test_value != correct_value) { \
        std::cout << "Test " << counter << ": " << __FUNCTION__ << RED << " FAILED on line " << __LINE__ << RESET << ". Expected " <<  correct_value << " but recieved " << test_value << "." << std::endl; \
    } else { \
        std::cout << "Test " << counter << ": " << __FUNCTION__ << GREEN << " PASSED" << RESET << "." << std::endl; \
    } \
    counter++; \
}

#define IS_SMALLER(test_value, correct_value) { \
    if (test_value >= correct_value) { \
        std::cout << "Test " << counter << ": " << __FUNCTION__ << RED << " FAILED on line " << __LINE__ << RESET << ". Expected the value to be smaller than " <<  correct_value << " but recieved " << test_value << "." << std::endl; \
    } else { \
        std::cout << "Test " << counter << ": " << __FUNCTION__ << GREEN << " PASSED" << RESET << "." << std::endl; \
    } \
    counter++; \
}

#define IS_GREATER(test_value, correct_value) { \
    if (test_value <= correct_value) { \
        std::cout << "Test " << counter << ": " << __FUNCTION__ << RED << " FAILED on line " << __LINE__ << RESET << ". Expected the value to be greater than " <<  correct_value << " but recieved " << test_value << "." << std::endl; \
    } else { \
        std::cout << "Test " << counter << ": " << __FUNCTION__ << GREEN << " PASSED" << RESET << "." << std::endl; \
    } \
    counter++; \
}

//Move Generation

void test_generateRookMoves() {
    MoveGeneration moveGeneration(Board("rnbqkb1r/ppp1p1pp/2n2p2/3p4/1R1N4/5R2/PPPPPPPP/2BQKBN1 w kq - 0 1"));
    IS_EQUAL(moveGeneration.generateRookMoves(CURRENT_POSITION, WHITE), 600480013025280);
} 
void test_generateBishopMoves() {
    MoveGeneration moveGeneration(Board("rnbqkb1r/ppp1p1pp/2n2p2/3pB3/1R1N4/3B1R2/PPPPPPPP/3QK1N1 w kq - 0 1"));
    IS_EQUAL(moveGeneration.generateBishopMoves(CURRENT_POSITION, WHITE), 37270292552220672);
}
void test_generateQueenMoves() {
    MoveGeneration moveGeneration(Board("rnbqkb1r/ppp1p1pp/2n2p2/3QB3/1R1N4/3B1R2/PPPPPPPP/4K1N1 w kq - 0 1"));
    IS_EQUAL(moveGeneration.generateQueenMoves(CURRENT_POSITION, WHITE), 5199436586525261824);
}

void test_generatePawnMoves() {
    //including en passent
    MoveGeneration moveGeneration(Board("r1b1k2r/p1p1p1pp/2n2p2/1pPQB1b1/nR1N1q1P/1P1B1RP1/P1PP1P2/4K1N1 w kq b6 0 1"));
    IS_EQUAL(moveGeneration.generatePawnMoves(CURRENT_POSITION, WHITE), 3025351802880);
}

void test_generateKnightMoves() {
    MoveGeneration moveGeneration(Board("r1b1k2r/p1p1p1pp/2n2p2/1pPQBqb1/nR1N2NP/1P1B1RP1/P1PP1P2/4K3 w kq b6 0 1"));
    IS_EQUAL(moveGeneration.generateKnightMoves(CURRENT_POSITION, WHITE), 198058122973184);
}
void test_generateKingMoves() {
    MoveGeneration moveGeneration(Board("rnbqkbnr/pppppppp/8/8/2B1P3/N2PBN2/PPP1QPPP/R3K2R w KQkq - 0 1"));
    IS_EQUAL(moveGeneration.generateKingMoves(CURRENT_POSITION, WHITE), 2156);
}

void test_legalKingMoves() {
    MoveGeneration moveGeneration(Board("8/4k3/8/8/4R3/8/4K3/8 w - - 0 1"));
    IS_EQUAL(moveGeneration.generateKingMoves(CURRENT_POSITION, BLACK), 2893606741050654720);
}

void test_legalKingMoves2() {
    MoveGeneration moveGeneration(Board("2K5/4k3/8/8/2B1RB2/8/8/8 w - - 0 1"));
    IS_EQUAL(moveGeneration.generateKingMoves(CURRENT_POSITION, BLACK), 2305878193585782784);
}

void test_generateAttackedSquares() {
    MoveGeneration moveGeneration{Board()};
    IS_EQUAL(moveGeneration.generateAttackedSquares(WHITE), 16711680);
}



void test_moveGeneration() {
    test_generateRookMoves(); 
    test_generateBishopMoves();
    test_generateQueenMoves();

    test_generatePawnMoves();

    test_generateKnightMoves();
    test_generateKingMoves();
    test_legalKingMoves();
    test_legalKingMoves2();
    test_generateAttackedSquares();
}

void test_all() {
    test_moveGeneration();
}

int main() {
    test_all();
    return 0;
}