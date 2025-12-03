#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <iomanip>
#include <limits>
#include "Diamond_TicTacToe.h"

using namespace std;

// ============================================================================
// Diamond_Board Implementation
// ============================================================================

Diamond_Board::Diamond_Board() : Board(5, 5) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Diamond_Board::is_valid_cell(int x, int y) const {
    if (x < 0 || x >= 5 || y < 0 || y >= 5)
        return false;

    if (x == 0 || x == 4)
        return y == 2;

    if (x == 1 || x == 3)
        return y >= 1 && y <= 3;

    return true;
}

bool Diamond_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!is_valid_cell(x, y) || board[x][y] != blank_symbol)
        return false;

    board[x][y] = toupper(mark);
    n_moves++;
    return true;
}

vector<pair<int, int>> Diamond_Board::get_valid_moves() const {
    vector<pair<int, int>> moves;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (is_valid_cell(i, j) && board[i][j] == blank_symbol) {
                moves.push_back({ i, j });
            }
        }
    }
    return moves;
}

int Diamond_Board::count_lines(char symbol, int length) const {
    int count = 0;

    // Horizontal
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= columns - length; j++) {
            bool valid_line = true;
            for (int k = 0; k < length; k++) {
                if (!is_valid_cell(i, j + k)) {
                    valid_line = false;
                    break;
                }
            }
            if (!valid_line) continue;

            bool line = true;
            for (int k = 0; k < length; k++) {
                if (board[i][j + k] != symbol) {
                    line = false;
                    break;
                }
            }
            if (line) count++;
        }
    }

    // Vertical
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i <= rows - length; i++) {
            bool valid_line = true;
            for (int k = 0; k < length; k++) {
                if (!is_valid_cell(i + k, j)) {
                    valid_line = false;
                    break;
                }
            }
            if (!valid_line) continue;

            bool line = true;
            for (int k = 0; k < length; k++) {
                if (board[i + k][j] != symbol) {
                    line = false;
                    break;
                }
            }
            if (line) count++;
        }
    }

    // Diagonal (\)
    for (int i = 0; i <= rows - length; i++) {
        for (int j = 0; j <= columns - length; j++) {
            bool valid_line = true;
            for (int k = 0; k < length; k++) {
                if (!is_valid_cell(i + k, j + k)) {
                    valid_line = false;
                    break;
                }
            }
            if (!valid_line) continue;

            bool line = true;
            for (int k = 0; k < length; k++) {
                if (board[i + k][j + k] != symbol) {
                    line = false;
                    break;
                }
            }
            if (line) count++;
        }
    }

    // Diagonal (/)
    for (int i = 0; i <= rows - length; i++) {
        for (int j = length - 1; j < columns; j++) {
            bool valid_line = true;
            for (int k = 0; k < length; k++) {
                if (!is_valid_cell(i + k, j - k)) {
                    valid_line = false;
                    break;
                }
            }
            if (!valid_line) continue;

            bool line = true;
            for (int k = 0; k < length; k++) {
                if (board[i + k][j - k] != symbol) {
                    line = false;
                    break;
                }
            }
            if (line) count++;
        }
    }

    return count;
}

bool Diamond_Board::check_line_length(char symbol, int length) const {
    return count_lines(symbol, length) > 0;
}

