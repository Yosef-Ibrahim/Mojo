/**
 * @file XO_Classes.h
 * @brief ملف الهيدر الخاص بلعبة X-O التقليدية (Tic-Tac-Toe).
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 * @date 2023-12-05
 */

#ifndef XO_CLASSES_H
#define XO_CLASSES_H

#include "BoardGame_Classes.h"
using namespace std;

/**
 * @class X_O_Board
 * @brief كلاس يمثل لوحة اللعب الخاصة بـ Tic-Tac-Toe 3x3.
 * @details يرث من Board<char> ويقوم بإدارة الحركات والفوز في شبكة 3x3.
 */
class X_O_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< الرمز المستخدم لتمثيل الخانة الفارغة.

public:
    /**
     * @brief كونستركتور البورد.
     * @details يقوم بتهيئة لوحة 3x3 فارغة.
     */
    X_O_Board();

    /**
     * @brief تحديث اللوحة بحركة اللاعب.
     * @param move مؤشر للحركة (x, y, symbol).
     * @return true لو الحركة تمت بنجاح.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief التحقق من الفوز (3 رموز متشابهة).
     * @param player اللاعب الحالي.
     * @return true لو اللاعب كسب.
     */
    bool is_win(Player<char>* player);

    /**
     * @brief التحقق من التعادل.
     * @return true لو اللوحة اتملت ومفيش فايز.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief التحقق من انتهاء اللعبة.
     */
    bool game_is_over(Player<char>* player);
};

/**
 * @class XO_UI
 * @brief واجهة المستخدم للعبة X-O.
 */
class XO_UI : public UI<char> {
public:
    XO_UI();
    ~XO_UI() {};

    /**
     * @brief إنشاء لاعب للعبة X-O.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /**
     * @brief استقبال حركة من اللاعب.
     */
    Move<char>* get_move(Player<char>* currentPlayer);
};

#endif