#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm> // لدالة advance
#include <vector>
#include "Numerical_TicTacToe.h"

using namespace std;

// =======================================================
// Board Implementation
// =======================================================

Numerical_TicTacToe_Board::Numerical_TicTacToe_Board() : Board(3, 3) {
    // تهيئة اللوحة بأصفار
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Numerical_TicTacToe_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int number = move->get_symbol();

    // التحقق من الحدود وإن المكان فاضي
    if (x >= 0 && x < rows && y >= 0 && y < columns &&
        (board[x][y] == blank_symbol || number == 0)) {

        if (number == 0) { // Undo logic if needed
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            n_moves++;
            board[x][y] = number;
        }
        return true;
    }
    return false;
}

bool Numerical_TicTacToe_Board::is_win(Player<int>* player) {
    // الفوز: مجموع 3 خانات يساوي 15
    // الصفوف
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != 0 && board[i][1] != 0 && board[i][2] != 0 &&
            (board[i][0] + board[i][1] + board[i][2] == 15)) return true;
    }
    // الأعمدة
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] != 0 && board[1][i] != 0 && board[2][i] != 0 &&
            (board[0][i] + board[1][i] + board[2][i] == 15)) return true;
    }
    // الأقطار
    if (board[0][0] != 0 && board[1][1] != 0 && board[2][2] != 0 &&
        (board[0][0] + board[1][1] + board[2][2] == 15)) return true;

    if (board[0][2] != 0 && board[1][1] != 0 && board[2][0] != 0 &&
        (board[0][2] + board[1][1] + board[2][0] == 15)) return true;

    return false;
}

bool Numerical_TicTacToe_Board::is_draw(Player<int>* player) {
    // التعادل إذا اللوحة امتلأت (9 حركات) ولم يفز أحد
    return (n_moves == 9 && !is_win(player));
}

bool Numerical_TicTacToe_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

// =======================================================
// UI Implementation
// =======================================================

Numerical_TicTacToe_UI::Numerical_TicTacToe_UI() : UI<int>("Welcome to Numerical Tic-Tac-Toe", 3) {
    player1_available = { 1, 3, 5, 7, 9 };
    player2_available = { 2, 4, 6, 8 };
}

// دالة الربط الجديدة
void Numerical_TicTacToe_UI::set_board(Board<int>* b) {
    this->boardPtr = b;
}

Player<int>* Numerical_TicTacToe_UI::create_player(string& name, int symbol, PlayerType type) {
    string setType = (symbol == 1) ? "Odd numbers" : "Even numbers";
    cout << "Creating " << (type == PlayerType::HUMAN ? "Human" : "Computer")
        << " player: " << name << " (" << setType << ")\n";
    return new Player<int>(name, symbol, type);
}

Player<int>** Numerical_TicTacToe_UI::setup_players() {
    Player<int>** players = new Player<int>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string name1 = get_player_name("Player 1 (Odd: 1,3,5,7,9)");
    PlayerType type1 = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(name1, 1, type1);

    string name2 = get_player_name("Player 2 (Even: 2,4,6,8)");
    PlayerType type2 = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(name2, 2, type2);

    return players;
}

void Numerical_TicTacToe_UI::mark_number_used(int number) {
    used_numbers.insert(number);
    player1_available.erase(number);
    player2_available.erase(number);
}

bool Numerical_TicTacToe_UI::is_number_available(int number, int player_symbol) {
    if (used_numbers.count(number)) return false;
    if (player_symbol == 1) return player1_available.count(number);
    else return player2_available.count(number);
}

void Numerical_TicTacToe_UI::display_available_numbers(int player_symbol) {
    cout << "Available numbers: ";
    const set<int>& available = (player_symbol == 1) ? player1_available : player2_available;
    for (int n : available) cout << n << " ";
    cout << endl;
}

Move<int>* Numerical_TicTacToe_UI::get_move(Player<int>* currentPlayer) {
    // 1. Human Player Logic
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c, number;
        display_available_numbers(currentPlayer->get_symbol());

        // التحقق من الإحداثيات وصحة المكان
        while (true) {
            cout << currentPlayer->get_name() << ", enter Row (0-2) and Column (0-2): ";
            if (!(cin >> r >> c)) {
                cout << "Invalid input! Enter numbers.\n";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            // التحقق من الحدود
            if (r < 0 || r > 2 || c < 0 || c > 2) {
                cout << "Coordinates out of bounds! Try again.\n";
                continue;
            }

            // التحقق من أن المكان فارغ (باستخدام مؤشر البورد)
            if (boardPtr != nullptr) {
                if (boardPtr->get_board_matrix()[r][c] != 0) {
                    cout << "Cell is already occupied! Choose another.\n";
                    continue;
                }
            }
            break;
        }

        // التحقق من الرقم
        while (true) {
            cout << "Enter Number to place: ";
            if (!(cin >> number)) {
                cout << "Invalid input.\n";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (!is_number_available(number, currentPlayer->get_symbol())) {
                cout << "Invalid number! Choose from available list.\n";
                continue;
            }
            break;
        }

        mark_number_used(number);
        return new Move<int>(r, c, number);
    }

    // 2. Computer Player Logic (Smart Random - No Infinite Loop)
    else {
        // تجميع كل الأماكن الفارغة في قائمة
        vector<pair<int, int>> empty_cells;
        if (boardPtr != nullptr) {
            auto& grid = boardPtr->get_board_matrix();
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (grid[i][j] == 0) {
                        empty_cells.push_back({ i, j });
                    }
                }
            }
        }

        // لو مفيش أماكن فاضية، نرجع حركة وهمية (اللعبة هتنتهي تعادل أصلاً)
        if (empty_cells.empty()) {
            return new Move<int>(0, 0, 0);
        }

        // اختيار مكان عشوائي من الأماكن الفارغة فقط
        int rand_idx = rand() % empty_cells.size();
        int r = empty_cells[rand_idx].first;
        int c = empty_cells[rand_idx].second;

        // اختيار رقم متاح
        const set<int>& available = (currentPlayer->get_symbol() == 1) ? player1_available : player2_available;
        if (available.empty()) return new Move<int>(0, 0, 0); // حماية إضافية

        int num_idx = rand() % available.size();
        auto it = available.begin();
        advance(it, num_idx);
        int number = *it;

        mark_number_used(number);
        cout << "Computer " << currentPlayer->get_name() << " chose (" << r << "," << c << ") with number " << number << endl;
        return new Move<int>(r, c, number);
    }
}