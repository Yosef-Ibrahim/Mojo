#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cstdlib> 

#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Numerical_TicTacToe.h" 
#include "FourByFour_Classes.h"
#include "FiveByFive_Classes.h"  // اللعبة الجديدة
#include "sus_classes.h"

using namespace std;

// -----------------------------------------------------------------------------
// دوال تشغيل الألعاب
// -----------------------------------------------------------------------------

// 1. دالة تشغيل لعبة X-O
void run_XO_Game() {
    UI<char>* game_ui = new XO_UI();
    Board<char>* xo_board = new X_O_Board();
    Player<char>** players = game_ui->setup_players();

    GameManager<char> x_o_game(xo_board, players, game_ui);
    x_o_game.run();

    // تنظيف الذاكرة
    delete xo_board;
    for (int i = 0; i < 2; ++i) delete players[i];
    delete[] players;
    delete game_ui;
}

// 2. دالة تشغيل لعبة Numerical Tic-Tac-Toe
void run_Numerical_TicTacToe_Game() {
    UI<int>* game_ui = new Numerical_TicTacToe_UI();
    Board<int>* num_board = new Numerical_TicTacToe_Board();
    Player<int>** players = game_ui->setup_players();

    GameManager<int> num_game(num_board, players, game_ui);
    num_game.run();

    // تنظيف الذاكرة
    delete num_board;
    for (int i = 0; i < 2; ++i) delete players[i];
    delete[] players;
    delete game_ui;
}

// 3. دالة تشغيل لعبة 4x4 Tic-Tac-Toe
void run_FourByFour_Game() {
    UI<char>* game_ui = new FourByFour_UI<char>();
    Board<char>* board = new FourByFour_Board<char>();
    Player<char>** players = game_ui->setup_players();

    GameManager<char> game(board, players, game_ui);
    game.run();

    // تنظيف الذاكرة
    delete board;
    for (int i = 0; i < 2; ++i) delete players[i];
    delete[] players;
    delete game_ui;
}

// 4. دالة تشغيل لعبة 5x5 Tic-Tac-Toe (اللعبة الجديدة)
void run_FiveByFive_Game() {
    FiveByFive_UI<char>* game_ui = new FiveByFive_UI<char>();
    FiveByFive_Board<char>* board = new FiveByFive_Board<char>();
    Player<char>** players = game_ui->setup_players();

    GameManager<char> game(board, players, game_ui);
    game.run();

    // بعد انتهاء اللعبة، نحسب عدد التسلسلات لكل لاعب
    if (board->game_is_over(players[0])) {
        int x_count = board->count_three_in_a_row('X');
        int o_count = board->count_three_in_a_row('O');

        game_ui->display_message("\n*** GAME ENDED! ***");
        game_ui->display_message("Player X three-in-a-row sequences: " + to_string(x_count));
        game_ui->display_message("Player O three-in-a-row sequences: " + to_string(o_count));

        if (x_count > o_count) {
            game_ui->display_message(players[0]->get_name() + " (X) wins with more sequences!");
        }
        else if (o_count > x_count) {
            game_ui->display_message(players[1]->get_name() + " (O) wins with more sequences!");
        }
        else {
            game_ui->display_message("It's a tie! Both players have the same number of sequences.");
        }
    }

    // تنظيف الذاكرة
    delete board;
    for (int i = 0; i < 2; ++i) delete players[i];
    delete[] players;
    delete game_ui;
}

// 5. دالة تشغيل لعبة SUS
void run_SUS_Game() {
    SUS_UI* game_ui = new SUS_UI();
    SUS_Board* sus_board = new SUS_Board();
    Player<char>** players = game_ui->setup_players();

    GameManager<char> sus_game(sus_board, players, game_ui);

    // تشغيل اللعبة
    sus_game.run();

    // ************* منطق الفوز بلعبة SUS *************
    if (sus_board->game_is_over(players[0])) {
        int total_sus = calculate_all_sus_sequences(sus_board->get_board_matrix());

        game_ui->display_message("\n*** GAME ENDED by Board Full! ***");
        game_ui->display_message("Total 'S-U-S' sequences found on board: " + to_string(total_sus));

        if (total_sus > 0) {
            game_ui->display_message("Winner determination requires specific scoring logic (which player created the most S-U-S).");
        }
        else {
            game_ui->display_message("Draw! No 'S-U-S' sequences were formed.");
        }
    }

    // تنظيف الذاكرة
    delete sus_board;
    for (int i = 0; i < 2; ++i) delete players[i];
    delete[] players;
    delete game_ui;
}

// -----------------------------------------------------------------------------
// Main Function
// -----------------------------------------------------------------------------
int main() {
    srand(static_cast<unsigned int>(time(0)));

    int choice;

    while (true) {
        system("cls"); // مسح الشاشة
        cout << "\n==============================\n";
        cout << "  Welcome to FCAI Games Menu  \n";
        cout << "==============================\n";
        cout << "1: Play X-O Game (3x3)\n";
        cout << "2: Play Numerical Tic-Tac-Toe\n";
        cout << "3: Play 4x4 Tic-Tac-Toe\n";
        cout << "4: Play 5x5 Tic-Tac-Toe\n";       // <-- الاختيار الجديد
        cout << "5: Play SUS Game\n";
        cout << "0: Exit\n";
        cout << "------------------------------\n";
        cout << "Enter your choice: ";

        // التحقق من الإدخال
        if (!(cin >> choice)) {
            cout << "\n!!! Invalid choice. Please enter a number. !!!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nPress Enter to continue...";
            cin.get();
            continue;
        }

        // توجيه الاختيار للدالة المناسبة
        if (choice == 1) {
            run_XO_Game();
        }
        else if (choice == 2) {
            run_Numerical_TicTacToe_Game();
        }
        else if (choice == 3) {
            run_FourByFour_Game();
        }
        else if (choice == 4) {
            run_FiveByFive_Game(); // <-- تشغيل اللعبة الجديدة
        }
        else if (choice == 5) {
            run_SUS_Game();
        }
        else if (choice == 0) {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Invalid choice. Please try again.\n";
        }

        // وقفة بعد نهاية اللعبة قبل العودة للقائمة
        cout << "\nPress Enter to return to menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    return 0;
}