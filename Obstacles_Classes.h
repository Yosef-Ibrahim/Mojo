#ifndef OBSTACLES_CLASSES_H
#define OBSTACLES_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>

using namespace std;

// كلاس البورد
template <typename T>
class Obstacles_Board : public Board<T> {
public:
    Obstacles_Board();
    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;

    // دالة زيادة عشان نضيف العوائق
    void add_random_obstacles();
};

// كلاس الواجهة
template <typename T>
class Obstacles_UI : public UI<T> {
public:
    Obstacles_UI();
    ~Obstacles_UI() {};
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;
    Move<T>* get_move(Player<T>* currentPlayer) override;
};

#endif // OBSTACLES_CLASSES_H