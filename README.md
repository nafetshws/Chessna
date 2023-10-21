# Chessna 
Chessna is a simple chess engine that is designed to beat most human players.

## Features
#### Move Generation:
- Bitboards
#### Search:
- Iterative deepening in combination with Alpha-Beta Search
- Quiescence Search
- Transposition Table
- Move ordering (MVV-LVA, Killer moves, Promotions, Castleing, best move from last iteration)
#### Evaluation:
- material difference
- position score (slightly different for endgame)
- Mop-up evaluation 

## Features to implement in the future:
- LAZY SMP: multithreading
- Killer moves
- Null-move heuristic
- Opening Book
- better evaluation (king safety, mobility, pawn structure, ...)
- improve move generation (use array instead of vector, magic bitboards etc.)
- UCI (Universal chess interface): create lichess bot account
- Late Move Reduction

