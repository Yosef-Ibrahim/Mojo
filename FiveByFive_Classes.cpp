#include "FiveByFive_Classes.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

// ==========================================
// 1. FiveByFive_Board Implementation
// ==========================================

// General Constructor
template <typename T>
FiveByFive_Board<T>::FiveByFive_Board() : Board<T>(5, 5) {}

// Specialized Constructor for char (initialize empty board)
template <>
FiveByFive_Board<char>::FiveByFive_Board() : Board<char>(5, 5) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            board[i][j] = '.';
        }
    }
}

template <typename T>
bool FiveByFive_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();
    T symbol = move->get_symbol();

    // Check bounds
    if (x < 0 || x >= 5 || y < 0 || y >= 5) return false;

    // Check if cell is empty
    if (this->board[x][y] != '.') return false;

    // Place the symbol
    this->board[x][y] = symbol;
    this->n_moves++;
    return true;
}

// Helper function to count three-in-a-row sequences for a player
template <typename T>
int FiveByFive_Board<T>::count_three_in_a_row(T symbol) {
    int count = 0;

    // Check rows (5 rows, each with 3 possible three-in-a-row sequences)
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j <= 2; ++j) {
            if (this->board[i][j] == symbol &&
                this->board[i][j + 1] == symbol &&
                this->board[i][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Check columns (5 columns, each with 3 possible three-in-a-row sequences)
    for (int i = 0; i <= 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (this->board[i][j] == symbol &&
                this->board[i + 1][j] == symbol &&
                this->board[i + 2][j] == symbol) {
                count++;
            }
        }
    }

    // Check diagonals (top-left to bottom-right)
    for (int i = 0; i <= 2; ++i) {
        for (int j = 0; j <= 2; ++j) {
            if (this->board[i][j] == symbol &&
                this->board[i + 1][j + 1] == symbol &&
                this->board[i + 2][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Check diagonals (top-right to bottom-left)
    for (int i = 0; i <= 2; ++i) {
        for (int j = 2; j < 5; ++j) {
            if (this->board[i][j] == symbol &&
                this->board[i + 1][j - 1] == symbol &&
                this->board[i + 2][j - 2] == symbol) {
                count++;
            }
        }
    }

    return count;
}

template <typename T>
bool FiveByFive_Board<T>::is_win(Player<T>* player) {
    // This game doesn't end on the first win
    // Winner is determined at the end by counting sequences
    return false;
}

template <typename T>
bool FiveByFive_Board<T>::is_draw(Player<T>* player) {
    // Game ends when only 1 square remains (24 moves total)
    return (this->n_moves == 24);
}

template <typename T>
bool FiveByFive_Board<T>::game_is_over(Player<T>* player) {
    return is_draw(player);
}

// ==========================================
// 2. FiveByFive_UI Implementation
// ==========================================

template <typename T>
FiveByFive_UI<T>::FiveByFive_UI() : UI<T>("Welcome to 5x5 Tic-Tac-Toe", 5) {}

template <typename T>
Player<T>* FiveByFive_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    return new Player<T>(name, symbol, type);
}

template <typename T>
Move<T>* FiveByFive_UI<T>::get_move(Player<T>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c;
        cout << "\n" << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ") turn.\n";

        while (true) {
            cout << "Enter Row (0-4): ";
            cin >> r;
            if (cin.fail() || r < 0 || r > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number between 0 and 4.\n";
                continue;
            }
            break;
        }

        while (true) {
            cout << "Enter Column (0-4): ";
            cin >> c;
            if (cin.fail() || c < 0 || c > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number between 0 and 4.\n";
                continue;
            }
            break;
        }

        return new Move<T>(r, c, currentPlayer->get_symbol());
    }
    else {
        // Random Computer Player
        int r = rand() % 5;
        int c = rand() % 5;
        cout << "Computer " << currentPlayer->get_name()
            << " chose (Row, Col): " << r << ", " << c << endl;
        return new Move<T>(r, c, currentPlayer->get_symbol());
    }
}

// =============================================================
// Explicit Instantiation
// =============================================================
template class FiveByFive_Board<char>;
template class FiveByFive_UI<char>;