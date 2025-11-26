#include "Diamond_TicTacToe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// -------------------------------------------------------------------
// Diamond_TicTacToe_Board Implementation
// -------------------------------------------------------------------
template <typename T>
Diamond_TicTacToe_Board<T>::Diamond_TicTacToe_Board() : Board<T>(5, 5) {
    // تهيئة اللوحة بقيم فارغة
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (is_valid_position(i, j)) {
                this->board[i][j] = static_cast<T>(' ');
            }
            else {
                this->board[i][j] = static_cast<T>('-'); // علامة للخانات غير الصالحة
            }
        }
    }
}

template <typename T>
bool Diamond_TicTacToe_Board<T>::is_valid_position(int x, int y) {
    // شكل الألماسة على اللوحة 5x5:
    // الصف 0: العمود 2 فقط
    // الصف 1: الأعمدة 1, 2, 3
    // الصف 2: الأعمدة 0, 1, 2, 3, 4
    // الصف 3: الأعمدة 1, 2, 3
    // الصف 4: العمود 2 فقط

    if (x == 0 || x == 4) {
        return y == 2;
    }
    else if (x == 1 || x == 3) {
        return y >= 1 && y <= 3;
    }
    else if (x == 2) {
        return y >= 0 && y <= 4;
    }
    return false;
}

template <typename T>
vector<vector<pair<int, int>>> Diamond_TicTacToe_Board<T>::find_all_lines(T symbol, int length) {
    vector<vector<pair<int, int>>> all_lines;

    // فحص جميع الاتجاهات الممكنة
    // 1. أفقي (→)
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j <= 5 - length; ++j) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < length; ++k) {
                if (is_valid_position(i, j + k) && this->board[i][j + k] == symbol) {
                    line.push_back(make_pair(i, j + k));
                }
                else {
                    valid = false;
                    break;
                }
            }
            if (valid && line.size() == length) {
                all_lines.push_back(line);
            }
        }
    }

    // 2. عمودي (↓)
    for (int j = 0; j < 5; ++j) {
        for (int i = 0; i <= 5 - length; ++i) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < length; ++k) {
                if (is_valid_position(i + k, j) && this->board[i + k][j] == symbol) {
                    line.push_back(make_pair(i + k, j));
                }
                else {
                    valid = false;
                    break;
                }
            }
            if (valid && line.size() == length) {
                all_lines.push_back(line);
            }
        }
    }

    // 3. قطري (↘)
    for (int i = 0; i <= 5 - length; ++i) {
        for (int j = 0; j <= 5 - length; ++j) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < length; ++k) {
                if (is_valid_position(i + k, j + k) && this->board[i + k][j + k] == symbol) {
                    line.push_back(make_pair(i + k, j + k));
                }
                else {
                    valid = false;
                    break;
                }
            }
            if (valid && line.size() == length) {
                all_lines.push_back(line);
            }
        }
    }

    // 4. قطري عكسي (↙)
    for (int i = 0; i <= 5 - length; ++i) {
        for (int j = length - 1; j < 5; ++j) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < length; ++k) {
                if (is_valid_position(i + k, j - k) && this->board[i + k][j - k] == symbol) {
                    line.push_back(make_pair(i + k, j - k));
                }
                else {
                    valid = false;
                    break;
                }
            }
            if (valid && line.size() == length) {
                all_lines.push_back(line);
            }
        }
    }

    return all_lines;
}

template <typename T>
bool Diamond_TicTacToe_Board<T>::check_simultaneous_win(T symbol) {
    // نجيب كل الخطوط بطول 3 و 4
    vector<vector<pair<int, int>>> lines_3 = find_all_lines(symbol, 3);
    vector<vector<pair<int, int>>> lines_4 = find_all_lines(symbol, 4);

    // لازم يكون في على الأقل خط من كل نوع
    if (lines_3.empty() || lines_4.empty()) {
        return false;
    }

    // نتحقق إن في خطين مختلفين في الاتجاه (واحد 3، واحد 4)
    // ويشتركوا في خانة واحدة على الأقل
    for (const auto& line3 : lines_3) {
        for (const auto& line4 : lines_4) {
            // نتحقق إن الخطين مش نفس الاتجاه
            bool same_direction = false;

            // لو كل الخطوط في نفس الصف أو نفس العمود أو نفس القطر = نفس الاتجاه
            bool line3_horizontal = (line3[0].first == line3[1].first && line3[1].first == line3[2].first);
            bool line3_vertical = (line3[0].second == line3[1].second && line3[1].second == line3[2].second);

            bool line4_horizontal = (line4[0].first == line4[1].first && line4[1].first == line4[2].first && line4[2].first == line4[3].first);
            bool line4_vertical = (line4[0].second == line4[1].second && line4[1].second == line4[2].second && line4[2].second == line4[3].second);

            if (line3_horizontal && line4_horizontal) same_direction = true;
            if (line3_vertical && line4_vertical) same_direction = true;

            // لو في نفس الاتجاه، نتخطى
            if (same_direction) continue;

            // نشوف لو في نقطة مشتركة
            bool has_common = false;
            for (const auto& p3 : line3) {
                for (const auto& p4 : line4) {
                    if (p3.first == p4.first && p3.second == p4.second) {
                        has_common = true;
                        break;
                    }
                }
                if (has_common) break;
            }

            // لو لقينا خطين مختلفين في الاتجاه وبينهم نقطة مشتركة = فوز!
            if (has_common) {
                return true;
            }
        }
    }

    return false;
}