bool Diamond_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();

    // لازم يكون عنده line of 3 AND line of 4
    if (!check_line_length(sym, 3) || !check_line_length(sym, 4)) {
        return false;
    }

    // دلوقتي نتأكد إن الخطين في اتجاهات مختلفة
    // نجمع كل خطوط الـ 3 وكل خطوط الـ 4
    vector<pair<string, vector<pair<int, int>>>> lines_of_3;
    vector<pair<string, vector<pair<int, int>>>> lines_of_4;

    // جمع خطوط الـ 3
    // Horizontal
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= columns - 3; j++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < 3; k++) {
                if (!is_valid_cell(i, j + k)) { valid = false; break; }
                line.push_back({ i, j + k });
            }
            if (!valid) continue;

            bool is_line = true;
            for (int k = 0; k < 3; k++) {
                if (board[i][j + k] != sym) { is_line = false; break; }
            }
            if (is_line) lines_of_3.push_back({ "H", line });
        }
    }

    // Vertical
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i <= rows - 3; i++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < 3; k++) {
                if (!is_valid_cell(i + k, j)) { valid = false; break; }
                line.push_back({ i + k, j });
            }
            if (!valid) continue;

            bool is_line = true;
            for (int k = 0; k < 3; k++) {
                if (board[i + k][j] != sym) { is_line = false; break; }
            }
            if (is_line) lines_of_3.push_back({ "V", line });
        }
    }

    // Diagonal (\)
    for (int i = 0; i <= rows - 3; i++) {
        for (int j = 0; j <= columns - 3; j++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < 3; k++) {
                if (!is_valid_cell(i + k, j + k)) { valid = false; break; }
                line.push_back({ i + k, j + k });
            }
            if (!valid) continue;

            bool is_line = true;
            for (int k = 0; k < 3; k++) {
                if (board[i + k][j + k] != sym) { is_line = false; break; }
            }
            if (is_line) lines_of_3.push_back({ "D1", line });
        }
    }

    // Diagonal (/)
    for (int i = 0; i <= rows - 3; i++) {
        for (int j = 2; j < columns; j++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < 3; k++) {
                if (!is_valid_cell(i + k, j - k)) { valid = false; break; }
                line.push_back({ i + k, j - k });
            }
            if (!valid) continue;

            bool is_line = true;
            for (int k = 0; k < 3; k++) {
                if (board[i + k][j - k] != sym) { is_line = false; break; }
            }
            if (is_line) lines_of_3.push_back({ "D2", line });
        }
    }

    // جمع خطوط الـ 4
    // Horizontal
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j <= columns - 4; j++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < 4; k++) {
                if (!is_valid_cell(i, j + k)) { valid = false; break; }
                line.push_back({ i, j + k });
            }
            if (!valid) continue;

            bool is_line = true;
            for (int k = 0; k < 4; k++) {
                if (board[i][j + k] != sym) { is_line = false; break; }
            }
            if (is_line) lines_of_4.push_back({ "H", line });
        }
    }

    // Vertical
    for (int j = 0; j < columns; j++) {
        for (int i = 0; i <= rows - 4; i++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < 4; k++) {
                if (!is_valid_cell(i + k, j)) { valid = false; break; }
                line.push_back({ i + k, j });
            }
            if (!valid) continue;

            bool is_line = true;
            for (int k = 0; k < 4; k++) {
                if (board[i + k][j] != sym) { is_line = false; break; }
            }
            if (is_line) lines_of_4.push_back({ "V", line });
        }
    }

    // Diagonal (\)
    for (int i = 0; i <= rows - 4; i++) {
        for (int j = 0; j <= columns - 4; j++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < 4; k++) {
                if (!is_valid_cell(i + k, j + k)) { valid = false; break; }
                line.push_back({ i + k, j + k });
            }
            if (!valid) continue;

            bool is_line = true;
            for (int k = 0; k < 4; k++) {
                if (board[i + k][j + k] != sym) { is_line = false; break; }
            }
            if (is_line) lines_of_4.push_back({ "D1", line });
        }
    }

    // Diagonal (/)
    for (int i = 0; i <= rows - 4; i++) {
        for (int j = 3; j < columns; j++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < 4; k++) {
                if (!is_valid_cell(i + k, j - k)) { valid = false; break; }
                line.push_back({ i + k, j - k });
            }
            if (!valid) continue;

            bool is_line = true;
            for (int k = 0; k < 4; k++) {
                if (board[i + k][j - k] != sym) { is_line = false; break; }
            }
            if (is_line) lines_of_4.push_back({ "D2", line });
        }
    }

    // دلوقتي نشوف لو في line of 3 و line of 4 في اتجاهات مختلفة
    for (size_t i = 0; i < lines_of_3.size(); i++) {
        for (size_t j = 0; j < lines_of_4.size(); j++) {
            // لازم يكونوا في اتجاهات مختلفة
            if (lines_of_3[i].first != lines_of_4[j].first) {
                return true; // فاز!
            }
        }
    }

    return false; // مفيش فوز لسه
}

