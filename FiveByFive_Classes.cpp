#include "FiveByFive_Classes.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <vector>

using namespace std;    

// ==========================================
// 1. FiveByFive_Board Implementation
// ==========================================

// General Constructor
template <typename T>
FiveByFive_Board<T>::FiveByFive_Board() : Board<T>(5, 5) {}

// Specialized Constructor for char (initialize empty board)
template <>
FiveByFive_Board<char>::FiveByFive_Board() : Board<char>(5, 5) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            board[i][j] = '.';
        }
    }
}

template <typename T>
bool FiveByFive_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();
    T symbol = move->get_symbol();

    // Check bounds
    if (x < 0 || x >= 5 || y < 0 || y >= 5) return false;

    // Check if cell is empty
    if (this->board[x][y] != '.') return false;

    // Place the symbol
    this->board[x][y] = symbol;
    this->n_moves++;
    return true;
}

// Helper function to count three-in-a-row sequences for a player
template <typename T>
int FiveByFive_Board<T>::count_three_in_a_row(T symbol) {
    int count = 0;

    // Check rows (5 rows, each with 3 possible three-in-a-row sequences)
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j <= 2; ++j) {
            if (this->board[i][j] == symbol &&
                this->board[i][j + 1] == symbol &&
                this->board[i][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Check columns (5 columns, each with 3 possible three-in-a-row sequences)
    for (int i = 0; i <= 2; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (this->board[i][j] == symbol &&
                this->board[i + 1][j] == symbol &&
                this->board[i + 2][j] == symbol) {
                count++;
            }
        }
    }

    // Check diagonals (top-left to bottom-right)
    for (int i = 0; i <= 2; ++i) {
        for (int j = 0; j <= 2; ++j) {
            if (this->board[i][j] == symbol &&
                this->board[i + 1][j + 1] == symbol &&
                this->board[i + 2][j + 2] == symbol) {
                count++;
            }
        }
    }

    // Check diagonals (top-right to bottom-left)
    for (int i = 0; i <= 2; ++i) {
        for (int j = 2; j < 5; ++j) {
            if (this->board[i][j] == symbol &&
                this->board[i + 1][j - 1] == symbol &&
                this->board[i + 2][j - 2] == symbol) {
                count++;
            }
        }
    }

    return count;
}

template <typename T>
bool FiveByFive_Board<T>::is_win(Player<T>* player) {
    // This game doesn't end on the first win
    // Winner is determined at the end by counting sequences
    return false;
}

template <typename T>
bool FiveByFive_Board<T>::is_draw(Player<T>* player) {
    // Game ends when only 1 square remains (24 moves total)
    return (this->n_moves == 24);
}

template <typename T>
bool FiveByFive_Board<T>::game_is_over(Player<T>* player) {
    return is_draw(player);
}

// ==========================================
// 2. FiveByFive_UI Implementation
// ==========================================

template <typename T>
FiveByFive_UI<T>::FiveByFive_UI() : UI<T>("Welcome to 5x5 Tic-Tac-Toe", 5) {}

template <typename T>
Player<T>** FiveByFive_UI<T>::setup_players() {
    Player<T>** players = new Player<T>*[2];

    // خياران فقط: Human وComputer (Smart AI)
    vector<string> type_options = { "Human", "Smart AI" };

    string nameX = this->get_player_name("Player 1 (X)");
    PlayerType typeX = this->get_player_type_choice("Player 1", type_options);
    // تحويل Computer (الخيار 2) إلى PlayerType::AI
    if (static_cast<int>(typeX) == 1) {
        typeX = PlayerType::AI;
    }
    players[0] = create_player(nameX, static_cast<T>('X'), typeX);

    string nameO = this->get_player_name("Player 2 (O)");
    PlayerType typeO = this->get_player_type_choice("Player 2", type_options);
    // تحويل Computer (الخيار 2) إلى PlayerType::AI
    if (static_cast<int>(typeO) == 1) {
        typeO = PlayerType::AI;
    }
    players[1] = create_player(nameO, static_cast<T>('O'), typeO);

    return players;
}


template <typename T>
Player<T>* FiveByFive_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    if (type == PlayerType::AI) {
        cout << "Creating Smart AI player: " << name << " (" << symbol << ")\n";
    }
    else {
        cout << "Creating Human player: " << name << " (" << symbol << ")\n";
    }
    return new Player<T>(name, symbol, type);
}

template <typename T>
Move<T>* FiveByFive_UI<T>::get_move(Player<T>* currentPlayer) {
    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        int r, c;
        cout << "\n" << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ") turn.\n";

        while (true) {
            cout << "Enter Row (0-4): ";
            cin >> r;
            if (cin.fail() || r < 0 || r > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number between 0 and 4.\n";
                continue;
            }
            break;
        }

        while (true) {
            cout << "Enter Column (0-4): ";
            cin >> c;
            if (cin.fail() || c < 0 || c > 4) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a number between 0 and 4.\n";
                continue;
            }
            break;
        }

        return new Move<T>(r, c, currentPlayer->get_symbol());
    }
    else {
        // AI player (Smart AI)
        return get_ai_move(currentPlayer);
    }
}



