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
#include "Memory_TicTacToe.h"
#include "Word_TicTacToe.h"

using namespace std;

// =============================================================================
//  Part 2: Run Functions for Each Game
// =============================================================================

void run_XO() {

    srand(static_cast<unsigned int>(time(0)));
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
}
void run_Pyramid_Game() {
    cout << "\n----------------------------------------------------\n";
    cout << "           Pyramid Tic-Tac-Toe Game                 \n";
    cout << "----------------------------------------------------\n";

    Pyramid_XO_Board<char>* board = new Pyramid_XO_Board<char>();
    Pyramid_XO_UI<char>* ui = new Pyramid_XO_UI<char>();

    ui->set_board(board);

    Player<char>** players = ui->setup_players();

    // GameManager بياخد 3 حاجات
    GameManager<char> pyramid_game(board, players, ui);

    pyramid_game.run();

    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;
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

// 3. 5x5 Tic-Tac-Toe (UPDATED with AI support)
void run_FiveByFive_Game() {
    FiveByFive_UI<char>* ui = new FiveByFive_UI<char>();
    FiveByFive_Board<char>* b = new FiveByFive_Board<char>();

    //  Important: Link UI with Board for AI
    ui->set_board(b);

    Player<char>** p = ui->setup_players();
    GameManager<char> game(b, p, ui);
    game.run();

    if (b->game_is_over(p[0])) {
        int x = b->count_three_in_a_row('X'), o = b->count_three_in_a_row('O');
        cout << "\n============================================\n";
        cout << "              FINAL SCORE                   \n";
        cout << "============================================\n";
        cout << "Player X: " << x << " three-in-a-rows\n";
        cout << "Player O: " << o << " three-in-a-rows\n";
        cout << "--------------------------------------------\n";
        if (x > o) cout << " Player X Wins!\n";
        else if (o > x) cout << " Player O Wins!\n";
        else cout << " It's a Draw!\n";
        cout << "============================================\n";
    }
    delete b;
    for (int i = 0; i < 2; ++i) delete p[i];
    delete[] p;
    delete ui;
}

// 4. Word Tic-Tac-Toe (UPDATED with AI support)
void run_Word_Game() {
    system("cls");
    cout << "\n=======================================\n";
    cout << "        WORD TIC-TAC-TOE               \n";
    cout << "=======================================\n";
    cout << "Goal: Form a valid 3-letter word!\n";
    cout << "Place letters to create words horizontally,\n";
    cout << "vertically, or diagonally.\n";
    cout << "=======================================\n\n";

    Word_UI* ui = new Word_UI();
    Word_Board* board = new Word_Board();

    //  Important: Link UI with Board for AI
    ui->set_board(board);

    Player<char>** players = ui->setup_players();

    GameManager<char> game(board, players, ui);
    game.run();

    // Cleanup
    delete board;
    for (int i = 0; i < 2; ++i) delete players[i];
    delete[] players;
    delete ui;
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

// 6. Misere Tic-Tac-Toe - مثل الألعاب الأخرى تماماً
void run_Misere_Game() {
    system("cls");
    cout << "\n=======================================\n";
    cout << "  MISERE TIC-TAC-TOE (Avoid 3-in-a-row!)";
    cout << "\n=======================================\n";

    // إنشاء اللوحة والواجهة
    Misere_Board* b = new Misere_Board();
    Misere_UI* ui = new Misere_UI();

    // ربط UI بالـ Board عشان الـ AI يشتغل
    ui->set_board(b);

    // إعداد اللاعبين باستخدام setup_players() اللي في UI
    Player<char>** p = ui->setup_players();

    // تشغيل اللعبة
    GameManager<char> game(b, p, ui);
    game.run();

    // تنظيف الذاكرة
    delete b;
    for (int i = 0; i < 2; ++i) delete p[i];
    delete[] p;
    delete ui;
}

// 7. 4x4 Tic-Tac-Toe
// في ملف XO_Demo.cpp
// استبدل دالة run_FourByFour_Game بالموجودة دي:

void run_FourByFour_Game() {
    cout << "\n----------------------------------------------------\n";
    cout << "        4x4 Tic-Tac-Toe (Moving Pieces)             \n";
    cout << "----------------------------------------------------\n";

    FourByFour_Board<char>* board = new FourByFour_Board<char>();
    FourByFour_UI<char>* ui = new FourByFour_UI<char>();

    ui->set_board(board);

    Player<char>** players = ui->setup_players();

    GameManager<char> game(board, players, ui);
    game.run();

    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;
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

// 12. Diamond Tic-Tac-Toe  مع Smart AI
void run_Diamond_Game() {
    cout << "\n=======================================\n";
    cout << "  Diamond Tic-Tac-Toe with Smart AI      ";
    cout << "\n=========================================\n\n";


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


// 13. Memory Tic-Tac-Toe
void run_Memory_Game() {
    system("cls");
    cout << "\n=======================================\n";
    cout << "     MEMORY TIC-TAC-TOE (Bonus)       \n";
    cout << "=======================================\n\n";

    cout << "RULES:\n";
    cout << "1. Players take turns placing X or O\n";
    cout << "2. Marks become HIDDEN after placement\n";
    cout << "3. You must remember positions\n";
    cout << "4. First to get 3 in a row WINS\n";
    cout << "=======================================\n\n";

    // إنشاء اللوحة والواجهة
    Memory_Board* board = new Memory_Board();
    Memory_UI* ui = new Memory_UI();

    // ربط UI بالـ Board
    ui->set_board(board);

    // إعداد اللاعبين
    Player<char>** players = ui->setup_players();

    // بدء اللعبة
    GameManager<char> game(board, players, ui);
    game.run();

    cout << "\nPress Enter to see the hidden board...";
    cin.ignore();
    cin.get();
    board->display_hidden_board();

    // تنظيف الذاكرة
    delete board;
    for (int i = 0; i < 2; ++i) delete players[i];
    delete[] players;
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
        cout << "4. Word Tic-Tac-Toe\n";
        cout << "5. Numerical Tic-Tac-Toe\n";
        cout << "6. Misere Tic-Tac-Toe\n";
        cout << "7. 4x4 Tic-Tac-Toe\n";
        cout << "8. Ultimate Tic-Tac-Toe\n";
        cout << "9. SUS Game\n";
        cout << "10. Obstacles Tic-Tac-Toe\n";
        cout << "11. Infinity Tic-Tac-Toe\n";
        cout << "12. Diamond Tic-Tac-Toe \n";
        cout << "13. Memory Tic-Tac-Toe \n";
        cout << "14. X-O game\n";
        cout << "0. Exit \n";
        cout << "------------------------------------------------\n";
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
        case 4: run_Word_Game(); break;
        case 5: run_Numerical_TicTacToe_Game(); break;
        case 6: run_Misere_Game(); break;
        case 7: run_FourByFour_Game(); break;
        case 8: run_Ultimate_Game(); break;
        case 9: run_SUS_Game(); break;
        case 10: run_Obstacles_Game(); break;
        case 11: run_Infinity_TicTacToe_Game(); break;
        case 12: run_Diamond_Game(); break;
        case 13: run_Memory_Game(); break;
        case 14: run_XO(); break;
        case 0: cout << "Goodbye!\n"; return 0;
        default: cout << "Invalid choice!\n";
        }

        cout << "\nPress Enter to return to menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
    return 0;
}