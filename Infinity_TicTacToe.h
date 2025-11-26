#ifndef _INFINITY_TICTACTOE_H
#define _INFINITY_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <deque>
#include <utility>
#include <map>

using namespace std;

template <typename T>
class Infinity_TicTacToe_Board : public Board<T> {
private:
    // تتبع نقلات كل لاعب على حدة
    map<T, deque<pair<int, int>>> player_moves;

    bool check_win_for_symbol(T symbol);

public:
    Infinity_TicTacToe_Board();
    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;
};


template <typename T>
class Infinity_TicTacToe_Player : public Player<T> {
public:
    Infinity_TicTacToe_Player(string name, T symbol, PlayerType type);
};


template <typename T>
class Infinity_TicTacToe_RandomPlayer : public Infinity_TicTacToe_Player<T> {
public:
    Infinity_TicTacToe_RandomPlayer(string name, T symbol);
    pair<int, int> get_random_move(Board<T>* board);
};


template <typename T>
class Infinity_TicTacToe_UI : public UI<T> {
public:
    Infinity_TicTacToe_UI();
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;
    Move<T>* get_move(Player<T>* currentPlayer) override;

    void set_board(Board<T>* b) { board_ptr = b; }

private:
    Board<T>* board_ptr;
};

#endif