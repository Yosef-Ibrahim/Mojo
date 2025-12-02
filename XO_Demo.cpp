#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cstdlib> 

// استدعاء ملفات الألعاب
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Pyramid_TicTacToe.h"
#include "FiveByFive_Classes.h"
#include "Misere.h"
#include "FourByFour_Classes.h"
#include "Numerical_TicTacToe.h"
#include "sus_classes.h"
#include "Obstacles_Classes.h"
#include "Infinity_TicTacToe.h"
#include "Four_In_A_Row_UI.h"
#include "Diamond_TicTacToe.h"

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
        cout << "1. Pyramid Tic-Tac-Toe\n";
        cout << "2. Four-in-a-row (Connect 4)\n";
        cout << "3. 5x5 Tic-Tac-Toe\n";
        cout << "4. Wordle\n";
        cout << "5. Numerical Tic-Tac-Toe\n";
        cout << "6. Misere Tic-Tac-Toe\n";
        cout << "7. 4x4 Tic-Tac-Toe\n";
        cout << "8. Ultimate Tic-Tac-Toe\n";
        cout << "9. SUS Game\n";
        cout << "10. Obstacles Tic-Tac-Toe\n";
        cout << "11. Infinity Tic-Tac-Toe\n";
        cout << "12. Diamond Tic-Tac-Toe\n";
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
            cout << "[ Pyramid Tic-Tac-Toe ]\n";
            cout << "Board: Pyramid shape (1 top, 3 middle, 5 bottom).\n";
            cout << "Goal: Align 3 symbols consecutively (Horizontally, Vertically, or Diagonally).\n";
            break;
        case 2: cout << "[ Connect 4 ]\nAlign 4 symbols vertically, horizontally, or diagonally.\n"; break;
        case 3: cout << "[ 5x5 Tic-Tac-Toe ]\nCreate as many 3-in-a-row sequences as possible. Max score wins.\n"; break;
        case 4: cout << "[ Wordle ]\nGuess the hidden 5-letter word in 6 tries.\n"; break;
        case 5: cout << "[ Numerical ]\nUse numbers 1-9. First to make a line summing to 15 wins.\n"; break;
        case 6: cout << "[ Misere ]\nAvoid getting 3 in a row. If you get 3 in a row, you LOSE.\n"; break;
        case 7: cout << "[ 4x4 Tic-Tac-Toe ]\nMove your existing tokens to adjacent spots to form a line of 3.\n"; break;
        case 8: cout << "[ Ultimate ]\nWin small boards to claim cells on the big board.\n"; break;
        case 9: cout << "[ SUS ]\nForm 'S-U-S' sequences. The player with the most sequences wins.\n"; break;
        case 10: cout << "[ Obstacles ]\n6x6 grid. Computer blocks cells randomly after every round.\n"; break;
        case 11: cout << "[ Infinity ]\nOnly 3 moves per player. The 4th move erases the oldest one.\n"; break;
        case 12:
            cout << "[ Diamond Tic-Tac-Toe ]\n";
            cout << "Rules: Players take turns placing their 'X' or 'O' in empty cells\n";
            cout << "Winning Condition: A player wins by simultaneously completing\n";
            cout << "a line of three marks AND a line of four marks.\n";
            cout << "The two lines must be in different directions but can share one common mark.\n";
            break;
        default: cout << "Invalid selection.\n";
        }
        cout << "\nPress Enter to go back...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}

// =============================================================================
//  Part 2: Run Functions for Each Game
// =============================================================================

// 1. Pyramid Tic-Tac-Toe
void run_Pyramid_Game() {
    Pyramid_XO_UI<char>* game_ui = new Pyramid_XO_UI<char>();
    Pyramid_XO_Board<char>* board = new Pyramid_XO_Board<char>();
    Player<char>** players = game_ui->setup_players();
    GameManager<char> game(board, players, game_ui);
    game.run();
    delete board;
    for (int i = 0; i < 2; ++i) delete players[i];
    delete[] players;
    delete game_ui;
}

// 2. Connect 4 
void run_Connect4_Game() {
    Board<char>* board = new Four_In_A_Row_Board();
    UI<char>* ui = new Four_In_A_Row_UI(board);
    Player<char>** players = ui->setup_players();
    GameManager<char> game(board, players, ui);
    game.run();
    delete board;
    for (int i = 0; i < 2; ++i) delete players[i];
    delete[] players;
    delete ui;
}

// 3. 5x5 Tic-Tac-Toe
void run_FiveByFive_Game() {
    FiveByFive_UI<char>* ui = new FiveByFive_UI<char>();
    FiveByFive_Board<char>* b = new FiveByFive_Board<char>();
    Player<char>** p = ui->setup_players();
    GameManager<char> game(b, p, ui);
    game.run();
    if (b->game_is_over(p[0])) {
        int x = b->count_three_in_a_row('X'), o = b->count_three_in_a_row('O');
        cout << "\nScore - X: " << x << " | O: " << o << endl;
        if (x > o) cout << "X Wins!\n";
        else if (o > x) cout << "O Wins!\n";
        else cout << "Draw!\n";
    }
    delete b;
    for (int i = 0; i < 2; ++i) delete p[i];
    delete[] p;
    delete ui;
}

// 4. Wordle (Not Implemented yet)
void run_Wordle_Game() {
    cout << "\n[Wordle] Not implemented yet.\n";
}

// 5. Numerical Tic-Tac-Toe
void run_Numerical_TicTacToe_Game() {
    Numerical_TicTacToe_UI* ui = new Numerical_TicTacToe_UI();
    Board<int>* b = new Numerical_TicTacToe_Board();
    Player<int>** p = ui->setup_players();
    ui->set_board(b);
    GameManager<int> game(b, p, ui);
    game.run();
    delete b;
    for (int i = 0; i < 2; ++i) delete p[i];
    delete[] p;
    delete ui;
}

