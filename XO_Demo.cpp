#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cstdlib> 

#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Numerical_TicTacToe.h"
using namespace std;

void run_XO_Game() {
    UI<char>* game_ui = new XO_UI();
    Board<char>* xo_board = new X_O_Board();
    Player<char>** players = game_ui->setup_players();

    GameManager<char> x_o_game(xo_board, players, game_ui);
    x_o_game.run();

    delete xo_board;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;
    delete game_ui;
}

void run_Numerical_TicTacToe_Game() {
    UI<int>* game_ui = new Numerical_TicTacToe_UI();
    Board<int>* num_board = new Numerical_TicTacToe_Board();
    Player<int>** players = game_ui->setup_players();

    GameManager<int> num_game(num_board, players, game_ui);
    num_game.run();

    delete num_board;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;
    delete game_ui;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    int choice;

    while (true) {
        system("cls");
        cout << "\n==============================\n";
        cout << "  Welcome to FCAI Games Menu  \n";
        cout << "==============================\n";
        cout << "1: Play X-O Game\n";
        cout << "2: Play Numerical Tic-Tac-Toe\n";
        cout << "3: Play Connect Four (Soon!)\n";
        cout << "0: Exit\n";
        cout << "------------------------------\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "\n!!! Invalid choice. Please enter a number. !!!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nPress Enter to continue...";
            cin.get();
            continue;
        }

        if (choice == 1) {
            run_XO_Game();
        }
        else if (choice == 2) {
            run_Numerical_TicTacToe_Game();
        }
        else if (choice == 3) {
            cout << "This game is not implemented yet.\n";
        }
        else if (choice == 0) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }

        cout << "\nPress Enter to return to menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    return 0;
}