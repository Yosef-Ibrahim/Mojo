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
    cout << "Goal: Create BOTH a line of 3 AND a line of 4 symbols!\n";
    cout << "Only diamond-shaped cells are playable.\n\n";
}

Player<char>* Diamond_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::AI) {
        cout << "✨ Creating Smart AI player: " << name << " (" << symbol << ") 🤖\n";
    }
    else if (type == PlayerType::RANDOM || type == PlayerType::COMPUTER) {
        cout << "Creating Random Computer player: " << name << " (" << symbol << ")\n";
    }
    else {
        cout << "Creating Human player: " << name << " (" << symbol << ")\n";
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
    score.lines_of_3 = 0;
    score.lines_of_4 = 0;
    score.potential_lines = 0;

    if (!board_ref) return score;

    // نجمع الخطوط حسب الاتجاه
    int h_lines_3 = 0, v_lines_3 = 0, d1_lines_3 = 0, d2_lines_3 = 0;
    int h_lines_4 = 0, v_lines_4 = 0, d1_lines_4 = 0, d2_lines_4 = 0;

    auto& matrix = board_ref->get_board_matrix();

    // حساب خطوط الـ 3
    // Horizontal
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j <= 2; j++) {
            bool valid = true, is_line = true;
            for (int k = 0; k < 3; k++) {
                if (!board_ref->is_valid_cell(i, j + k)) { valid = false; break; }
            }
            if (valid) {
                for (int k = 0; k < 3; k++) {
                    if (matrix[i][j + k] != symbol) { is_line = false; break; }
                }
                if (is_line) h_lines_3++;
            }
        }
    }

    // Vertical
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i <= 2; i++) {
            bool valid = true, is_line = true;
            for (int k = 0; k < 3; k++) {
                if (!board_ref->is_valid_cell(i + k, j)) { valid = false; break; }
            }
            if (valid) {
                for (int k = 0; k < 3; k++) {
                    if (matrix[i + k][j] != symbol) { is_line = false; break; }
                }
                if (is_line) v_lines_3++;
            }
        }
    }

    // Diagonal (\)
    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
            bool valid = true, is_line = true;
            for (int k = 0; k < 3; k++) {
                if (!board_ref->is_valid_cell(i + k, j + k)) { valid = false; break; }
            }
            if (valid) {
                for (int k = 0; k < 3; k++) {
                    if (matrix[i + k][j + k] != symbol) { is_line = false; break; }
                }
                if (is_line) d1_lines_3++;
            }
        }
    }

    // Diagonal (/)
    for (int i = 0; i <= 2; i++) {
        for (int j = 2; j < 5; j++) {
            bool valid = true, is_line = true;
            for (int k = 0; k < 3; k++) {
                if (!board_ref->is_valid_cell(i + k, j - k)) { valid = false; break; }
            }
            if (valid) {
                for (int k = 0; k < 3; k++) {
                    if (matrix[i + k][j - k] != symbol) { is_line = false; break; }
                }
                if (is_line) d2_lines_3++;
            }
        }
    }

    // حساب خطوط الـ 4
    // Horizontal
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j <= 1; j++) {
            bool valid = true, is_line = true;
            for (int k = 0; k < 4; k++) {
                if (!board_ref->is_valid_cell(i, j + k)) { valid = false; break; }
            }
            if (valid) {
                for (int k = 0; k < 4; k++) {
                    if (matrix[i][j + k] != symbol) { is_line = false; break; }
                }
                if (is_line) h_lines_4++;
            }
        }
    }

    // Vertical
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i <= 1; i++) {
            bool valid = true, is_line = true;
            for (int k = 0; k < 4; k++) {
                if (!board_ref->is_valid_cell(i + k, j)) { valid = false; break; }
            }
            if (valid) {
                for (int k = 0; k < 4; k++) {
                    if (matrix[i + k][j] != symbol) { is_line = false; break; }
                }
                if (is_line) v_lines_4++;
            }
        }
    }

    // Diagonal (\)
    for (int i = 0; i <= 1; i++) {
        for (int j = 0; j <= 1; j++) {
            bool valid = true, is_line = true;
            for (int k = 0; k < 4; k++) {
                if (!board_ref->is_valid_cell(i + k, j + k)) { valid = false; break; }
            }
            if (valid) {
                for (int k = 0; k < 4; k++) {
                    if (matrix[i + k][j + k] != symbol) { is_line = false; break; }
                }
                if (is_line) d1_lines_4++;
            }
        }
    }

    // Diagonal (/)
    for (int i = 0; i <= 1; i++) {
        for (int j = 3; j < 5; j++) {
            bool valid = true, is_line = true;
            for (int k = 0; k < 4; k++) {
                if (!board_ref->is_valid_cell(i + k, j - k)) { valid = false; break; }
            }
            if (valid) {
                for (int k = 0; k < 4; k++) {
                    if (matrix[i + k][j - k] != symbol) { is_line = false; break; }
                }
                if (is_line) d2_lines_4++;
            }
        }
    }

    // التقييم الذكي: نحسب التركيبات الممكنة
    score.lines_of_3 = h_lines_3 + v_lines_3 + d1_lines_3 + d2_lines_3;
    score.lines_of_4 = h_lines_4 + v_lines_4 + d1_lines_4 + d2_lines_4;

    // نحسب potential lines (خطوط قريبة من الاكتمال)
    score.potential_lines = count_potential_lines(symbol, 3) + count_potential_lines(symbol, 4);

    return score;
}

