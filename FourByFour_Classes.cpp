#include "FourByFour_Classes.h"
#include <iostream>
#include <iomanip>
#include <limits> 
#include <cmath> // عشان دالة abs

using namespace std;

// ==========================================
// 1. تنفيذ FourByFour_Board
// ==========================================

// Constructor (General)
template <typename T>
FourByFour_Board<T>::FourByFour_Board() : Board<T>(4, 4) {}

// Constructor (Specialized for char) - ترتيب القطع المتبادل
template <>
FourByFour_Board<char>::FourByFour_Board() : Board<char>(4, 4) {
    // 1. تصفير اللوحة
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            board[i][j] = '.';
        }
    }

    // 2. وضع القطع بالتبادل (X O X O)
    for (int j = 0; j < 4; ++j) {
        if (j % 2 == 0) { // الأعمدة الزوجية (0, 2)
            board[0][j] = 'X'; // فوق
            board[3][j] = 'O'; // تحت
        }
        else { // الأعمدة الفردية (1, 3)
            board[0][j] = 'O'; // فوق
            board[3][j] = 'X'; // تحت
        }
    }
}

template <typename T>
bool FourByFour_Board<T>::update_board(Move<T>* move) {
    int encoded_from = move->get_x();
    int encoded_to = move->get_y();
    T symbol = move->get_symbol();

    int from_r = encoded_from / 10;
    int from_c = encoded_from % 10;
    int to_r = encoded_to / 10;
    int to_c = encoded_to % 10;

    // 1. التأكد من الحدود
    if (from_r < 0 || from_r >= 4 || from_c < 0 || from_c >= 4 ||
        to_r < 0 || to_r >= 4 || to_c < 0 || to_c >= 4) return false;

    // 2. هل بيحرك قطعته؟
    if (this->board[from_r][from_c] != symbol) return false;

    // 3. هل المكان الجديد فاضي؟
    if (this->board[to_r][to_c] != '.') return false;

    // 4. هل الحركة خطوة واحدة (أفقي أو رأسي)؟
    int row_diff = abs(to_r - from_r);
    int col_diff = abs(to_c - from_c);

    // المجموع لازم يكون 1 (يعني اتحرك خطوة واحدة في اتجاه واحد)
    if ((row_diff + col_diff) != 1) return false;

    // تنفيذ الحركة
    this->board[from_r][from_c] = '.'; // فضي القديم
    this->board[to_r][to_c] = symbol;  // املأ الجديد
    this->n_moves++;
    return true;
}

template <typename T>
bool FourByFour_Board<T>::is_win(Player<T>* player) {
    T sym = player->get_symbol();
    // فحص الصفوف (3 ورا بعض)
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j <= 1; ++j) {
            if (this->board[i][j] == sym && this->board[i][j + 1] == sym && this->board[i][j + 2] == sym) return true;
        }
    }
    // فحص الأعمدة
    for (int i = 0; i <= 1; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (this->board[i][j] == sym && this->board[i + 1][j] == sym && this->board[i + 2][j] == sym) return true;
        }
    }
    // فحص الأقطار
    for (int i = 0; i <= 1; ++i) {
        for (int j = 0; j <= 1; ++j) {
            if (this->board[i][j] == sym && this->board[i + 1][j + 1] == sym && this->board[i + 2][j + 2] == sym) return true;
        }
    }
    for (int i = 0; i <= 1; ++i) {
        for (int j = 2; j < 4; ++j) {
            if (this->board[i][j] == sym && this->board[i + 1][j - 1] == sym && this->board[i + 2][j - 2] == sym) return true;
        }
    }
    return false;
}

template <typename T>
bool FourByFour_Board<T>::is_draw(Player<T>* player) {
    return false;
}

template <typename T>
bool FourByFour_Board<T>::game_is_over(Player<T>* player) {
    return is_win(player);
}

// ==========================================
// 2. تنفيذ FourByFour_UI
// ==========================================

template <typename T>
FourByFour_UI<T>::FourByFour_UI() : UI<T>("Welcome to 4x4 Tic-Tac-Toe (Moving Tokens)", 4) {}

template <typename T>
Player<T>* FourByFour_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    return new Player<T>(name, symbol, type);
}

template <typename T>
Move<T>* FourByFour_UI<T>::get_move(Player<T>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int from_r, from_c, to_r, to_c;
        cout << "\n" << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ") turn.\n";

        // 1. طلب مكان القطعة
        while (true) {
            cout << "Select token to move (Row Col): ";
            cin >> from_r >> from_c;
            if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            break;
        }

        // 2. طلب المكان الجديد
        while (true) {
            cout << "Select destination (Row Col): ";
            cin >> to_r >> to_c;
            if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            break;
        }

        // التشفير (بنبعت رقمين مدمجين عشان الـ Move بياخد x,y بس)
        return new Move<T>(from_r * 10 + from_c, to_r * 10 + to_c, currentPlayer->get_symbol());
    }
    else {
        // Simple Random AI (بيحاول يلاقي حركة صحيحة)
        int from_r, from_c, to_r, to_c, attempts = 0;
        while (attempts < 2000) {
            from_r = rand() % 4; from_c = rand() % 4;
            int dir = rand() % 4;
            to_r = from_r; to_c = from_c;
            if (dir == 0) to_r--; else if (dir == 1) to_r++;
            else if (dir == 2) to_c--; else if (dir == 3) to_c++;

            if (to_r >= 0 && to_r < 4 && to_c >= 0 && to_c < 4) {
                return new Move<T>(from_r * 10 + from_c, to_r * 10 + to_c, currentPlayer->get_symbol());
            }
            attempts++;
        }
        return new Move<T>(0, 0, currentPlayer->get_symbol());
    }
}

// =============================================================
// أهم جزء: Explicit Instantiation (عشان يربط الـ .h بالـ .cpp)
// =============================================================
template class FourByFour_Board<char>;
template class FourByFour_UI<char>;