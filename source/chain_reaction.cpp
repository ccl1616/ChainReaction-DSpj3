#include <iostream>
#include <time.h>
#include <chrono>
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
    bool debug = false;

    if(debug) FILE *tempo = freopen("log.txt","w",stdout); //me

    while(1){

        //////////// Red Player operations ////////////
        auto start = chrono::high_resolution_clock::now(); 
        timearr[0] = time(NULL);
        algorithm_A(board, red_player, index);
        timearr[1] = time(NULL);
        auto stop = chrono::high_resolution_clock::now(); 
        board.place_orb(index[0], index[1], &red_player);
        
        if(rules_violation(red_player)) {
            cout << index[0] << "," << index[1] << endl;
            return 0;
        }
        double duration = (double) difftime(timearr[1],timearr[0]);
        auto duration2 = chrono::duration_cast<chrono::microseconds>(stop - start); 
        
        if( duration2.count() > 1000000 ) {
            cout << duration2.count() << endl;
            cout << "Red Player timeout xxxxxxxxxxxx" << endl;
            return 0;
        }
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
    return 0;
} 