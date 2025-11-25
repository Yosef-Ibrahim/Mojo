#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cstdlib> 

// استدعاء ملفات الألعاب
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Numerical_TicTacToe.h" 
#include "FourByFour_Classes.h"
#include "FiveByFive_Classes.h"
#include "sus_classes.h"
#include "Misere.h"
#include "Infinity_TicTacToe.h"
#include "Obstacles_Classes.h"

using namespace std;

// =============================================================================
//  Part 1: Game Rules & Instructions Function
// =============================================================================
void show_game_rules() {
    int choice;
    while (true) {
        system("cls");
        cout << "\n============================================\n";
        cout << "       GAME RULES & INSTRUCTIONS           \n";
        cout << "============================================\n";
        cout << "Which game do you want to learn about?\n";
        cout << "1. X-O (Classic)\n";
        cout << "2. SUS Game\n";
        cout << "3. 5x5 Tic-Tac-Toe\n";
        cout << "4. Misere Tic-Tac-Toe\n";
        cout << "5. 4x4 Tic-Tac-Toe\n";
        cout << "6. Numerical Tic-Tac-Toe\n";
        cout << "7. Obstacles Tic-Tac-Toe\n";
        cout << "8. Infinity Tic-Tac-Toe\n";
        cout << "0. Return to Main Menu\n";
        cout << "--------------------------------------------\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0) break;

        system("cls");
        cout << "\n--- Game Instructions ---\n\n";

        switch (choice) {
        case 1:
            cout << "[ Classic X-O ]\n";
            cout << "Board: 3x3 Grid.\n";
            cout << "Goal: Get 3 of your symbols in a row (Horizontal, Vertical, Diagonal).\n";
            cout << "Winning: First player to align 3 wins.\n";
            break;
        case 2:
            cout << "[ SUS Game ]\n";
            cout << "Board: 3x3 Grid.\n";
            cout << "Goal: Form the sequence 'S-U-S'.\n";
            cout << "Rules: Players place 'S' or 'U'. The game continues until the board is full.\n";
            cout << "Winning: The player with the most 'S-U-S' sequences counts wins.\n";
            break;
        case 3:
            cout << "[ 5x5 Tic-Tac-Toe ]\n";
            cout << "Board: 5x5 Grid.\n";
            cout << "Goal: Get 3 of your symbols in a row.\n";
            cout << "Rules: Game continues until board is full. Count how many 3-in-a-row you made.\n";
            cout << "Winning: Player with the HIGHEST number of 3-in-a-row sequences wins.\n";
            break;
        case 4:
            cout << "[ Misere Tic-Tac-Toe ]\n";
            cout << "Board: 3x3 Grid.\n";
            cout << "Goal: Force your opponent to get 3 in a row.\n";
            cout << "Winning: If you get 3 in a row, you LOSE. If the other player gets 3 in a row, YOU WIN.\n";
            break;
        case 5:
            cout << "[ 4x4 Tic-Tac-Toe ]\n";
            cout << "Board: 4x4 Grid (Tokens start on top and bottom rows).\n";
            cout << "Goal: Align 3 of your tokens in a row.\n";
            cout << "Rules: You DO NOT place new tokens. You MOVE existing tokens to adjacent empty spots.\n";
            cout << "Movement: Horizontal or Vertical only (No Diagonal, No Jumping).\n";
            break;
        case 6:
            cout << "[ Numerical Tic-Tac-Toe ]\n";
            cout << "Board: 3x3 Grid.\n";
            cout << "Pieces: Player 1 (Odd: 1,3,5,7,9), Player 2 (Even: 2,4,6,8).\n";
            cout << "Winning: First player to make a line (row/col/diag) that SUMS to 15 wins.\n";
            break;
        case 7:
            cout << "[ Obstacles Tic-Tac-Toe ]\n";
            cout << "Board: 6x6 Grid.\n";
            cout << "Goal: Align 4 of your symbols in a row.\n";
            cout << "Rules: After each round, the computer randomly places 'Obstacles' (#) on the board.\n";
            cout << "       You cannot play on obstacles.\n";
            break;
        case 8:
            cout << "[ Infinity Tic-Tac-Toe ]\n";
            cout << "Board: 3x3 Grid.\n";
            cout << "Rules: Each player has only 3 moves (slots).\n";
            cout << "       If you play a 4th move, your OLDEST move disappears.\n";
            cout << "Winning: Align 3 symbols in a row.\n";
            break;
        default:
            cout << "Invalid selection.\n";
        }
        cout << "\nPress Enter to go back...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}

// =============================================================================
//  Part 2: Run Functions for Each Game
// =============================================================================

void run_XO_Game() {
    UI<char>* game_ui = new XO_UI();
    Board<char>* xo_board = new X_O_Board();
    Player<char>** players = game_ui->setup_players();
    GameManager<char> game(xo_board, players, game_ui);
    game.run();
    delete xo_board; for (int i = 0; i < 2; ++i) delete players[i]; delete[] players; delete game_ui;
}

void run_SUS_Game() {
    SUS_UI* game_ui = new SUS_UI();
    SUS_Board* sus_board = new SUS_Board();
    Player<char>** players = game_ui->setup_players();
    GameManager<char> game(sus_board, players, game_ui);
    game.run();
    if (sus_board->game_is_over(players[0])) {
        int total = calculate_all_sus_sequences(sus_board->get_board_matrix());
        cout << "\n*** GAME OVER ***\nTotal SUS sequences found: " << total << endl;
    }
    delete sus_board; for (int i = 0; i < 2; ++i) delete players[i]; delete[] players; delete game_ui;
}

void run_FiveByFive_Game() {
    FiveByFive_UI<char>* game_ui = new FiveByFive_UI<char>();
    FiveByFive_Board<char>* board = new FiveByFive_Board<char>();
    Player<char>** players = game_ui->setup_players();
    GameManager<char> game(board, players, game_ui);
    game.run();
    if (board->game_is_over(players[0])) {
        int x = board->count_three_in_a_row('X');
        int o = board->count_three_in_a_row('O');
        cout << "\n*** Final Score ***\nX: " << x << " | O: " << o << endl;
        if (x > o) cout << "X Wins!\n"; else if (o > x) cout << "O Wins!\n"; else cout << "Draw!\n";
    }
    delete board; for (int i = 0; i < 2; ++i) delete players[i]; delete[] players; delete game_ui;
}

void run_Misere_Game() {
    UI<char>* game_ui = new Misere_UI();
    Board<char>* board = new Misere_Board();
    Player<char>** players = game_ui->setup_players();
    GameManager<char> game(board, players, game_ui);
    game.run();
    delete board; for (int i = 0; i < 2; ++i) delete players[i]; delete[] players; delete game_ui;
}

void run_FourByFour_Game() {
    UI<char>* game_ui = new FourByFour_UI<char>();
    Board<char>* board = new FourByFour_Board<char>();
    Player<char>** players = game_ui->setup_players();
    GameManager<char> game(board, players, game_ui);
    game.run();
    delete board; for (int i = 0; i < 2; ++i) delete players[i]; delete[] players; delete game_ui;
}

void run_Numerical_TicTacToe_Game() {
    UI<int>* game_ui = new Numerical_TicTacToe_UI();
    Board<int>* board = new Numerical_TicTacToe_Board();
    Player<int>** players = game_ui->setup_players();
    GameManager<int> game(board, players, game_ui);
    game.run();
    delete board; for (int i = 0; i < 2; ++i) delete players[i]; delete[] players; delete game_ui;
}

void run_Obstacles_Game() {
    UI<char>* game_ui = new Obstacles_UI<char>();
    Board<char>* board = new Obstacles_Board<char>();
    Player<char>** players = game_ui->setup_players();
    GameManager<char> game(board, players, game_ui);
    game.run();
    delete board; for (int i = 0; i < 2; ++i) delete players[i]; delete[] players; delete game_ui;
}

void run_Infinity_TicTacToe_Game() {
    Infinity_TicTacToe_UI<char>* game_ui = new Infinity_TicTacToe_UI<char>();
    Infinity_TicTacToe_Board<char>* board = new Infinity_TicTacToe_Board<char>();
    Player<char>** players = game_ui->setup_players();
    game_ui->set_board(board);
    GameManager<char> game(board, players, game_ui);
    game.run();
    delete board; for (int i = 0; i < 2; ++i) delete players[i]; delete[] players; delete game_ui;
}

// =============================================================================
//  Part 3: Main Menu
// =============================================================================
int main() {
    srand(static_cast<unsigned int>(time(0)));
    int choice;

    while (true) {
        system("cls"); // يمسح المنيو فقط
        cout << "\n============================================\n";
        cout << "      Welcome to FCAI Board Game Station    \n";
        cout << "============================================\n";
        cout << "1:  X-O (Classic)\n";
        cout << "2:  SUS Game                 (Game 1)\n";
        cout << "3:  5x5 Tic-Tac-Toe          (Game 3)\n";
        cout << "4:  Misere Tic-Tac-Toe       (Game 5)\n";
        cout << "5:  4x4 Tic-Tac-Toe          (Game 7)\n";
        cout << "6:  Numerical Tic-Tac-Toe    (Game 9)\n";
        cout << "7:  Obstacles Tic-Tac-Toe    (Game 10)\n";
        cout << "8:  Infinity Tic-Tac-Toe     (Game 11)\n";
        cout << "--------------------------------------------\n";
        cout << "9:  Game Rules & Instructions [?]\n"; // زرار التعليمات
        cout << "0:  Exit\n";
        cout << "--------------------------------------------\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "\n!!! Invalid input. Please enter a number. !!!\n";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Press Enter to continue..."; cin.get(); continue;
        }

        switch (choice) {
        case 1: run_XO_Game(); break;
        case 2: run_SUS_Game(); break;
        case 3: run_FiveByFive_Game(); break;
        case 4: run_Misere_Game(); break;
        case 5: run_FourByFour_Game(); break;
        case 6: run_Numerical_TicTacToe_Game(); break;
        case 7: run_Obstacles_Game(); break;
        case 8: run_Infinity_TicTacToe_Game(); break;
        case 9: show_game_rules(); break; // تشغيل قائمة التعليمات
        case 0:
            cout << "Goodbye! Thanks for playing.\n";
            return 0;
        default:
            cout << "Invalid choice! Please select from 0 to 9.\n";
        }

        cout << "\nPress Enter to return to menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
    return 0;
}