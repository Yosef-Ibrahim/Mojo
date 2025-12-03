#include "Ultimate_TicTacToe_Classes.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

// ==========================================
// تنفيذ Ultimate_Board
// ==========================================

Ultimate_Board::Ultimate_Board()
    : Board<char>(9, 9), main_board(3, vector<char>(3, char())) {

    // تصفير البورد الكبير 9x9
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            board[i][j] = '.';

    // تصفير البورد المصغر 3x3
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            main_board[i][j] = '.';
}

Ultimate_Board::~Ultimate_Board() {}

bool Ultimate_Board::check_subboard_winner(int sub_r, int sub_c, char& winner) {
    int start_r = sub_r * 3;
    int start_c = sub_c * 3;

    // Rows
    for (int i = 0; i < 3; ++i) {
        char a = board[start_r + i][start_c + 0];
        char b = board[start_r + i][start_c + 1];
        char c = board[start_r + i][start_c + 2];
        if (a != '.' && a == b && b == c) { winner = a; return true; }
    }
    // Cols
    for (int j = 0; j < 3; ++j) {
        char a = board[start_r + 0][start_c + j];
        char b = board[start_r + 1][start_c + j];
        char c = board[start_r + 2][start_c + j];
        if (a != '.' && a == b && b == c) { winner = a; return true; }
    }
    // Diagonals
    char center = board[start_r + 1][start_c + 1];
    if (center != '.') {
        if (board[start_r][start_c] == center && board[start_r + 2][start_c + 2] == center) { winner = center; return true; }
        if (board[start_r][start_c + 2] == center && board[start_r + 2][start_c] == center) { winner = center; return true; }
    }
    winner = '.';
    return false;
}

bool Ultimate_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char sym = move->get_symbol();

    if (r < 0 || r >= 9 || c < 0 || c >= 9) return false;
    if (board[r][c] != '.') return false;

    // تسجيل الحركة
    board[r][c] = sym;
    n_moves++;

    // التحقق من فوز الـ sub-board
    int sub_r = r / 3;
    int sub_c = c / 3;

    if (main_board[sub_r][sub_c] == '.') {
        char w = '.';
        if (check_subboard_winner(sub_r, sub_c, w)) {
            main_board[sub_r][sub_c] = w;
        }
    }
    return true;
}

bool Ultimate_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();
    // Rows
    for (int i = 0; i < 3; ++i) {
        if (main_board[i][0] == s && main_board[i][1] == s && main_board[i][2] == s) return true;
    }
    // Cols
    for (int j = 0; j < 3; ++j) {
        if (main_board[0][j] == s && main_board[1][j] == s && main_board[2][j] == s) return true;
    }
    // Diagonals
    if (main_board[0][0] == s && main_board[1][1] == s && main_board[2][2] == s) return true;
    if (main_board[0][2] == s && main_board[1][1] == s && main_board[2][0] == s) return true;

    return false;
}

bool Ultimate_Board::is_draw(Player<char>* player) {
    if (n_moves == 81) return !is_win(player);
    return false;
}

bool Ultimate_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ==========================================
// تنفيذ Ultimate_UI (بنفس هيكلة Pyramid)
// ==========================================

Ultimate_UI::Ultimate_UI() : UI<char>("Ultimate Tic-Tac-Toe 9x9", 9) {
    this->boardPtr = nullptr;
}

Ultimate_UI::~Ultimate_UI() {}

// دالة الربط (نسخة طبق الأصل من Pyramid)
void Ultimate_UI::set_board(Board<char>* b) {
    this->boardPtr = b;
}

// دالة إنشاء اللاعب (نسخة طبق الأصل من Pyramid باستخدام &)
Player<char>* Ultimate_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Ultimate_UI::get_move(Player<char>* currentPlayer) {
    // 1. دور الإنسان
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c;
        cout << "\nTurn for " << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ")\n";
        while (true) {
            cout << "Enter coords (Row Col) [0-8]: ";
            if (!(cin >> r >> c)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (r < 0 || r > 8 || c < 0 || c > 8) { cout << "Out of range.\n"; continue; }
            if (boardPtr->get_board_matrix()[r][c] != '.') { cout << "Occupied! Try again.\n"; continue; }
            break;
        }
        return new Ultimate_Move(r, c, currentPlayer->get_symbol());
    }

    // 2. دور الكمبيوتر (Random سريع زي ما طلبت)
    else {
        cout << "\nComputer (" << currentPlayer->get_symbol() << ") is thinking...\n";

        // تجميع الأماكن الفارغة
        vector<pair<int, int>> emptyCells;
        auto& b = boardPtr->get_board_matrix();

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (b[i][j] == '.') {
                    emptyCells.push_back({ i, j });
                }
            }
        }

        // اختيار عشوائي آمن
        if (emptyCells.empty()) return new Ultimate_Move(-1, -1, currentPlayer->get_symbol());

        int randIdx = rand() % emptyCells.size();
        int r = emptyCells[randIdx].first;
        int c = emptyCells[randIdx].second;

        return new Ultimate_Move(r, c, currentPlayer->get_symbol());
    }
}

void Ultimate_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    cout << "\n     0 1 2   3 4 5   6 7 8\n";
    cout << "   +-------+-------+-------+\n";
    for (int i = 0; i < 9; ++i) {
        cout << " " << i << " | ";
        for (int j = 0; j < 9; ++j) {
            cout << matrix[i][j];
            if (j % 3 == 2) cout << " | "; else cout << ' ';
        }
        cout << "\n";
        if (i % 3 == 2) cout << "   +-------+-------+-------+\n";
    }

    // عشان نعرض حالة البورد الرئيسي لازم نعمل cast بسيط لأن ال boardPtr من نوع Board generic
    // أو نعتمد على المتغير اللي جوا Ultimate_Board
    // بما إننا عايزين نفس هيكلة Pyramid، الـ casting هنا ضروري للعرض فقط
    Ultimate_Board* uBoard = dynamic_cast<Ultimate_Board*>(boardPtr);
    if (uBoard) {
        cout << "\n   Main Board Status (Winners):\n";
        auto& mainB = uBoard->get_main_board_matrix();
        for (int i = 0; i < 3; ++i) {
            cout << "   | " << mainB[i][0] << " " << mainB[i][1] << " " << mainB[i][2] << " |\n";
        }
    }
    cout << endl;
}