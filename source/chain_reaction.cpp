#include <iostream>
#include <time.h>
#include "../include/board.h"
#include "../include/player.h"
#include "../include/rules.h"
#include "../include/algorithm.h"

using namespace std;
time_t timearr[2];

int main(){

    Board board;
    Player red_player(RED);
    Player blue_player(BLUE);

    bool first_two_step = true;
    int round = 1;
    int index[2];

    FILE *tempo = freopen("log.txt","w",stdout); //me

    while(1){

        //////////// Red Player operations ////////////
        timearr[0] = time(NULL);
        algorithm_A(board, red_player, index);
        timearr[1] = time(NULL);
        board.place_orb(index[0], index[1], &red_player);
        
        if(rules_violation(red_player)) return 0;
        double duration = (double) difftime(timearr[0],timearr[1]);
        board.print_current_board(index[0], index[1], round, duration);
        round++;

        if(board.win_the_game(red_player) && !first_two_step){
            cout << "Red Player won the game !!!" << endl;
            return 0;
        }

        //////////// Blue Player operations ////////////
        timearr[0] = time(NULL);
        algorithm_B(board, blue_player, index);
        timearr[1] = time(NULL);
        board.place_orb(index[0], index[1], &blue_player);

        if(rules_violation(blue_player)) return 0;
        duration = (double) difftime(timearr[0],timearr[1]);
        board.print_current_board(index[0], index[1], round, duration);
        round++;

        if(board.win_the_game(blue_player) && !first_two_step){
            cout << "Blue Player won the game !!!" << endl;
            return 0;
        }

        first_two_step = false;
    }
    fclose(tempo);
    return 0;
} 