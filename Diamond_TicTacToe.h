#ifndef _DIAMOND_TICTACTOE_H
#define _DIAMOND_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <utility>

using namespace std;

// -------------------------------------------------------------------
// Diamond_TicTacToe_Board Class
// -------------------------------------------------------------------
template <typename T>
class Diamond_TicTacToe_Board : public Board<T> {
private:
    bool is_valid_position(int x, int y);
    vector<vector<pair<int, int>>> find_all_lines(T symbol, int length);
    bool check_simultaneous_win(T symbol);

public:
    Diamond_TicTacToe_Board();
    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;
    void display_diamond_board();
};

// -------------------------------------------------------------------
// Diamond_TicTacToe_Player Class
// -------------------------------------------------------------------
template <typename T>
class Diamond_TicTacToe_Player : public Player<T> {
public:
    Diamond_TicTacToe_Player(string name, T symbol, PlayerType type);
};

// -------------------------------------------------------------------
// Diamond_TicTacToe_RandomPlayer Class
// -------------------------------------------------------------------
template <typename T>
class Diamond_TicTacToe_RandomPlayer : public Diamond_TicTacToe_Player<T> {
public:
    Diamond_TicTacToe_RandomPlayer(string name, T symbol);
    pair<int, int> get_random_move(Board<T>* board);
};

// -------------------------------------------------------------------
// Diamond_TicTacToe_UI Class
// -------------------------------------------------------------------
template <typename T>
class Diamond_TicTacToe_UI : public UI<T> {
public:
    Diamond_TicTacToe_UI();
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;
    Move<T>* get_move(Player<T>* currentPlayer) override;

    void set_board(Diamond_TicTacToe_Board<T>* b) { board_ptr = b; }
    void display_diamond_board(const vector<vector<T>>& matrix);

private:
    Diamond_TicTacToe_Board<T>* board_ptr;
};

#endif