/**
 * @file FourByFour_Classes.h
 * @brief ملف الهيدر للعبة 4x4 Tic-Tac-Toe (Moving Pieces).
 * @details اللعبة تعتمد على تحريك القطع وليس وضعها فقط، وتحتوي على ذكاء اصطناعي (Minimax).
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 */

#ifndef FOURBYFOUR_CLASSES_H
#define FOURBYFOUR_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// ==========================================
// كلاس البورد (FourByFour_Board)
// ==========================================

/**
 * @class FourByFour_Board
 * @brief لوحة بحجم 4x4.
 * @details القطع توضع في البداية ثم يتم تحريكها.
 */
template <typename T>
class FourByFour_Board : public Board<T> {
public:
    /**
     * @brief تهيئة اللوحة وتوزيع القطع المبدئي.
     */
    FourByFour_Board();

    /**
     * @brief تحريك قطعة من مكان لآخر.
     * @details يتأكد من أن الحركة قانونية (لخانة مجاورة فارغة).
     */
    bool update_board(Move<T>* move) override;

    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;
};

// ==========================================
// كلاس الواجهة (FourByFour_UI)
// ==========================================

/**
 * @class FourByFour_UI
 * @brief واجهة المستخدم للعبة 4x4، تحتوي على منطق الذكاء الاصطناعي.
 */
template <typename T>
class FourByFour_UI : public UI<T> {
private:
    /**
     * @brief مؤشر للبورد لتمكين الـ AI من قراءة الحالة.
     */
    Board<T>* boardPtr;

public:
    FourByFour_UI();

    /**
     * @brief ربط الواجهة بالبورد.
     * @param b مؤشر للبورد الحالي.
     */
    void set_board(Board<T>* b);

    /**
     * @brief إنشاء لاعب.
     */
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;

    /**
     * @brief الحصول على حركة.
     * @details لو اللاعب AI، يتم استدعاء خوارزمية Minimax لتحديد أفضل نقلة.
     */
    Move<T>* get_move(Player<T>* currentPlayer) override;

    /**
     * @brief خوارزمية Minimax مع Alpha-Beta Pruning.
     * @details تحسب أفضل حركة ممكنة للكمبيوتر عن طريق تجربة كل الاحتمالات المستقبلية.
     * @param board حالة اللوحة الحالية.
     * @param depth عمق البحث.
     * @param isMaximizing دور مين (الكمبيوتر ولا الخصم).
     * @param aiSymbol رمز الكمبيوتر.
     * @param humanSymbol رمز الإنسان.
     * @param alpha قيمة Alpha للتقليم.
     * @param beta قيمة Beta للتقليم.
     * @return قيمة تقييمية (Score) للحركة.
     */
    int minimax(Board<T>* board, int depth, bool isMaximizing, T aiSymbol, T humanSymbol, int alpha, int beta);
};

#endif