#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;
bool first_step2(Board curnode);

void algorithm_B(Board board, Player player, int index[]){
    srand(time(NULL)*time(NULL));
    int row, col;
    int color = player.get_color();
    
    while(1){
        row = rand() % 5;
        col = rand() % 6;
        if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    }
    if( first_step2(board) ){
        if(board.get_cell_color(0,0) == color || board.get_cell_color(row, col) == 'w')
            index[0] = 0, index[1] = 0; 
        else if(board.get_cell_color(0,5) == color || board.get_cell_color(row, col) == 'w')
            index[0] = 0, index[1] = 5;
        else if(board.get_cell_color(4,0) == color || board.get_cell_color(row, col) == 'w')
            index[0] = 4, index[1] = 0;
        else if(board.get_cell_color(4,5) == color || board.get_cell_color(row, col) == 'w')
            index[0] = 4, index[1] = 5;
    }

    index[0] = row;
    index[1] = col;
}

bool first_step2(Board curnode){
    for(int i = 0; i < 30; i ++){
        int r = i / 6;
        int c = i % 6;
        int cell_color = curnode.get_cell_color(r,c);
        if(cell_color != 'w') return false;
    }
    return true;
}