template <typename T>
bool Diamond_TicTacToe_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();
    T symbol = move->get_symbol();

    // التحقق من صحة الموقع
    if (!is_valid_position(x, y)) {
        return false;
    }

    // التحقق من أن الخانة فارغة
    if (this->board[x][y] != static_cast<T>(' ')) {
        return false;
    }

    // وضع الرمز في اللوحة
    this->board[x][y] = symbol;
    this->n_moves++;

    return true;
}

template <typename T>
bool Diamond_TicTacToe_Board<T>::is_win(Player<T>* player) {
    return check_simultaneous_win(player->get_symbol());
}

template <typename T>
bool Diamond_TicTacToe_Board<T>::is_draw(Player<T>* player) {
    // نعد الخانات الفارغة الصالحة
    int empty_count = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (is_valid_position(i, j) && this->board[i][j] == static_cast<T>(' ')) {
                empty_count++;
            }
        }
    }

    // لو اللوحة اتملت ومحدش فاز = تعادل
    return (empty_count == 0);
}

template <typename T>
bool Diamond_TicTacToe_Board<T>::game_is_over(Player<T>* player) {
    return is_win(player) || is_draw(player);
}

// -------------------------------------------------------------------
// Diamond_TicTacToe_Player Implementation
// -------------------------------------------------------------------
template <typename T>
Diamond_TicTacToe_Player<T>::Diamond_TicTacToe_Player(string name, T symbol, PlayerType type)
    : Player<T>(name, symbol, type) {}

// -------------------------------------------------------------------
// Diamond_TicTacToe_RandomPlayer Implementation
// -------------------------------------------------------------------
template <typename T>
Diamond_TicTacToe_RandomPlayer<T>::Diamond_TicTacToe_RandomPlayer(string name, T symbol)
    : Diamond_TicTacToe_Player<T>(name, symbol, PlayerType::RANDOM) {}

template <typename T>
pair<int, int> Diamond_TicTacToe_RandomPlayer<T>::get_random_move(Board<T>* board) {
    vector<pair<int, int>> available_moves;
    auto board_matrix = board->get_board_matrix();

    Diamond_TicTacToe_Board<T>* diamond_board = dynamic_cast<Diamond_TicTacToe_Board<T>*>(board);

    // جمع الخانات الفارغة الصالحة
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (board_matrix[i][j] == static_cast<T>(' ')) {
                available_moves.push_back(make_pair(i, j));
            }
        }
    }

    // اختيار حركة عشوائية
    if (!available_moves.empty()) {
        int random_index = rand() % available_moves.size();
        return available_moves[random_index];
    }

    return make_pair(-1, -1);
}

// -------------------------------------------------------------------
// Diamond_TicTacToe_UI Implementation
// -------------------------------------------------------------------
template <typename T>
Diamond_TicTacToe_UI<T>::Diamond_TicTacToe_UI()
    : UI<T>("Welcome to Diamond Tic-Tac-Toe!", 5), board_ptr(nullptr) {}

template <typename T>
Player<T>* Diamond_TicTacToe_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    if (type == PlayerType::HUMAN) {
        return new Diamond_TicTacToe_Player<T>(name, symbol, PlayerType::HUMAN);
    }
    else {
        return new Diamond_TicTacToe_RandomPlayer<T>(name, symbol);
    }
}

template <typename T>
Move<T>* Diamond_TicTacToe_UI<T>::get_move(Player<T>* currentPlayer) {
    int x, y;

    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        cout << "\n" << currentPlayer->get_name()
            << " (" << currentPlayer->get_symbol() << "), enter your move (row column): ";
        cin >> x >> y;
    }
    else {
        auto* random_player = dynamic_cast<Diamond_TicTacToe_RandomPlayer<T>*>(currentPlayer);
        if (random_player && board_ptr) {
            pair<int, int> random_move = random_player->get_random_move(board_ptr);
            x = random_move.first;
            y = random_move.second;
            cout << "\n" << currentPlayer->get_name()
                << " (Computer) chose: Row " << x << ", Column " << y << endl;
        }
        else {
            x = y = -1;
        }
    }

    return new Move<T>(x, y, currentPlayer->get_symbol());
}

template <typename T>
void Diamond_TicTacToe_UI<T>::display_diamond_board(const vector<vector<T>>& matrix) {
    cout << "\n";
    cout << "       2\n";
    cout << "     +---+\n";
    cout << "  0  | " << matrix[0][2] << " |\n";
    cout << "     +---+---+---+\n";
    cout << "  1  | " << matrix[1][1] << " | " << matrix[1][2] << " | " << matrix[1][3] << " |\n";
    cout << " +---+---+---+---+---+\n";
    cout << " | " << matrix[2][0] << " | " << matrix[2][1] << " | " << matrix[2][2] << " | " << matrix[2][3] << " | " << matrix[2][4] << " |  2\n";
    cout << " +---+---+---+---+---+\n";
    cout << "  3  | " << matrix[3][1] << " | " << matrix[3][2] << " | " << matrix[3][3] << " |\n";
    cout << "     +---+---+---+\n";
    cout << "  4  | " << matrix[4][2] << " |\n";
    cout << "     +---+\n";
    cout << "   0   1   2   3   4\n\n";
}

// Template instantiation
template class Diamond_TicTacToe_Board<char>;
template class Diamond_TicTacToe_Player<char>;
template class Diamond_TicTacToe_RandomPlayer<char>;
template class Diamond_TicTacToe_UI<char>;