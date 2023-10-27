# Chessna 
Chessna is a simple chess engine that is designed to beat most human players.

## Features
#### Move Generation:
- Bitboards
#### Search:
- Iterative deepening in combination with Alpha-Beta Search
- Quiescence Search
- Transposition Table
- Move ordering (MVV-LVA, Killer moves, Promotions, Castling, best move from last iteration)
- Late Move Reduction
#### Evaluation:
- material difference
- position score (slightly different for endgame)
- Mop-up evaluation 
#### Other:
- UCI (Universal chess interface): needed to communicate with lichess 

## How to test the engine
Compile the engine and run it:
```sh
stefan@chessna:~/Chessna$ make 
stefan@chessna:~/Chessna$ build/chessna
```
Chessna uses [UCI](http://page.mi.fu-berlin.de/block/uci.htm) (partly implemented) for communication. You can setup the start position or any custom position using the according [FEN](https://www.chessprogramming.org/Forsyth-Edwards_Notation) followed by the moves:
```sh
stefan@chessna:~/Chessna$ build/chessna 
position startpos moves e2e4 e7e5 
```
To calculate the best move for a certain depth type:
```sh
stefan@chessna:~/Chessna$ build/chessna 
position 8/6p1/5n1p/3rpk2/1PB2b2/5P2/5PK1/8 b - - 0 52 
go depth 9
```
To calculate the best move given a time limit (in ms) type:
```sh
stefan@chessna:~/Chessna$ build/chessna 
position 8/6p1/5n1p/3rpk2/1PB2b2/5P2/5PK1/8 b - - 0 52 moves d5d2 c4a6 
go movetime 10000
```
Note: The engine will only use 90% of the given time to calculate the best move.
You can also tell the engine its remaining time (in ms) for a match and it will adjust its move time accordingly: 
```sh
stefan@chessna:~/Chessna$ build/chessna 
position 8/6p1/5n1p/3rpk2/1PB2b2/5P2/5PK1/8 b - - 0 52 moves d5d2 c4a6 
go wtime 300000 btime 300000 
```
Furthermore you can add an increment: 
```sh
stefan@chessna:~/Chessna$ build/chessna 
position 8/6p1/5n1p/3rpk2/1PB2b2/5P2/5PK1/8 b - - 0 52 moves d5d2 c4a6 
go wtime 300000 btime 300000 winc 2000 binc 2000 
```

You can also use a GUI to run Chessna. So far I've only tested that it works with [Lichess](https://lichess.org) as I use [lichess-bot](https://github.com/lichess-bot-devs/lichess-bot) to let Chessna play against other bots and players.


## Features to implement in the future:
- LAZY SMP: multithreading
- Null-move heuristic
- Opening Book
- better evaluation (king safety, mobility, pawn structure, ...)
- improve move generation (use array instead of vector, magic bitboards etc.)
- pondering
