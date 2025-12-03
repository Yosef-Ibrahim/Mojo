#pragma once
#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

// Misere Tic-Tac-Toe Board
class Misere_Board : public Board<char> {
private:
    char blank_symbol = '.';

public:
    Misere_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Helper functions for AI
    bool check_lose(const vector<vector<char>>& board, char symbol) const;
    bool is_board_full(const vector<vector<char>>& board) const;
};

// Smart AI Player for Misere Tic-Tac-Toe
class Misere_AI_Player : public Player<char> {
private:
    Misere_Board* board_ptr;

    // Minimax algorithm with alpha-beta pruning
    int minimax(vector<vector<char>>& board, int depth, bool is_maximizing,
        char ai_symbol, char opponent_symbol, int alpha, int beta);

public:
    Misere_AI_Player(string n, char s, Misere_Board* board)
        : Player<char>(n, s, PlayerType::AI), board_ptr(board) {}

    // Find the best move using Minimax
    pair<int, int> get_best_move(char opponent_symbol);
};

// Misere Game UI
class Misere_UI : public UI<char> {
private:
    Misere_Board* board_ptr;

public:
    Misere_UI();
    ~Misere_UI() = default;

    void set_board(Misere_Board* board);
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* currentPlayer) override;

    // 🔥 هذه هي الإضافة: دالة setup_players الخاصة بـ Misere فقط
    Player<char>** setup_players() override;
};


