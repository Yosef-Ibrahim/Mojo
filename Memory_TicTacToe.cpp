#include "Memory_TicTacToe.h"
#include <iostream>
#include <vector>

using namespace std;

// ========== Memory_Board Implementation ==========

Memory_Board::Memory_Board() : Board(3, 3), hidden_board(3, vector<char>(3, ' ')) {
    // تبدأ اللوحة الظاهرة فارغة (board matrix من الـ parent)
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

bool Memory_Board::update_board(Move<char>* move) {
    int x = move->get_x(), y = move->get_y();

    // التحقق من حدود اللوحة
    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        cout << "Invalid position! Please enter 0-2 for row and column.\n";
        return false;
    }

    // التحقق إذا كانت الخلية مشغولة (في الذاكرة الخفية)
    if (hidden_board[x][y] != ' ') {
        cout << "This cell is already taken! Try another one.\n";
        return false;
    }

    // وضع العلامة في اللوحة الخفية فقط
    hidden_board[x][y] = move->get_symbol();

    // تحديث العداد
    n_moves++;

    return true;
}

bool Memory_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();

    // التحقق من الصفوف
    for (int i = 0; i < 3; i++) {
        if (hidden_board[i][0] == sym &&
            hidden_board[i][1] == sym &&
            hidden_board[i][2] == sym)
            return true;
    }

    // التحقق من الأعمدة
    for (int i = 0; i < 3; i++) {
        if (hidden_board[0][i] == sym &&
            hidden_board[1][i] == sym &&
            hidden_board[2][i] == sym)
            return true;
    }

    // التحقق من الأقطاب
    if (hidden_board[0][0] == sym &&
        hidden_board[1][1] == sym &&
        hidden_board[2][2] == sym)
        return true;

    if (hidden_board[0][2] == sym &&
        hidden_board[1][1] == sym &&
        hidden_board[2][0] == sym)
        return true;

    return false;
}

bool Memory_Board::is_draw(Player<char>* player) {
    // تعادل إذا امتلأت اللوحة ولا يوجد فائز
    return (n_moves == 9 && !is_win(player));
}

bool Memory_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

void Memory_Board::display_hidden_board() const {
    cout << "\n--- Hidden Board (For Debugging) ---\n";
    cout << "    0   1   2\n";
    cout << "  +---+---+---+\n";
    for (int i = 0; i < 3; i++) {
        cout << i << " | ";
        for (int j = 0; j < 3; j++) {
            if (hidden_board[i][j] == ' ')
                cout << "  | ";
            else
                cout << hidden_board[i][j] << " | ";
        }
        cout << "\n  +---+---+---+\n";
    }
    cout << endl;
}

// ========== Memory_UI Implementation ==========

Memory_UI::Memory_UI() : UI<char>("Memory Tic-Tac-Toe", 3), boardPtr(nullptr) {
    // Constructor
}

Memory_UI::~Memory_UI() {
    // Destructor
}

Player<char>* Memory_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Memory_UI::get_move(Player<char>* currentPlayer) {
    int x, y;
    cout << "\n" << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ") - ";
    cout << "Enter row and column (0-2 each): ";
    cin >> x >> y;
    return new Move<char>(x, y, currentPlayer->get_symbol());
}

void Memory_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    // تعرض لوحة فارغة دائماً
    cout << "\n============ MEMORY BOARD ============\n";
    cout << "   (All marks are hidden from view)\n";
    cout << "=====================================\n\n";

    // رسم اللوحة الفارغة
    cout << "    0   1   2\n";
    cout << "  +---+---+---+\n";
    for (int i = 0; i < 3; i++) {
        cout << i << " |   |   |   |\n";
        cout << "  +---+---+---+\n";
    }

    // إظهار عدد الخطوات المتبقية
    if (boardPtr) {
        cout << "\nMoves made: " << boardPtr->get_board_matrix().size() * 3 - boardPtr->get_rows() * boardPtr->get_columns() << endl;
    }
}