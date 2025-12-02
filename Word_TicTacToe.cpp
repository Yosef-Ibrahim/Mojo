#include "Word_TicTacToe.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>
#include <vector>

using namespace std;

// ============================================================================
// Word_Board Implementation
// ============================================================================

Word_Board::Word_Board() : Board(3, 3) {
    // Initialize empty board
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            board[i][j] = blank_symbol;
        }
    }

    // Load dictionary
    if (!load_dictionary("dic.txt")) {
        cout << "Warning: Could not load dictionary file 'dic.txt'\n";
        cout << "Make sure the file exists in the same directory.\n";
    }
}

bool Word_Board::load_dictionary(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    string word;
    while (file >> word) {
        // Convert to uppercase and store only 3-letter words
        if (word.length() == 3) {
            transform(word.begin(), word.end(), word.begin(), ::toupper);
            dictionary.insert(word);
        }
    }
    file.close();

    cout << "Dictionary loaded: " << dictionary.size() << " words.\n";
    return !dictionary.empty();
}

bool Word_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char letter = move->get_symbol();

    // Check bounds
    if (x < 0 || x >= 3 || y < 0 || y >= 3) return false;

    // Check if cell is empty
    if (board[x][y] != blank_symbol) return false;

    // Place the letter (uppercase)
    board[x][y] = toupper(letter);
    n_moves++;
    return true;
}

bool Word_Board::is_valid_word(const string& word) const {
    if (word.length() != 3) return false;
    string upper_word = word;
    transform(upper_word.begin(), upper_word.end(), upper_word.begin(), ::toupper);
    return dictionary.count(upper_word) > 0;
}

bool Word_Board::check_word_at(int row, int col, int dr, int dc) const {
    string word = "";

    // Collect 3 characters in the given direction
    for (int i = 0; i < 3; i++) {
        int r = row + i * dr;
        int c = col + i * dc;

        if (r < 0 || r >= 3 || c < 0 || c >= 3) return false;
        if (board[r][c] == blank_symbol) return false;

        word += board[r][c];
    }

    return is_valid_word(word);
}

bool Word_Board::is_win(Player<char>* player) {
    // Check all possible 3-letter sequences

    // Horizontal (3 rows)
    for (int i = 0; i < 3; i++) {
        if (check_word_at(i, 0, 0, 1)) return true;
    }

    // Vertical (3 columns)
    for (int j = 0; j < 3; j++) {
        if (check_word_at(0, j, 1, 0)) return true;
    }

    // Diagonal (top-left to bottom-right)
    if (check_word_at(0, 0, 1, 1)) return true;

    // Diagonal (top-right to bottom-left)
    if (check_word_at(0, 2, 1, -1)) return true;

    return false;
}

bool Word_Board::is_draw(Player<char>* player) {
    // Draw if board is full and no winner
    return (n_moves == 9 && !is_win(player));
}

bool Word_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ============================================================================
// Word_UI Implementation
// ============================================================================

Word_UI::Word_UI() : UI<char>("Welcome to Word Tic-Tac-Toe", 3) {
    cout << "Goal: Form a valid 3-letter word horizontally, vertically, or diagonally!\n";
    cout << "Players place one letter at a time.\n\n";
}

Player<char>** Word_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Random Computer", "Smart AI" };

    string name1 = get_player_name("Player 1");
    PlayerType type1 = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(name1, 'X', type1);

    string name2 = get_player_name("Player 2");
    PlayerType type2 = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(name2, 'O', type2);

    return players;
}

Player<char>* Word_UI::create_player(string& name, char symbol, PlayerType type) {
    if (type == PlayerType::AI) {
        cout << "? Creating Smart AI player: " << name << " (" << symbol << ") ??\n";
    }
    else if (type == PlayerType::RANDOM || type == PlayerType::COMPUTER) {
        cout << "Creating Random Computer player: " << name << " (" << symbol << ")\n";
    }
    else {
        cout << "Creating Human player: " << name << " (" << symbol << ")\n";
    }
    return new Player<char>(name, symbol, type);
}

// Check if placing a letter creates a winning word
bool Word_UI::creates_winning_word(Word_Board* board, int row, int col, char letter) {
    auto& matrix = board->get_board_matrix();
    char old_val = matrix[row][col];
    matrix[row][col] = toupper(letter);

    // Check all directions from this position
    bool wins = false;

    // Horizontal
    int start_col = max(0, col - 2);
    for (int c = start_col; c <= min(0, col); c++) {
        if (board->check_word_at(row, c, 0, 1)) {
            wins = true;
            break;
        }
    }

    // Vertical
    if (!wins) {
        int start_row = max(0, row - 2);
        for (int r = start_row; r <= min(0, row); r++) {
            if (board->check_word_at(r, col, 1, 0)) {
                wins = true;
                break;
            }
        }
    }

    // Diagonals
    if (!wins && row == col) {
        if (board->check_word_at(0, 0, 1, 1)) wins = true;
    }
    if (!wins && row + col == 2) {
        if (board->check_word_at(0, 2, 1, -1)) wins = true;
    }

    matrix[row][col] = old_val;
    return wins;
}

// Check if this move blocks opponent from winning
bool Word_UI::blocks_opponent_win(Word_Board* board, int row, int col, char letter, char opponent_symbol) {
    // Try all letters the opponent might use
    string common_letters = "AEIOURSTLNDHCMPBFGYWKVXZJQ";

    for (char opp_letter : common_letters) {
        if (creates_winning_word(board, row, col, opp_letter)) {
            return true;
        }
    }
    return false;
}

