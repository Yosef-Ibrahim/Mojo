/**
 * @file Numerical_TicTacToe.h
 * @brief ملف الهيدر للعبة Numerical Tic-Tac-Toe.
 * @details تعتمد اللعبة على الأرقام بدلاً من X و O، الفوز بمجموع 15.
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 * @date 2025-12-05
 */

#ifndef NUMERICAL_TICTACTOE_H
#define NUMERICAL_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @class Numerical_TicTacToe_Board
 * @brief لوحة اللعب الرقمية.
 * @details تتعامل مع الأرقام (int) وتفحص المجموع 15.
 */
class Numerical_TicTacToe_Board : public Board<int> {
private:
    int blank_symbol = 0; ///< 0 يمثل الخانة الفارغة.

public:
    Numerical_TicTacToe_Board();
    bool update_board(Move<int>* move) override;

    /**
     * @brief التحقق من الفوز.
     * @return true إذا كان مجموع أي صف أو عمود أو قطر يساوي 15.
     */
    bool is_win(Player<int>* player) override;

    bool is_draw(Player<int>* player) override;
    bool game_is_over(Player<int>* player) override;
};

/**
 * @class Numerical_TicTacToe_UI
 * @brief واجهة المستخدم للعبة الأرقام.
 * @details تدير الأرقام المتاحة (فردي وزوجي) لكل لاعب.
 */
class Numerical_TicTacToe_UI : public UI<int> {
private:
    set<int> used_numbers;      ///< الأرقام التي تم لعبها.
    set<int> player1_available; ///< أرقام اللاعب الأول (فردية: 1,3,5,7,9).
    set<int> player2_available; ///< أرقام اللاعب الثاني (زوجية: 2,4,6,8).

    Board<int>* boardPtr = nullptr;

public:
    Numerical_TicTacToe_UI();
    ~Numerical_TicTacToe_UI() {};

    void set_board(Board<int>* b);

    Player<int>** setup_players() override;
    Player<int>* create_player(string& name, int symbol, PlayerType type) override;
    Move<int>* get_move(Player<int>* currentPlayer) override;

    /**
     * @brief وضع علامة على الرقم أنه مستخدم.
     */
    void mark_number_used(int number);

    /**
     * @brief هل الرقم متاح لهذا اللاعب؟
     */
    bool is_number_available(int number, int player_symbol);

    /**
     * @brief عرض الأرقام المتاحة للاعب الحالي.
     */
    void display_available_numbers(int player_symbol);
};

#endif