/**
 * @file Misere.h
 * @brief ملف الهيدر للعبة Misere Tic-Tac-Toe.
 * @details في هذه اللعبة، اللاعب الذي يحقق 3-في-صف **يخسر** بدلاً من أن يفوز.
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 * @date 2025-12-05
 */

#pragma once
#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

/**
 * @class Misere_Board
 * @brief لوحة اللعب لمنطق Misere.
 */
class Misere_Board : public Board<char> {
private:
    char blank_symbol = '.';

public:
    Misere_Board();
    bool update_board(Move<char>* move) override;

    /**
     * @brief هل فاز اللاعب؟
     * @details يتم عكس المنطق هنا، الفوز يعني بقاء الخصم دون حركات آمنة.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief هل خسر اللاعب؟
     * @details اللاعب يخسر إذا كون 3 رموز متتالية.
     */
    bool is_lose(Player<char>* player) override;

    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // دوال مساعدة للـ AI
    bool check_lose(const vector<vector<char>>& board, char symbol) const;
    bool is_board_full(const vector<vector<char>>& board) const;
};

/**
 * @class Misere_AI_Player
 * @brief لاعب ذكي خاص بلعبة Misere.
 * @details يستخدم خوارزمية Minimax لتجنب الخسارة.
 */
class Misere_AI_Player : public Player<char> {
private:
    Misere_Board* board_ptr;

    int minimax(vector<vector<char>>& board, int depth, bool is_maximizing,
        char ai_symbol, char opponent_symbol, int alpha, int beta);

public:
    Misere_AI_Player(string n, char s, Misere_Board* board)
        : Player<char>(n, s, PlayerType::AI), board_ptr(board) {}

    /**
     * @brief الحصول على أفضل حركة تتجنب الخسارة.
     */
    pair<int, int> get_best_move(char opponent_symbol);
};

/**
 * @class Misere_UI
 * @brief واجهة المستخدم للعبة Misere.
 */
class Misere_UI : public UI<char> {
private:
    Misere_Board* board_ptr;

public:
    Misere_UI();
    ~Misere_UI() = default;

    void set_board(Misere_Board* board);
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* currentPlayer) override;

    Player<char>** setup_players() override;
};