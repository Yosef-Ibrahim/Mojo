#include "sus_classes.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <algorithm>

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
    s_score = 0;
    u_score = 0;
}

// تحديث اللوحة
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

    // 3. إعادة حساب النقاط
    calculate_scores();
    return true;
}

// عرض اللوحة مع النقاط
void SUS_Board::display_board() {
    cout << "\nCurrent Board:\n";
    cout << "S Score: " << s_score << " | U Score: " << u_score << "\n\n";

    for (int i = 0; i < rows; ++i) {
        cout << "   ";
        for (int j = 0; j < columns; ++j) {
            cout << "  " << board[i][j] << "  ";
            if (j < columns - 1) cout << "|";
        }
        cout << endl;
        if (i < rows - 1) {
            cout << "   -----+-----+-----\n";
        }
    }
    cout << endl;
}

// التحقق من تسلسل SUS
bool SUS_Board::is_sus_sequence(const vector<char>& seq) const {
    return (seq.size() == 3 && seq[0] == 'S' && seq[1] == 'U' && seq[2] == 'S');
}

// حساب النقاط
void SUS_Board::calculate_scores() {
    s_score = 0;
    u_score = 0;

    // فحص الصفوف
    for (int i = 0; i < 3; ++i) {
        vector<char> row = { board[i][0], board[i][1], board[i][2] };
        if (is_sus_sequence(row)) {
            s_score++;  // اللاعب S يحصل على النقاط
        }
    }

    // فحص الأعمدة
    for (int j = 0; j < 3; ++j) {
        vector<char> col = { board[0][j], board[1][j], board[2][j] };
        if (is_sus_sequence(col)) {
            s_score++;  // اللاعب S يحصل على النقاط
        }
    }

    // فحص القطر الرئيسي
    vector<char> diag1 = { board[0][0], board[1][1], board[2][2] };
    if (is_sus_sequence(diag1)) {
        s_score++;  // اللاعب S يحصل على النقاط
    }

    // فحص القطر الثانوي
    vector<char> diag2 = { board[0][2], board[1][1], board[2][0] };
    if (is_sus_sequence(diag2)) {
        s_score++;  // اللاعب S يحصل على النقاط
    }
}

// الحصول على نقاط لاعب معين
int SUS_Board::get_player_score(char symbol) const {
    return (symbol == 'S') ? s_score : u_score;
}

// is_win: تحقق من الفوز بناءً على النقاط في النهاية
bool SUS_Board::is_win(Player<char>* player) {
    // لا يوجد فوز فردي خلال اللعبة، فقط في النهاية
    return false;
}

// is_draw: اللعبة تنتهي فقط عندما تمتلئ اللوحة (9 حركات)
bool SUS_Board::is_draw(Player<char>* player) {
    return (n_moves == 9);
}

bool SUS_Board::game_is_over(Player<char>* player) {
    return (n_moves == 9);
}

// ==========================================
// دوال AI
// ==========================================

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
    n_moves++;
    calculate_scores();
}

void SUS_Board::undo_move(int r, int c) {
    board[r][c] = blank_symbol;
    n_moves--;
    calculate_scores();
}

// تقييم اللوحة بناءً على عدد تسلسلات "S-U-S"
int SUS_Board::evaluate_board(char aiSymbol, char humanSymbol) {
    int aiScore = get_player_score(aiSymbol);
    int humanScore = get_player_score(humanSymbol);
    return aiScore - humanScore;
}

