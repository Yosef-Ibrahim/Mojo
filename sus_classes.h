/**
 * @file sus_classes.h
 * @brief ملف الهيدر للعبة SUS.
 * @details الهدف هو تكوين كلمة "SUS" للحصول على نقاط.
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 * @date 2025-12-05
 */

#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <map>

using namespace std;

/**
 * @class SUS_Board
 * @brief لوحة اللعب لـ SUS (شبكة 3x3).
 */
class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';
    int s_score = 0;
    int u_score = 0;

public:
    SUS_Board();

    bool update_board(Move<char>* move) override;
    void display_board();  
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // دوال لتتبع النقاط
    void calculate_scores();
    int get_s_score() const { return s_score; }
    int get_u_score() const { return u_score; }
    int get_player_score(char symbol) const;

    // ==== AI FUNCTIONS ====
    pair<int, int> get_best_ai_move(char aiSymbol, char humanSymbol);
    int minimax(int depth, bool isMax, char aiSymbol, char humanSymbol);
    int evaluate_board(char aiSymbol, char humanSymbol);
    vector<pair<int, int>> get_legal_moves();


	//functions to make and undo moves for minimax
    void make_move(int r, int c, char s);
    void undo_move(int r, int c);


    bool is_sus_sequence(const vector<char>& seq) const;  
};

/**
 * @class SUS_UI
 * @brief واجهة المستخدم للعبة SUS.
 */
class SUS_UI : public UI<char> {
private:
    Board<char>* board_ptr = nullptr;
public:
    SUS_UI();
    ~SUS_UI() {};

    void set_board(Board<char>* b) { board_ptr = b; }

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief إعداد اللاعبين برموز ثابتة (S و U).
     */
    Player<char>** setup_players() override;

    Move<char>* get_move(Player<char>* currentPlayer) override;
    void display_scores();
};

#endif // SUS_CLASSES_H