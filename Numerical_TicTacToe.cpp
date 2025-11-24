#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm> // عشان دالة advance
#include "Numerical_TicTacToe.h"

using namespace std;

Numerical_TicTacToe_Board::Numerical_TicTacToe_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Numerical_TicTacToe_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int number = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || number == 0)) {

        if (number == 0) {
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
    // Check all rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != blank_symbol &&
            board[i][1] != blank_symbol &&
            board[i][2] != blank_symbol &&
            (board[i][0] + board[i][1] + board[i][2] == 15))
            return true;
    }

    // Check all columns
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] != blank_symbol &&
            board[1][i] != blank_symbol &&
            board[2][i] != blank_symbol &&
            (board[0][i] + board[1][i] + board[2][i] == 15))
            return true;
    }

    // Check diagonals
    if (board[0][0] != blank_symbol &&
        board[1][1] != blank_symbol &&
        board[2][2] != blank_symbol &&
        (board[0][0] + board[1][1] + board[2][2] == 15))
        return true;

    if (board[0][2] != blank_symbol &&
        board[1][1] != blank_symbol &&
        board[2][0] != blank_symbol &&
        (board[0][2] + board[1][1] + board[2][0] == 15))
        return true;

    return false;
}

bool Numerical_TicTacToe_Board::is_draw(Player<int>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool Numerical_TicTacToe_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

//--------------------------------------- UI Implementation

Numerical_TicTacToe_UI::Numerical_TicTacToe_UI() : UI<int>("Welcome to Numerical Tic-Tac-Toe", 3) {
    player1_available = { 1, 3, 5, 7, 9 };
    player2_available = { 2, 4, 6, 8 };
}

Player<int>* Numerical_TicTacToe_UI::create_player(string& name, int symbol, PlayerType type) {
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (Set: " << (symbol == 1 ? "Odd" : "Even") << ")\n";
    return new Player<int>(name, symbol, type);
}

// Override setup_players to set symbols correctly (1 for Player 1, 2 for Player 2)
Player<int>** Numerical_TicTacToe_UI::setup_players() {
    Player<int>** players = new Player<int>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string name1 = get_player_name("Player 1 (Odd numbers)");
    PlayerType type1 = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(name1, 1, type1); // Symbol 1 represents Odd set

    string name2 = get_player_name("Player 2 (Even numbers)");
    PlayerType type2 = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(name2, 2, type2); // Symbol 2 represents Even set

    return players;
}

void Numerical_TicTacToe_UI::mark_number_used(int number) {
    used_numbers.insert(number);
    if (player1_available.count(number)) player1_available.erase(number);
    if (player2_available.count(number)) player2_available.erase(number);
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
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c, number;

        display_available_numbers(currentPlayer->get_symbol());

        while (true) {
            cout << currentPlayer->get_name() << ", enter Row (0-2): ";
            if (!(cin >> r) || r < 0 || r > 2) {
                cout << "Invalid row! Try again.\n";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }

        while (true) {
            cout << currentPlayer->get_name() << ", enter Column (0-2): ";
            if (!(cin >> c) || c < 0 || c > 2) {
                cout << "Invalid column! Try again.\n";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }

        while (true) {
            cout << currentPlayer->get_name() << ", enter Number: ";
            if (!(cin >> number)) {
                cout << "Invalid input! Enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (!is_number_available(number, currentPlayer->get_symbol())) {
                cout << "\n!!! Invalid number. Number is either already used or not in your set. !!!\n";
                display_available_numbers(currentPlayer->get_symbol());
                continue;
            }
            break;
        }

        mark_number_used(number);
        cout << endl;

        return new Move<int>(r, c, number);
    }
    else {
        // Computer Logic (Random for now)
        int x = rand() % 3;
        int y = rand() % 3;

        // Get a random available number
        int number;
        const set<int>& available = (currentPlayer->get_symbol() == 1) ? player1_available : player2_available;

        if (available.empty()) {
            // Should not happen if game logic is correct
            return nullptr;
        }

        int idx = rand() % available.size();
        auto it = available.begin();
        advance(it, idx);
        number = *it;

        mark_number_used(number);
        cout << "Computer " << currentPlayer->get_name()
            << " chose (Row, Col, Num): " << x << ", " << y << ", " << number << endl;
        return new Move<int>(x, y, number);
    }
}