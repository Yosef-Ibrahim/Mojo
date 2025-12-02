#ifndef INFINITY_CLASSES_H
#define INFINITY_CLASSES_H

#include "BoardGame_Classes.h"
#include <deque>

class Infinity_Board : public Board<char> {
private:
    char blank_symbol = '.';
    std::deque<Move<char>*> moveHistory; // لتخزين الحركات بالترتيب
    int moveCounter = 0;

public:
    Infinity_Board();
    bool update_board(Move<char>* move);
    void removeOldestMove();
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>* player) { return false; }
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};

class Infinity_UI : public UI<char> {
public:
    Infinity_UI();
    ~Infinity_UI() {}
    Player<char>* create_player(std::string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};

#endif