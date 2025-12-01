#include "Pyramid_TicTacToe.h" // التأكد من تضمين ملف الرأس الصحيح
#include <iostream>
#include <iomanip>
#include <limits> 

using namespace std;

// ==========================================
// 1. تطبيق Pyramid_XO_Board
// ==========================================

// المُنشئ العام
template <typename T>
Pyramid_XO_Board<T>::Pyramid_XO_Board() : Board<T>(3, 5) {
    // 3 صفوف، بحد أقصى 5 أعمدة
}

// تخصيص لنوع char
template <>
Pyramid_XO_Board<char>::Pyramid_XO_Board() : Board<char>(3, 5) {
    // تهيئة اللوحة بالنقاط
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 5; ++j) {
            board[i][j] = '.';
        }
    }
}

// دالة تحديث اللوحة
template <typename T>
bool Pyramid_XO_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();
    T symbol = move->get_symbol();

    // التحقق من صحة المواضع الخاصة بشكل الهرم
    // الصف 0: العمود 2 فقط متاح
    // الصف 1: الأعمدة 1, 2, 3 متاحة
    // الصف 2: الأعمدة 0, 1, 2, 3, 4 متاحة

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

// دالة التحقق من الفوز
template <typename T>
bool Pyramid_XO_Board<T>::is_win(Player<T>* player) {
    T s = player->get_symbol();

    // التحقق الأفقي
    // الصف 1 (المؤشرات 1,2,3)
    if (this->board[1][1] == s && this->board[1][2] == s && this->board[1][3] == s) return true;
    // الصف 2 (المؤشرات 0,1,2 أو 1,2,3 أو 2,3,4)
    if (this->board[2][0] == s && this->board[2][1] == s && this->board[2][2] == s) return true;
    if (this->board[2][1] == s && this->board[2][2] == s && this->board[2][3] == s) return true;
    if (this->board[2][2] == s && this->board[2][3] == s && this->board[2][4] == s) return true;

    // التحقق الرأسي (عمود رأسي واحد فقط: العمود 2)
    if (this->board[0][2] == s && this->board[1][2] == s && this->board[2][2] == s) return true;

    // التحقق القطري
    // النوع القطري الرئيسي (\)
    if (this->board[0][2] == s && this->board[1][3] == s && this->board[2][4] == s) return true;
    // النوع القطري الآخر (/)
    if (this->board[0][2] == s && this->board[1][1] == s && this->board[2][0] == s) return true;

    return false;
}

// دالة التحقق من التعادل
template <typename T>
bool Pyramid_XO_Board<T>::is_draw(Player<T>* player) {
    return (this->n_moves == 9 && !is_win(player));
}

// دالة التحقق من انتهاء اللعبة
template <typename T>
bool Pyramid_XO_Board<T>::game_is_over(Player<T>* player) {
    return is_win(player) || is_draw(player);
}

// ==========================================
// 2. تطبيق Pyramid_XO_UI
// ==========================================

// المُنشئ
template <typename T>
Pyramid_XO_UI<T>::Pyramid_XO_UI() : UI<T>("مرحبًا بك في لعبة الهرم إكس أو", 3) {}

// دالة إنشاء لاعب
template <typename T>
Player<T>* Pyramid_XO_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    return new Player<T>(name, symbol, type);
}

// دالة الحصول على الحركة
template <typename T>
Move<T>* Pyramid_XO_UI<T>::get_move(Player<T>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int x, y;
        cout << "\nدور " << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ").\n";
        // إضافة توضيح للإحداثيات لمساعدة اللاعب
        cout << "الإحداثيات: القمة(0 2) | الوسط(1 1-3) | القاعدة(2 0-4)\n";
        while (true) {
            cout << "أدخل الإحداثيات (صف عمود): ";
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
        // كمبيوتر عشوائي
        int x, y;
        // الاستمرار في المحاولة حتى العثور على مكان صالح للهرم
        for (int k = 0; k < 1000; k++) {
            x = rand() % 3;
            y = rand() % 5;
            // تحقق بسيط قبل الإرسال للوحة
            bool valid = false;
            if (x == 0 && y == 2) valid = true;
            else if (x == 1 && y >= 1 && y <= 3) valid = true;
            else if (x == 2 && y >= 0 && y <= 4) valid = true;

            if (valid) return new Move<T>(x, y, currentPlayer->get_symbol());
        }
        return new Move<T>(0, 2, currentPlayer->get_symbol());
    }
}

// التجسيد الصريح (Explicit Instantiation)
template class Pyramid_XO_Board<char>;
template class Pyramid_XO_UI<char>;