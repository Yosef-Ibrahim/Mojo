#ifndef MEMORY_TICTACTOE_H
#define MEMORY_TICTACTOE_H

#include "BoardGame_Classes.h"
#include <string>

using namespace std;

class Memory_Board : public Board<char> {
private:
    vector<vector<char>> hidden_board;  // تخزن العلامات الحقيقية

public:
    Memory_Board();
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // دالة خاصة لعرض اللوحة الخفية (للتجربة فقط)
    void display_hidden_board() const;
};

class Memory_UI : public UI<char> {
private:
    Memory_Board* boardPtr;  // مؤشر للوصول إلى اللوحة الخفية

public:
    Memory_UI();
    ~Memory_UI();

    void set_board(Memory_Board* b) { boardPtr = b; }

    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* currentPlayer) override;

    // إعادة تعريف display_board_matrix لعرض لوحة فارغة
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif