#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Infinity_TicTacToe.h"

using namespace std;

Infinity_Board::Infinity_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Infinity_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != blank_symbol)
        return false;

    board[x][y] = toupper(mark);
    n_moves++;
    moveHistory.push_back(new Move<char>(x, y, mark));
    moveCounter++;


    if (moveCounter >= 3) {
        removeOldestMove();
        moveCounter = 0;
    }

    return true;
}

void Infinity_Board::removeOldestMove() {
    if (moveHistory.empty()) return;

    Move<char>* oldest = moveHistory.front();
    int x = oldest->get_x();
    int y = oldest->get_y();

    board[x][y] = blank_symbol;
    moveHistory.pop_front();
    delete oldest;
}

bool Infinity_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol && a == sym;
        };

    for (int i = 0; i < 3; i++) {
        if (all_equal(board[i][0], board[i][1], board[i][2]) ||
            all_equal(board[0][i], board[1][i], board[2][i]))
            return true;
    }

    if (all_equal(board[0][0], board[1][1], board[2][2]) ||
        all_equal(board[0][2], board[1][1], board[2][0]))
        return true;

    return false;
}

bool Infinity_Board::is_draw(Player<char>* player) {

    return n_moves >= 9 && !is_win(player);
}

bool Infinity_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

Infinity_UI::Infinity_UI() : UI<char>("Welcome to Infinity Tic-Tac-Toe", 3) {}

Player<char>* Infinity_UI::create_player(string& name, char symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (" << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* Infinity_UI::get_move(Player<char>* player) {
    int x, y;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << ", enter your move (Row Column from 0 to 2): ";
        cin >> x >> y;
    }
    else {

        x = rand() % 3;
        y = rand() % 3;
        cout << "\nComputer chooses: " << x << " " << y << endl;
    }
    return new Move<char>(x, y, player->get_symbol());
}