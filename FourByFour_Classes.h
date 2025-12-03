#ifndef FOURBYFOUR_CLASSES_H
#define FOURBYFOUR_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
class FourByFour_Board : public Board<T> {
public:
    FourByFour_Board();
    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;
};

template <typename T>
class FourByFour_UI : public UI<T> {
private:
    Board<T>* boardPtr;

public:
    FourByFour_UI();

    // دالة الربط
    void set_board(Board<T>* b);

    // string& ضرورية
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;

    Move<T>* get_move(Player<T>* currentPlayer) override;

    int minimax(Board<T>* board, int depth, bool isMaximizing, T aiSymbol, T humanSymbol, int alpha, int beta);
};

#endif