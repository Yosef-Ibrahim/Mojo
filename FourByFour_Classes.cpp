#include "FourByFour_Classes.h"
#include <iostream>
#include <iomanip>
#include <limits> 
#include <cmath>

using namespace std;

// --- Board Implementation ---
template <typename T>
FourByFour_Board<T>::FourByFour_Board() : Board<T>(4, 4) {}

template <>
FourByFour_Board<char>::FourByFour_Board() : Board<char>(4, 4) {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            board[i][j] = '.';

    for (int j = 0; j < 4; ++j) {
        if (j % 2 == 0) { board[0][j] = 'X'; board[3][j] = 'O'; }
        else { board[0][j] = 'O'; board[3][j] = 'X'; }
    }
}

template <typename T>
bool FourByFour_Board<T>::update_board(Move<T>* move) {
    int from_code = move->get_x();
    int to_code = move->get_y();
    int from_r = from_code / 10, from_c = from_code % 10;
    int to_r = to_code / 10, to_c = to_code % 10;

    if (from_r < 0 || from_r > 3 || from_c < 0 || from_c > 3 ||
        to_r < 0 || to_r > 3 || to_c < 0 || to_c > 3) return false;

    if (this->board[from_r][from_c] != move->get_symbol()) return false;
    if (this->board[to_r][to_c] != '.') return false;
    if (abs(from_r - to_r) + abs(from_c - to_c) != 1) return false;

    this->board[to_r][to_c] = this->board[from_r][from_c];
    this->board[from_r][from_c] = '.';
    this->n_moves++;
    return true;
}

template <typename T>
bool FourByFour_Board<T>::is_win(Player<T>* player) {
    T sym = player->get_symbol();
    auto& b = this->board;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j <= 1; j++) {
            if (b[i][j] == sym && b[i][j + 1] == sym && b[i][j + 2] == sym) return true;
            if (b[j][i] == sym && b[j + 1][i] == sym && b[j + 2][i] == sym) return true;
        }
    }
    return false;
}

template <typename T>
bool FourByFour_Board<T>::is_draw(Player<T>* player) {
    return (this->n_moves >= 50);
}

template <typename T>
bool FourByFour_Board<T>::game_is_over(Player<T>* player) {
    return is_win(player) || is_draw(player);
}

// --- UI Implementation ---
template <typename T>
FourByFour_UI<T>::FourByFour_UI() : UI<T>("4x4 Tic-Tac-Toe (Moving Pieces)", 4) {
    this->boardPtr = nullptr;
}

template <typename T>
void FourByFour_UI<T>::set_board(Board<T>* b) {
    this->boardPtr = b;
}

// التعديل: string&
template <typename T>
Player<T>* FourByFour_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    return new Player<T>(name, symbol, type);
}

// AI Minimax
template <typename T>
int FourByFour_UI<T>::minimax(Board<T>* board, int depth, bool isMaximizing, T aiSymbol, T humanSymbol, int alpha, int beta) {
    Player<T> aiPlayer("AI", aiSymbol, PlayerType::AI);
    Player<T> humanPlayer("Human", humanSymbol, PlayerType::HUMAN);

    if (board->is_win(&aiPlayer)) return 1000 - depth;
    if (board->is_win(&humanPlayer)) return -1000 + depth;
    if (depth >= 3) return 0;

    auto& b = board->get_board_matrix();

    if (isMaximizing) {
        int maxEval = -100000;
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                if (b[r][c] == aiSymbol) {
                    int dr[] = { -1, 1, 0, 0 }, dc[] = { 0, 0, -1, 1 };
                    for (int k = 0; k < 4; ++k) {
                        int nr = r + dr[k], nc = c + dc[k];
                        if (nr >= 0 && nr < 4 && nc >= 0 && nc < 4 && b[nr][nc] == '.') {
                            b[nr][nc] = aiSymbol; b[r][c] = '.';
                            int eval = minimax(board, depth + 1, false, aiSymbol, humanSymbol, alpha, beta);
                            b[r][c] = aiSymbol; b[nr][nc] = '.';
                            maxEval = max(maxEval, eval);
                            alpha = max(alpha, eval);
                            if (beta <= alpha) break;
                        }
                    }
                }
            }
        }
        return maxEval;
    }
    else {
        int minEval = 100000;
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                if (b[r][c] == humanSymbol) {
                    int dr[] = { -1, 1, 0, 0 }, dc[] = { 0, 0, -1, 1 };
                    for (int k = 0; k < 4; ++k) {
                        int nr = r + dr[k], nc = c + dc[k];
                        if (nr >= 0 && nr < 4 && nc >= 0 && nc < 4 && b[nr][nc] == '.') {
                            b[nr][nc] = humanSymbol; b[r][c] = '.';
                            int eval = minimax(board, depth + 1, true, aiSymbol, humanSymbol, alpha, beta);
                            b[r][c] = humanSymbol; b[nr][nc] = '.';
                            minEval = min(minEval, eval);
                            beta = min(beta, eval);
                            if (beta <= alpha) break;
                        }
                    }
                }
            }
        }
        return minEval;
    }
}

template <typename T>
Move<T>* FourByFour_UI<T>::get_move(Player<T>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int from_r, from_c, to_r, to_c;
        cout << "\nTurn for " << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ")\n";
        while (true) {
            cout << "Select piece to move (Row Col): "; cin >> from_r >> from_c;
            if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            break;
        }
        while (true) {
            cout << "Select destination (Row Col): "; cin >> to_r >> to_c;
            if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            break;
        }
        return new Move<T>(from_r * 10 + from_c, to_r * 10 + to_c, currentPlayer->get_symbol());
    }
    else {
        cout << "\nComputer (AI) is thinking...\n";
        T aiSymbol = currentPlayer->get_symbol();
        T humanSymbol = (aiSymbol == 'X') ? 'O' : 'X';

        int bestFrom = -1, bestTo = -1;
        int bestScore = -100000;
        auto& b = this->boardPtr->get_board_matrix();

        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                if (b[r][c] == aiSymbol) {
                    int dr[] = { -1, 1, 0, 0 }, dc[] = { 0, 0, -1, 1 };
                    for (int k = 0; k < 4; ++k) {
                        int nr = r + dr[k], nc = c + dc[k];
                        if (nr >= 0 && nr < 4 && nc >= 0 && nc < 4 && b[nr][nc] == '.') {
                            b[nr][nc] = aiSymbol; b[r][c] = '.';
                            int score = minimax(this->boardPtr, 0, false, aiSymbol, humanSymbol, -100000, 100000);
                            b[r][c] = aiSymbol; b[nr][nc] = '.';
                            if (score > bestScore) {
                                bestScore = score;
                                bestFrom = r * 10 + c;
                                bestTo = nr * 10 + nc;
                            }
                        }
                    }
                }
            }
        }

        if (bestFrom == -1) {
            while (true) {
                int r = rand() % 4, c = rand() % 4;
                if (b[r][c] == aiSymbol) {
                    int k = rand() % 4, dr[] = { -1,1,0,0 }, dc[] = { 0,0,-1,1 };
                    int nr = r + dr[k], nc = c + dc[k];
                    if (nr >= 0 && nr < 4 && nc >= 0 && nc < 4 && b[nr][nc] == '.') return new Move<T>(r * 10 + c, nr * 10 + nc, aiSymbol);
                }
            }
        }
        return new Move<T>(bestFrom, bestTo, aiSymbol);
    }
}

template class FourByFour_Board<char>;
template class FourByFour_UI<char>;