#ifndef FIVEBYFIVE_CLASSES_H
#define FIVEBYFIVE_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>

using namespace std;

// Board class for 5x5 Tic-Tac-Toe
template <typename T>
class FiveByFive_Board : public Board<T> {
public:
    FiveByFive_Board();
    bool update_board(Move<T>* move) override;
    bool is_win(Player<T>* player) override;
    bool is_draw(Player<T>* player) override;
    bool game_is_over(Player<T>* player) override;

    // Public method for counting three-in-a-row sequences
    int count_three_in_a_row(T symbol);
};

// UI class for 5x5 Tic-Tac-Toe
template <typename T>
class FiveByFive_UI : public UI<T> {
private:
    FiveByFive_Board<T>* boardPtr = nullptr;

    // AI helper structures and methods
    struct MoveEval {
        int row, col;
        int score;
    };

    // Evaluation functions for AI
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