// الحصول على أفضل حركة للكمبيوتر باستخدام Minimax
int SUS_Board::minimax(int depth, bool isMax, char aiSymbol, char humanSymbol) {
    if (depth == 0 || get_legal_moves().empty() || game_is_over(nullptr)) {
        return evaluate_board(aiSymbol, humanSymbol);
    }

    if (isMax) {
        int best = -1000;
        for (auto& mv : get_legal_moves()) {
            make_move(mv.first, mv.second, aiSymbol);
            best = max(best, minimax(depth - 1, false, aiSymbol, humanSymbol));
            undo_move(mv.first, mv.second);
        }
        return best;
    }
    else {
        int best = 1000;
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
    int bestScore = -1000;
    pair<int, int> bestMove = { -1, -1 };

    vector<pair<int, int>> legalMoves = get_legal_moves();
    if (legalMoves.empty()) return bestMove;

    for (auto& mv : legalMoves) {
        make_move(mv.first, mv.second, aiSymbol);
        int score = minimax(4, false, aiSymbol, humanSymbol);
        undo_move(mv.first, mv.second);

        if (score > bestScore) {
            bestScore = score;
            bestMove = mv;
        }
    }

    // إذا لم تجد حركة جيدة، اختر أول حركة متاحة
    if (bestMove.first == -1 && !legalMoves.empty()) {
        bestMove = legalMoves[0];
    }

    return bestMove;
}

// ==========================================
// 2. تنفيذ SUS_UI (واجهة المستخدم)
// ==========================================

SUS_UI::SUS_UI() : UI<char>("Welcome to FCAI SUS Game (S-U-S)", 3) {}

Player<char>* SUS_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Player<char>** SUS_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    // اللاعب 1 يأخذ 'S'
    string nameS = get_player_name("Player 1 (Symbol S)");
    PlayerType typeS = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(nameS, 'S', typeS);

    // اللاعب 2 يأخذ 'U'
    string nameU = get_player_name("Player 2 (Symbol U)");
    PlayerType typeU = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(nameU, 'U', typeU);

    return players;
}

void SUS_UI::display_scores() {
    SUS_Board* susBoard = dynamic_cast<SUS_Board*>(board_ptr);
    if (susBoard) {
        cout << "\n=== Current Scores ===" << endl;
        cout << "Player S: " << susBoard->get_s_score() << " points" << endl;
        cout << "Player U: " << susBoard->get_u_score() << " points" << endl;
        cout << "======================" << endl;
    }
}

Move<char>* SUS_UI::get_move(Player<char>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c;
        cout << "\n" << currentPlayer->get_name() << " (Symbol " << currentPlayer->get_symbol()
            << " - fixed), enter Row and Column (0-2 each): ";

        while (!(cin >> r >> c) || r < 0 || r > 2 || c < 0 || c > 2) {
            cout << "\n!!! Invalid input. Please enter numbers between 0 and 2. !!!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << currentPlayer->get_name() << ", enter Row and Column (0-2): ";
        }

        return new Move<char>(r, c, currentPlayer->get_symbol());
    }
    else {
        SUS_Board* susBoard = nullptr;
        if (board_ptr) {
            susBoard = dynamic_cast<SUS_Board*>(board_ptr);
        }

        if (!susBoard) {

            int r, c;
            vector<pair<int, int>> moves;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board_ptr->get_board_matrix()[i][j] == '.') {
                        moves.push_back({ i, j });
                    }
                }
            }

            if (moves.empty()) {
                return nullptr;
            }

            int idx = rand() % moves.size();
            r = moves[idx].first;
            c = moves[idx].second;

            cout << "Computer (random) chose (Row, Col): " << r << ", " << c << endl;
            return new Move<char>(r, c, currentPlayer->get_symbol());
        }

		//ai move
        char aiSymbol = currentPlayer->get_symbol();
        char humanSymbol = (aiSymbol == 'S') ? 'U' : 'S';

        auto best = susBoard->get_best_ai_move(aiSymbol, humanSymbol);

        if (best.first != -1) {
            cout << "Computer (AI) chose (Row, Col): " << best.first << ", " << best.second << endl;
            return new Move<char>(best.first, best.second, currentPlayer->get_symbol());
        }
        else {
            
			//ai failed fallback
            vector<pair<int, int>> moves = susBoard->get_legal_moves();
            if (!moves.empty()) {
                int idx = rand() % moves.size();
                cout << "Computer (fallback) chose (Row, Col): " << moves[idx].first << ", " << moves[idx].second << endl;
                return new Move<char>(moves[idx].first, moves[idx].second, currentPlayer->get_symbol());
            }
        }
    }
    return nullptr;
}