// Check if a move creates a new three-in-a-row
template <typename T>
bool FiveByFive_UI<T>::creates_three_in_row(FiveByFive_Board<T>* board, int row, int col, T symbol) {
    auto& matrix = board->get_board_matrix();
    T old_val = matrix[row][col];

    int before_count = board->count_three_in_a_row(symbol);
    matrix[row][col] = symbol;
    int after_count = board->count_three_in_a_row(symbol);

    matrix[row][col] = old_val;

    return after_count > before_count;
}

// Count how many potential three-in-a-rows this position could create
template <typename T>
int FiveByFive_UI<T>::count_potential_sequences(FiveByFive_Board<T>* board, int row, int col, T symbol) {
    auto& matrix = board->get_board_matrix();
    int potential = 0;

    // Check all directions for potential sequences
    vector<pair<int, int>> directions = { {0,1}, {1,0}, {1,1}, {1,-1} };

    for (auto [dr, dc] : directions) {
        // Check if we can form a sequence in this direction
        for (int offset = -2; offset <= 0; offset++) {
            int count_symbol = 0;
            int count_empty = 0;
            bool valid = true;

            for (int i = 0; i < 3; i++) {
                int r = row + (offset + i) * dr;
                int c = col + (offset + i) * dc;

                if (r < 0 || r >= 5 || c < 0 || c >= 5) {
                    valid = false;
                    break;
                }

                if (r == row && c == col) {
                    count_symbol++;
                }
                else if (matrix[r][c] == symbol) {
                    count_symbol++;
                }
                else if (matrix[r][c] == '.') {
                    count_empty++;
                }
                else {
                    valid = false; // Blocked by opponent
                    break;
                }
            }

            // If we have at least one of our symbols and the rest are empty
            if (valid && count_symbol >= 1 && count_empty >= 1) {
                potential++;
            }
        }
    }

    return potential;
}

// Evaluate the overall board position
template <typename T>
int FiveByFive_UI<T>::evaluate_position(FiveByFive_Board<T>* board, T ai_symbol, T opp_symbol) {
    int ai_score = board->count_three_in_a_row(ai_symbol);
    int opp_score = board->count_three_in_a_row(opp_symbol);

    // Score is based on the difference in completed sequences
    return (ai_score - opp_score) * 100;
}

// Get AI move using strategic evaluation
template <typename T>
Move<T>* FiveByFive_UI<T>::get_ai_move(Player<T>* player) {
    if (!boardPtr) return nullptr;

    T ai_symbol = player->get_symbol();
    T opp_symbol = (ai_symbol == 'X') ? static_cast<T>('O') : static_cast<T>('X');
    auto& matrix = boardPtr->get_board_matrix();

    cout << "\n AI is analyzing the board..." << flush;

    MoveEval best_move = { -1, -1, -999999 };
    vector<MoveEval> good_moves;

    // Evaluate all possible moves
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == '.') {
                int score = 0;

                // High priority: Creating our own three-in-a-row
                if (creates_three_in_row(boardPtr, i, j, ai_symbol)) {
                    score += 1000;
                }

                // Medium priority: Blocking opponent's three-in-a-row
                if (creates_three_in_row(boardPtr, i, j, opp_symbol)) {
                    score += 500;
                }

                // Strategic value: potential for future sequences
                score += count_potential_sequences(boardPtr, i, j, ai_symbol) * 50;

                // Center positions are slightly better
                int center_dist = abs(i - 2) + abs(j - 2);
                score += (4 - center_dist) * 10;

                // Add some randomness to avoid predictability
                score += rand() % 20;

                if (score > best_move.score) {
                    best_move = { i, j, score };
                }

                // Keep track of good moves
                if (score >= 800) {
                    good_moves.push_back({ i, j, score });
                }
            }
        }
    }

    cout << " Done! \n";

    // If we have multiple good moves, pick one randomly for variety
    if (good_moves.size() > 1) {
        int idx = rand() % good_moves.size();
        best_move = good_moves[idx];
    }

    if (best_move.row == -1) {
        // Fallback: find first empty cell
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (matrix[i][j] == '.') {
                    cout << " AI chooses: (" << i << ", " << j << ")\n";
                    return new Move<T>(i, j, ai_symbol);
                }
            }
        }
    }

    cout << " AI chooses: (" << best_move.row << ", " << best_move.col << ")";

    if (best_move.score >= 1000) cout << " [Creates sequence! ]";
    else if (best_move.score >= 500) cout << " [Defensive move ]";
    else if (best_move.score >= 100) cout << " [Strategic position ]";

    cout << " [Score: " << best_move.score << "]\n";

    return new Move<T>(best_move.row, best_move.col, ai_symbol);
}

// =============================================================
// Explicit Instantiation
// =============================================================
template class FiveByFive_Board<char>;
template class FiveByFive_UI<char>;