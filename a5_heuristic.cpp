
// Heuristic and class structure refers to: https://github.com/RobertSzkutak/SUNY-Fredonia/blob/696a39a00df3e775c8d40be73bfb4159c2aeda98/CSIT461-Intro-To-AI-Engineering/Connect4

#include <iostream>
#include <ctime>

using namespace std;

#define ROWS    6
#define COLUMNS 7

class Connect4{
private:
public:
    Connect4();
    ~Connect4();
    
    void display();
    
    void player_move(int player);
    
    int check_win();
    
    void setChip(int set) { chip = set; }
    
    void dropChip(int col, int val);
    
    void removeChip(int col);
    
    void agentMove();
    
    int board[ROWS][COLUMNS] = {0};
    
    int chip = 1;
};




Connect4::Connect4(){
}
Connect4::~Connect4(){
}

void Connect4::dropChip(int col, int val){
    for(int i = 5; i >= 0; i--){
        if(board[i][col] == 0){
            board[i][col] = val;
            break;
        }
    }
}

void Connect4::removeChip(int col){
    for(int i = 0; i < ROWS; i++){
        if(board[i][col] > 0){
            board[i][col] = 0;
            break;
        }
    }
}

void Connect4::agentMove(){
    //check if any move can win.
    for(int i = 0; i < COLUMNS; i++){
        if(board[0][i] == 0){   //no need to test if column full
            dropChip(i, chip);
            if(check_win() == chip)
                return;
            removeChip(i);
        }
    }
    
    // prevent opponent win
    int oppchip = 0;//This represents the value of the opoonents chip
    if(chip == 1)
        oppchip = 2;
    if(chip == 2)
        oppchip = 1;
    for(int i = 0; i < COLUMNS; i++){
        if(board[0][i] == 0){   //no need to test if column full
            dropChip(i, oppchip);
            if(check_win() == oppchip){
                removeChip(i);
                dropChip(i, chip);
                return;
            }
            removeChip(i);
        }
    }
    
    int priority[COLUMNS];  //List of score a move can earn
    
    //Test each column, assign a value according to how good of a move it is
    for(int i = 0; i < COLUMNS; i++){
        priority[i] = 0;
        
        if(board[0][i] == 0){   //no need to test if column full
            dropChip(i, chip);
            
            //Make sure the opponent can't win after this move
            for(int j = 0; j < COLUMNS; j++){
                if(board[0][j] == 0){       //no need to test if column full
                    dropChip(j, oppchip);
                    if(check_win() == oppchip){
                        priority[i] = -1;   // score -1 if the move let opponent win
                    }
                    removeChip(j);
                }
            }
            
            //Calculate how good is this move
            if(priority[i] == 0){
                
                for(int j = 0; j < COLUMNS; j++){
                    if(board[0][j] == 0){
                        dropChip(j, chip);
                        
                        if(check_win() == chip)
                            priority[i] += 2; // if a move can lead to a win in 1 more step, earn 2 scores
                        else{
                            
                            for(int h = 0; h < COLUMNS; h++){
                                if(board[0][h] == 0){
                                    dropChip(h, chip);
                                    
                                    if(check_win() == chip)
                                        priority[i]++;
                                    removeChip(h);
                                }
                            }
                        }
                        removeChip(j);
                    }
                }
            }
            removeChip(i);
        }
        else
            priority[i] = -2;   //if it definate a win of opponent
    }
    
    //Which column has the highest priority
    int maxPriority = INT_MIN;  // initialize max score
    int index = 0;
    for(int i = 0; i < COLUMNS; i++){
        if(priority[i] > maxPriority){
            maxPriority = priority[i];
            index = i;
        }
    }
    dropChip(index, chip);
}

