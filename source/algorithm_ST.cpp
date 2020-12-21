#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/

/*
container:
-> use one dimentional method to record (x,y)
    a local board
    next_valid_step -- an array that save next_valid_steps, record size
    ans_array -- an array that saved final point , record size

function:
    put_disk
    abprune
    heuristic
    min, max

abprune(board, depth, alpha, beta, player)
{
    if end, return heuristic
    if maximizer{
        find out next_valid_steps
        for(all next_valid_steps){
            clone a now board
            put it on
            int eval = abprune( next,depth-1,alpha,beta); 
            maxeval = max(maxeval,eval);
            if(depth == MaxDepth)
                h_map.insert(pair<int,Point>(eval,i)); //save into ans_array
            
            alpha = max(alpha,eval);
            if(beta <= alpha)
                break;
        }
    }
    else{
        do same thing for minimizer
    }
}

*/

void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////
    srand(time(NULL)*time(NULL));
    int row, col;
    int color = player.get_color();
    
    while(1){
        row = rand() % 5;
        col = rand() % 6;
        if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    }

    index[0] = row;
    index[1] = col;
    
}