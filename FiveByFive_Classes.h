#ifndef FIVEBYFIVE_CLASSES_H
#define FIVEBYFIVE_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>

using namespace std;

// Board class for 5x5 Tic-Tac-Toe
template <typename T>
class FiveByFive_Board : public Board<T> {
public:
    FiveByFive_Board();
    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;

    // Change this method from private/protected to public:
    int count_three_in_a_row(T symbol);

private:
    // ... existing private members ...
};

// UI class for 5x5 Tic-Tac-Toe
template <typename T>
class FiveByFive_UI : public UI<T> {
public:
    FiveByFive_UI();
    ~FiveByFive_UI() {};
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;
    Move<T>* get_move(Player<T>* currentPlayer) override;
};

#endif // FIVEBYFIVE_CLASSES_H#pragma once