int Connect4::check_win()
{
    //Vertical |||
    for(int column = 0; column < COLUMNS; column++)
        for(int row = 0; row+3 < ROWS; row++)
            if(board[row][column] > 0                       &&
               board[row][column] == board[row+1][column]   &&
               board[row+1][column] == board[row+2][column] &&
               board[row+2][column] == board[row+3][column])
                return board[row][column];
    
    //Horizontal ---
    for(int row = 0; row < ROWS; row++)
        for(int column = 0; column+3 < COLUMNS; column++)
            if(board[row][column] > 0                       &&
               board[row][column] == board[row][column+1]   &&
               board[row][column+1] == board[row][column+2] &&
               board[row][column+2] == board[row][column+3])
                return board[row][column];
    
    //diagonal\\\ upper part
    
    for(int row = 0; row+3 < ROWS; row++)
        for(int column = 0; column+3 < COLUMNS; column++)
            if(board[row][column] > 0                           &&
               board[row][column] == board[row+1][column+1]     &&
               board[row+1][column+1] == board[row+2][column+2] &&
               board[row+2][column+2] == board[row+3][column+3])
                return board[row][column];
    
    //diagonal /// upper part
    
//    for(int row = 0; row < 4; row++)
//        for (int col = 6; col > 2; col++)
//            if(board[row][col] > 0 && board[row][col] == board[row+1][col-1] && board[row+1][col-1] == board[row+2][col-2]
//               && board[row+2][col-2] == board[row+3][col-3])
//                return board[row][col];
//
    
    
    
    
    for(int row = ROWS-1; row >= 3; row--)
        for(int column = 0; column+3 < COLUMNS; column++)
            if(board[row][column] > 0                           &&
               board[row][column] == board[row-1][column+1]     &&
               board[row-1][column+1] == board[row-2][column+2] &&
               board[row-2][column+2] == board[row-3][column+3])
                return board[row][column];

    //diagonal /// lower part
    for(int row = 0; row+3 < ROWS; row++)
        for(int column = COLUMNS-1; column >= 3; column--)
            if(board[row][column] > 0                           &&
               board[row][column] == board[row+1][column-1]     &&
               board[row+1][column-1] == board[row+2][column-2] &&
               board[row+2][column-2] == board[row+3][column-3])
                return board[row][column];
//
    //diagonal \\\ lower part
    
//    for(int row = 0; row < 4; row++){
//        for (int col = 0; col < 4; col++){
//            if (board[row][col] > 0 && board[row][col] == board[row+1][col+1] && board[row+1][col+1] == board[row+2][col+2]
//                && board[row+2][col+2] == board[row+3][col+3]){
//                return board[row][col];
//            }
//        }
//    }
//
//
    for(int row = ROWS-1; row <= 3; row--)
        for(int column = COLUMNS-1; column > 3; column--)
            if(board[row][column] > 0                           &&
               board[row][column] == board[row-1][column-1]     &&
               board[row-1][column-1] == board[row-2][column-2] &&
               board[row-2][column-2] == board[row-3][column-3])
                return board[row][column];
//
    //Check for a tie
    for(int i = 0; i < ROWS; i++)
        for(int j = 0; j < COLUMNS; j++)
            if(board[i][j] == 0)
                return 0;
            else
                if(i == ROWS-1 && j == COLUMNS-1)
                    return 3;
    
    return 0;
}


