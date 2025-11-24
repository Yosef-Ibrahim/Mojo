#ifndef FOURBYFOUR_CLASSES_H
#define FOURBYFOUR_CLASSES_H

#include "BoardGame_Classes.h"
#include <iostream>
#include <iomanip>
#include <limits> 

using namespace std;

// -------------------------------------------------------------------------
// 1. تعريف الكلاسات (Class Declarations)
// -------------------------------------------------------------------------

template <typename T>
class FourByFour_Board : public Board<T> {
public:
    FourByFour_Board();
    bool update_board(Move<T>* move);
    bool is_win(Player<T>* player);
    bool is_draw(Player<T>* player);
    bool game_is_over(Player<T>* player);
};

template <typename T>
class FourByFour_UI : public UI<T> {
public:
    FourByFour_UI();
    Player<T>* create_player(string& name, T symbol, PlayerType type);
    Move<T>* get_move(Player<T>* currentPlayer);
};

// -------------------------------------------------------------------------
// 2. تنفيذ الدوال (Implementation) - كله هنا عشان Templates
// -------------------------------------------------------------------------

// --- FourByFour_Board Implementation ---

template <typename T>
FourByFour_Board<T>::FourByFour_Board() : Board<T>(4, 4) {
    // Constructor عام
}

// تخصص لنوع char (عشان نملأ اللوحة نقط بدل أصفار)
template <>
FourByFour_Board<char>::FourByFour_Board() : Board<char>(4, 4) {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->columns; ++j) {
            this->board[i][j] = '.';
        }
    }
}

template <typename T>
bool FourByFour_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();
    T symbol = move->get_symbol();

    if (x >= 0 && x < this->rows && y >= 0 && y < this->columns &&
        (this->board[x][y] == '.' || this->board[x][y] == 0)) {

        if (symbol == 0) { // في حالة Undo
            this->n_moves--;
            this->board[x][y] = 0; // أو '.' لو char
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
bool FourByFour_Board<T>::is_win(Player<T>* player) {
    T sym = player->get_symbol();

    // فحص الصفوف (أي 3 ورا بعض)
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j <= 1; ++j) {
            if (this->board[i][j] == sym && this->board[i][j + 1] == sym && this->board[i][j + 2] == sym)
                return true;
        }
    }

    // فحص الأعمدة
    for (int i = 0; i <= 1; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (this->board[i][j] == sym && this->board[i + 1][j] == sym && this->board[i + 2][j] == sym)
                return true;
        }
    }

    // فحص الأقطار
    for (int i = 0; i <= 1; ++i) {
        for (int j = 0; j <= 1; ++j) {
            if (this->board[i][j] == sym && this->board[i + 1][j + 1] == sym && this->board[i + 2][j + 2] == sym)
                return true;
        }
    }

    for (int i = 0; i <= 1; ++i) {
        for (int j = 2; j < 4; ++j) {
            if (this->board[i][j] == sym && this->board[i + 1][j - 1] == sym && this->board[i + 2][j - 2] == sym)
                return true;
        }
    }

    return false;
}

template <typename T>
bool FourByFour_Board<T>::is_draw(Player<T>* player) {
    return (this->n_moves == 16 && !is_win(player));
}

template <typename T>
bool FourByFour_Board<T>::game_is_over(Player<T>* player) {
    return is_win(player) || is_draw(player);
}

// --- FourByFour_UI Implementation ---

template <typename T>
FourByFour_UI<T>::FourByFour_UI() : UI<T>("Welcome to 4x4 Tic-Tac-Toe Game", 4) {}

template <typename T>
Player<T>* FourByFour_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    return new Player<T>(name, symbol, type);
}

template <typename T>
Move<T>* FourByFour_UI<T>::get_move(Player<T>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c;
        while (true) {
            cout << currentPlayer->get_name() << ", enter Row (0-3): ";
            if (!(cin >> r) || r < 0 || r > 3) {
                cout << "Invalid row! Try again.\n";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }
        while (true) {
            cout << currentPlayer->get_name() << ", enter Column (0-3): ";
            if (!(cin >> c) || c < 0 || c > 3) {
                cout << "Invalid column! Try again.\n";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }
        return new Move<T>(r, c, currentPlayer->get_symbol());
    }
    else {
        // كمبيوتر عشوائي
        int x = rand() % 4;
        int y = rand() % 4;
        cout << "Computer " << currentPlayer->get_name() << " chose: " << x << " " << y << endl;
        return new Move<T>(x, y, currentPlayer->get_symbol());
    }
}

#endif // FOURBYFOUR_CLASSES_H