// Count potential words that could be formed with this move
int Word_UI::count_potential_words(Word_Board* board, int row, int col, char letter) {
    auto& matrix = board->get_board_matrix();
    const auto& dict = board->get_dictionary();
    int count = 0;

    char old_val = matrix[row][col];
    matrix[row][col] = toupper(letter);

    // Check all 8 directions for potential word formations
    vector<pair<int, int>> directions = { {0,1}, {1,0}, {1,1}, {1,-1}, {0,-1}, {-1,0}, {-1,-1}, {-1,1} };

    for (auto [dr, dc] : directions) {
        // Try different starting positions
        for (int offset = -2; offset <= 0; offset++) {
            string potential = "";
            bool has_blank = false;

            for (int i = 0; i < 3; i++) {
                int r = row + (offset + i) * dr;
                int c = col + (offset + i) * dc;

                if (r < 0 || r >= 3 || c < 0 || c >= 3) {
                    potential = "";
                    break;
                }

                if (matrix[r][c] == '.') {
                    has_blank = true;
                    potential += '?'; // Wildcard
                }
                else {
                    potential += matrix[r][c];
                }
            }

            // If we have a partial match with dictionary words
            if (potential.length() == 3 && has_blank) {
                for (const string& word : dict) {
                    bool matches = true;
                    for (int i = 0; i < 3; i++) {
                        if (potential[i] != '?' && potential[i] != word[i]) {
                            matches = false;
                            break;
                        }
                    }
                    if (matches) count++;
                }
            }
        }
    }

    matrix[row][col] = old_val;
    return count;
}

// Evaluate a move's strategic value
int Word_UI::evaluate_move(Word_Board* board, int row, int col, char letter, char ai_symbol) {
    int score = 0;

    // Immediate win is highest priority
    if (creates_winning_word(board, row, col, letter)) {
        return 10000;
    }

    // Blocking opponent win is second priority
    char opp_symbol = (ai_symbol == 'X') ? 'O' : 'X';
    if (blocks_opponent_win(board, row, col, letter, opp_symbol)) {
        score += 5000;
    }

    // Potential word formations
    score += count_potential_words(board, row, col, letter) * 10;

    // Center position bonus
    if (row == 1 && col == 1) score += 50;

    // Corner bonus
    if ((row == 0 || row == 2) && (col == 0 || col == 2)) score += 30;

    return score;
}

// Smart AI move using backtracking and evaluation
Move<char>* Word_UI::get_ai_move(Player<char>* player) {
    if (!boardPtr) return nullptr;

    char ai_symbol = player->get_symbol();
    auto& matrix = boardPtr->get_board_matrix();

    cout << "\n?? AI is thinking..." << flush;

    // Common letters to try (ordered by frequency in English)
    string letters_to_try = "ETAOINSRHLDCUMFPGWYBVKXJQZ";

    MoveScore best_move = { -1, -1, ' ', -999999 };

    // Try all empty positions
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (matrix[i][j] == '.') {
                // Try different letters
                for (char letter : letters_to_try) {
                    int score = evaluate_move(boardPtr, i, j, letter, ai_symbol);

                    if (score > best_move.score) {
                        best_move = { i, j, letter, score };
                    }

                    // If we found a winning move, use it immediately
                    if (score >= 10000) {
                        cout << " Found winning move! ?\n";
                        cout << "?? AI places '" << letter << "' at (" << i << ", " << j << ") [WINNING MOVE! ??]\n";
                        return new Move<char>(i, j, letter);
                    }
                }
            }
        }
    }

    cout << " Done! ?\n";

    if (best_move.row == -1) {
        // Fallback: random move
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (matrix[i][j] == '.') {
                    char random_letter = letters_to_try[rand() % 10]; // Use common letters
                    cout << "?? AI places '" << random_letter << "' at (" << i << ", " << j << ")\n";
                    return new Move<char>(i, j, random_letter);
                }
            }
        }
    }

    cout << "?? AI places '" << best_move.letter << "' at (" << best_move.row << ", " << best_move.col << ")";
    if (best_move.score > 5000) cout << " [Defensive move ???]";
    else if (best_move.score > 100) cout << " [Strategic move ??]";
    cout << " [Score: " << best_move.score << "]\n";

    return new Move<char>(best_move.row, best_move.col, best_move.letter);
}

Move<char>* Word_UI::get_move(Player<char>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int x, y;
        char letter;

        cout << "\n?? " << currentPlayer->get_name() << "'s turn\n";
        cout << "Enter row (0-2): ";
        cin >> x;
        cout << "Enter column (0-2): ";
        cin >> y;
        cout << "Enter letter (A-Z): ";
        cin >> letter;

        return new Move<char>(x, y, toupper(letter));
    }
    else if (currentPlayer->get_type() == PlayerType::AI) {
        // Smart AI
        return get_ai_move(currentPlayer);
    }
    else {
        // Random computer
        auto& matrix = boardPtr->get_board_matrix();
        int x, y;

        // Find random empty cell
        do {
            x = rand() % 3;
            y = rand() % 3;
        } while (matrix[x][y] != '.');

        // Random common letter
        string common = "ETAOINSRH";
        char letter = common[rand() % common.length()];

        cout << "\n?? Computer " << currentPlayer->get_name()
            << " places '" << letter << "' at (" << x << ", " << y << ")\n";

        return new Move<char>(x, y, letter);
    }
}