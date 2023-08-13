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

void test_isInCheck() {
    MoveGeneration moveGeneration{Board("8/4k3/6N1/4Q3/4R3/8/4K3/8 w - - 0 1")};
    IS_EQUAL(moveGeneration.isInCheck(BLACK).numberOfChecks, 2);
}

void test_isInCheck2() {
    MoveGeneration moveGeneration{Board("8/4k3/8/2B3Q1/8/8/8/8 w - - 0 1")};
    IS_EQUAL(moveGeneration.isInCheck(BLACK).numberOfChecks, 2);
}

void test_isInCheck3() {
    MoveGeneration moveGeneration{Board("rnb1k2r/pppppppp/8/4q3/4r3/2bP1n2/PPP2PPP/RNBQKBNR w KQkq - 0 1")};
    IS_EQUAL(moveGeneration.isInCheck(WHITE).numberOfChecks, 3);
}

void test_moveGenerationInDoubleCheck() {
    MoveGeneration moveGeneration{Board("8/4k3/6N1/4Q3/4R3/8/4K3/8 w - - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(BLACK).size(), 3);
}

void test_moveGenerationInDoubleCheck2() {
    MoveGeneration moveGeneration{Board("rnb1k2r/pppppppp/8/4q3/4r3/2bP1n2/PPP2PPP/RNBQKBNR w KQkq - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(WHITE).size(), 0);
}

void test_moveGenerationInSinlgeCheck() {
    MoveGeneration moveGeneration{Board("rnb1k2r/pppppppp/8/8/8/2bP4/PPP2PPP/RNBQKBNR w KQkq - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(WHITE).size(), 6);
}

void test_moveGenerationInSinlgeCheck2() {
    MoveGeneration moveGeneration{Board("r1b1k2r/pppppppp/8/8/8/3P1n2/PPP2PPP/RNBQKBNR w KQkq - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(WHITE).size(), 4);
}

void test_moveGenerationInSinlgeCheck3() {
    MoveGeneration moveGeneration{Board("r1b1k2r/pppppppp/8/8/8/3P4/PPPp1PPP/RNBQKBNR w KQkq - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(WHITE).size(), 5);
}

void test_enPassentCheckEvasion(){
    MoveGeneration moveGeneration{Board("8/8/8/2k5/3Pp3/8/8/3K4 b - d3 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(BLACK).size(), 9);
}

void test_enPassentCheckEvasion2(){
    MoveGeneration moveGeneration{Board("k7/8/8/5Pp1/7K/8/8/8 w - g6 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(BLACK).size(), 5);
}

void test_enPassentCheckEvasion3(){
    MoveGeneration moveGeneration{Board("8/8/8/k7/2Pp4/8/8/3KQ3 b - c3 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(BLACK).size(), 4);
}

void test_getPinnedPieces(){
    Bitboard realValue = 11259067787902976;
    MoveGeneration moveGeneration{Board("4k3/3n1p2/8/1Q2r2B/8/8/8/2K1R3 b - - 0 1")};
    IS_EQUAL(moveGeneration.getPinnedPieces(BLACK).absolutePins, realValue);
}

void test_pinnedPiecesMoveGeneration() {
    MoveGeneration moveGeneration{Board("4k3/5p2/2b3B1/4r3/Q7/8/8/2K1R3 b - - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(BLACK).size(), 14);
}

void test_pinnedPiecesMoveGeneration2() {
    MoveGeneration moveGeneration{Board("6k1/8/3r4/8/1q1BN3/2P5/3KR2r/r7 w - - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(WHITE).size(), 15);
}

void test_pinnedPiecesMoveGeneration3() {
    MoveGeneration moveGeneration{Board("6k1/8/3r4/6b1/1q1BN3/2P5/3KR2r/r7 w - - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(WHITE).size(), 4);
}

//function to execute all move generation tests
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

    test_isInCheck();
    test_isInCheck2();
    test_isInCheck3();

    test_moveGenerationInDoubleCheck();
    test_moveGenerationInDoubleCheck2();

    test_moveGenerationInSinlgeCheck();
    test_moveGenerationInSinlgeCheck2();
    test_moveGenerationInSinlgeCheck3();

    test_enPassentCheckEvasion();
    test_enPassentCheckEvasion2();
    test_enPassentCheckEvasion3();

    test_getPinnedPieces();
    test_pinnedPiecesMoveGeneration();
    test_pinnedPiecesMoveGeneration2();
    test_pinnedPiecesMoveGeneration3();
}

//function to execute all tests
void test_all() {
    test_moveGeneration();
}

int main() {
    test_all();
    return 0;
}