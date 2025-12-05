/**
 * @file Pyramid_TicTacToe.h
 * @brief ملف الهيدر الخاص بلعبة الهرم (Pyramid Tic-Tac-Toe).
 * @details يحتوي على تعريف كلاسات البورد والواجهة الخاصة بلعبة الهرم.
 *@author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 * @date 2023-12-05
 */

#ifndef PYRAMID_XO_H
#define PYRAMID_XO_H

#include "BoardGame_Classes.h"
#include <vector>
#include <algorithm>

using namespace std;

// ==========================================================
// 1. كلاس البورد (Pyramid_XO_Board)
// ==========================================================

/**
 * @class Pyramid_XO_Board
 * @brief كلاس يمثل لوحة اللعب الخاصة بـ Pyramid Tic-Tac-Toe.
 * @details هذا الكلاس يرث من الكلاس العام Board ويقوم بتطبيق قوانين الهرم (3 صفوف، 5 أعمدة).
 */
template <typename T>
class Pyramid_XO_Board : public Board<T> {
public:
    /**
     * @brief الكونستركتور الافتراضي.
     * @details يقوم بتهيئة البورد بأبعاد 3x5 وتصفير الخانات لتكون جاهزة للعب.
     */
    Pyramid_XO_Board();

    /**
     * @brief تحديث اللوحة بحركة اللاعب.
     * @param move مؤشر للكائن الذي يحتوي على تفاصيل الحركة (الإحداثيات والرمز).
     * @return true لو الحركة صحيحة وتمت، false لو الحركة غير صالحة.
     */
    bool update_board(Move<T>* move) override;

    /**
     * @brief التحقق من الفوز.
     * @details يفحص الخطوط الأفقية والرأسية والأقطار الخاصة بشكل الهرم.
     * @param player مؤشر للاعب الحالي المراد فحص فوزه.
     * @return true لو اللاعب كسب، false لو لسه.
     */
    bool is_win(Player<T>* player) override;

    /**
     * @brief التحقق من التعادل.
     * @param player مؤشر للاعب الحالي.
     * @return true لو اللوحة اتملت ومحدش كسب.
     */
    bool is_draw(Player<T>* player) override;

    /**
     * @brief التحقق من انتهاء اللعبة.
     * @return true لو فيه فوز أو تعادل.
     */
    bool game_is_over(Player<T>* player) override;

    /**
     * @brief التحقق من الخسارة (غير مستخدمة في هذه اللعبة).
     * @return دائماً false.
     */
    bool is_lose(Player<T>* player) override;
};

// ==========================================================
// 2. كلاس الواجهة (Pyramid_XO_UI)
// ==========================================================

/**
 * @class Pyramid_XO_UI
 * @brief كلاس يمثل واجهة المستخدم (User Interface) للعبة الهرم.
 * @details مسئول عن التعامل مع اللاعبين، طباعة البورد، وأخذ الحركات (سواء من بشر أو AI).
 */
template <typename T>
class Pyramid_XO_UI : public UI<T> {
private:
    Board<T>* boardPtr; ///< مؤشر للبورد لتمكين الـ AI من رؤية الحالة.

public:
    /**
     * @brief كونستركتور الواجهة.
     */
    Pyramid_XO_UI();

    /**
     * @brief دالة لربط البورد بالواجهة.
     * @param b مؤشر للبورد المستخدم في اللعبة.
     */
    void set_board(Board<T>* b);

    /**
     * @brief إنشاء لاعب جديد.
     * @param name اسم اللاعب (بالمرجع).
     * @param symbol رمز اللاعب (X أو O).
     * @param type نوع اللاعب (Human أو Computer).
     * @return مؤشر للكائن Player الجديد.
     */
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;

    /**
     * @brief الحصول على حركة من اللاعب.
     * @details لو اللاعب بشر، بيطلب إحداثيات. لو كمبيوتر، بيشغل الـ Minimax.
     * @param currentPlayer اللاعب اللي عليه الدور.
     * @return مؤشر لكائن Move يحتوي على الحركة المختارة.
     */
    Move<T>* get_move(Player<T>* currentPlayer) override;

    /**
     * @brief خوارزمية الذكاء الاصطناعي (Minimax).
     * @details تقوم بحساب أفضل حركة ممكنة للكمبيوتر لضمان عدم الخسارة.
     * @param board حالة البورد الحالية.
     * @param depth عمق البحث في الشجرة.
     * @param isMaximizing هل الدور للكمبيوتر (تعظيم المكسب) أم للخصم (تقليل الخسارة).
     * @param aiSymbol رمز الـ AI.
     * @param humanSymbol رمز اللاعب البشري.
     * @return قيمة تقييمية للحالة (Score).
     */
    int minimax(Board<T>* board, int depth, bool isMaximizing, T aiSymbol, T humanSymbol);
};

#endif // PYRAMID_XO_H