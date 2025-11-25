#include "Misere.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits> 

using namespace std;

Misere_Board::Misere_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Misere_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0)) {

        if (mark == 0) {
            n_moves--;
            board[x][y] = blank_symbol;
        }
        else {
            n_moves++;
            board[x][y] = toupper(mark);
        }
        return true;
    }
    return false;
}

bool Misere_Board::is_win(Player<char>* player) {
    return false;
}

bool Misere_Board::is_lose(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    for (int i = 0; i < 3; ++i) {
        if (all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym)
            return true;

        if (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym)
            return true;
    }

    if (all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym)
        return true;

    if (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym)
        return true;

    return false;
}

bool Misere_Board::is_draw(Player<char>* player) {
    return (n_moves == 9);
}

bool Misere_Board::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

Misere_UI::Misere_UI()
    : UI<char>("Welcome to Misere Tic Tac Toe Game\n(Goal: AVOID getting 3 in a row!)", 3) {
}

Player<char>* Misere_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* Misere_UI::get_move(Player<char>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c;

        while (true) {
            cout << currentPlayer->get_name()
                << ", enter Row and Column numbers (0 1 2), separated by space: ";

            if (!(cin >> r >> c)) {
                cout << "\n!!! Invalid input. Please enter two numbers. !!!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (r < 0 || r > 2 || c < 0 || c > 2) {
                cout << "\n!!! Out of range. Both numbers must be between 0 and 2. !!!\n";
                continue;
            }
            break;
        }

        cout << endl;
        return new Move<char>(r, c, currentPlayer->get_symbol());
    }
    else {
        int x = rand() % 3;
        int y = rand() % 3;

        cout << "Computer " << currentPlayer->get_name()
            << " chose (Row, Col): " << x << ", " << y << endl;

        return new Move<char>(x, y, currentPlayer->get_symbol());
    }
}