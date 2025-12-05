/**
 * @file Ultimate_TicTacToe_Classes.h
 * @brief ملف الهيدر للعبة Ultimate Tic-Tac-Toe (9x9).
 * @details تحتوي اللعبة على لوحة كبيرة 9x9 مقسمة إلى 9 لوحات صغيرة (3x3). الفوز في لوحة صغيرة يمنحك خانة في اللوحة الرئيسية.
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 * @date 2023-12-05
 */

#ifndef _ULTIMATE_TICTACTOE_CLASSES_H
#define _ULTIMATE_TICTACTOE_CLASSES_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// ==========================================
// 1. كلاس الحركة (Ultimate_Move)
// ==========================================

/**
 * @class Ultimate_Move
 * @brief كلاس يمثل حركة اللاعب في لعبة Ultimate Tic-Tac-Toe.
 * @details يرث من Move<char> ويخزن الإحداثيات (الصف والعمود) والرمز (X أو O).
 */
class Ultimate_Move : public Move<char> {
public:
    /**
     * @brief كونستركتور الحركة.
     * @param r رقم الصف (0-8).
     * @param c رقم العمود (0-8).
     * @param s رمز اللاعب.
     */
    Ultimate_Move(int r, int c, char s) : Move<char>(r, c, s) {}

    virtual ~Ultimate_Move() {}
};

// ==========================================
// 2. كلاس البورد (Ultimate_Board)
// ==========================================

/**
 * @class Ultimate_Board
 * @brief كلاس يدير منطق اللوحة الكبيرة (9x9) واللوحات الفرعية.
 * @details يحتوي على مصفوفة اللعب الأساسية ومصفوفة إضافية (main_board) لتتبع الفائزين في الـ 9 مربعات الصغيرة.
 */
class Ultimate_Board : public Board<char> {
private:
    /**
     * @brief اللوحة الرئيسية المصغرة (3x3).
     * @details تخزن الفائز في كل قطاع فرعي (Sub-board). إذا فاز أحدهم في القطاع العلوي الأيسر، يتم وضع رمزه هنا.
     */
    vector<vector<char>> main_board;

public:
    /**
     * @brief كونستركتور البورد.
     * @details يهيئ اللوحة الكبيرة 9x9 واللوحة المصغرة 3x3 بالرموز الفارغة.
     */
    Ultimate_Board();

    virtual ~Ultimate_Board();

    /**
     * @brief تنفيذ حركة على اللوحة وتحديث الحالة.
     * @details تتأكد من صحة الحركة، وتحدث اللوحة الكبيرة، ثم تتحقق مما إذا كانت الحركة قد أنهت لوحة فرعية (Sub-board).
     * @param move مؤشر لكائن الحركة.
     * @return true إذا كانت الحركة صحيحة وتمت بنجاح.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief التحقق من الفوز باللعبة بالكامل.
     * @details الفوز يحدث عند تكوين خط في اللوحة الرئيسية المصغرة (main_board).
     * @param player اللاعب المراد فحصه.
     * @return true إذا حقق اللاعب شروط الفوز.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief التحقق من التعادل.
     * @details يحدث التعادل إذا امتلأت اللوحة الكبيرة (81 خانة) ولم يفز أحد.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief التحقق من انتهاء اللعبة (فوز أو تعادل).
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief دالة مساعدة للتحقق من فوز لوحة فرعية (3x3).
     * @param sub_r صف اللوحة الفرعية (0-2).
     * @param sub_c عمود اللوحة الفرعية (0-2).
     * @param winner مرجع (Reference) لتبديله برمز الفائز إذا وجد.
     * @return true إذا كان هناك فائز في هذه اللوحة الفرعية.
     */
    bool check_subboard_winner(int sub_r, int sub_c, char& winner);

    /**
     * @brief الحصول على مصفوفة اللوحة الرئيسية المصغرة.
     * @return مرجع للمصفوفة main_board.
     */
    vector<vector<char>>& get_main_board_matrix() { return main_board; }
};

// ==========================================
// 3. كلاس الواجهة (Ultimate_UI)
// ==========================================

/**
 * @class Ultimate_UI
 * @brief واجهة المستخدم للعبة Ultimate Tic-Tac-Toe.
 * @details تدير الإدخال والإخراج، طباعة اللوحة بشكل منسق، ومنطق الكمبيوتر العشوائي.
 */
class Ultimate_UI : public UI<char> {
private:
    /**
     * @brief مؤشر للوحة اللعب.
     * @details يستخدم لتمكين الواجهة (والكمبيوتر) من قراءة حالة اللوحة (مثل الأماكن الفارغة).
     */
    Board<char>* boardPtr;

public:
    Ultimate_UI();
    virtual ~Ultimate_UI();

    /**
     * @brief ربط الواجهة بكائن البورد.
     * @param b مؤشر للبورد.
     */
    void set_board(Board<char>* b);

    /**
     * @brief إنشاء لاعب جديد.
     * @details تأخذ الاسم بالمرجع (Reference) لتتوافق مع الكلاس الأب.
     * @param name اسم اللاعب.
     * @param symbol رمز اللاعب.
     * @param type نوع اللاعب (بشر أو كمبيوتر).
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief الحصول على حركة اللاعب.
     * @details إذا كان اللاعب بشراً، تطلب إحداثيات. إذا كان كمبيوتر، تختار حركة عشوائية من الأماكن الفارغة.
     */
    Move<char>* get_move(Player<char>* currentPlayer) override;

    /**
     * @brief طباعة اللوحة.
     * @details تعرض اللوحة الكبيرة 9x9 مع فواصل توضح حدود اللوحات الفرعية، وتعرض حالة اللوحة الرئيسية.
     */
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif // _ULTIMATE_TICTACTOE_CLASSES_H