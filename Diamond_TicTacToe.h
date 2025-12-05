/**
 * @file Diamond_TicTacToe.h
 * @brief ملف الهيدر للعبة Diamond Tic-Tac-Toe.
 * @details تعتمد اللعبة على تشكيل خطوط من رموز متشابهة في شبكة معينة.
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 * @date 2025-12-05
 */

#ifndef DIAMOND_TICTACTOE_H
#define DIAMOND_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>

using namespace std;

/**
 * @class Diamond_Board
 * @brief كلاس يمثل لوحة اللعب الخاصة بـ Diamond X-O.
 */
class Diamond_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< الرمز المستخدم للخانة الفارغة.

    /**
     * @brief التحقق من وجود خط بطول معين.
     */
    bool check_line_length(char symbol, int length) const;

public:
    Diamond_Board();

    /**
     * @brief تحديث اللوحة بحركة جديدة.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief التحقق من الفوز.
     */
    bool is_win(Player<char>* player) override;

    // دالة الخسارة غير مستخدمة هنا ولكن يجب تعريفها لأنها pure virtual في بعض النسخ
    bool is_lose(Player<char>* player) override { return false; }

    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief التحقق هل الخانة داخل حدود الشكل المعين (Diamond).
     */
    bool is_valid_cell(int x, int y) const;

    // دوال مساعدة للـ AI
    int count_lines(char symbol, int length) const;
    std::vector<std::pair<int, int>> get_valid_moves() const;
};

/**
 * @class Diamond_UI
 * @brief واجهة المستخدم للعبة Diamond.
 */
class Diamond_UI : public UI<char> {
private:
    Diamond_Board* board_ref;

    // هيكل لتقييم حالة اللوحة للـ AI
    struct BoardScore {
        int lines_of_3;
        int lines_of_4;
        int potential_lines;

        int get_score() const {
            // تقييم بسيط وسريع
            if (lines_of_3 > 0 && lines_of_4 > 0) return 100000; // فوز فوري
            return lines_of_4 * 500 + lines_of_3 * 100;
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

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief دالة مساعدة لعرض المصفوفة (تم إزالة override لتجنب الخطأ).
     */
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const;

    /**
     * @brief إعداد اللاعبين (Override من الكلاس UI).
     */
    Player<char>** setup_players() override;
};

#endif // DIAMOND_TICTACTOE_H