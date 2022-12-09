#include <iostream>
#include <string>
#include<iomanip>
using namespace std;

class Board {
protected:
    char board[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    int n_moves = 0;
public:
    bool updete_board(int x, int y, char symbol);
    bool is_winner();
    bool is_draw();
    void display_board();
    char get_slot(int i,int j);
    void undo(int i,int j);
    bool available_slot(int i,int j);
    Board operator=(Board t);
};

class player {
protected:
    string name;
    char symbol;
public:
    player(char symbol);
    player(int ordar, char symbol);
    virtual void get_move(int& x, int& y);
    string to_string();
    char get_symbol();
};

class ComputerPlayer:public player{
    char symbol;
    Board xoboard;
public:
    ComputerPlayer(Board a,char s);
    void get_move(int&x,int&y);
    void set_board(Board t);
    int minimax(int ind,bool isMax);
};

class GameManger {
public:
    void Run();
};



//Computer Implementation
ComputerPlayer::ComputerPlayer(Board a,char s): player(s),symbol(s), xoboard(a) {

}

void ComputerPlayer::set_board(Board t) {
    xoboard = t;
}

int ComputerPlayer::minimax(int ind, bool isMax) {
    if(xoboard.is_winner()){
        if (isMax){
            return -1;
        }
        else{
            return 1;
        }
    }
    bool isDraw = true;
    int bestScore;
    if (isMax){
        bestScore = -1;
        for (int i = 0;i < 3;++i){
            for (int j = 0;j < 3;++j){
                if (xoboard.available_slot(i,j)){
                    xoboard.updete_board(i,j,'o');
                    int temp = minimax((i * 3)+j,false);
                    bestScore = max(temp,bestScore);
                    xoboard.undo(i,j);
                    isDraw = false;
                }
            }
        }
        if (isDraw){
            return 0;
        }
        return bestScore;
    }
    else{
        bestScore = 1;
        for (int i = 0;i < 3;++i){
            for (int j = 0;j < 3;++j){
                if (xoboard.available_slot(i,j)){
                    xoboard.updete_board(i,j,'x');
                    int temp = minimax((i * 3) + j,true);
                    bestScore = min(temp,bestScore);
                    xoboard.undo(i,j);
                    isDraw = false;
                }
            }
        }
        if(isDraw){
            return 0;
        }
        return bestScore;
    }
}

void ComputerPlayer::get_move(int &x, int &y) {
    pair<int,int> bestMove;
    int bestScore = -999;
    for (int i = 0;i < 3;++i){
        for (int j = 0;j < 3;++j){
            if (xoboard.available_slot(i,j)){
                xoboard.updete_board(i,j,'o');
                int temp = minimax((i * 3) + j,false);
                if (temp > bestScore){
                    bestScore = temp;
                    bestMove.first = i;
                    bestMove.second = j;
                }
                xoboard.undo(i,j);
            }
        }
    }
    x = bestMove.first;
    y = bestMove.second;
}




//Board Implementation
bool Board :: updete_board(int x, int y, char mark) {
    if (!(x < 0 || x > 2 || y < 0 || y > 2) && (board[x][y] == 0)) {
        board[x][y] = toupper (mark);
        n_moves++;
        return true;
    }
    else
        return false;
}

char Board::get_slot(int i, int j) {
    return board[i][j];
}

Board Board::operator=(Board t) {
    for (int i = 0;i < 3;++i){
        for (int j = 0;j < 3;++j){
            board[i][j] = t.get_slot(i,j);
        }
    }
}

void Board::undo(int i, int j) {
    n_moves--;
    board[i][j] = 0;
}

bool Board::available_slot(int i, int j) {
    return (board[i][j] == 0);
}

bool Board:: is_winner() {
    char row[3], col[3], dig[2];
    for (int i = 0; i < 3; i++) {
        row[i] = board[i][0] & board[i][1] & board[i][2];
        col[i] = board[0][i] & board[1][i] & board[2][i];
    }

    dig[0] = board[0][0] & board[1][1] & board[2][2];
    dig[1] = board[0][2] & board[1][1] & board[2][0];


    for (int i : {0, 1, 2}) {
        if ((row[i] && (row[i] == board[i][0])) ||
            (col[i] && (col[i] == board[0][i])))
        {
            return true;
        }
    }

    if ((dig[0] && dig[0] == board[1][1]) ||
        (dig[1] && dig[1] == board[1][1])) {
        return true;
    }
    else
        return false;

}

bool Board:: is_draw() {
    return (n_moves == 9 && !is_winner());
}

void Board::display_board() {
    for (int i : {0, 1, 2}) {
        cout << "\n| ";
        for (int j : {0, 1, 2}) {
            cout << "(" << i << "," << j << ")";
            cout << setw(2) << board[i][j] << " |";
        }
        cout << "\n-----------------------------";
    }
    cout << endl;
}

player ::player(char symbol) {
    this->symbol = symbol;
}
player :: player(int ordar, char symbol) {
    cout << "welcome player " << ordar << endl;
    cout << "please enter your name ";
    cin >> name;
    this->symbol = symbol;
}

void player:: get_move(int& x, int& y) {
    cout << "please enter your move x to y ";
    cin >> x >> y;
}

string player:: to_string() {
    return "player:" + name;
}

char player:: get_symbol() {
    return symbol;
}

void GameManger::Run() {
    Board X_O;
    int x, y;
    player players =  player(1, 'x');
    ComputerPlayer computer(X_O,'o');
    X_O.display_board();
    while (true) {
        for (int i : {1, 0}) {
            if (i){
                computer.set_board(X_O);
                computer.get_move(x, y);
                while (!X_O.updete_board(x, y, 'o')) {
                    computer.get_move(x, y);
                }
                X_O.display_board();
                if (X_O.is_winner()) {
                    cout <<  "Computer wins" << endl;
                    return;
                }
                if (X_O.is_draw()) {
                    cout << "Draw" << endl;
                    return;
                }
            }
            else{
                players.get_move(x, y);
                while (!X_O.updete_board(x, y, players.get_symbol())) {
                    players.get_move(x, y);
                }
                X_O.display_board();
                if (X_O.is_winner()) {
                    cout << players.to_string() << " wins" << endl;
                    return;
                }
                if (X_O.is_draw()) {
                    cout << "Draw" << endl;
                    return;
                }
            }

        }
    }
}


int main() {
    GameManger game;
    game.Run();
    system("pause");

}