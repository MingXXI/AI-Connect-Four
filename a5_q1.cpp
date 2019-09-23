#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <iterator>
#include <time.h>
#include <algorithm>
#include <cstring>

using namespace std;

bool comp(int a, int b){
    return (a < b);
}

class connect4{
private:
public:
    int board[6][7] = {0};
    int empty[7] = {5,5,5,5,5,5,5};
    int sim[6][7] = {0};
    int sim_empty[7] = {5,5,5,5,5,5,5};
    int total = 42;
    
    connect4();
    ~connect4();
    
    
    int move_first();
    
    void display();
    
    int player_move();
    
    int check_win(int arr[6][7], int height[7], int last_move_index);
    
    vector<int> legal_move(int arr[7]);
    
    int next_move(int arr[6][7]);
    
};


connect4::connect4(){
}

connect4::~connect4(){
}

void connect4::display(){
    cout << "\n  1   2   3   4   5   6   7  \n";
    for (int i = 0; i < 6; i++){
        for (int j = 0; j < 7; j++){
            if (board[i][j] == 0){
                cout << "| " << " " << " ";
            }
            else if (board[i][j] == 1){
                cout << "| " << "O" << " ";
            }
            else{
                cout << "| " << "X" << " ";
            }
        }
        cout << "|\n-----------------------------\n";
    }
}

int connect4::move_first(){
    cout << "Now, if you wish to make the first move, please enter 1, else enter 2: " << flush;
    int choice;
    while (!(cin >> choice) || (choice != 1 && choice != 2)){
        cout << "Error: Please enter 1 or 2 to declare your position: ";
        cin.clear();
        cin.ignore(123,'\n');
    }
//    cout << "Please enter 1 to move first or 2 to move second: ";
//    cin >> choice;
//    while (choice != 1 && choice != 2){
//        cout << "Error: Please enter 1 or 2 to choose your positon: ";
//        cin >> choice;
//    }
    return choice;
}


int connect4::player_move(){

    int move;
    cout << "Please enter your move: " << flush;
    while (!(cin >> move) || (move > 7 || move < 1)){
        cout << "Error: Please enter a number between 1~7: ";
        cin.clear();
        cin.ignore(123,'\n');
    }
    total -= 1;
    return move - 1;

 }

int connect4::check_win(int arr[6][7], int height[7], int last_move_index){
    // "arr" is the board
    // "last_move_index" indicate whichever column changed since last move
    // "height" is array of height of each column

    int last_height_index = height[last_move_index];
    height[last_move_index] -= 1;

    // check row ----
    int leftend = max(0, last_move_index-3);
    int rightend = min(3, last_move_index);

    for(int i = leftend; i<=rightend; i++){
//        cout <<"check row: "<< i << endl;
        if(arr[last_height_index][i] == arr[last_height_index][i+1] && arr[last_height_index][i+1] == arr[last_height_index][i+2] && arr[last_height_index][i+2] == arr[last_height_index][i+3]){
            return arr[last_height_index][last_move_index];
        }
    }

    // check c0lumn ||||
    if (last_height_index < 3){
//        cout << "check column" << last_height_index << endl;
        if(arr[last_height_index][last_move_index]==arr[last_height_index+1][last_move_index] && arr[last_height_index+1][last_move_index]==arr[last_height_index+2][last_move_index] && arr[last_height_index+2][last_move_index]==arr[last_height_index+3][last_move_index])
            return arr[last_height_index][last_move_index];
    }


    int i = last_height_index;
    int j = last_move_index;

    // check diagnal
    
    for (int check_index = 3; check_index >= 0; check_index--){
//        cout << "diagnal1:" << check_index << endl;
        if(i-check_index >= 0 && j-check_index >= 0 && i+(3-check_index) <= 5 && j+(3-check_index) <= 6){
            if(arr[i-check_index][j-check_index] == arr[i-check_index+1][j-check_index+1] && arr[i-check_index+1][j-check_index+1] == arr[i-check_index+2][j-check_index+2] && arr[i-check_index+2][j-check_index+2] == arr[i-check_index+3][j-check_index+3]){
                return arr[i][j];
            }
        }
    }
    
    
    for (int check_index2 = 3; check_index2 >= 0; check_index2--){
//        cout << "diagnal2:" << check_index2 << endl;
        if(i+check_index2 <= 5 && j-check_index2 >= 0 && i-(3-check_index2) >= 0 && j+(3-check_index2) <= 6){
            if(arr[i+check_index2][j-check_index2] == arr[i+check_index2-1][j-check_index2+1] && arr[i+check_index2-1][j-check_index2+1] == arr[i+check_index2-2][j-check_index2+2] && arr[i+check_index2-2][j-check_index2+2] == arr[i+check_index2-3][j-check_index2+3]){
                return arr[i][j];
            }
        }
    }
    
    
    
    for (i=0; i<7; i++){        // if top row is not full, game is in progress
        if (!arr[0][i]){
            return -1;
        }
    }
    return 0;
}

