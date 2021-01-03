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
 * 3. The function that return the color of the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/

int MaxDepth = 4;
#define ROW 5
#define COL 6

int heuristic_val[30]; // save heuristic value
int local_player_color;

bool check_board(Board my, Board ori); // check the wheather the board is identical or not
bool low_height(Board curnode); // height < 2

int abprune(Board curnode, int depth, int alpha, int beta, int color);
int get_next_player(int color);
int heuristic(Board curnode);

void algorithm_A(Board board, Player player, int index[]){

    //////your algorithm design///////////
    srand(time(NULL)*time(NULL));
    int row, col;
    int color = player.get_color();
    local_player_color = color;
    
    // random
    while(1){
        row = rand() % ROW;
        col = rand() % COL;
        if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    }

    // algo
    int ans[2]; //[0] = max id, [1] = max val; [2] = second id, [3] = second maxval
    if(!low_height(board) ){
        ans[0] = ans[1] = -1;
        
        // general solution, no further think
        for(int i = 0; i < 30; i ++){
            int r = i / 6;
            int c = i % 6;
            if(board.get_cell_color(r,c) == color){
                // cout << myboard.get_orbs_num(r,c) << " at " << r << "," << c << endl;
                if(board.get_orbs_num(r,c) > ans[1] ){
                    ans[1] = board.get_orbs_num(r,c);
                    ans[0] = i;
                }
            }
        }
        if(ans[0] != -1 && (board.get_cell_color(ans[0]/COL, ans[0]%COL) == color || board.get_cell_color(ans[0]/COL, ans[0]%COL) == 'w') ) {
            row = ans[0] / COL;
            col = ans[0] % COL;
        }
        
        // abprune solution
        int cur_H = abprune(board, MaxDepth, INT32_MIN, INT32_MAX, color);
        for(int i = 0; i < 30; i ++){
            if(cur_H == heuristic_val[i] && ( board.get_cell_color(i/COL, i%COL) == color || board.get_cell_color(i/COL, i%COL) == 'w' )){
                row = i / COL;
                col = i % COL;
                break;
            }
        }
    }
    else{
        for(int i = 0; i < 30; i ++){
            int r = i / 6;
            int c = i % 6;
            if(board.get_cell_color(r,c) == 'w'){
                ans[1] = board.get_orbs_num(r,c);
                ans[0] = i;
                break;
            }
        }
        row = ans[0] / COL;
        col = ans[0] % COL;
    }
    
    index[0] = row;
    index[1] = col;
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

int get_next_player(int color){
    if(color == 'r') return 'b';
    else return 'r';
}

int abprune(Board curnode, int depth, int alpha, int beta, int color){
    
    bool maximizer = (color == local_player_color );
    
    if(depth == 0 || curnode.win_the_game( Player(color) )){
        return heuristic(curnode);
    }

    // get next valid steps
    bool next_valid_steps[30];
    for(int i = 0; i < 30; i ++){
        int r = i / 6;
        int c = i % 6;
        if(curnode.get_cell_color(r,c) == color || curnode.get_cell_color(r,c) == 'w' )
            next_valid_steps[i] = true;
        else next_valid_steps[i] = false;
    }

    if(maximizer){
        int maxeval = INT32_MIN;
        for(int i = 0; i < 30; i ++){
            if(!next_valid_steps[i]) continue;
            Board next = curnode;
            Player temp(color);
            if(next.place_orb( i/6, i%6, &temp) ){
                int eval = abprune( next,depth-1,alpha,beta, get_next_player(color)); 
                maxeval = max(maxeval,eval);
                if(depth == MaxDepth)
                    heuristic_val[i] = eval;
                
                alpha = max(alpha,eval);
                if(beta <= alpha)
                    break;
            }
        }
        return maxeval;
    }
    else{
        int mineval = INT32_MAX;
        for(int i = 0; i < 30; i ++){
            if(!next_valid_steps[i]) continue;
            Board next = curnode;
            Player temp(color);
            if(next.place_orb( i/6, i%6, &temp) ){
                int eval = abprune( next,depth-1,alpha,beta, get_next_player(color)); 
                mineval = min(mineval,eval);
                if(depth == MaxDepth)
                    heuristic_val[i] = eval;
                
                beta = min(beta,eval);
                if(beta <= alpha) 
                    break;
            }
        }
        return mineval;
    }

} 
// end function

int heuristic(Board curnode)
{
    int H = 0; // H for heuristic val
    int max_H = 0;
    int min_H = 0;
    int max_occupy = 0;
    int min_occupy = 0;
    for(int i = 0; i < 30; i ++){
        int r = i / COL;
        int c = i % COL;
        int cell_color = curnode.get_cell_color(r,c);
        if(cell_color != 'w'){
            if(cell_color == local_player_color){
                max_H += curnode.get_orbs_num(r,c);
                max_occupy++;
                
                if( abs(curnode.get_orbs_num(r,c) - curnode.get_capacity(r,c)) == 2 ){
                    max_H += curnode.get_capacity(r,c)*10;
                }
                else if( abs(curnode.get_orbs_num(r,c) - curnode.get_capacity(r,c)) == 1 ){
                    max_H += curnode.get_capacity(r,c)*50;
                }

            }
            else {
                min_H +=  curnode.get_orbs_num(r,c);
                min_occupy++;
                
                if( abs(curnode.get_orbs_num(r,c) - curnode.get_capacity(r,c)) == 2 ){
                    min_H += curnode.get_capacity(r,c)*10;
                }
                else if( abs(curnode.get_orbs_num(r,c) - curnode.get_capacity(r,c)) == 1 ){
                    min_H += curnode.get_capacity(r,c)*50;
                }

            }
        }
    }
    if(max_occupy != 0 && min_occupy == 0)
        H += 1000000;
    else if(min_occupy != 0 && max_occupy == 0)
        H -= 1000000;
    // H += ((max_occupy-min_occupy)/30) *2;
    H += max_H;
    H -= min_H;
    return H;
}

bool low_height(Board curnode){
    for(int i = 0; i < 30; i ++){
        int r = i / 6;
        int c = i % 6;
        if(curnode.get_orbs_num(r,c) >= 2 ) return false;
    }
    return true;
}
