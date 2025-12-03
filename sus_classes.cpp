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
// ==========================================
// 3. تنفيذ دوال AI في SUS_Board
vector<pair<int, int>> SUS_Board::get_legal_moves() {
    vector<pair<int, int>> moves;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (board[r][c] == blank_symbol)
                moves.push_back({ r,c });
    return moves;
}

void SUS_Board::make_move(int r, int c, char s) {
    board[r][c] = s;
}

void SUS_Board::undo_move(int r, int c) {
    board[r][c] = blank_symbol;
}

// تقييم اللوحة بناءً على عدد تسلسلات "S-U-S"
int SUS_Board::evaluate_board(char aiSymbol, char humanSymbol) {
    int aiScore = calculate_all_sus_sequences(board);
    int humanScore = 0;

    // احسبي لسلاسل U-S-U (لو عايزة تحسبيه)
    // بسيبهوليك كده لو عايزة أفعّله اكتبيه وانا أزبطهولك

    return aiScore - humanScore;
}


// الحصول على أفضل حركة للكمبيوتر باستخدام Minimax
int SUS_Board::minimax(int depth, bool isMax, char aiSymbol, char humanSymbol) {
    if (depth == 0 || get_legal_moves().empty())
        return evaluate_board(aiSymbol, humanSymbol);

    if (isMax) {
        int best = -1e9;
        for (auto& mv : get_legal_moves()) {
            make_move(mv.first, mv.second, aiSymbol);
            best = max(best, minimax(depth - 1, false, aiSymbol, humanSymbol));
            undo_move(mv.first, mv.second);
        }
        return best;
    }
    else {
        int best = 1e9;
        for (auto& mv : get_legal_moves()) {
            make_move(mv.first, mv.second, humanSymbol);
            best = min(best, minimax(depth - 1, true, aiSymbol, humanSymbol));
            undo_move(mv.first, mv.second);
        }
        return best;
    }
}

// دالة للحصول على أفضل حركة للكمبيوتر
pair<int, int> SUS_Board::get_best_ai_move(char aiSymbol, char humanSymbol) {
    int bestScore = -1e9;
    pair<int, int> bestMove = { -1,-1 };

    for (auto& mv : get_legal_moves()) {
        make_move(mv.first, mv.second, aiSymbol);

        int score = minimax(5, false, aiSymbol, humanSymbol);

        undo_move(mv.first, mv.second);

        if (score > bestScore) {
            bestScore = score;
            bestMove = mv;
        }
    }

    return bestMove;
}


Move<char>* SUS_UI::get_move(Player<char>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c;
        cout << "\n" << currentPlayer->get_name() << " (Symbol " << currentPlayer->get_symbol() << " - fixed), enter Row and Column (e.g., 0 2): ";

        while (!(cin >> r >> c)) {
            cout << "\n!!! Invalid input. Please enter numbers (Row Col). !!!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << currentPlayer->get_name() << ", enter Row and Column (e.g., 0 2): ";
        }

        return new Move<char>(r, c, currentPlayer->get_symbol());
    }
    else {
        // ----- استخدم الـ board_ptr اللي ربطناه مسبقاً -----
        SUS_Board* susBoard = nullptr;
        if (board_ptr) {
            susBoard = dynamic_cast<SUS_Board*>(board_ptr);
        }

        if (!susBoard) {
            // fallback آمن: لو ما فيش Board مربوط أو cast فشل => اختار حركة عشوائية
            int r, c;
            do {
                r = rand() % 3;
                c = rand() % 3;
            } while (board_ptr && board_ptr->get_board_matrix()[r][c] != '.');

            cout << "Computer (random fallback) chose (Row, Col): " << r << ", " << c << endl;
            return new Move<char>(r, c, currentPlayer->get_symbol());
        }

        // لو عندنا SUS_Board صالح => خلّي الـ AI يشتغل
        auto best = susBoard->get_best_ai_move(
            currentPlayer->get_symbol(),
            (currentPlayer->get_symbol() == 'S' ? 'U' : 'S')
        );

        cout << "Computer (AI) chose (Row, Col): " << best.first << ", " << best.second << endl;
        return new Move<char>(best.first, best.second, currentPlayer->get_symbol());
    }
}

