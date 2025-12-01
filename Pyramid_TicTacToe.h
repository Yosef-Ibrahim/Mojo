#ifndef PYRAMID_XO_H
#define PYRAMID_XO_H

#include "BoardGame_Classes.h"
#include <vector>
#include <algorithm> 

// تعريف كلاس البورد
template <typename T>
class Pyramid_XO_Board : public Board<T> {
public:
    Pyramid_XO_Board();
    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;
};

// تعريف كلاس الواجهة
template <typename T>
class Pyramid_XO_UI : public UI<T> {
private:
    Board<T>* boardPtr; // مؤشر للبورد عشان الـ AI يشوف اللوحة

public:
    Pyramid_XO_UI();

    // دالة لربط البورد بالواجهة
    void set_board(Board<T>* b);

    Player<T>* create_player(string& name, T symbol, PlayerType type) override;
    Move<T>* get_move(Player<T>* currentPlayer) override;

    // دالة الذكاء الاصطناعي (Minimax)
    int minimax(Board<T>* board, int depth, bool isMaximizing, T aiSymbol, T humanSymbol);
};

#endif // PYRAMID_XO_H