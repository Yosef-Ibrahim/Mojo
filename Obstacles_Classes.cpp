#include "Obstacles_Classes.h"
#include <iostream>
#include <iomanip>
#include <limits> 
#include <cstdlib> // عشان rand()

using namespace std;

// ==========================================
// تنفيذ Obstacles_Board
// ==========================================

// Constructor (General)
template <typename T>
Obstacles_Board<T>::Obstacles_Board() : Board<T>(6, 6) {} // 6x6 Grid

// Constructor (Specialized for char)
template <>
Obstacles_Board<char>::Obstacles_Board() : Board<char>(6, 6) {
    // تصفير اللوحة
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            board[i][j] = '.';
        }
    }
}

// دالة إضافة العوائق العشوائية
template <typename T>
void Obstacles_Board<T>::add_random_obstacles() {
    int obstacles_added = 0;
    int attempts = 0;

    // بنحاول نضيف 2 عوائق، بس بنتأكد إن اللوحة مش مليانة
    // بنعمل حد أقصى للمحاولات عشان ميعلقش لو اللوحة مليانة
    while (obstacles_added < 2 && attempts < 100) {
        int r = rand() % 6;
        int c = rand() % 6;

        if (this->board[r][c] == '.') {
            this->board[r][c] = '#'; // الرمز '#' يمثل العائق
            obstacles_added++;
        }
        attempts++;
    }
}

template <typename T>
bool Obstacles_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();
    T symbol = move->get_symbol();

    // 1. التأكد من الحدود وإن المكان فاضي (مش لاعب ومش عائق)
    if (x >= 0 && x < 6 && y >= 0 && y < 6 && this->board[x][y] == '.') {

        this->n_moves++;
        this->board[x][y] = symbol;

        // 2. إضافة العوائق بعد كل جولة (Round)
        // الجولة بتخلص لما اللاعبين هما الاتنين يلعبوا (يعني عدد الحركات زوجي)
        if (this->n_moves % 2 == 0) {
            add_random_obstacles();
        }

        return true;
    }
    return false;
}

template <typename T>
bool Obstacles_Board<T>::is_win(Player<T>* player) {
    T sym = player->get_symbol();

    // شرط الفوز: 4 ورا بعض (مش 3)

    // 1. فحص الصفوف
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j <= 2; ++j) { // بنشيك لحد العمود 2 عشان نلاقي مساحة لـ 4 خانات
            if (this->board[i][j] == sym && this->board[i][j + 1] == sym &&
                this->board[i][j + 2] == sym && this->board[i][j + 3] == sym)
                return true;
        }
    }

    // 2. فحص الأعمدة
    for (int i = 0; i <= 2; ++i) { // بنشيك لحد الصف 2
        for (int j = 0; j < 6; ++j) {
            if (this->board[i][j] == sym && this->board[i + 1][j] == sym &&
                this->board[i + 2][j] == sym && this->board[i + 3][j] == sym)
                return true;
        }
    }

    // 3. فحص الأقطار الرئيسية
    for (int i = 0; i <= 2; ++i) {
        for (int j = 0; j <= 2; ++j) {
            if (this->board[i][j] == sym && this->board[i + 1][j + 1] == sym &&
                this->board[i + 2][j + 2] == sym && this->board[i + 3][j + 3] == sym)
                return true;
        }
    }

    // 4. فحص الأقطار العكسية
    for (int i = 0; i <= 2; ++i) {
        for (int j = 3; j < 6; ++j) {
            if (this->board[i][j] == sym && this->board[i + 1][j - 1] == sym &&
                this->board[i + 2][j - 2] == sym && this->board[i + 3][j - 3] == sym)
                return true;
        }
    }

    return false;
}

template <typename T>
bool Obstacles_Board<T>::is_draw(Player<T>* player) {
    // التعادل لو اللوحة اتملت ومحدش كسب
    // بنعرف إنها اتملت لو مفيش ولا خانة '.' فاضية
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (this->board[i][j] == '.') return false;
        }
    }
    return !is_win(player);
}

template <typename T>
bool Obstacles_Board<T>::game_is_over(Player<T>* player) {
    return is_win(player) || is_draw(player);
}

// ==========================================
// تنفيذ Obstacles_UI
// ==========================================

template <typename T>
Obstacles_UI<T>::Obstacles_UI() : UI<T>("Welcome to Obstacles Tic-Tac-Toe (Game 10)", 6) {}

template <typename T>
Player<T>* Obstacles_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    return new Player<T>(name, symbol, type);
}

template <typename T>
Move<T>* Obstacles_UI<T>::get_move(Player<T>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\n" << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ") turn.\n";
        while (true) {
            cout << "Enter coordinate (Row Col): ";
            cin >> x >> y;
            if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            break;
        }
        return new Move<T>(x, y, currentPlayer->get_symbol());
    }
    else {
        // AI عشوائي بسيط
        int x, y;
        // بيحاول 100 مرة يلاقي مكان فاضي
        for (int k = 0; k < 100; ++k) {
            x = rand() % 6;
            y = rand() % 6;
            // لازم نبعت حركة وخلاص، والـ Board هي اللي هتقول لو المكان ده فيه عائق
            return new Move<T>(x, y, currentPlayer->get_symbol());
        }
        return new Move<T>(0, 0, currentPlayer->get_symbol());
    }
}

// =============================================================
// Explicit Instantiation
// =============================================================
template class Obstacles_Board<char>;
template class Obstacles_UI<char>;