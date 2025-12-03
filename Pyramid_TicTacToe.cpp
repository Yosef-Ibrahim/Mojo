#include "Pyramid_TicTacToe.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

// ==========================================================
// تنفيذ كلاس Pyramid_XO_Board
// ==========================================================

template <typename T>
Pyramid_XO_Board<T>::Pyramid_XO_Board() : Board<T>(3, 5) {
}

template <>
Pyramid_XO_Board<char>::Pyramid_XO_Board() : Board<char>(3, 5) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            this->board[i][j] = '.';
        }
    }
}

template <typename T>
bool Pyramid_XO_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();

    if (x < 0 || x > 2 || y < 0 || y > 4) return false;

    // أماكن الهرم الصحيحة
    bool validSpot = (x == 0 && y == 2) ||
        (x == 1 && y >= 1 && y <= 3) ||
        (x == 2);

    if (!validSpot) return false;
    if (this->board[x][y] != '.') return false;

    this->board[x][y] = move->get_symbol();
    this->n_moves++;
    return true;
}

template <typename T>
bool Pyramid_XO_Board<T>::is_win(Player<T>* player) {
    T sym = player->get_symbol();
    auto& b = this->board;

    if (b[0][2] == sym && b[1][2] == sym && b[2][2] == sym) return true;
    if (b[1][1] == sym && b[1][2] == sym && b[1][3] == sym) return true;
    if (b[2][0] == sym && b[2][1] == sym && b[2][2] == sym) return true;
    if (b[2][1] == sym && b[2][2] == sym && b[2][3] == sym) return true;
    if (b[2][2] == sym && b[2][3] == sym && b[2][4] == sym) return true;
    if (b[0][2] == sym && b[1][1] == sym && b[2][0] == sym) return true;
    if (b[0][2] == sym && b[1][3] == sym && b[2][4] == sym) return true;

    return false;
}

template <typename T>
bool Pyramid_XO_Board<T>::is_lose(Player<T>* player) {
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

// ==========================================================
// تنفيذ كلاس Pyramid_XO_UI
// ==========================================================

template <typename T>
Pyramid_XO_UI<T>::Pyramid_XO_UI() : UI<T>("Pyramid Tic-Tac-Toe", 3) {
    this->boardPtr = nullptr;
}

template <typename T>
void Pyramid_XO_UI<T>::set_board(Board<T>* b) {
    this->boardPtr = b;
}

// التعديل: string& (بالمرجع) لتطابق الأصل
template <typename T>
Player<T>* Pyramid_XO_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    return new Player<T>(name, symbol, type);
}

// --- Minimax ---
template <typename T>
int Pyramid_XO_UI<T>::minimax(Board<T>* board, int depth, bool isMaximizing, T aiSymbol, T humanSymbol) {
    Player<T> aiPlayer("AI", aiSymbol, PlayerType::AI);
    Player<T> humanPlayer("Human", humanSymbol, PlayerType::HUMAN);

    if (board->is_win(&aiPlayer)) return 10 - depth;
    if (board->is_win(&humanPlayer)) return depth - 10;

    bool full = true;
    auto& b = board->get_board_matrix();
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 5; ++j)
            if (((i == 0 && j == 2) || (i == 1 && j >= 1 && j <= 3) || (i == 2)) && b[i][j] == '.') full = false;

    if (full) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 5; ++j) {
                if ((i == 0 && j == 2) || (i == 1 && j >= 1 && j <= 3) || (i == 2)) {
                    if (b[i][j] == '.') {
                        b[i][j] = aiSymbol;
                        int score = minimax(board, depth + 1, false, aiSymbol, humanSymbol);
                        b[i][j] = '.';
                        bestScore = max(score, bestScore);
                    }
                }
            }
        }
        return bestScore;
    }
    else {
        int bestScore = 1000;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 5; ++j) {
                if ((i == 0 && j == 2) || (i == 1 && j >= 1 && j <= 3) || (i == 2)) {
                    if (b[i][j] == '.') {
                        b[i][j] = humanSymbol;
                        int score = minimax(board, depth + 1, true, aiSymbol, humanSymbol);
                        b[i][j] = '.';
                        bestScore = min(score, bestScore);
                    }
                }
            }
        }
        return bestScore;
    }
}

template <typename T>
Move<T>* Pyramid_XO_UI<T>::get_move(Player<T>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\nTurn for " << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ")\n";
        cout << "Valid Coords: Top(0 2) | Middle(1 1-3) | Base(2 0-4)\n";
        while (true) {
            cout << "Enter coords (x y): ";
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
        // AI Logic
        cout << "\nComputer (AI) is thinking...\n";
        T aiSymbol = currentPlayer->get_symbol();
        T humanSymbol = (aiSymbol == 'X') ? 'O' : 'X';

        int bestScore = -10000;

        // التعديل الأهم لحل إيرور set_x
        // بنستخدم متغيرات عادية نخزن فيها أفضل مكان
        int bestX = -1;
        int bestY = -1;

        auto& b = this->boardPtr->get_board_matrix();
        bool firstMove = true;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 5; ++j) {
                if ((i == 0 && j == 2) || (i == 1 && j >= 1 && j <= 3) || (i == 2)) {
                    if (b[i][j] == '.') {
                        b[i][j] = aiSymbol;
                        int score = minimax(this->boardPtr, 0, false, aiSymbol, humanSymbol);
                        b[i][j] = '.';

                        if (score > bestScore || firstMove) {
                            bestScore = score;
                            // حفظنا الإحداثيات في متغيرات عادية
                            bestX = i;
                            bestY = j;
                            firstMove = false;
                        }
                    }
                }
            }
        }
        // هنا بنعمل كائن الحركة مرة واحدة في الآخر
        return new Move<T>(bestX, bestY, aiSymbol);
    }
}

// Explicit instantiation
template class Pyramid_XO_Board<char>;
template class Pyramid_XO_UI<char>;