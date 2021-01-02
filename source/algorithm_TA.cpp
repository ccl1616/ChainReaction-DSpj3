#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

int MaxDepth2 = 4;
#define ROW 5
#define COL 6
int heuristic_val2[30]; // save heuristic value
int local_player_color2;
bool first_step2(Board curnode);
int abprune2(Board curnode, int depth, int alpha, int beta, int color);
int get_next_player2(int color);
int heuristic2(Board curnode);

void algorithm_C(Board board, Player player, int index[])
{
    // random algo
    srand(time(NULL)*time(NULL));
    int row, col;
    int color = local_player_color2 = player.get_color();
    
    while(1){
        row = rand() % 5;
        col = rand() % 6;
        if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    }
    index[0] = row;
    index[1] = col;
}

void algorithm_B(Board board, Player player, int index[]){
    srand(time(NULL)*time(NULL));
    int row, col;
    int color = local_player_color2 = player.get_color();
    
    while(1){
        row = rand() % 5;
        col = rand() % 6;
        if(board.get_cell_color(row, col) == color || board.get_cell_color(row, col) == 'w') break;
    }
    /*
    if( first_step2(board) ){
        if(board.get_cell_color(0,0) == color || board.get_cell_color(row, col) == 'w')
            index[0] = 0, index[1] = 0; 
        else if(board.get_cell_color(0,5) == color || board.get_cell_color(row, col) == 'w')
            index[0] = 0, index[1] = 5;
        else if(board.get_cell_color(4,0) == color || board.get_cell_color(row, col) == 'w')
            index[0] = 4, index[1] = 0;
        else if(board.get_cell_color(4,5) == color || board.get_cell_color(row, col) == 'w')
            index[0] = 4, index[1] = 5;
    }*/

    if( !first_step2(board) ){
        // general solution
        int ans[2] = {-1,-1};
        for(int i = 0; i < 30; i ++){
            int r = i / 6;
            int c = i % 6;
            if(board.get_cell_color(r,c) == color){
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
        
        int cur_H = abprune2(board, MaxDepth2, INT32_MIN, INT32_MAX, color);
        for(int i = 0; i < 30; i ++){
            if(cur_H == heuristic_val2[i] && (board.get_cell_color(i/COL, i%COL) == color || board.get_cell_color(i/COL, i%COL) == 'w')){
                row = i / COL;
                col = i % COL;
                break;
            }
        }

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

int get_next_player2(int color){
    if(color == 'r') return 'b';
    else return 'r';
}

int abprune2(Board curnode, int depth, int alpha, int beta, int color){
    //cout << "hi abprune\n";
    bool maximizer = (color == local_player_color2 );
    
    if(depth == 0 || curnode.win_the_game( Player(color) )){
        return heuristic2(curnode);
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
            if(!next.place_orb( i/6, i%6, &temp) ){
                cout << "illegal\n";
                continue;
            }
            else{
                int eval = abprune2( next,depth-1,alpha,beta, get_next_player2(color)); 
                maxeval = max(maxeval,eval);
                if(depth == MaxDepth2)
                    heuristic_val2[i] = eval;
                
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
            if(!next.place_orb(i/6, i%6, &temp) ){
                cout << "illegal\n";
                continue;
            }
            else{
                int eval = abprune2( next,depth-1,alpha,beta, get_next_player2(color)); 
                mineval = min(mineval,eval);
                if(depth == MaxDepth2)
                    heuristic_val2[i] = eval;
                
                beta = min(beta,eval);
                if(beta <= alpha) 
                    break;
            }
        }
        return mineval;
    }

} 
// end function

int heuristic2(Board curnode)
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
            if(cell_color == local_player_color2){
                max_H += curnode.get_orbs_num(r,c);
                max_occupy++;
                /*
                if( abs(curnode.get_orbs_num(r,c) - curnode.get_capacity(r,c)) == 2 )
                    max_H += curnode.get_capacity(r,c)*5;
                else if( abs(curnode.get_orbs_num(r,c) - curnode.get_capacity(r,c)) == 1 )
                    max_H += curnode.get_capacity(r,c)*50;*/

            }
            else {
                min_H +=  curnode.get_orbs_num(r,c);
                min_occupy++;
                /*
                if( abs(curnode.get_orbs_num(r,c) - curnode.get_capacity(r,c)) == 2 )
                    min_H += curnode.get_capacity(r,c)*5;
                else if( abs(curnode.get_orbs_num(r,c) - curnode.get_capacity(r,c)) == 1 )
                    min_H += curnode.get_capacity(r,c)*50;*/
            }
        }
    }
    H += (max_occupy/30)*50 , H += max_H;
    H -= (min_occupy/30)*50 , H -= min_H;
    return H;
}