bool Diamond_Board::is_draw(Player<char>* player) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (is_valid_cell(i, j) && board[i][j] == blank_symbol) {
                return false;
            }
        }
    }
    return !is_win(player);
}

bool Diamond_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ============================================================================
// Diamond_UI Implementation
// ============================================================================

Diamond_UI::Diamond_UI() : UI<char>("=== Diamond Tic-Tac-Toe ===", 3), board_ref(nullptr) {

}

Player<char>* Diamond_UI::create_player(std::string& name, char symbol, PlayerType type) {
    if (type == PlayerType::AI) {
        cout << " Creating Smart AI player: " << name << " (" << symbol << ") \n";
    }
    else { // Human فقط
        cout << " Creating Human player: " << name << " (" << symbol << ")\n";
    }
    return new Player<char>(name, symbol, type);
}

// حساب الخطوط المحتملة (خطوط فيها رمزين أو أكثر وباقي فاضي)
int Diamond_UI::count_potential_lines(char symbol, int length) {
    if (!board_ref) return 0;

    auto& matrix = board_ref->get_board_matrix();
    char opponent = (symbol == 'X') ? 'O' : 'X';
    int count = 0;

    auto check_line = [&](vector<pair<int, int>>& positions) -> bool {
        int my_count = 0;
        int empty_count = 0;

        for (size_t i = 0; i < positions.size(); i++) {
            int r = positions[i].first;
            int c = positions[i].second;

            if (matrix[r][c] == symbol) my_count++;
            else if (matrix[r][c] == '.') empty_count++;
            else return false; // خط مسدود بسبب الخصم
        }

        return (my_count >= 2 && empty_count > 0);
        };

    // Horizontal
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j <= 5 - length; j++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < length; k++) {
                if (!board_ref->is_valid_cell(i, j + k)) {
                    valid = false;
                    break;
                }
                line.push_back({ i, j + k });
            }
            if (valid && check_line(line)) count++;
        }
    }

    // Vertical
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i <= 5 - length; i++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < length; k++) {
                if (!board_ref->is_valid_cell(i + k, j)) {
                    valid = false;
                    break;
                }
                line.push_back({ i + k, j });
            }
            if (valid && check_line(line)) count++;
        }
    }

    // Diagonal (\)
    for (int i = 0; i <= 5 - length; i++) {
        for (int j = 0; j <= 5 - length; j++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < length; k++) {
                if (!board_ref->is_valid_cell(i + k, j + k)) {
                    valid = false;
                    break;
                }
                line.push_back({ i + k, j + k });
            }
            if (valid && check_line(line)) count++;
        }
    }

    // Diagonal (/)
    for (int i = 0; i <= 5 - length; i++) {
        for (int j = length - 1; j < 5; j++) {
            vector<pair<int, int>> line;
            bool valid = true;
            for (int k = 0; k < length; k++) {
                if (!board_ref->is_valid_cell(i + k, j - k)) {
                    valid = false;
                    break;
                }
                line.push_back({ i + k, j - k });
            }
            if (valid && check_line(line)) count++;
        }
    }

    return count;
}

// تقييم وضع اللوحة
Diamond_UI::BoardScore Diamond_UI::evaluate_position(char symbol) {
    BoardScore score;
    score.lines_of_3 = board_ref->count_lines(symbol, 3);
    score.lines_of_4 = board_ref->count_lines(symbol, 4);

    // ✅ إلغاء حساب potential_lines (بياخد وقت طويل)
    score.potential_lines = 0;

    return score;
}