// خوارزمية Minimax مع Alpha-Beta Pruning
int Diamond_UI::minimax(int depth, bool is_maximizing, char ai_symbol, char opp_symbol, int alpha, int beta) {
    // فحص حالات النهاية
    Player<char> temp_ai("temp", ai_symbol, PlayerType::AI);
    Player<char> temp_opp("temp", opp_symbol, PlayerType::AI);

    // فوز = أعلى قيمة ممكنة
    if (board_ref->is_win(&temp_ai)) return 100000 - depth;
    // خسارة = أقل قيمة ممكنة
    if (board_ref->is_win(&temp_opp)) return -100000 + depth;
    // تعادل
    if (board_ref->is_draw(&temp_ai)) return 0;

    // وصلنا للعمق المحدد
    if (depth == 0) {
        BoardScore ai_score = evaluate_position(ai_symbol);
        BoardScore opp_score = evaluate_position(opp_symbol);
        return ai_score.get_score() - opp_score.get_score();
    }

    vector<pair<int, int>> moves = board_ref->get_valid_moves();
    if (moves.empty()) return 0;

    if (is_maximizing) {
        int max_eval = numeric_limits<int>::min();

        for (size_t i = 0; i < moves.size(); i++) {
            int x = moves[i].first;
            int y = moves[i].second;

            // نجرب الحركة
            auto& matrix = board_ref->get_board_matrix();
            char old_val = matrix[x][y];
            matrix[x][y] = ai_symbol;

            int eval = minimax(depth - 1, false, ai_symbol, opp_symbol, alpha, beta);

            // نلغي الحركة
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
Move<char>* Diamond_UI::get_ai_move(Player<char>* player) {
    char ai_symbol = player->get_symbol();
    char opp_symbol = (ai_symbol == 'X') ? 'O' : 'X';

    vector<pair<int, int>> moves = board_ref->get_valid_moves();
    if (moves.empty()) return nullptr;

    cout << "\n🤖 AI is analyzing " << moves.size() << " possible moves..." << flush;

    int best_score = numeric_limits<int>::min();
    pair<int, int> best_move = moves[0];

    const int MAX_DEPTH = 7; // زودنا العمق شوية

    for (size_t i = 0; i < moves.size(); i++) {
        int x = moves[i].first;
        int y = moves[i].second;

        // نجرب الحركة
        auto& matrix = board_ref->get_board_matrix();
        char old_val = matrix[x][y];
        matrix[x][y] = ai_symbol;

        // نفحص لو الحركة دي مباشرة بتخلينا نكسب
        Player<char> temp("test", ai_symbol, PlayerType::AI);
        if (board_ref->is_win(&temp)) {
            matrix[x][y] = old_val;
            cout << " Found winning move! ✓\n";
            cout << "🎯 AI chooses: (" << x << ", " << y << ") [WINNING MOVE! 🏆]\n";
            return new Move<char>(x, y, ai_symbol);
        }

        int score = minimax(MAX_DEPTH - 1, false, ai_symbol, opp_symbol,
            numeric_limits<int>::min(), numeric_limits<int>::max());

        // نلغي الحركة
        matrix[x][y] = old_val;

        if (score > best_score) {
            best_score = score;
            best_move = moves[i];
        }
    }

    cout << " Done! ✓\n";
    cout << "🎯 AI chooses: (" << best_move.first << ", " << best_move.second << ")";

    if (best_score > 50000) cout << " [Winning strategy! 🏆]";
    else if (best_score > 10000) cout << " [Very strong! 💪]";
    else if (best_score > 1000) cout << " [Good position 👍]";
    else if (best_score > 0) cout << " [Decent move]";
    else if (best_score < -10000) cout << " [Defensive move 🛡️]";

    cout << " [Score: " << best_score << "]\n";

    return new Move<char>(best_move.first, best_move.second, ai_symbol);
}

Move<char>* Diamond_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n👤 " << player->get_name() << ", enter your move (row col, 0-4): ";
        cin >> x >> y;
        return new Move<char>(x, y, player->get_symbol());
    }
    else if (player->get_type() == PlayerType::AI) {
        // حركة ذكية باستخدام Minimax
        return get_ai_move(player);
    }
    else {
        // حركة عشوائية
        auto& matrix = board_ref->get_board_matrix();
        do {
            x = rand() % 5;
            y = rand() % 5;
        } while (!board_ref->is_valid_cell(x, y) || matrix[x][y] != '.');

        cout << "\n🎲 Computer " << player->get_name() << " chooses: (" << x << ", " << y << ")\n";
        return new Move<char>(x, y, player->get_symbol());
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