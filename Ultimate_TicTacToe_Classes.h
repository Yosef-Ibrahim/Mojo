#ifndef _ULTIMATE_TICTACTOE_CLASSES_H
#define _ULTIMATE_TICTACTOE_CLASSES_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// 1. كلاس الحركة الخاصة بالـ Ultimate
class Ultimate_Move : public Move<char> {
public:
    Ultimate_Move(int r, int c, char s) : Move<char>(r, c, s) {}
    virtual ~Ultimate_Move() {}
};

// 2. كلاس البورد
class Ultimate_Board : public Board<char> {
private:
    vector<vector<char>> main_board; // البورد الـ 3x3 الصغير

public:
    Ultimate_Board();
    virtual ~Ultimate_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // دالة مساعدة
    bool check_subboard_winner(int sub_r, int sub_c, char& winner);

    // عشان نقدر نوصل للبورد الداخلي لو احتجنا
    vector<vector<char>>& get_main_board_matrix() { return main_board; }
};

// 3. كلاس الـ UI
class Ultimate_UI : public UI<char> {
private:
    Board<char>* boardPtr; // نفس اسم وطريقة Pyramid بالظبط

public:
    Ultimate_UI();
    virtual ~Ultimate_UI();

    // دالة الربط (نفس Pyramid)
    void set_board(Board<char>* b);

    // دالة إنشاء اللاعب (نفس Pyramid بالظبط باستخدام string&)
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    Move<char>* get_move(Player<char>* currentPlayer) override;

    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif