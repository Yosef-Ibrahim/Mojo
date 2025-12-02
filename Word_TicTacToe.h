#ifndef WORD_TICTACTOE_H
#define WORD_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <set>
#include <string>
#include <fstream>

using namespace std;

// -------------------------------------------------------
// Word Board Class
// -------------------------------------------------------
class Word_Board : public Board<char> {
private:
    char blank_symbol = '.';
    set<string> dictionary; // Dictionary of valid 3-letter words

    bool load_dictionary(const string& filename);

public:
    Word_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Add this declaration inside the `public:` section of the `Word_Board` class.
    // It makes the existing `Word_Board::check_word_at` implementation accessible
    // to `Word_UI` and other callers.
    bool check_word_at(int row, int col, int dr, int dc) const;

    // Helper functions for AI
    bool is_valid_word(const string& word) const;
    set<string> get_dictionary() const { return dictionary; }
};

// -------------------------------------------------------
// Word UI Class
// -------------------------------------------------------
class Word_UI : public UI<char> {
private:
    Word_Board* boardPtr = nullptr;

    // AI helper functions
    struct MoveScore {
        int row, col;
        char letter;
        int score;
    };

    int evaluate_move(Word_Board* board, int row, int col, char letter, char ai_symbol);
    bool creates_winning_word(Word_Board* board, int row, int col, char letter);
    bool blocks_opponent_win(Word_Board* board, int row, int col, char letter, char opponent_symbol);
    int count_potential_words(Word_Board* board, int row, int col, char letter);
    Move<char>* get_ai_move(Player<char>* player);

public:
    Word_UI();
    ~Word_UI() {};

    void set_board(Word_Board* b) { boardPtr = b; }

    Player<char>** setup_players() override;
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* currentPlayer) override;
};

#endif