#ifndef XO_CLASSES_H
#define XO_CLASSES_H

#include "BoardGame_Classes.h"
using namespace std;

class X_O_Board : public Board<char> {
private:
    char blank_symbol = '.';

public:
    X_O_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};


class XO_UI : public UI<char> {
public:
    XO_UI();
    ~XO_UI() {};
    Player<char>* create_player(string& name, char symbol, PlayerType type);
    Move<char>* get_move(Player<char>* currentPlayer);
};

#endif