// خوارزمية Minimax مع Alpha-Beta Pruning
int Diamond_UI::minimax(int depth, bool is_maximizing, char ai_symbol,
    char opp_symbol, int alpha, int beta) {
    // فحص حالات النهاية
    Player<char> temp_ai("temp", ai_symbol, PlayerType::AI);
    Player<char> temp_opp("temp", opp_symbol, PlayerType::AI);

    if (board_ref->is_win(&temp_ai)) return 100000 - depth;
    if (board_ref->is_win(&temp_opp)) return -100000 + depth;
    if (board_ref->is_draw(&temp_ai)) return 0;

    // وصلنا للعمق المحدد
    if (depth == 0) {
        BoardScore ai_score = evaluate_position(ai_symbol);
        BoardScore opp_score = evaluate_position(opp_symbol);
        return ai_score.get_score() - opp_score.get_score();
    }

    vector<pair<int, int>> moves = board_ref->get_valid_moves();
    if (moves.empty()) return 0;

    // ✅ Move Ordering: ترتيب الحركات من المركز للأطراف (تسريع Alpha-Beta)
    sort(moves.begin(), moves.end(), [](const auto& a, const auto& b) {
        int dist_a = abs(a.first - 2) + abs(a.second - 2);
        int dist_b = abs(b.first - 2) + abs(b.second - 2);
        return dist_a < dist_b;
        });

    if (is_maximizing) {
        int max_eval = numeric_limits<int>::min();

        for (size_t i = 0; i < moves.size(); i++) {
            int x = moves[i].first;
            int y = moves[i].second;

            auto& matrix = board_ref->get_board_matrix();
            char old_val = matrix[x][y];
            matrix[x][y] = ai_symbol;

            int eval = minimax(depth - 1, false, ai_symbol, opp_symbol, alpha, beta);

            matrix[x][y] = old_val;

            max_eval = max(max_eval, eval);
            alpha = max(alpha, eval);

            if (beta <= alpha) break; // Alpha-Beta Pruning
        }
        return max_eval;

    }
    else {
        int min_eval = numeric_limits<int>::max();

        for (size_t i = 0; i < moves.size(); i++) {
            int x = moves[i].first;
            int y = moves[i].second;

            auto& matrix = board_ref->get_board_matrix();
            char old_val = matrix[x][y];
            matrix[x][y] = opp_symbol;

            int eval = minimax(depth - 1, true, ai_symbol, opp_symbol, alpha, beta);

            matrix[x][y] = old_val;

            min_eval = min(min_eval, eval);
            beta = min(beta, eval);

            if (beta <= alpha) break;
        }
        return min_eval;
    }
}

// حركة الـ AI الذكي
// ========== في Diamond_TicTacToe.cpp ==========
// استبدل دالة get_ai_move بالكود ده:

