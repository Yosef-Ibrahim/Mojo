/**
 * @file Memory_TicTacToe.h
 * @brief ملف الهيدر للعبة Memory Tic-Tac-Toe.
 * @details الرموز تختفي بعد لعبها، وعلى اللاعب تذكر أماكنها.
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 */

#ifndef MEMORY_TICTACTOE_H
#define MEMORY_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <string>

using namespace std;

/**
 * @class Memory_Board
 * @brief بورد يخزن حالتين: الحالة الحقيقية (للحسابات) والحالة الظاهرة (للعرض).
 */
class Memory_Board : public Board<char> {
private:
    vector<vector<char>> hidden_board;  ///< المصفوفة التي تخزن القيم الحقيقية المخفية.

public:
    Memory_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief عرض اللوحة الحقيقية (لأغراض الاختبار أو نهاية اللعبة).
     */
    void display_hidden_board() const;
};

/**
 * @class Memory_UI
 * @brief واجهة المستخدم، تعرض لوحة فارغة أغلب الوقت.
 */
class Memory_UI : public UI<char> {
private:
    Memory_Board* boardPtr;

public:
    Memory_UI();
    ~Memory_UI();

    void set_board(Memory_Board* b) { boardPtr = b; }

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* currentPlayer) override;

    /**
     * @brief إعادة تعريف دالة العرض لإخفاء الرموز.
     */
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif