#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cstdlib> 

#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "Numerical_TicTacToe.h" 
#include "FourByFour_Classes.h" // ملف اللعبة الجديدة
<<<<<<< HEAD
#include "sus_classes.h"
=======
>>>>>>> d0362c2e1ca755f79a425dddebdb1a309a615d77

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

// 3. دالة تشغيل لعبة 4x4 Tic-Tac-Toe (اللعبة الجديدة)
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

<<<<<<< HEAD


// 4. دالة تشغيل لعبة SUS
void run_SUS_Game() {
    SUS_UI* game_ui = new SUS_UI();
    SUS_Board* sus_board = new SUS_Board();
    Player<char>** players = game_ui->setup_players();

    GameManager<char> sus_game(sus_board, players, game_ui);

    // تشغيل اللعبة
    sus_game.run();

    // ************* منطق الفوز بلعبة SUS *************
    // اللعبة تنتهي في "تعادل" (is_draw) عندما تمتلئ اللوحة.
    // بعد نهاية اللعبة، نحتسب النقاط لتحديد الفائز.

    if (sus_board->game_is_over(players[0])) {
        int total_sus = calculate_all_sus_sequences(sus_board->get_board_matrix());

        game_ui->display_message("\n*** GAME ENDED by Board Full! ***");
        game_ui->display_message("Total 'S-U-S' sequences found on board: " + to_string(total_sus));

        if (total_sus > 0) {
            // بما أن منطق احتساب النقاط وتحديد من هو اللاعب الذي أكمل التسلسل معقد جداً 
            // ولا يمكن تنفيذه بسهولة في الـ Framework الحالي، نكتفي بإعلان العدد.
            // (هذا يجب أن يتم تحديده من قبل المدرس)
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

=======
>>>>>>> d0362c2e1ca755f79a425dddebdb1a309a615d77
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
        cout << "1: Play X-O Game\n";
        cout << "2: Play Numerical Tic-Tac-Toe\n";
        cout << "3: Play 4x4 Tic-Tac-Toe\n";        // <-- الاختيار الجديد
<<<<<<< HEAD
        cout << "4: Play SUS Game\n";
=======
>>>>>>> d0362c2e1ca755f79a425dddebdb1a309a615d77
        cout << "0: Exit\n";
        cout << "------------------------------\n";
        cout << "Enter your choice: ";

        // التحقق من الإدخال (Robust Input Validation)
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
            run_FourByFour_Game(); // <-- تشغيل اللعبة الجديدة
        }
<<<<<<< HEAD
        else if (choice == 4) { 
            run_SUS_Game();
        }
=======
>>>>>>> d0362c2e1ca755f79a425dddebdb1a309a615d77
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