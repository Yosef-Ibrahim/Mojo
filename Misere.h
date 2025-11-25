#pragma once
#include "BoardGame_Classes.h"
using namespace std;

class Misere_Board : public Board<char> {
private:
    char blank_symbol = '.';

public:
    Misere_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

class Misere_UI : public UI<char> {
public:
    Misere_UI();
    ~Misere_UI() {}
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* currentPlayer) override;
};