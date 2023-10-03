#include <iostream>
#include <fstream>
#include <string>
#include "../include/board.hpp"
#include "../include/moveGeneration.hpp"
#include "../include/types.hpp"
#include "../include/moveOrder.hpp"
#include "../include/zobrist.hpp"
#include "../include/functions.hpp"

#define GREEN "\033[32m"
#define RED   "\033[31m"
#define RESET "\033[0m"

int counter = 1;
bool failed = false;

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
        failed = true; \
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
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, BLACK).size(), 3);
}

void test_moveGenerationInDoubleCheck2() {
    MoveGeneration moveGeneration{Board("rnb1k2r/pppppppp/8/4q3/4r3/2bP1n2/PPP2PPP/RNBQKBNR w KQkq - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, WHITE).size(), 0);
}

void test_moveGenerationInSinlgeCheck() {
    MoveGeneration moveGeneration{Board("rnb1k2r/pppppppp/8/8/8/2bP4/PPP2PPP/RNBQKBNR w KQkq - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, WHITE).size(), 6);
}

void test_moveGenerationInSinlgeCheck2() {
    MoveGeneration moveGeneration{Board("r1b1k2r/pppppppp/8/8/8/3P1n2/PPP2PPP/RNBQKBNR w KQkq - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, WHITE).size(), 4);
}

void test_moveGenerationInSinlgeCheck3() {
    MoveGeneration moveGeneration{Board("r1b1k2r/pppppppp/8/8/8/3P4/PPPp1PPP/RNBQKBNR w KQkq - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, WHITE).size(), 5);
}

void test_enPassentCheckEvasion(){
    MoveGeneration moveGeneration{Board("8/8/8/2k5/3Pp3/8/8/3K4 b - d3 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, BLACK).size(), 9);
}

void test_enPassentCheckEvasion2(){
    MoveGeneration moveGeneration{Board("k7/8/8/5Pp1/7K/8/8/8 w - g6 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, WHITE).size(), 6);
}

void test_enPassentCheckEvasion3(){
    MoveGeneration moveGeneration{Board("8/8/8/k7/2Pp4/8/8/3KQ3 b - c3 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, BLACK).size(), 4);
}

void test_getPinnedPieces(){
    Bitboard realValue = 11259067787902976;
    MoveGeneration moveGeneration{Board("4k3/3n1p2/8/1Q2r2B/8/8/8/2K1R3 b - - 0 1")};
    IS_EQUAL(moveGeneration.getPinnedPieces(BLACK).absolutePins, realValue);
}

void test_pinnedPiecesMoveGeneration() {
    MoveGeneration moveGeneration{Board("4k3/5p2/2b3B1/4r3/Q7/8/8/2K1R3 b - - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, BLACK).size(), 14);
}

void test_pinnedPiecesMoveGeneration2() {
    MoveGeneration moveGeneration{Board("6k1/8/3r4/6b1/1q1BN3/2P5/3KR2r/r7 w - - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, WHITE).size(), 3);
}


void test_pinnedPiecesMoveGeneration3() {
    MoveGeneration moveGeneration{Board("6k1/8/3r4/6b1/1q1BN3/2P5/3KR3/r7 w - - 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, WHITE).size(), 4);
}

void test_enPassent_discoveredCheck() {
    MoveGeneration moveGeneration{Board("8/8/8/8/k2Pp2Q/8/8/4K3 w - d3 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, BLACK).size(), 6);
}

void test_enPassent_discoveredCheck2() {
    MoveGeneration moveGeneration{Board("8/8/8/8/k1pPp2Q/8/8/4K3 w - d3 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, BLACK).size(), 9);
}

void test_enPassent_discoveredCheck3() {
    MoveGeneration moveGeneration{Board("5k2/8/8/K2Pp2r/8/8/8/8 w - d3 0 1")};
    IS_EQUAL(moveGeneration.generateMoves(moveGeneration.board, WHITE).size(), 6);
}

void testZobrist(std::string fen) {
    //Board board("r3k2r/pppq1pp1/2np1n1p/2b1p1B1/2B1P1b1/2NP1N1P/PPPQ1PP1/R3K2R w KQkq - 0 1");
    //Board board("4r3/3P4/k7/8/8/8/K3p3/5R2 w - - 0 1");
    Board board(fen);
    MoveGeneration mg(board);

    std::vector<Move> moves = mg.generateMoves(board, WHITE, false);

    for(Move move : moves) {
        Board copyBoard = board;
        std::cout << "Making move: " << printableMove(move) << std::endl; 
        board.makeMove(move);
        u64 zobristKeyAfterMove = board.zobristKey; 

        Zobrist z;
        u64 zobristKeyAfterCreation = z.createZobristKey(board);

        IS_EQUAL(zobristKeyAfterMove, zobristKeyAfterCreation);
        board = copyBoard;
    }
}

void runExtensiveZobristTest(int depth, Board board, MoveGeneration mg) {
    if(depth == 0 || failed == true) return;

    std::vector<Move> moves = mg.generateMoves(board, board.sideToMove, false); 

    for(int i = 0; i < moves.size(); i++) {
        Board copyBoard = board;
        board.makeMove(moves.at(i));
        u64 zobristKeyAfterMove = board.zobristKey; 

        Zobrist z;
        u64 zobristKeyAfterCreation = z.createZobristKey(board);

        IS_EQUAL(zobristKeyAfterMove, zobristKeyAfterCreation);

        if(failed) {
            std::cout << "**********************" << std::endl;
            std::cout << "Move: " << printableMove(moves.at(i)) << std::endl << std::endl;
            copyBoard.prettyPrintBoard();
            std::cout << "**********************" << std::endl;
            return;
        }

        runExtensiveZobristTest(depth-1, board, mg);
        //unmake move
        board = copyBoard;
    }
}

void extensiveZobristTest(std::string fen) {
    Board board(fen);
    MoveGeneration mg(board);

    runExtensiveZobristTest(5, board, mg);
}

void perft_test(std::string fen, u64 expectedRes, int depth) {
    Board board(fen);
    MoveGeneration mG(board);

    Color color = board.sideToMove;
    std::cout << "testing fen: " << fen << std::endl;
    //disable console output
    std::cout.setstate(std::ios_base::failbit);
    u64 nodes = mG.perft(depth, color);
    std::cout.clear();
    IS_EQUAL(nodes, expectedRes);
}

void test_perftsuite(int depth) {
    std::string line;
    std::ifstream file("test/perftsuite.epd");

    while(std::getline(file, line)) {
        std::string d6;
        std::string delimter = " ;";
        int pos = 0;

        std::string fen = line.substr(pos, line.find(delimter, pos) - pos);
        pos += fen.length()+1;
        std::size_t strPos = line.find("D" + std::to_string(depth));

        if(strPos != std::string::npos) {
            d6 = line.substr(strPos);
            u64 nodes = std::stoull(d6.substr(3));
            
            //test engine
            perft_test(fen, nodes, depth);
        }
    }

    file.close();
}

void quick_perft(bool quick) {
    std::vector<std::string> test_fens = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", //mirror position
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"
    };

    std::vector<u64> totalNodesD6 = {
        119060324,
        193690690,
        11030083,
        15833292,
        15833292, //mirror position
        89941194,
        164075551
    };

    std::vector<u64> totalNodesD5 = {
        4865609,
        4085603,
        674624,
        422333,
        422333, //mirror position
        2103487,
        3894594
    };

    std::vector<int> depths = {
        6,
        5,
        6,
        5,
        5,
        5,
        5
    };

    for(int i = 0; i < test_fens.size(); i++) {
        if(quick) {
            //extensiveZobristTest(test_fens.at(i));
            perft_test(test_fens.at(i), totalNodesD5.at(i), depths.at(i)-1);
        }
        else perft_test(test_fens.at(i), totalNodesD6.at(i), depths.at(i));
    }
}

void testQuickSort() {
    std::vector<Move> moves = {
        Move(),
        Move(),
        Move(),
        Move(),
        Move(),
        Move(),
        Move(),
        Move(),
        Move(),
        Move(),
    };

    std::vector<int> moveScores = {20, 23, 6, 17, 45, 33, 2, 72, 28, 10};

    MoveOrder mo = MoveOrder();
    mo.quickSort(moveScores, moves, 0, moves.size()-1);

    std::string order = "";

    for(int i = 0; i < moveScores.size(); i++) {
        order.append(std::to_string(moveScores.at(i)));
        order.append(", ");
    }

    IS_EQUAL(order, "72, 45, 33, 28, 23, 20, 17, 10, 6, 2, ");
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
    //test_generateAttackedSquares();

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

    test_enPassent_discoveredCheck();
    test_enPassent_discoveredCheck2();
    test_enPassent_discoveredCheck3();

    testQuickSort();
}


//function to execute all tests
void test_all() {
    test_moveGeneration();
    test_perftsuite(6);
}

int main(int argc, char* argv[]) {
    Zobrist::init();

    if(argc == 1) {
        test_all();
    } else {
        std::string input = argv[1];
        if(input.compare("quick") == 0) quick_perft(true);
        else if(input.compare("middle") == 0) quick_perft(false); 
        else if(input.compare("full") == 0) test_perftsuite(6);
        else if(input.compare("zobrist") == 0) testZobrist("rnbqkbnr/1ppppppp/8/8/p7/P2P4/1PP1PPPP/RNBQKBNR w KQkq - 0 1"); 
        else if(input.compare("extensive_zobrist") == 0) extensiveZobristTest(DEFAULT_FEN); 
    }
    return 0;
}