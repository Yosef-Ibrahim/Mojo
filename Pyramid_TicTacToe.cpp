#include "Pyramid_TicTacToe.h" // 👈 تم التصحيح ليتطابق مع اسم الملف
#include <iostream>
#include <iomanip>
#include <limits> 

using namespace std;

// ==========================================
// 1. Pyramid_XO_Board Implementation
// ==========================================

template <typename T>
Pyramid_XO_Board<T>::Pyramid_XO_Board() : Board<T>(3, 5) {
    // 3 rows, max 5 columns
}

// Specialization for char
template <>
Pyramid_XO_Board<char>::Pyramid_XO_Board() : Board<char>(3, 5) {
    // Initialize with dots
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            board[i][j] = '.';
        }
    }
}

template <typename T>
bool Pyramid_XO_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();
    T symbol = move->get_symbol();

    // Validations specifically for the Pyramid shape
    // Row 0: only column 2 is valid
    // Row 1: columns 1, 2, 3 are valid
    // Row 2: columns 0, 1, 2, 3, 4 are valid

    bool isValidPos = false;
    if (x == 0 && y == 2) isValidPos = true;
    else if (x == 1 && (y >= 1 && y <= 3)) isValidPos = true;
    else if (x == 2 && (y >= 0 && y <= 4)) isValidPos = true;

    if (isValidPos && (this->board[x][y] == '.' || symbol == 0)) {
        if (symbol == 0) {
            this->n_moves--;
            this->board[x][y] = '.';
        }
        else {
            this->n_moves++;
            this->board[x][y] = symbol;
        }
        return true;
    }
    return false;
}

template <typename T>
bool Pyramid_XO_Board<T>::is_win(Player<T>* player) {
    T s = player->get_symbol();

    // Check Horizontals
    // Row 1 (indices 1,2,3)
    if (this->board[1][1] == s && this->board[1][2] == s && this->board[1][3] == s) return true;
    // Row 2 (indices 0,1,2 or 1,2,3 or 2,3,4)
    if (this->board[2][0] == s && this->board[2][1] == s && this->board[2][2] == s) return true;
    if (this->board[2][1] == s && this->board[2][2] == s && this->board[2][3] == s) return true;
    if (this->board[2][2] == s && this->board[2][3] == s && this->board[2][4] == s) return true;

    // Check Verticals (Only one vertical column: col 2)
    if (this->board[0][2] == s && this->board[1][2] == s && this->board[2][2] == s) return true;

    // Check Diagonals
    // Main Diagonal type (\)
    if (this->board[0][2] == s && this->board[1][3] == s && this->board[2][4] == s) return true;
    // Other Diagonal type (/)
    if (this->board[0][2] == s && this->board[1][1] == s && this->board[2][0] == s) return true;

    return false;
}

template <typename T>
bool Pyramid_XO_Board<T>::is_draw(Player<T>* player) {
    return (this->n_moves == 9 && !is_win(player));
}

template <typename T>
bool Pyramid_XO_Board<T>::game_is_over(Player<T>* player) {
    return is_win(player) || is_draw(player);
}

// ==========================================
// 2. Pyramid_XO_UI Implementation
// ==========================================

template <typename T>
Pyramid_XO_UI<T>::Pyramid_XO_UI() : UI<T>("Welcome to Pyramid Tic-Tac-Toe", 3) {}

template <typename T>
Player<T>* Pyramid_XO_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    return new Player<T>(name, symbol, type);
}

template <typename T>
Move<T>* Pyramid_XO_UI<T>::get_move(Player<T>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\n" << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ") turn.\n";
        // إضافة توضيح للإحداثيات عشان اللاعب يعرف يختار
        cout << "Coords: Top(0 2) | Middle(1 1-3) | Base(2 0-4)\n";
        while (true) {
            cout << "Enter coordinate (Row Col): ";
            cin >> x >> y;
            if (cin.fail()) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }
        return new Move<T>(x, y, currentPlayer->get_symbol());
    }
    else {
        // Random Computer
        int x, y;
        // Keep trying until a valid spot for pyramid is found
        for (int k = 0; k < 1000; k++) {
            x = rand() % 3;
            y = rand() % 5;
            // Basic validation before sending to board
            bool valid = false;
            if (x == 0 && y == 2) valid = true;
            else if (x == 1 && y >= 1 && y <= 3) valid = true;
            else if (x == 2 && y >= 0 && y <= 4) valid = true;

            if (valid) return new Move<T>(x, y, currentPlayer->get_symbol());
        }
        return new Move<T>(0, 2, currentPlayer->get_symbol());
    }
}

// Explicit Instantiation
template class Pyramid_XO_Board<char>;
template class Pyramid_XO_UI<char>;