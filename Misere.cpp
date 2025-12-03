#include "Misere.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <algorithm>

using namespace std;

// -------------------------------
// Misere_Board Implementation
// -------------------------------
Misere_Board::Misere_Board() : Board(3, 3) {
    for (auto& row : board)
        for (auto& cell : row)
            cell = blank_symbol;
}

bool Misere_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x >= 0 && x < rows && y >= 0 && y < columns && board[x][y] == blank_symbol) {
        board[x][y] = toupper(mark);
        n_moves++;
        return true;
    }
    return false;
}

bool Misere_Board::is_win(Player<char>* player) {
    // In Misere, no one wins - you only lose
    return false;
}

bool Misere_Board::is_lose(Player<char>* player) {
    return check_lose(board, player->get_symbol());
}

bool Misere_Board::is_draw(Player<char>* player) {
    return (n_moves == 9);
}

bool Misere_Board::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

bool Misere_Board::check_lose(const vector<vector<char>>& board, char symbol) const {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol)
            return true;
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == symbol && board[1][j] == symbol && board[2][j] == symbol)
            return true;
    }

    // Check diagonals
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol)
        return true;
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol)
        return true;

    return false;
}

bool Misere_Board::is_board_full(const vector<vector<char>>& board) const {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '.') return false;
        }
    }
    return true;
}

// -------------------------------
// Misere_AI_Player Implementation
// -------------------------------
int Misere_AI_Player::minimax(vector<vector<char>>& board, int depth, bool is_maximizing,
    char ai_symbol, char opponent_symbol, int alpha, int beta) {

    // Check terminal states using board's helper functions
    Misere_Board temp_board;
    if (temp_board.check_lose(board, ai_symbol)) {
        return -10 + depth; // AI loses (bad) - prefer later losses
    }
    if (temp_board.check_lose(board, opponent_symbol)) {
        return 10 - depth; // Opponent loses (good) - prefer earlier wins
    }
    if (temp_board.is_board_full(board)) {
        return 0; // Draw
    }

    if (is_maximizing) {
        int max_eval = numeric_limits<int>::min();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == '.') {
                    board[i][j] = ai_symbol;
                    int eval = minimax(board, depth + 1, false, ai_symbol,
                        opponent_symbol, alpha, beta);
                    board[i][j] = '.';

                    max_eval = max(max_eval, eval);
                    alpha = max(alpha, eval);

                    if (beta <= alpha) break; // Alpha-beta pruning
                }
            }
        }
        return max_eval;
    }
    else {
        int min_eval = numeric_limits<int>::max();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == '.') {
                    board[i][j] = opponent_symbol;
                    int eval = minimax(board, depth + 1, true, ai_symbol,
                        opponent_symbol, alpha, beta);
                    board[i][j] = '.';

                    min_eval = min(min_eval, eval);
                    beta = min(beta, eval);

                    if (beta <= alpha) break; // Alpha-beta pruning
                }
            }
        }
        return min_eval;
    }
}

pair<int, int> Misere_AI_Player::get_best_move(char opponent_symbol) {
    vector<vector<char>>& board = board_ptr->get_board_matrix();
    int best_score = numeric_limits<int>::min();
    pair<int, int> best_move = { -1, -1 };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == '.') {
                // Try this move
                board[i][j] = symbol;

                int score = minimax(board, 0, false, symbol, opponent_symbol,
                    numeric_limits<int>::min(),
                    numeric_limits<int>::max());

                // Undo the move
                board[i][j] = '.';

                // Update best move
                if (score > best_score) {
                    best_score = score;
                    best_move = { i, j };
                }
            }
        }
    }

    // If no valid move found (shouldn't happen), return first available
    if (best_move.first == -1) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == '.') {
                    return { i, j };
                }
            }
        }
    }

    return best_move;
}

// -------------------------------
// Misere_UI Implementation
// -------------------------------
Misere_UI::Misere_UI()
    : UI<char>("Welcome to Misere Tic Tac Toe Game\n(Goal: AVOID getting 3 in a row!)", 3) {
    board_ptr = nullptr;
}

void Misere_UI::set_board(Misere_Board* board) {
    board_ptr = board;
}

Player<char>* Misere_UI::create_player(string& name, char symbol, PlayerType type) {
    // Only Human and Smart AI are available for Misere
    if (type == PlayerType::AI) {
        cout << "Creating Smart AI player: " << name << " (" << symbol << ")\n";
        return new Misere_AI_Player(name, symbol, board_ptr);
    }
    else {
        // Default to Human player
        cout << "Creating human player: " << name << " (" << symbol << ")\n";
        return new Player<char>(name, symbol, PlayerType::HUMAN);
    }
}


Move<char>* Misere_UI::get_move(Player<char>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c;

        while (true) {
            cout << currentPlayer->get_name()
                << ", enter Row and Column numbers (0 1 2), separated by space: ";

            if (!(cin >> r >> c)) {
                cout << "\n!!! Invalid input. Please enter two numbers. !!!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (r < 0 || r > 2 || c < 0 || c > 2) {
                cout << "\n!!! Out of range. Both numbers must be between 0 and 2. !!!\n";
                continue;
            }
            break;
        }

        cout << endl;
        return new Move<char>(r, c, currentPlayer->get_symbol());
    }
    else { // Smart AI فقط
        Misere_AI_Player* ai_player = dynamic_cast<Misere_AI_Player*>(currentPlayer);

        // Determine opponent symbol
        char opponent_symbol = (currentPlayer->get_symbol() == 'X') ? 'O' : 'X';

        // Get best move from AI
        pair<int, int> best_move = ai_player->get_best_move(opponent_symbol);

        cout << "Smart AI " << currentPlayer->get_name()
            << " chose (Row, Col): " << best_move.first << ", " << best_move.second << endl;

        return new Move<char>(best_move.first, best_move.second, currentPlayer->get_symbol());
    }
}
// 🔥 دالة setup_players الخاصة بـ Misere فقط (بدون Random Computer)
Player<char>** Misere_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];

    // خيارات Misere فقط: Human أو Smart AI
    std::vector<std::string> type_options = { "Human", "Smart AI" };

    // Player 1 (X)
    std::string nameX = get_player_name("Player 1");
    PlayerType typeX = get_player_type_choice("Player 1", type_options);
    // تحويل Smart AI (الخيار 2) إلى PlayerType::AI
    if (static_cast<int>(typeX) == 1) {
        typeX = PlayerType::AI;
    }
    players[0] = create_player(nameX, 'X', typeX);

    // Player 2 (O)
    std::string nameO = get_player_name("Player 2");
    PlayerType typeO = get_player_type_choice("Player 2", type_options);
    // تحويل Smart AI (الخيار 2) إلى PlayerType::AI
    if (static_cast<int>(typeO) == 1) {
        typeO = PlayerType::AI;
    }
    players[1] = create_player(nameO, 'O', typeO);

    return players;
}
