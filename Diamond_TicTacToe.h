#ifndef DIAMOND_TICTACTOE_H
#define DIAMOND_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>

class Diamond_Board : public Board<char> {
private:
    char blank_symbol = '.';
    bool check_line_length(char symbol, int length) const;

public:
    Diamond_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
    bool is_valid_cell(int x, int y) const;
};

class Diamond_UI : public UI<char> {
public:
    Diamond_UI();
    ~Diamond_UI() {}
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    virtual Move<char>* get_move(Player<char>* player) override;
    virtual void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;
    virtual Player<char>** setup_players() override { return UI<char>::setup_players(); }
};

#endif