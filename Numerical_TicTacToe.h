#ifndef NUMERICAL_TICTACTOE_H
#define NUMERICAL_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

// -------------------------------------------------------
// Numerical Board Class
// -------------------------------------------------------
class Numerical_TicTacToe_Board : public Board<int> {
private:
    int blank_symbol = 0; // 0 يمثل الخانة الفارغة

public:
    Numerical_TicTacToe_Board();
    bool update_board(Move<int>* move) override;
    bool is_win(Player<int>* player) override;
    bool is_draw(Player<int>* player) override;
    bool game_is_over(Player<int>* player) override;
};

// -------------------------------------------------------
// Numerical UI Class
// -------------------------------------------------------
class Numerical_TicTacToe_UI : public UI<int> {
private:
    set<int> used_numbers;
    set<int> player1_available; // Odd: 1, 3, 5, 7, 9
    set<int> player2_available; // Even: 2, 4, 6, 8

    // مؤشر للبورد عشان نقدر نشوف الأماكن الفاضية
    Board<int>* boardPtr = nullptr;

public:
    Numerical_TicTacToe_UI();
    ~Numerical_TicTacToe_UI() {};

    // دالة جديدة لربط البورد بالواجهة
    void set_board(Board<int>* b);

    Player<int>** setup_players() override;
    Player<int>* create_player(string& name, int symbol, PlayerType type) override;
    Move<int>* get_move(Player<int>* currentPlayer) override;

    void mark_number_used(int number);
    bool is_number_available(int number, int player_symbol);
    void display_available_numbers(int player_symbol);
};

#endif