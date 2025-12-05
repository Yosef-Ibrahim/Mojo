/**
 * @file FiveByFive_Classes.h
 * @brief ملف الهيدر للعبة 5x5 Tic-Tac-Toe.
 * @details الفوز يعتمد على من يملك أكبر عدد من تسلسلات 3-في-صف.
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 * @date 2025-12-05
 */

#ifndef FIVEBYFIVE_CLASSES_H
#define FIVEBYFIVE_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>

using namespace std;

/**
 * @class FiveByFive_Board
 * @brief لوحة اللعب بحجم 5x5.
 * @details تدير حساب عدد التسلسلات الثلاثية لكل لاعب.
 */
template <typename T>
class FiveByFive_Board : public Board<T> {
public:
    FiveByFive_Board();

    /**
     * @brief تحديث اللوحة.
     */
    bool update_board(Move<T>* move) override;

    /**
     * @brief التحقق من الفوز.
     * @details في هذه اللعبة، الفوز لا ينهي اللعبة فوراً بل يتم حسابه عند امتلاء اللوحة.
     */
    bool is_win(Player<T>* player) override;

    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;

    /**
     * @brief عد التسلسلات المكونة من 3 رموز متطابقة.
     * @param symbol رمز اللاعب.
     * @return عدد التسلسلات.
     */
    int count_three_in_a_row(T symbol);
};

/**
 * @class FiveByFive_UI
 * @brief واجهة المستخدم للعبة 5x5.
 */
template <typename T>
class FiveByFive_UI : public UI<T> {
private:
    FiveByFive_Board<T>* boardPtr = nullptr;

    struct MoveEval {
        int row, col;
        int score;
    };

    // دوال التقييم للذكاء الاصطناعي
    int evaluate_position(FiveByFive_Board<T>* board, T ai_symbol, T opp_symbol);
    int count_potential_sequences(FiveByFive_Board<T>* board, int row, int col, T symbol);
    bool creates_three_in_row(FiveByFive_Board<T>* board, int row, int col, T symbol);
    Move<T>* get_ai_move(Player<T>* player);

public:
    FiveByFive_UI();
    ~FiveByFive_UI() {};

    void set_board(FiveByFive_Board<T>* b) { boardPtr = b; }

    Player<T>** setup_players() override;
    Player<T>* create_player(string& name, T symbol, PlayerType type) override;
    Move<T>* get_move(Player<T>* currentPlayer) override;
};

#endif // FIVEBYFIVE_CLASSES_H