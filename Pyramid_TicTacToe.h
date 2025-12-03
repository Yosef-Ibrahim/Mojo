#ifndef PYRAMID_XO_H
#define PYRAMID_XO_H

#include "BoardGame_Classes.h"
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
class Pyramid_XO_Board : public Board<T> {
public:
    Pyramid_XO_Board();
    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;
    bool is_lose(Player<T>* player) override;
};

template <typename T>
class Pyramid_XO_UI : public UI<T> {
private:
    Board<T>* boardPtr;

public:
    Pyramid_XO_UI();
    void set_board(Board<T>* b);

    // التعديل: string& (بالمرجع) عشان يتوافق مع BoardGame_Classes.h
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;

    Move<T>* get_move(Player<T>* currentPlayer) override;
    int minimax(Board<T>* board, int depth, bool isMaximizing, T aiSymbol, T humanSymbol);
};

#endif