vector<int> connect4::legal_move(int arr[7]){
    vector<int> move;
    for (int i=0; i<7; i++){
        if (arr[i] != -1){
            move.push_back(i);
        }
    }
    return move;
}


int connect4::next_move(int arr[6][7]){
    int playout = 15000;
    int win;
    int index;
    int outcome;
    int i;
    int j;
    bool player;
    int cpy_total;
    
    vector<int> moves = legal_move(empty); // legal moves of original board
    vector<int>::iterator itr;
    
    vector<int> score;
    vector<int> sim_moves;

    for(itr = moves.begin(); itr != moves.end(); itr++){    // for each legal move
//        cout << "yoyo" << endl;
        board[empty[*itr]][*itr] = 1;

//        cout << "yoyo" << endl;
        if (check_win(board, empty, *itr)==1){        // computer win after first move
//            cout << "yoyo" << endl;
            empty[*itr]++;
//            cout << "yoyo" << endl;
            return *itr;
        }
        win = 0;
//        cout << "yiyi" << endl;
        for (int k=0; k<playout; k++){    // simulate n times n=playout
//            cout << "yoyo" << endl;
            cpy_total = total - 1;
            player = true;
            memcpy(sim, board, sizeof(int)*6*7);
            memcpy(sim_empty, empty, sizeof(int)*7);
            sim_moves = legal_move(sim_empty);

            while(sim_moves.size() != 0){
//                cout << player << endl;
//                cout << "yoyo" << endl;
                index = rand() % sim_moves.size();
                j = sim_moves[index];
                i = sim_empty[j];
                sim[i][j] = player? 2:1;
                player = !player;
                cpy_total-= 1;

                outcome = check_win(sim, sim_empty, j);
                sim_moves = legal_move(sim_empty);

                if (outcome == 1){    // computer win
                    win += cpy_total;
                    break;
                }else if (outcome == 2){
                    win -= 2*cpy_total;
                    break;
                }
            }
        }
        score.push_back(win);
        board[empty[*itr]+1][*itr] = 0;
        empty[*itr]++;
    }
    for (i=0; i<score.size(); i++){
        cout<< score[i] << " ";
    }
    return moves[max_element(score.begin(),score.end())-score.begin()];    // return the best move
}



int main()
{
//    cout << "Welcome to Connect 4 Game!!!" << endl;
//    connect4 * test = new connect4();
//    test->display(test->board);
//

    srand((unsigned)time(NULL));    // rng
    
    int move;
    int win;
    connect4 * game = new connect4();
    cout << "Welcome to Our Connect 4 Game!!\n";
    cout << "To make a move, please enter the number on top of game board that corresponds to your move!!\n";
    game->display();


//    cout << "To make a move, please enter the number on top of game board that corresponds to your move\n\n";
//    cout << "Now, if you wish to make the first move, please enter 1, else enter 2: " << flush;
//
//    while (!(cin >> first) || (first != 1 && first != 2)){
//        cout << "Error: Please enter 1 or 2: ";
//        cin.clear();
//        cin.ignore(123,'\n');
//    }
//
    if (game->move_first() == 1){
        move = game->player_move();
        game->board[5][move] = 2;
        game->empty[move]--;
        game->display();
    }

    do{
        move = game->next_move(game->board);    // move by computer
        cout << "\nComputer made move at: " << move+1<< endl;
        game->board[game->empty[move]][move] = 1;

        game->display();
        win = game->check_win(game->board, game->empty, move);
        if (win != -1){    // game over
            if (win == 1){
                cout << "Computer Wins! You Loser!" << endl;
            }else{
                cout << "You Beat the Computer! Congratulations" << endl;
            }
            break;
        }

        while (1){    // get a valid player move
            move = game->player_move();
            if (game->board[0][move]){
                cout << "please enter a valid move where the location of move is empty" << endl;
            }else{
                break;
            }
        }

        game->board[game->empty[move]][move] = 2;    // move by player
        win = game->check_win(game->board, game->empty, move);
        if (win!=-1){    // game over
            game->display();
            if (win == 1){
                cout << "Computer Wins! You Losser!" << endl;
            }else if (win == 2){
                cout << "You Beat the Computer! Congratulations" << endl;
            } else{
                cout << "Draw! Practice More!" << endl;
            }
            break;
        }

    }
    while (game->legal_move(game->empty).size()>0);
    

//     return 0;
//     int computer = 2;
//     cout << "First, please decide what position you want to take!" << endl;
//     int player = move_first();
//     if (player == computer){
//         computer = 1;
//     }


// //    display(board);
//     cout << move_first() << endl;
//     cout << player_move() << endl;
}



