#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <iomanip>
#include "Diamond_TicTacToe.h"

using namespace std;

Diamond_Board::Diamond_Board() : Board(5, 5) {

    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Diamond_Board::is_valid_cell(int x, int y) const {

    if (x < 0 || x >= 5 || y < 0 || y >= 5)
        return false;

    if (x == 0 || x == 4)
        return y == 2;

    if (x == 1 || x == 3)
        return y >= 1 && y <= 3;

    return true;
}

bool Diamond_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!is_valid_cell(x, y) || board[x][y] != blank_symbol)
        return false;

    board[x][y] = toupper(mark);
    n_moves++;
    return true;
}

bool Diamond_Board::check_line_length(char symbol, int length) const {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= columns - length; j++) {
            bool valid_line = true;

            for (int k = 0; k < length; k++) {
                if (!is_valid_cell(i, j + k)) {
                    valid_line = false;
                    break;
                }
            }
            if (!valid_line) continue;


            bool line = true;
            for (int k = 0; k < length; k++) {
                if (board[i][j + k] != symbol) {
                    line = false;
                    break;
                }
            }
            if (line) return true;
        }
    }


    for (int j = 0; j < columns; j++) {
        for (int i = 0; i <= rows - length; i++) {
            bool valid_line = true;
            for (int k = 0; k < length; k++) {
                if (!is_valid_cell(i + k, j)) {
                    valid_line = false;
                    break;
                }
            }
            if (!valid_line) continue;

            bool line = true;
            for (int k = 0; k < length; k++) {
                if (board[i + k][j] != symbol) {
                    line = false;
                    break;
                }
            }
            if (line) return true;
        }
    }


    for (int i = 0; i <= rows - length; i++) {
        for (int j = 0; j <= columns - length; j++) {
            bool valid_line = true;
            for (int k = 0; k < length; k++) {
                if (!is_valid_cell(i + k, j + k)) {
                    valid_line = false;
                    break;
                }
            }
            if (!valid_line) continue;

            bool line = true;
            for (int k = 0; k < length; k++) {
                if (board[i + k][j + k] != symbol) {
                    line = false;
                    break;
                }
            }
            if (line) return true;
        }
    }


    for (int i = 0; i <= rows - length; i++) {
        for (int j = length - 1; j < columns; j++) {
            bool valid_line = true;
            for (int k = 0; k < length; k++) {
                if (!is_valid_cell(i + k, j - k)) {
                    valid_line = false;
                    break;
                }
            }
            if (!valid_line) continue;

            bool line = true;
            for (int k = 0; k < length; k++) {
                if (board[i + k][j - k] != symbol) {
                    line = false;
                    break;
                }
            }
            if (line) return true;
        }
    }

    return false;
}

bool Diamond_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();
    return check_line_length(sym, 3) && check_line_length(sym, 4);
}

bool Diamond_Board::is_draw(Player<char>* player) {
    // Check if all valid diamond cells are filled
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (is_valid_cell(i, j) && board[i][j] == blank_symbol) {
                return false;
            }
        }
    }
    return !is_win(player);
}

bool Diamond_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Diamond_UI::Diamond_UI() : UI<char>("=== Diamond Tic-Tac-Toe ===", 3) {
    cout << "Goal: Create BOTH a line of 3 AND a line of 4 symbols!\n";
    cout << "Only diamond-shaped cells are playable.\n\n\n";
}

Player<char>* Diamond_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Diamond_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << ", enter your move (x y from 0 to 4): ";
        cin >> x >> y;
    }
    else {

        x = rand() % 5;
        y = rand() % 5;
        cout << "\nComputer " << player->get_name() << " chooses: " << x << " " << y << endl;
    }

    return new Move<char>(x, y, player->get_symbol());
}

void Diamond_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    if (matrix.empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();

    cout << "\n    ";
    for (int j = 0; j < cols; ++j)
        cout << " " << j << "  ";
    cout << "\n    +";
    for (int j = 0; j < cols; ++j) cout << "---+";
    cout << "\n";

    for (int i = 0; i < rows; ++i) {
        cout << "  " << i << " |";
        for (int j = 0; j < cols; ++j) {

            bool valid_cell = false;
            if (i == 0 || i == 4) valid_cell = (j == 2);
            else if (i == 1 || i == 3) valid_cell = (j >= 1 && j <= 3);
            else valid_cell = true;

            if (!valid_cell) {
                cout << "   |";
            }
            else {
                cout << " " << matrix[i][j] << " |";
            }
        }
        cout << "\n    +";
        for (int j = 0; j < cols; ++j) cout << "---+";
        cout << "\n";
    }
    cout << endl;
    cout << "Note: Only cells with symbols are playable.\n";
}