Move<char>* Diamond_UI::get_ai_move(Player<char>* player) {
    char ai_symbol = player->get_symbol();
    char opp_symbol = (ai_symbol == 'X') ? 'O' : 'X';

    vector<pair<int, int>> moves = board_ref->get_valid_moves();
    if (moves.empty()) return nullptr;

    // ✅ 1. فحص الفوز الفوري (Instant Win Check)
    for (auto& move : moves) {
        auto& matrix = board_ref->get_board_matrix();
        matrix[move.first][move.second] = ai_symbol;

        Player<char> temp("test", ai_symbol, PlayerType::AI);
        if (board_ref->is_win(&temp)) {
            matrix[move.first][move.second] = '.';
            cout << "\n AI found INSTANT WIN at (" << move.first << ", " << move.second << ")!\n";
            return new Move<char>(move.first, move.second, ai_symbol);
        }
        matrix[move.first][move.second] = '.';
    }

    // ✅ 2. فحص منع فوز الخصم (Block Opponent Win)
    for (auto& move : moves) {
        auto& matrix = board_ref->get_board_matrix();
        matrix[move.first][move.second] = opp_symbol;

        Player<char> temp("test", opp_symbol, PlayerType::AI);
        if (board_ref->is_win(&temp)) {
            matrix[move.first][move.second] = '.';
            cout << "\n AI BLOCKING opponent's win at (" << move.first << ", " << move.second << ")\n";
            return new Move<char>(move.first, move.second, ai_symbol);
        }
        matrix[move.first][move.second] = '.';
    }

    // ✅ 3. عمق أقل حسب عدد الحركات المتبقية (Adaptive Depth)
    int remaining_moves = moves.size();
    int depth;

    if (remaining_moves > 7) depth = 3;       // بداية اللعبة - سريع
    else if (remaining_moves > 4) depth = 5;  // وسط اللعبة - متوسط
    else depth = 7;                            // نهاية اللعبة - عميق

    cout << "\n AI analyzing " << moves.size() << " moves (depth=" << depth << ")..." << flush;

    int best_score = numeric_limits<int>::min();
    pair<int, int> best_move = moves[0];

    for (auto& move : moves) {
        auto& matrix = board_ref->get_board_matrix();
        matrix[move.first][move.second] = ai_symbol;

        int score = minimax(depth - 1, false, ai_symbol, opp_symbol,
            numeric_limits<int>::min(), numeric_limits<int>::max());

        matrix[move.first][move.second] = '.';

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
    }

    cout << " Done!\n";
    cout << " AI chooses: (" << best_move.first << ", " << best_move.second << ")";

    if (best_score > 50000) cout << " [Winning!]";
    else if (best_score > 1000) cout << " [Strong]";
    else if (best_score > 0) cout << " [Good]";

    cout << "\n";
    return new Move<char>(best_move.first, best_move.second, ai_symbol);
}

Move<char>* Diamond_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n " << player->get_name() << ", enter your move (row col, 0-4): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
    else { // AI فقط
        // حركة ذكية باستخدام Minimax
        return get_ai_move(player);
    }
}

void Diamond_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    if (matrix.empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();

    cout << "\n    ";
    for (int j = 0; j < cols; ++j)
        cout << " " << j << "  ";
    cout << "\n    +";
    for (int j = 0; j < cols; ++j) cout << "---+";
    cout << "\n";

    for (int i = 0; i < rows; ++i) {
        cout << "  " << i << " |";
        for (int j = 0; j < cols; ++j) {
            bool valid_cell = false;
            if (i == 0 || i == 4) valid_cell = (j == 2);
            else if (i == 1 || i == 3) valid_cell = (j >= 1 && j <= 3);
            else valid_cell = true;

            if (!valid_cell) {
                cout << "   |";
            }
            else {
                cout << " " << matrix[i][j] << " |";
            }
        }
        cout << "\n    +";
        for (int j = 0; j < cols; ++j) cout << "---+";
        cout << "\n";
    }
    cout << endl;
}
Player<char>** Diamond_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];

    // 🔥 خيارات Diamond فقط: Human أو Smart AI
    std::vector<std::string> type_options = { "Human", "Smart AI" };

    std::string nameX = get_player_name("Player 1");
    PlayerType typeX = get_player_type_choice("Player 1", type_options);
    // تحويل Smart AI (الخيار 2) إلى PlayerType::AI
    if (typeX == PlayerType::RANDOM || static_cast<int>(typeX) == 1) {
        typeX = PlayerType::AI;
    }
    players[0] = create_player(nameX, 'X', typeX);

    std::string nameO = get_player_name("Player 2");
    PlayerType typeO = get_player_type_choice("Player 2", type_options);
    // تحويل Smart AI (الخيار 2) إلى PlayerType::AI
    if (typeO == PlayerType::RANDOM || static_cast<int>(typeO) == 1) {
        typeO = PlayerType::AI;
    }
    players[1] = create_player(nameO, 'O', typeO);

    return players;
}