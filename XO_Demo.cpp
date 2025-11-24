#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cstdlib> 

#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Numerical_TicTacToe.h"
// ده السطر المهم عشان نربط الملف الجديد
#include "FourByFour_Classes.h" 

using namespace std;

// --- دوال تشغيل الألعاب ---

void run_XO_Game() {
    UI<char>* game_ui = new XO_UI();
    Board<char>* xo_board = new X_O_Board();
    Player<char>** players = game_ui->setup_players();
    GameManager<char> x_o_game(xo_board, players, game_ui);
    x_o_game.run();
    delete xo_board; delete players[0]; delete players[1]; delete[] players; delete game_ui;
}

void run_Numerical_TicTacToe_Game() {
    UI<int>* game_ui = new Numerical_TicTacToe_UI();
    Board<int>* num_board = new Numerical_TicTacToe_Board();
    Player<int>** players = game_ui->setup_players();
    GameManager<int> num_game(num_board, players, game_ui);
    num_game.run();
    delete num_board; delete players[0]; delete players[1]; delete[] players; delete game_ui;
}

// دالة تشغيل اللعبة الجديدة (Game 7)
void run_FourByFour_Game() {
    UI<char>* game_ui = new FourByFour_UI<char>();
    Board<char>* board = new FourByFour_Board<char>();
    Player<char>** players = game_ui->setup_players();

    GameManager<char> game(board, players, game_ui);
    game.run();

    delete board; delete players[0]; delete players[1]; delete[] players; delete game_ui;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int choice;

    while (true) {
        system("cls");
        cout << "\n==============================\n";
        cout << "  Welcome to FCAI Games Menu  \n";
        cout << "==============================\n";
        cout << "1: Play X-O Game (3x3)\n";
        cout << "2: Play Numerical Tic-Tac-Toe\n";
        cout << "3: Play 4x4 Tic-Tac-Toe (Game 7)\n"; // الاختيار الجديد
        cout << "0: Exit\n";
        cout << "------------------------------\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) run_XO_Game();
        else if (choice == 2) run_Numerical_TicTacToe_Game();
        else if (choice == 3) run_FourByFour_Game(); // تشغيل اللعبة
        else if (choice == 0) break;
        else cout << "Invalid choice!\n";

        cout << "\nPress Enter to return to menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
    return 0;
}