// 6. Misere Tic-Tac-Toe
void run_Misere_Game() {
    UI<char>* ui = new Misere_UI();
    Board<char>* b = new Misere_Board();
    Player<char>** p = ui->setup_players();
    GameManager<char> game(b, p, ui);
    game.run();
    delete b;
    for (int i = 0; i < 2; ++i) delete p[i];
    delete[] p;
    delete ui;
}

// 7. 4x4 Tic-Tac-Toe
void run_FourByFour_Game() {
    UI<char>* ui = new FourByFour_UI<char>();
    Board<char>* b = new FourByFour_Board<char>();
    Player<char>** p = ui->setup_players();
    GameManager<char> game(b, p, ui);
    game.run();
    delete b;
    for (int i = 0; i < 2; ++i) delete p[i];
    delete[] p;
    delete ui;
}

// 8. Ultimate Tic-Tac-Toe (Not Implemented yet)
void run_Ultimate_Game() {
    cout << "\n[Ultimate Tic-Tac-Toe] Not implemented yet.\n";
}

// 9. SUS Game
void run_SUS_Game() {
    SUS_UI* ui = new SUS_UI();
    SUS_Board* b = new SUS_Board();
    Player<char>** p = ui->setup_players();
    GameManager<char> game(b, p, ui);
    game.run();
    if (b->game_is_over(p[0])) {
        int t = calculate_all_sus_sequences(b->get_board_matrix());
        cout << "\nTotal SUS: " << t << endl;
    }
    delete b;
    for (int i = 0; i < 2; ++i) delete p[i];
    delete[] p;
    delete ui;
}

// 10. Obstacles Tic-Tac-Toe
void run_Obstacles_Game() {
    UI<char>* ui = new Obstacles_UI<char>();
    Board<char>* b = new Obstacles_Board<char>();
    Player<char>** p = ui->setup_players();
    GameManager<char> game(b, p, ui);
    game.run();
    delete b;
    for (int i = 0; i < 2; ++i) delete p[i];
    delete[] p;
    delete ui;
}

// 11. Infinity Tic-Tac-Toe
void run_Infinity_TicTacToe_Game() {
    Infinity_UI* ui = new Infinity_UI();
    Infinity_Board* b = new Infinity_Board();
    Player<char>** p = ui->setup_players();
    GameManager<char> game(b, p, ui);
    game.run();
    delete b;
    for (int i = 0; i < 2; ++i) delete p[i];
    delete[] p;
    delete ui;
}

// 12. Diamond Tic-Tac-Toe 🤖 مع Smart AI
void run_Diamond_Game() {
    cout << "\n=======================================\n";
    cout << "  Diamond Tic-Tac-Toe with Smart AI      ";
    cout << "\n=========================================\n\n";

    cout << "# WINNING CONDITION:\n";
    cout << "---------------------------\n";
    cout << "   Create BOTH a line of 3 AND a line of 4!\n";
    cout << "   The lines must be in different directions.\n\n";

    Diamond_Board* b = new Diamond_Board();
    Diamond_UI* ui = new Diamond_UI();

    // 🔥 مهم جداً: ربط الـ UI مع الـ Board عشان الـ AI يشتغل صح
    ui->set_board(b);

    Player<char>** p = ui->setup_players();

    GameManager<char> game(b, p, ui);
    game.run();

    // تنظيف الذاكرة
    delete b;
    for (int i = 0; i < 2; ++i) delete p[i];
    delete[] p;
    delete ui;
}

// =============================================================================
//  Part 3: Main Menu
// =============================================================================
int main() {
    srand(static_cast<unsigned int>(time(0)));
    int choice;

    while (true) {
        system("cls");
        cout << "\n============================================\n";
        cout << "      Welcome to FCAI Board Game Station    \n";
        cout << "============================================\n";
        cout << "1. Pyramid Tic-Tac-Toe\n";
        cout << "2. Four-in-a-row (Connect 4)\n";
        cout << "3. 5x5 Tic-Tac-Toe\n";
        cout << "4. Wordle\n";
        cout << "5. Numerical Tic-Tac-Toe\n";
        cout << "6. Misere Tic-Tac-Toe\n";
        cout << "7. 4x4 Tic-Tac-Toe\n";
        cout << "8. Ultimate Tic-Tac-Toe\n";
        cout << "9. SUS Game\n";
        cout << "10. Obstacles Tic-Tac-Toe\n";
        cout << "11. Infinity Tic-Tac-Toe\n";
        cout << "12. Diamond Tic-Tac-Toe \n";
        cout << "--------------------------------------------\n";
        cout << "13. Game Rules & Instructions [?]\n";
        cout << "0.  Exit\n";
        cout << "--------------------------------------------\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "\nInvalid input.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Press Enter...";
            cin.get();
            continue;
        }

        switch (choice) {
        case 1: run_Pyramid_Game(); break;
        case 2: run_Connect4_Game(); break;
        case 3: run_FiveByFive_Game(); break;
        case 4: run_Wordle_Game(); break;
        case 5: run_Numerical_TicTacToe_Game(); break;
        case 6: run_Misere_Game(); break;
        case 7: run_FourByFour_Game(); break;
        case 8: run_Ultimate_Game(); break;
        case 9: run_SUS_Game(); break;
        case 10: run_Obstacles_Game(); break;
        case 11: run_Infinity_TicTacToe_Game(); break;
        case 12: run_Diamond_Game(); break;
        case 13: show_game_rules(); break;
        case 0: cout << "Goodbye!\n"; return 0;
        default: cout << "Invalid choice!\n";
        }

        cout << "\nPress Enter to return to menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
    return 0;
}