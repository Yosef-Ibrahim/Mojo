#include "sus_classes.h"
#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

// ==========================================
// 1. تنفيذ SUS_Board (اللوحة)
// ==========================================

SUS_Board::SUS_Board() : Board<char>(3, 3) {
    // تصفير اللوحة
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = blank_symbol;
        }
    }
}

// تحديث اللوحة: هذه الدالة فقط تتأكد من أن الخانة فارغة وتضع الرمز.
bool SUS_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // 1. التأكد من الحدود ومن أن الخانة فارغة
    if (x < 0 || x >= rows || y < 0 || y >= columns || board[x][y] != blank_symbol) {
        return false;
    }

    // 2. تحديث اللوحة
    board[x][y] = mark;
    n_moves++;
    return true;
}

// is_win: دائماً false لأن الفوز يعتمد على النقاط في النهاية
bool SUS_Board::is_win(Player<char>* player) {
    return false;
}

// is_draw: اللعبة تنتهي فقط عندما تمتلئ اللوحة (9 حركات)
bool SUS_Board::is_draw(Player<char>* player) {
    return (n_moves == 9);
}

bool SUS_Board::game_is_over(Player<char>* player) {
    return is_draw(player);
}

// دالة مساعدة لحساب جميع تسلسلات "S-U-S" على اللوحة
int calculate_all_sus_sequences(const vector<vector<char>>& board) {
    int count = 0;
    int rows = board.size();
    int cols = board[0].size();

    // فحص الصفوف (3)
    for (int i = 0; i < rows; ++i) {
        if (board[i][0] == 'S' && board[i][1] == 'U' && board[i][2] == 'S') count++;
    }

    // فحص الأعمدة (3)
    for (int j = 0; j < cols; ++j) {
        if (board[0][j] == 'S' && board[1][j] == 'U' && board[2][j] == 'S') count++;
    }

    // فحص الأقطار (2)
    // الرئيسي
    if (board[0][0] == 'S' && board[1][1] == 'U' && board[2][2] == 'S') count++;
    // الثانوي
    if (board[0][2] == 'S' && board[1][1] == 'U' && board[2][0] == 'S') count++;

    return count;
}


// ==========================================
// 2. تنفيذ SUS_UI (واجهة المستخدم)
// ==========================================

SUS_UI::SUS_UI() : UI<char>("Welcome to FCAI SUS Game (S-U-S)", 3) {}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    // نستخدم الـ Player العادي ونعتمد على نوع اللاعب (Type) للتمييز بين البشر والكمبيوتر
    return new Player<char>(name, symbol, type);
}

Player<char>** SUS_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    // بما أنك تستخدم PlayerType::COMPUTER لـ Random، سنستخدمها
    vector<string> type_options = { "Human", "Computer" };

    // اللاعب 1 دائماً يأخذ 'S' (بناء على قاعدتك "يجب أن يستخدم اللاعب نفس الحرف")
    string nameS = get_player_name("Player 1 (Symbol S)");
    PlayerType typeS = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(nameS, 'S', typeS);

    // اللاعب 2 دائماً يأخذ 'U'
    string nameU = get_player_name("Player 2 (Symbol U)");
    PlayerType typeU = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(nameU, 'U', typeU);

    return players;
}

Move<char>* SUS_UI::get_move(Player<char>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c;

        cout << "\n" << currentPlayer->get_name() << " (Symbol " << currentPlayer->get_symbol() << " - fixed), enter Row and Column (e.g., 0 2): ";

        // التحقق من الإدخال
        while (!(cin >> r >> c)) {
            cout << "\n!!! Invalid input. Please enter numbers (Row Col). !!!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << currentPlayer->get_name() << ", enter Row and Column (e.g., 0 2): ";
        }

        return new Move<char>(r, c, currentPlayer->get_symbol());
    }
    else {
        // الكمبيوتر العشوائي (Random/Computer)
        int r, c;
        // هنا يجب أن يختار الكمبيوتر حركة صحيحة (لكن نعتمد على الـ Board للتحقق)
        r = rand() % 3;
        c = rand() % 3;

        cout << "Computer " << currentPlayer->get_name() << " chose (Row, Col): " << r << ", " << c << endl;
        return new Move<char>(r, c, currentPlayer->get_symbol());
    }
}