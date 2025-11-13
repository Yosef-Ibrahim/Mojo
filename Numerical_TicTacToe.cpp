#include <iostream>
#include <iomanip>
#include <limits>
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
            board[i][2] != blank_symbol) {
            if (board[i][0] + board[i][1] + board[i][2] == 15) {
                return true;
            }
        }
    }

    // Check all columns
    for (int j = 0; j < 3; ++j) {
        if (board[0][j] != blank_symbol &&
            board[1][j] != blank_symbol &&
            board[2][j] != blank_symbol) {
            if (board[0][j] + board[1][j] + board[2][j] == 15) {
                return true;
            }
        }
    }

    // Check main diagonal (top-left to bottom-right)
    if (board[0][0] != blank_symbol &&
        board[1][1] != blank_symbol &&
        board[2][2] != blank_symbol) {
        if (board[0][0] + board[1][1] + board[2][2] == 15) {
            return true;
        }
    }

    // Check anti-diagonal (top-right to bottom-left)
    if (board[0][2] != blank_symbol &&
        board[1][1] != blank_symbol &&
        board[2][0] != blank_symbol) {
        if (board[0][2] + board[1][1] + board[2][0] == 15) {
            return true;
        }
    }

    return false;
}

bool Numerical_TicTacToe_Board::is_draw(Player<int>* player) {
    return (n_moves == 9 && !is_win(player));
}

bool Numerical_TicTacToe_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}

Numerical_TicTacToe_UI::Numerical_TicTacToe_UI() : UI<int>("Welcome to FCAI Numerical Tic-Tac-Toe Game", 3) {
    // Initialize available numbers for each player
    player1_available = { 1, 3, 5, 7, 9 }; // Odd numbers
    player2_available = { 2, 4, 6, 8 };    // Even numbers
}

Player<int>** Numerical_TicTacToe_UI::setup_players() {
    Player<int>** players = new Player<int>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string name1 = get_player_name("Player 1 (Odd Numbers)");
    PlayerType type1 = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(name1, 1, type1); // Symbol 1 for odd numbers

    string name2 = get_player_name("Player 2 (Even Numbers)");
    PlayerType type2 = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(name2, 2, type2); // Symbol 2 for even numbers

    return players;
}

Player<int>* Numerical_TicTacToe_UI::create_player(string& name, int symbol, PlayerType type) {
    // Symbol 1 means odd numbers (player 1), symbol 2 means even numbers (player 2)
    cout << "Creating " << (type == PlayerType::HUMAN ? "human" : "computer")
        << " player: " << name << " (Numbers: "
        << (symbol == 1 ? "Odd - 1,3,5,7,9" : "Even - 2,4,6,8") << ")\n";

    return new Player<int>(name, symbol, type);
}

void Numerical_TicTacToe_UI::mark_number_used(int number) {
    used_numbers.insert(number);
    player1_available.erase(number);
    player2_available.erase(number);
}

bool Numerical_TicTacToe_UI::is_number_available(int number, int player_symbol) {
    if (used_numbers.count(number)) {
        return false;
    }

    if (player_symbol == 1) { // Player 1 - Odd numbers
        return player1_available.count(number) > 0;
    }
    else { // Player 2 - Even numbers
        return player2_available.count(number) > 0;
    }
}

void Numerical_TicTacToe_UI::display_available_numbers(int player_symbol) {
    cout << "Available numbers: ";
    if (player_symbol == 1) {
        cout << "[";
        bool first = true;
        for (int num : player1_available) {
            if (!first) cout << ", ";
            cout << num;
            first = false;
        }
        cout << "]";
    }
    else {
        cout << "[";
        bool first = true;
        for (int num : player2_available) {
            if (!first) cout << ", ";
            cout << num;
            first = false;
        }
        cout << "]";
    }
    cout << endl;
}

Move<int>* Numerical_TicTacToe_UI::get_move(Player<int>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c, number;

        // Get row input
        while (true) {
            cout << currentPlayer->get_name() << ", enter Row number (0, 1, or 2): ";
            if (!(cin >> r)) {
                cout << "\n!!! That's not a number. Please enter a valid number. !!!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (r < 0 || r > 2) {
                cout << "\n!!! Out of Range. Please enter a number between 0 and 2. !!!\n";
                continue;
            }
            break;
        }

        // Get column input
        while (true) {
            cout << currentPlayer->get_name() << ", enter Column number (0, 1, or 2): ";
            if (!(cin >> c)) {
                cout << "\n!!! That's not a number. Please enter a valid number. !!!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (c < 0 || c > 2) {
                cout << "\n!!! Out of Range. Please enter a number between 0 and 2. !!!\n";
                continue;
            }
            break;
        }

        // Display available numbers and get number input
        display_available_numbers(currentPlayer->get_symbol());
        while (true) {
            cout << currentPlayer->get_name() << ", enter your number: ";
            if (!(cin >> number)) {
                cout << "\n!!! That's not a number. Please enter a valid number. !!!\n";
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
    else if (currentPlayer->get_type() == PlayerType::COMPUTER) {
        int x = rand() % 3;
        int y = rand() % 3;

        // Get a random available number
        int number;
        if (currentPlayer->get_symbol() == 1) { // Player 1 - Odd numbers
            int idx = rand() % player1_available.size();
            auto it = player1_available.begin();
            advance(it, idx);
            number = *it;
        }
        else { // Player 2 - Even numbers
            int idx = rand() % player2_available.size();
            auto it = player2_available.begin();
            advance(it, idx);
            number = *it;
        }

        mark_number_used(number);
        cout << "Computer " << currentPlayer->get_name()
            << " chose (Row, Col, Number): " << x << ", " << y << ", " << number << endl;

        return new Move<int>(x, y, number);
    }
    return nullptr;
}