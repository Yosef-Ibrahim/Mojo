#ifndef DIAMOND_TICTACTOE_H
#define DIAMOND_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <limits>
#include <algorithm>

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

    // دوال مساعدة للـ AI
    int count_lines(char symbol, int length) const;
    std::vector<std::pair<int, int>> get_valid_moves() const;
};

class Diamond_UI : public UI<char> {
private:
    Diamond_Board* board_ref;

    // دوال الـ Minimax للـ AI
    struct BoardScore {
        int lines_of_3;
        int lines_of_4;
        int potential_lines;

        int get_score() const {
            if (lines_of_3 > 0 && lines_of_4 > 0) return 10000;
            return lines_of_4 * 100 + lines_of_3 * 50 + potential_lines * 10;
        }
    };

    BoardScore evaluate_position(char symbol);
    int minimax(int depth, bool is_maximizing, char ai_symbol, char opp_symbol, int alpha, int beta);
    Move<char>* get_ai_move(Player<char>* player);
    int count_potential_lines(char symbol, int length);

public:
    Diamond_UI();
    ~Diamond_UI() {}

    void set_board(Diamond_Board* board) { board_ref = board; }

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;
    Player<char>** setup_players() override { return UI<char>::setup_players(); }
};

#endif