void Connect4::display(){
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

void Connect4::player_move(int player){
    if(chip == player){
        agentMove();
        return;
    }
    
    int column = 0;
    while(true){
        cout << endl << "Choose a column 1 - 7: ";
        cin >> column;
        column -= 1;
        
        if(column >=0 && column < COLUMNS)
            if(board[0][column] == 0)
                break;
            else
                cout << endl << "That column is full. Please choose a different column.";
            else
                cout << endl << "That is an invalid column. Please choose a valid column.";
    }
    
    dropChip(column, player);
}


int main(){
    
    Connect4 connect4;
    int  win = 0, mode = 0;
    int player[2] = {2,1};
    int position;
    int turn = 0;
    int player_position = 1;
    
    cout << "Welcome to Our Connect 4 Game!!\n";
    
    mode = 1; // player counter computer only
    if(mode == 1){
        cout << endl << "If you wish to make the first move, please enter 1, else enter 2: ";
        cin >> position;
        if(position == 1){
            player_position = 1;
            connect4.setChip(1);
        }else{
            player_position = 2;
            connect4.setChip(2);
        }
    }
    
    while(!win){
        connect4.display();
        cout << "To make a move, please enter the number on top of game board that corresponds to your move!!\n";
        connect4.player_move(player[turn % 2]);
        turn += 1;
        win = connect4.check_win();
    }
    
    connect4.display();
    
    if(win == 3){
        cout << endl << "Draw! Practice More!";
    }else{
        if (mode == 1){
            if (win != player_position){
                cout << endl << "You Beat the Computer! Congratulations" << endl;
            }else{
                cout << endl << "Computer Wins! You Losser!" << endl;
            }
        }else{
            cout << endl << "Player " << win << " wins!" << endl;
        }

    }
}


//First try

//#include <iostream>
//#include <stdlib.h>
//#include <utility>
//#include <vector>
//#include <iterator>
//#include <time.h>
//#include <algorithm>
//#include <cstring>
//#include <string.h>
//#include <limits>
//
//
//
//
//using namespace std;
//int N = 15000;
//
//
//bool comp(int a, int b){
//    return (a < b);
//}
//
//
//
//class connect4{
//private:
//public:
//    int board[6][7] = {0};
//    int empty[7] = {5,5,5,5,5,5,5};
//    int sim[6][7] = {0};
//    int sim_empty[7] = {5,5,5,5,5,5,5};
//    int total = 42;
//    int empty1;
//
//    connect4();
//    ~connect4();
//
//
//    int move_first();
//
//    void display();
//
//    int player_move();
//
//    int check_win(int arr[6][7], int height[7], int last_move_index);
//
//    vector<int> legal_move(int arr[7]);
//
//    int next_move(int arr[6][7]);
//
//};
//
//
//connect4::connect4(){
//}
//
//connect4::~connect4(){
//}
//
//void connect4::display(){
//    cout << "\n  1   2   3   4   5   6   7  \n";
//    for (int i = 0; i < 6; i++){
//        for (int j = 0; j < 7; j++){
//            if (board[i][j] == 0){
//                cout << "| " << " " << " ";
//            }
//            else if (board[i][j] == 1){
//                cout << "| " << "O" << " ";
//            }
//            else{
//                cout << "| " << "X" << " ";
//            }
//        }
//        cout << "|\n-----------------------------\n";
//    }
//}
//
//int connect4::move_first(){
//    cout << "Now, if you wish to make the first move, please enter 1, else enter 2: " << flush;
//    int choice;
//    while (!(cin >> choice) || (choice != 1 && choice != 2)){
//        cout << "Error: Please enter 1 or 2 to declare your position: ";
//        cin.clear();
//        cin.ignore(123,'\n');
//    }
//    //    cout << "Please enter 1 to move first or 2 to move second: ";
//    //    cin >> choice;
//    //    while (choice != 1 && choice != 2){
//    //        cout << "Error: Please enter 1 or 2 to choose your positon: ";
//    //        cin >> choice;
//    //    }
//    return choice;
//}
//
//
//int connect4::player_move(){
//
//    int move;
//    cout << "Please enter your move: " << flush;
//    while (!(cin >> move) || (move > 7 || move < 1)){
//        cout << "Error: Please enter a number between 1~7: ";
//        cin.clear();
//        cin.ignore(123,'\n');
//    }
//    total -= 1;
//    return move - 1;
//
//}
//
//int connect4::check_win(int arr[6][7], int height[7], int last_move_index){
//    // "arr" is the board
//    // "last_move_index" indicate whichever column changed since last move
//    // "height" is array of height of each column
//
//    int last_height_index = height[last_move_index];
//    height[last_move_index] -= 1;
//
//    // check row ----
//    int leftend = max(0, last_move_index-3);
//    int rightend = min(3, last_move_index);
//
//    for(int i = leftend; i<=rightend; i++){
//        //        cout <<"check row: "<< i << endl;
//        if(arr[last_height_index][i] == arr[last_height_index][i+1] && arr[last_height_index][i+1] == arr[last_height_index][i+2] && arr[last_height_index][i+2] == arr[last_height_index][i+3]){
//            return arr[last_height_index][last_move_index];
//        }
//    }
//
//    // check c0lumn ||||
//    if (last_height_index < 3){
//        //        cout << "check column" << last_height_index << endl;
//        if(arr[last_height_index][last_move_index]==arr[last_height_index+1][last_move_index] && arr[last_height_index+1][last_move_index]==arr[last_height_index+2][last_move_index] && arr[last_height_index+2][last_move_index]==arr[last_height_index+3][last_move_index])
//            return arr[last_height_index][last_move_index];
//    }
//
//
//    int i = last_height_index;
//    int j = last_move_index;
//
//    // check diagnal
//
//    for (int check_index = 3; check_index >= 0; check_index--){
//        //        cout << "diagnal1:" << check_index << endl;
//        if(i-check_index >= 0 && j-check_index >= 0 && i+(3-check_index) <= 5 && j+(3-check_index) <= 6){
//            if(arr[i-check_index][j-check_index] == arr[i-check_index+1][j-check_index+1] && arr[i-check_index+1][j-check_index+1] == arr[i-check_index+2][j-check_index+2] && arr[i-check_index+2][j-check_index+2] == arr[i-check_index+3][j-check_index+3]){
//                return arr[i][j];
//            }
//        }
//    }
//
//
//    for (int check_index2 = 3; check_index2 >= 0; check_index2--){
//        //        cout << "diagnal2:" << check_index2 << endl;
//        if(i+check_index2 <= 5 && j-check_index2 >= 0 && i-(3-check_index2) >= 0 && j+(3-check_index2) <= 6){
//            if(arr[i+check_index2][j-check_index2] == arr[i+check_index2-1][j-check_index2+1] && arr[i+check_index2-1][j-check_index2+1] == arr[i+check_index2-2][j-check_index2+2] && arr[i+check_index2-2][j-check_index2+2] == arr[i+check_index2-3][j-check_index2+3]){
//                return arr[i][j];
//            }
//        }
//    }
//
//
//
//    for (i=0; i<7; i++){        // if top row is not full, game is in progress
//        if (!arr[0][i]){
//            return -1;
//        }
//    }
//    return 0;
//}
//
//vector<int> connect4::legal_move(int arr[7]){
//    vector<int> move;
//    for (int i=0; i<7; i++){
//        if (arr[i] != -1){
//            move.push_back(i);
//        }
//    }
//    return move;
//}
//
//int connect4::next_move(int arr[6][7]){
//    int playout = 15000;
//    int win;
//    int index;
//    int outcome;
//    int i;
//    int j;
//    bool player;
//    int cpy_total;
//    int flag = 0;
//    int col;
//    int row;
//
//    vector<int> moves = legal_move(empty); // legal moves of original board
//    vector<int> moves2;
//    vector<int>::iterator itr;
//    vector<int>::iterator itr2;
//
//    vector<int> score;
//    vector<int> sim_moves;
//
//    for(itr = moves.begin(); itr != moves.end(); itr++){    // for each legal move
////        cout << "yoyo" << endl;
//        board[empty[*itr]][*itr] = 1;
//
////        cout << "yoyo" << endl;
//        if (check_win(board, empty, *itr)==1){        // computer win after first move
////            cout << "yoyo" << endl;
//            empty[*itr]++;
////            cout << "yoyo" << endl;
//            return *itr;
//        }
//        moves2 = legal_move(empty);
//        for(itr2 = moves2.begin(); itr2 != moves2.end(); itr2++){
//            board[empty[*itr2]][*itr2] = 2;
//            if (check_win(board, empty, *itr2)==2){        // computer win after first move
//                flag = 1;
//                empty[*itr2] ++;
//                board[empty[*itr2]][*itr2] = 0;
//                break;
//            }
//            empty[*itr2]++;
//            board[empty[*itr2]][*itr2] = 0;
//        }
//        if(flag == 0){
//            row = empty[*itr];
//            col = *itr;
//
//
//        }
//    }
//    empty[*itr] ++;
//    board[empty[*itr]][*itr] = 0;
//    return 0;
//}


//int connect4::next_move(int arr[6][7]){
//    bool player = false;
//    int *empty1 = nullptr;
//    int *empty2 = nullptr;
//    int *empty3 = nullptr;
//    int *empty4 = nullptr;
//    int flag = 0;
//    int win;
//    int cpy_total;
//    int index;
//    int i,j, outcome;
//    vector<int> moves1; // legal moves of original board
//    vector<int> moves2;
//    vector<int> moves3;
//    vector<int> moves4;
//    vector<int>::iterator itr1;
//    vector<int>::iterator itr2;
//    vector<int>::iterator itr3;
////    vector<int>::iterator itr4;
//    
//    vector<int> score;
//    vector<int> sim_moves;
//
//    memcpy(empty1, empty, 28);
////    memcpy(board1, board, 168);
//    moves1 = legal_move(empty1);
//    
//    for(itr1 = moves1.begin(); itr1 != moves1.end(); itr1++){    // for each legal move
//        cpy_total = total;
//        board[empty1[*itr1]][*itr1] = 1;
//        cpy_total --;
//        if (check_win(board, empty1, *itr1)==1){        // computer win after first move
//            empty1[*itr1]++;
//            return *itr1;
//        }
//        
//        player = !player;
//        
//        memcpy(empty2, empty1, 28);
//        moves2 = legal_move(empty2);
//        
//        win = 0;
//        
//        for(itr2 = moves2.begin(); itr2 != moves2.end(); itr2++){
//            board[empty2[*itr2]][*itr2] = 2;
//            if (check_win(board, empty2, *itr2)==2){        // computer win after first move
//                flag = 1;
//                empty2[*itr2] ++;
//                board[empty2[*itr2]][*itr2] = 0;
//                break;
//            }
//            empty2++;
//            board[empty2[*itr2]][*itr2] = 0;
//        }
//        
//        if (flag == 1){
//            flag = 0;
//            score.push_back(INT_MIN);
//            continue;
//        } else{
//            for(itr2 = moves2.begin(); itr2 != moves2.end(); itr2++){
//                board[empty2[*itr2]][*itr2] = 2;
//                cpy_total --;
//                memcpy(empty3, empty2, 28);
//                moves3 = legal_move(empty3);
//                for(itr3 = moves3.begin(); itr3 != moves3.end(); itr3++){
//                    board[empty3[*itr3]][*itr3] = 1;
//                    cpy_total --;
//                    if (check_win(board, empty3, *itr3)==1){        // computer win after first move
//                        empty3[*itr3]++;
//                        win += cpy_total;
//                    }
//                    empty3[*itr3]++;
//                    
//                    for (int k=0; k<N; k++){    // simulate n times n=playout
//                        player = true;
//                        memcpy(empty4, empty3, 28);
//                        moves4 = legal_move(empty4);
//                        
//                        while(moves4.size() != 0){
//                            index = rand() % moves4.size();
//                            j = moves4[index];
//                            i = empty4[j];
//                            board[i][j] = player? 2:1;
//                            cpy_total -= 1;
//                            
//                            player = !player;
//                            
//                            outcome = check_win(sim, empty4, j);
//                            
//                            moves4 = legal_move(empty4);
//                            
//                            if (outcome == 1){    // computer win
//                                board[i][j] = 0;
//                                win += cpy_total;
//                                break;
//                            }
//                            else if (outcome == 2){
//                                board[i][j] = 0;
//                                win -= 2*cpy_total ;
//                                break;
//                            }
//                            cpy_total += 1;
//                            board[i][j] = 0;
//                        }
//                    }// end of later random
//                    board[empty3[*itr3]][*itr3] = 0;
//                    cpy_total ++;
//                }// end of step 3
//                cpy_total ++;
//            } // end of step 2
//        }
//        empty1[*itr1] ++;
//        board[empty1[*itr1]][*itr1] = 0;
//        
//    } // end of step 1
//    for (i=0; i<score.size(); i++){
//        cout<< score[i] << " ";
//    }
//    cout << "\n";
//    return moves1[max_element(score.begin(),score.end())-score.begin()];    // return the best move
//}


//
//int main()
//{
//    //    cout << "Welcome to Connect 4 Game!!!" << endl;
//    //    connect4 * test = new connect4();
//    //    test->display(test->board);
//    //
//
//    srand((unsigned)time(NULL));    // rng
//
//    int move;
//    int win;
//    connect4 * game = new connect4();
//    cout << "Welcome to Our Connect 4 Game!!\n";
//    cout << "To make a move, please enter the number on top of game board that corresponds to your move!!\n";
//    game->display();
//
//
//    //    cout << "To make a move, please enter the number on top of game board that corresponds to your move\n\n";
//    //    cout << "Now, if you wish to make the first move, please enter 1, else enter 2: " << flush;
//    //
//    //    while (!(cin >> first) || (first != 1 && first != 2)){
//    //        cout << "Error: Please enter 1 or 2: ";
//    //        cin.clear();
//    //        cin.ignore(123,'\n');
//    //    }
//    //
//    if (game->move_first() == 1){
//        move = game->player_move();
//        game->board[5][move] = 2;
//        game->empty[move]--;
//        game->display();
//    }
//
//    do{
//        move = game->next_move(game->board);    // move by computer
//        cout << "\nComputer made move at: " << move+1<< endl;
//        game->board[game->empty[move]][move] = 1;
//
//        game->display();
//        win = game->check_win(game->board, game->empty, move);
//        if (win != -1){    // game over
//            if (win == 1){
//                cout << "Computer Wins! You Loser!" << endl;
//            }else{
//                cout << "You Beat the Computer! Congratulations" << endl;
//            }
//            break;
//        }
//
//        while (1){    // get a valid player move
//            move = game->player_move();
//            if (game->board[0][move]){
//                cout << "please enter a valid move where the location of move is empty" << endl;
//            }else{
//                break;
//            }
//        }
//
//        game->board[game->empty[move]][move] = 2;    // move by player
//        win = game->check_win(game->board, game->empty, move);
//        if (win!=-1){    // game over
//            game->display();
//            if (win == 1){
//                cout << "Computer Wins! You Losser!" << endl;
//            }else if (win == 2){
//                cout << "You Beat the Computer! Congratulations" << endl;
//            } else{
//                cout << "Draw! Practice More!" << endl;
//            }
//            break;
//        }
//
//    }
//    while (game->legal_move(game->empty).size()>0);
//
//
//    return 0;
//    //    int computer = 2;
//    //    cout << "First, please decide what position you want to take!" << endl;
//    //    int player = move_first();
//    //    if (player == computer){
//    //        computer = 1;
//    //    }
//    //
//    //
//    ////    display(board);
//    //    cout << move_first() << endl;
//    //    cout << player_move() << endl;
//}
//

//
