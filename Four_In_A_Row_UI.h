/**
 * @file Four_In_A_Row_UI.h
 * @brief ملف الهيدر للعبة Connect 4 (أربعة في صف).
 * @details اللعبة تعتمد على الجاذبية، القطع تسقط لأدنى صف متاح في العمود.
 * @author Shahd Ayman ,Youssef Ibrahim,  Marwan Medhat and Mohamed Rami
 */

#ifndef FOUR_IN_A_ROW_UI_H
#define FOUR_IN_A_ROW_UI_H

#include "BoardGame_Classes.h"

using namespace std;

/**
 * @class Four_In_A_Row_Board
 * @brief لوحة اللعب الخاصة بـ Connect 4.
 * @details تدير منطق سقوط القطع والفوز بتوصيل 4 قطع.
 */
class Four_In_A_Row_Board : public Board<char>
{
public:
    /**
     * @brief كونستركتور، الأبعاد الافتراضية 6x7.
     */
    Four_In_A_Row_Board(int r = 6, int c = 7);

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief العثور على الصف المتاح في عمود معين (محاكاة الجاذبية).
     */
    int find_lowest_row(int column);

    // دوال الذكاء الاصطناعي
    int evaluate_board(char aiSym);
    int get_best_ai_move(char aiSym);
    int minimax(int depth, bool maximizing, char aiSym, int alpha, int beta);
    bool is_win_char(char sym);
};

/**
 * @class Four_In_A_Row_UI
 * @brief واجهة المستخدم للعبة Connect 4.
 */
class Four_In_A_Row_UI : public UI<char>
{
private:
    Board<char>* boardPtr;

public:
    Four_In_A_Row_UI(Board<char>* board);
    virtual ~Four_In_A_Row_UI();

    Move<char>* get_move(Player<char>* player) override;
    Player<char>** setup_players() override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif // FOUR_IN_A_ROW_UI_H