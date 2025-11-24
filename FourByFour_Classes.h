#ifndef FOURBYFOUR_CLASSES_H
#define FOURBYFOUR_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>

using namespace std;

// تعريف الكلاس FourByFour_Board
template <typename T>
class FourByFour_Board : public Board<T> {
public:
    FourByFour_Board();
    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;
};

// تعريف الكلاس FourByFour_UI
template <typename T>
class FourByFour_UI : public UI<T> {
public:
    FourByFour_UI();
    ~FourByFour_UI() {};
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;
    Move<T>* get_move(Player<T>* currentPlayer) override;
};

#endif // FOURBYFOUR_CLASSES_H