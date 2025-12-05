/**
 * @file Obstacles_Classes.h
 * @brief ملف الهيدر الخاص بلعبة العوائق (Obstacles Tic-Tac-Toe).
 * @details لعبة شبيهة بـ XO لكن الكمبيوتر يضع عوائق عشوائية تمنع اللعب في خانات معينة.
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 */

#ifndef OBSTACLES_CLASSES_H
#define OBSTACLES_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>

using namespace std;

// ==========================================
// كلاس البورد (Obstacles_Board)
// ==========================================

/**
 * @class Obstacles_Board
 * @brief لوحة اللعب الخاصة بلعبة العوائق.
 * @details حجم اللوحة عادة أكبر (مثلاً 5x5) وتحتوي على منطق لإضافة عوائق.
 */
template <typename T>
class Obstacles_Board : public Board<T> {
public:
    /**
     * @brief الكونستركتور.
     * @details يجهز البورد ويستدعي دالة وضع العوائق.
     */
    Obstacles_Board();

    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;

    /**
     * @brief إضافة عوائق عشوائية.
     * @details تقوم بوضع رموز (مثلاً 'X' أو لون مختلف) في أماكن عشوائية لا يمكن اللعب فيها.
     */
    void add_random_obstacles();
};

// ==========================================
// كلاس الواجهة (Obstacles_UI)
// ==========================================

/**
 * @class Obstacles_UI
 * @brief واجهة المستخدم للعبة العوائق.
 */
template <typename T>
class Obstacles_UI : public UI<T> {
public:
    Obstacles_UI();
    ~Obstacles_UI() {};

    /**
     * @brief إنشاء لاعب جديد.
     * @param name اسم اللاعب (بالمرجع).
     */
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;

    /**
     * @brief الحصول على حركة.
     * @details يطلب إحداثيات من المستخدم ويتأكد أنها ليست عائقاً.
     */
    Move<T>* get_move(Player<T>* currentPlayer) override;
};

#endif // OBSTACLES_CLASSES_H