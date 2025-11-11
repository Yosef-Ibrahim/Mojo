#include <iostream>
#include <iomanip>
#include <cctype>
#include "XO_Classes.h"

using namespace std;

X_O_Board::X_O_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool X_O_Board::update_board(Move<char>* move) {
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

bool X_O_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
        };

    for (int i = 0; i < 3; ++i) {
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;
    }

    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

bool X_O_Board::is_draw(Player<char>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool X_O_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

XO_UI::XO_UI() : UI<char>("Welcome to FCAI X-O Game", 3) {}

Player<char>* XO_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";

    return new Player<char>(name, symbol, type);
}

Move<char>* XO_UI::get_move(Player<char>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int x, y;

        cout << currentPlayer->get_name() << ", enter X coordinate (0, 1, or 2): ";
        cin >> x;

        cout << currentPlayer->get_name() << ", enter Y coordinate (0, 1, or 2): ";
        cin >> y;

        cout << endl;

        return new Move<char>(x, y, currentPlayer->get_symbol());
    }
    else if (currentPlayer->get_type() == PlayerType::COMPUTER) {
        int x = rand() % 3;
        int y = rand() % 3;

        cout << "Computer " << currentPlayer->get_name() << " chose (x y): " << x << " " << y << endl;
        return new Move<char>(x, y, currentPlayer->get_symbol());
    }
    return nullptr;
}