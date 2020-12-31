# DS-project3
Implementing a chess game algorithm.

TA: https://github.com/Hhhho/Chain-Reaction

compile: 
g++ chain_reaction.cpp board.cpp rules.cpp player.cpp algorithm_ST.cpp algorithm_TA.cpp
g++ -std=c++11 chain_reaction.cpp board.cpp rules.cpp player.cpp algorithm_ST.cpp algorithm_TA.cpp

player:
O is the red player, is the first one that can take the step(odd turn).
'w' = empty cell

5*6 board
0   1   2   3   4   5
6   7   8   9   10  11
12  13  14  15  16  17
18  19  20  21  22  23
24  25  26  27  28  29