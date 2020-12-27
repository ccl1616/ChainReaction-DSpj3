#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;
int MaxDepth = 4;
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
 * 3. The function that return the color of the cell(row, col)
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
*/

int id = -1;
int ans[4]; //[0] = max id, [1] = max val; [2] = second id, [3] = second maxval
bool checker = false;
int heuristic_val[30]; // save heuristic value
int local_player_color;
Board original;

bool check_board(Board my, Board ori); // check the wheather the board is identical or not
void get_valid_spots(Board myboard, int color, bool next[30]); // get next valid spot
int abprune(Board curnode, int depth, int alpha, int beta, int color);
int get_next_player(int color);
int heuristic(Board curnode);
bool first_step(Board curnode);

void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////
    srand(time(NULL)*time(NULL));
    int row, col;
    int color = player.get_color();
    local_player_color = color;
    original = board;
    
    // random
    while(1){
        row = rand() % 5;
        col = rand() % 6;
        //if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
        if(board.get_cell_color(row, col) == color) break;
        else if(board.get_cell_color(row, col) == 'w') break;
    }

    // algo
    Board myboard = board;
    if(!first_step(board)){
        bool next[30];
        ans[0] = ans[2] = -1;
        get_valid_spots(myboard, color, next);

        // general solution, no further think
        for(int i = 0; i < 30; i ++){
            int r = i / 6;
            int c = i % 6;
            if(next[i]){
                if(myboard.get_orbs_num(r,c) > ans[1] ){
                    ans[3] = ans[1];
                    ans[2] = ans[0];
                    ans[1] = myboard.get_orbs_num(r,c);
                    ans[0] = i;
                }
            }
        }
        id = ans[0];
        if(id != -1 && myboard.get_cell_color(id/6, id%6) == color ) {
            checker = 1; 
            row = id / 6;
            col = id % 6;
        }
        else if(ans[2] != -1 && myboard.get_cell_color(ans[2]/6, ans[2]%6) == color ){
            checker = 1; 
            row = ans[2] / 6;
            col = ans[2] % 6;
        }

        // abprune solution
        
        for(int i = 1; i <= 9; i+= 2){
            MaxDepth = i;
            int cur_H = abprune(myboard, MaxDepth, INT32_MIN, INT32_MAX, color);
            for(int j = 0; i < 30; j ++){
                if(heuristic_val[j] == cur_H){
                    if(myboard.get_cell_color(j/6, j%6) == color){
                        row = j / 6;
                        row = j % 6;
                    }
                }
            }
        }
    }
    
    index[0] = row;
    index[1] = col;
    std::cout << check_board(myboard,board) << " checker:" << checker << ' ' << myboard.get_cell_color(row, col) << endl;
}

bool check_board(Board my, Board ori)
{
    bool ans = true;
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 6; j ++){
            if( !(my.get_orbs_num(i,j) == ori.get_orbs_num(i,j)) ) ans = false; 
            else if( !(my.get_capacity(i,j) == ori.get_capacity(i,j)) ) ans = false;
            else if( !(my.get_cell_color(i,j) == ori.get_cell_color(i,j)) ) ans = false;

            if(!ans) return false;
        }
    }
    return true;
}

void get_valid_spots(Board myboard, int color, bool next[30])
{
    for(int i = 0; i < 30; i ++){
        next[i] = false;
        int r = i / 6;
        int c = i % 6;
        if(myboard.get_cell_color(r, c) == color || myboard.get_cell_color(r, c) == 'w') {
            next[i] = true;
        }
    }
}

int get_next_player(int color){
    if(color == 'r') return 'b';
    else return 'r';
}

int abprune(Board curnode, int depth, int alpha, int beta, int color){
    cout << "hi abprune\n";
    bool maximizer = (color == local_player_color );
    
    if(depth == 0 || curnode.win_the_game( Player(color) )){
        cout << "hi call H\n";
        return heuristic(curnode);
    }
    if(maximizer){
        int maxeval = INT32_MIN;
        bool next_valid_steps[30];
        get_valid_spots(curnode, color, next_valid_steps);

        for(int i = 0; i < 30; i ++){
            Board next = curnode;
            if(!next_valid_steps[i]) continue;
            Player temp(color);

            if(!next.place_orb( i/6, i%6, &temp) ){
                cout << "illegal\n";
                continue;
            }
            else{
                int eval = abprune( next,depth-1,alpha,beta, get_next_player(color)); 
                maxeval = max(maxeval,eval);
                if(depth == MaxDepth)
                    heuristic_val[i] = eval;
                
                alpha = max(alpha,eval);
                if(beta <= alpha)
                    break;
            }
        }
        // curnode.heuristic = maxeval;
        return maxeval;
    }
    else{
        int mineval = INT32_MAX;
        bool next_valid_steps[30];
        get_valid_spots(curnode, color, next_valid_steps);

        for(int i = 0; i < 30; i ++){
            Board next = curnode;
            if(!next_valid_steps[i]) continue;
            Player temp(color);

            if(!next.place_orb(i/6, i%6, &temp) ){
                cout << "illegal\n";
                continue;
            }
            else{
                int eval = abprune( next,depth-1,alpha,beta, get_next_player(color)); 
                mineval = min(mineval,eval);
                if(depth == MaxDepth)
                    heuristic_val[i] = eval;
                
                beta = min(beta,eval);
                if(beta <= alpha) 
                    break;
            }
        }
        // curnode.heuristic = mineval;
        return mineval;
    }

} // end function

int heuristic(Board curnode)
{
    int H = 0; // H for heuristic val
    for(int i = 0; i < 30; i ++){
        int r = i / 6;
        int c = i % 6;
        int cell_color = curnode.get_cell_color(r,c);
        if(cell_color != 'w'){
            if(cell_color == local_player_color){
                H += curnode.get_orbs_num(r,c);
            }
            else H -= curnode.get_orbs_num(r,c);
        }
    }
    return H;
}

bool first_step(Board curnode){
    for(int i = 0; i < 30; i ++){
        int r = i / 6;
        int c = i % 6;
        int cell_color = curnode.get_cell_color(r,c);
        if(cell_color != 'w') return false;
    }
    return true;
}