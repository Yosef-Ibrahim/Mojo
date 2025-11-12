#ifndef NUMERICAL_TICTACTOE_H
#define NUMERICAL_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <set>
using namespace std;

class Numerical_TicTacToe_Board : public Board<int> {
private:
    int blank_symbol = 0;

public:
    Numerical_TicTacToe_Board();
    bool update_board(Move<int>* move);
    bool is_win(Player<int>* player);
    bool is_draw(Player<int>* player);
    bool game_is_over(Player<int>* player);
};


class Numerical_TicTacToe_UI : public UI<int> {
private:
    set<int> used_numbers;
    set<int> player1_available; // Odd numbers: 1, 3, 5, 7, 9
    set<int> player2_available; // Even numbers: 2, 4, 6, 8

public:
    Numerical_TicTacToe_UI();
    ~Numerical_TicTacToe_UI() {};
    Player<int>** setup_players() override;
    Player<int>* create_player(string& name, int symbol, PlayerType type);
    Move<int>* get_move(Player<int>* currentPlayer);
    void mark_number_used(int number);
    bool is_number_available(int number, int player_symbol);
    void display_available_numbers(int player_symbol);
};

#endif