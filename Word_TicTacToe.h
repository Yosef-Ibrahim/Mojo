/**
 * @file Word_TicTacToe.h
 * @brief ملف الهيدر للعبة Word Tic-Tac-Toe.
 * @details اللعبة تعتمد على تكوين كلمات إنجليزية صحيحة (3 حروف) بدلاً من الأشكال الهندسية.
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 * @date 2023-12-05
 */

#ifndef WORD_TICTACTOE_H
#define WORD_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <set>
#include <string>
#include <fstream>

using namespace std;

// -------------------------------------------------------
// Word Board Class
// -------------------------------------------------------

/**
 * @class Word_Board
 * @brief كلاس يمثل لوحة اللعب للعبة الكلمات.
 * @details يرث من Board<char> ويحتوي على قاموس للكلمات الصحيحة للتحقق من الفوز.
 */
class Word_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< الرمز الافتراضي للخانة الفارغة.
    set<string> dictionary;  ///< مجموعة تخزن الكلمات الصحيحة من ملف القاموس.

    /**
     * @brief تحميل القاموس من ملف نصي.
     * @param filename اسم ملف القاموس.
     * @return true إذا تم التحميل بنجاح.
     */
    bool load_dictionary(const string& filename);

public:
    /**
     * @brief الكونستركتور.
     * @details يقوم بتهيئة اللوحة وتحميل ملف dic.txt.
     */
    Word_Board();

    /**
     * @brief تحديث اللوحة بحركة اللاعب.
     * @param move مؤشر للحركة.
     * @return true لو الحركة صحيحة.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief التحقق من الفوز.
     * @details يبحث عن أي كلمة صحيحة مكونة من 3 حروف.
     * @param player اللاعب الحالي.
     * @return true لو فيه فوز.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief التحقق من التعادل.
     * @param player اللاعب الحالي.
     * @return true لو اللوحة اتملت ومحدش كسب.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief هل انتهت اللعبة؟
     * @param player اللاعب الحالي.
     * @return true لو انتهت بفوز أو تعادل.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief التحقق من وجود كلمة صحيحة في اتجاه معين.
     * @param row الصف الحالي.
     * @param col العمود الحالي.
     * @param dr التغير في الصف (Direction Row).
     * @param dc التغير في العمود (Direction Col).
     * @return true لو تكونت كلمة صحيحة.
     */
    bool check_word_at(int row, int col, int dr, int dc) const;

    // --- دوال مساعدة للذكاء الاصطناعي ---

    /**
     * @brief هل الكلمة دي موجودة في القاموس؟
     * @param word الكلمة المراد فحصها.
     * @return true لو موجودة.
     */
    bool is_valid_word(const string& word) const;

    /**
     * @brief الحصول على القاموس كامل.
     * @return مجموعة الكلمات (Set).
     */
    set<string> get_dictionary() const { return dictionary; }
};

// -------------------------------------------------------
// Word UI Class
// -------------------------------------------------------

/**
 * @class Word_UI
 * @brief واجهة المستخدم للعبة الكلمات.
 * @details تدير التفاعل مع المستخدم وتحتوي على منطق الذكاء الاصطناعي لاختيار الحروف.
 */
class Word_UI : public UI<char> {
private:
    Word_Board* boardPtr = nullptr; ///< مؤشر للبورد.

    /**
     * @struct MoveScore
     * @brief هيكل لتخزين تقييم الحركة للـ AI.
     */
    struct MoveScore {
        int row, col;
        char letter;
        int score;
    };

    // --- دوال تقييم الحركات للـ AI ---

    /**
     * @brief تقييم حركة معينة.
     */
    int evaluate_move(Word_Board* board, int row, int col, char letter, char ai_symbol);

    /**
     * @brief هل الحركة دي بتكسب فوراً؟
     */
    bool creates_winning_word(Word_Board* board, int row, int col, char letter);

    /**
     * @brief هل الحركة دي بتمنع الخصم يكسب؟
     */
    bool blocks_opponent_win(Word_Board* board, int row, int col, char letter, char opponent_symbol);

    /**
     * @brief عد الكلمات المحتملة مستقبلاً.
     */
    int count_potential_words(Word_Board* board, int row, int col, char letter);

    /**
     * @brief المنطق الرئيسي لاختيار حركة الـ AI.
     */
    Move<char>* get_ai_move(Player<char>* player);

public:
    Word_UI();
    ~Word_UI() {};

    /**
     * @brief ربط الواجهة بالبورد.
     * @param b مؤشر للبورد.
     */
    void set_board(Word_Board* b) { boardPtr = b; }

    /**
     * @brief إعداد اللاعبين.
     */
    Player<char>** setup_players() override;

    /**
     * @brief إنشاء لاعب جديد.
     * @param name اسم اللاعب (بالمرجع كما هو في كودك).
     * @param symbol رمز اللاعب.
     * @param type نوع اللاعب.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief استقبال الحركة.
     * @param currentPlayer اللاعب الحالي.
     * @return مؤشر للحركة المختارة.
     */
    Move<char>* get_move(Player<char>* currentPlayer) override;
};

#endif