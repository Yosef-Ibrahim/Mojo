#include "Four_In_A_Row_UI.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include "BoardGame_Classes.h"

using namespace std;

//constructor
Four_In_A_Row_UI::Four_In_A_Row_UI(Board<char>* board)
    : UI<char>("Welcome to Four-in-a-Row Game!", 7) {
    this->boardPtr = board;
}
//constructor
Four_In_A_Row_Board::Four_In_A_Row_Board(int r, int c) : Board<char>(r, c) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            board[i][j] = '.';
        }
    }
}
//destructor
Four_In_A_Row_UI::~Four_In_A_Row_UI() {
}


//display board
void Four_In_A_Row_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    int rows = matrix.size();
    int cols = matrix[0].size();

    cout << "\n============================================\n";
    cout << "               FOUR-IN-A-ROW BOARD\n";
    cout << "============================================\n";

    // print column headers
    cout << "  ";
    for (int j = 0; j < cols; ++j) {
        cout << "  " << j << " ";
    }
    cout << "\n +";
    cout << " ";
    for (int j = 0; j < cols; ++j) {
        cout << "---+";
    }
    cout << endl;

    // print rows
    for (int i = 0; i < rows; ++i) {
        cout << i << " |";
        for (int j = 0; j < cols; ++j) {
            char symbol = matrix[i][j];
            if (symbol == '.') {
                cout << "   |";
            }
            else {
                cout << " " << symbol << " |";
            }
        }
        cout << "\n +";
        cout << " ";

        for (int j = 0; j < cols; ++j) {
            cout << "---+";
        }
        cout << endl;
    }
    cout << "\n";
}

//setup players
Player<char>** Four_In_A_Row_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    // Player 1 
    string nameX = get_player_name("Player 1 (Symbol X)");
    PlayerType typeX = get_player_type_choice("Player 1", type_options);

    players[0] = new Player<char>(nameX, 'X', typeX);

    // Player 2 
    string nameO = get_player_name("Player 2 (Symbol O)");
    PlayerType typeO = get_player_type_choice("Player 2", type_options);
    players[1] = new Player<char>(nameO, 'O', typeO);

    return players;
}
//move input
Move<char>* Four_In_A_Row_UI::get_move(Player<char>* currentPlayer) {
    int c; //Column
    int rows = this->boardPtr->get_rows(); // 6
    int cols = this->boardPtr->get_columns(); // 7

    if (currentPlayer->get_type() == PlayerType::HUMAN) {

        cout << "\n" << currentPlayer->get_name() << " (" << currentPlayer->get_symbol() << ") turn.\n";

        while (true) {
            cout << "Enter Column number (0 to " << cols - 1 << "): ";

            //Check valid input
            if (!(cin >> c)) {
                cout << "\n!!! Invalid input. Please enter a number. !!!\n";
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            //Check range
            if (c < 0 || c >= cols) {
                cout << "\n!!! Out of Range. Column must be between 0 and " << cols - 1 << ". !!!\n";
                continue;
            }


            //Check if column is full
            if (this->boardPtr->get_board_matrix()[0][c] != '.') {
                cout << "\n!!! Invalid Move. Column " << c << " is full. Try another column. !!!\n";
                continue;
            }

            break;
        }
    }
    else { // Computer Player
        Four_In_A_Row_Board* realBoard = dynamic_cast<Four_In_A_Row_Board*>(this->boardPtr);

        c = realBoard->get_best_ai_move(currentPlayer->get_symbol());

        cout << "Computer chose Column: " << c << endl;

    }


    return new Move<char>(0, c, currentPlayer->get_symbol());
}

//find lowest empty row in column
int Four_In_A_Row_Board::find_lowest_row(int column) {
    for (int r = rows - 1; r >= 0; --r) {
        if (board[r][column] == '.') {//if empty
            return r;
        }
    }
    return -1; //no empty row found
}

//update board with move
bool Four_In_A_Row_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    int row = find_lowest_row(col);
    if (row != -1) {// Valid row found
        board[row][col] = move->get_symbol();
        n_moves++;
        return true;// Move successful
    }
    return false; // Column is full
}


//check win condition
bool Four_In_A_Row_Board::is_win(Player<char>* player) {
    char sym = player->get_symbol();


    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            // Check horizontal
            if (c + 3 < columns &&
                board[r][c] == sym && board[r][c + 1] == sym &&
                board[r][c + 2] == sym && board[r][c + 3] == sym)
                return true;
            // Check vertical and diagonal
            if (r + 3 < rows) {
                // Check vertical
                if (board[r][c] == sym && board[r + 1][c] == sym &&
                    board[r + 2][c] == sym && board[r + 3][c] == sym)
                    return true;
                // Check diagonal 
                if (c + 3 < columns &&
                    board[r][c] == sym && board[r + 1][c + 1] == sym &&
                    board[r + 2][c + 2] == sym && board[r + 3][c + 3] == sym)
                    return true;
                // Check diagonal
                if (c - 3 >= 0 &&
                    board[r][c] == sym && board[r + 1][c - 1] == sym &&
                    board[r + 2][c - 2] == sym && board[r + 3][c - 3] == sym)
                    return true;
            }
        }
    }
    return false;
}

//check draw condition
bool Four_In_A_Row_Board::is_draw(Player<char>* player) {
    return n_moves >= rows * columns;
}
//check if game is over
bool Four_In_A_Row_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


//create player
Player<char>* Four_In_A_Row_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}


// تقييم البورد: يعطي نقاطًا أعلى لمجموعات أقوى ويفرق بين الحالات
int Four_In_A_Row_Board::evaluate_board(char aiSym) {
    char opp = (aiSym == 'X') ? 'O' : 'X';
    int score = 0;

    auto score_segment = [&](char a, char b, char c, char d) -> int {
        int aiCount = (a == aiSym) + (b == aiSym) + (c == aiSym) + (d == aiSym);
        int oppCount = (a == opp) + (b == opp) + (c == opp) + (d == opp);

        if (aiCount > 0 && oppCount == 0) {
            // مزايا للـ AI إذا المقطع خاص بيه فقط
            if (aiCount == 4) return 100000;   // فوز مؤكد
            if (aiCount == 3) return 1000;
            if (aiCount == 2) return 100;
            if (aiCount == 1) return 10;
        }
        else if (oppCount > 0 && aiCount == 0) {
            // خصم قوي => نقاط سلبية (نحاول نمنعه)
            if (oppCount == 4) return -90000; // خسارة مؤقتة (قريبة من -100000)
            if (oppCount == 3) return -800;
            if (oppCount == 2) return -80;
            if (oppCount == 1) return -5;
        }
        return 0;
        };

    // Bonus for controlling center column
    int center = columns / 2;
    int centerCount = 0;
    for (int r = 0; r < rows; r++)
        if (board[r][center] == aiSym)
            centerCount++;

    score += centerCount * 50;   // Boost center control


    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            if (c + 3 < columns)
                score += score_segment(board[r][c], board[r][c + 1], board[r][c + 2], board[r][c + 3]);

            if (r + 3 < rows)
                score += score_segment(board[r][c], board[r + 1][c], board[r + 2][c], board[r + 3][c]);

            if (r + 3 < rows && c + 3 < columns)
                score += score_segment(board[r][c], board[r + 1][c + 1], board[r + 2][c + 2], board[r + 3][c + 3]);

            if (r + 3 < rows && c - 3 >= 0)
                score += score_segment(board[r][c], board[r + 1][c - 1], board[r + 2][c - 2], board[r + 3][c - 3]);
        }
    }

    return score;
}

int Four_In_A_Row_Board::get_best_ai_move(char aiSym) {
    int bestScore = INT_MIN;
    int bestCol = 3; // Center as default
    int depth = 5;   // AI search depth

    for (int col = 0; col < columns; col++) {
        int row = find_lowest_row(col);
        if (row == -1) continue;

        board[row][col] = aiSym;

        int score = minimax(depth - 1, false, aiSym, INT_MIN, INT_MAX);

        board[row][col] = '.';

        if (score > bestScore) {
            bestScore = score;
            bestCol = col;
        }
    }

    return bestCol;
}


bool Four_In_A_Row_Board::is_win_char(char sym) {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            // Check horizontal
            if (c + 3 < columns &&
                board[r][c] == sym && board[r][c + 1] == sym &&
                board[r][c + 2] == sym && board[r][c + 3] == sym)
                return true;
            // Check vertical and diagonal
            if (r + 3 < rows) {
                // Check vertical
                if (board[r][c] == sym && board[r + 1][c] == sym &&
                    board[r + 2][c] == sym && board[r + 3][c] == sym)
                    return true;
                // Check diagonal down-right
                if (c + 3 < columns &&
                    board[r][c] == sym && board[r + 1][c + 1] == sym &&
                    board[r + 2][c + 2] == sym && board[r + 3][c + 3] == sym)
                    return true;
                // Check diagonal down-left
                if (c - 3 >= 0 &&
                    board[r][c] == sym && board[r + 1][c - 1] == sym &&
                    board[r + 2][c - 2] == sym && board[r + 3][c - 3] == sym)
                    return true;
            }
        }
    }
    return false;
}


int Four_In_A_Row_Board::minimax(int depth, bool maximizing, char aiSym, int alpha, int beta) {
    char opp = (aiSym == 'X') ? 'O' : 'X';

    // 1) حالة الفوز/الخسارة
    if (is_win_char(aiSym)) return 1000000;
    if (is_win_char(opp))   return -1000000;


    // 2) عمق البحث انتهى
    if (depth == 0) return evaluate_board(aiSym);

    if (maximizing) {
        int best = INT_MIN;

        for (int col = 0; col < columns; col++) {
            int row = find_lowest_row(col);
            if (row == -1) continue;

            board[row][col] = aiSym;

            int val = minimax(depth - 1, false, aiSym, alpha, beta);

            board[row][col] = '.';

            best = max(best, val);
            alpha = max(alpha, val);
            if (beta <= alpha) break; // prune
        }
        return best;
    }
    else { // minimizing (opponent)
        int best = INT_MAX;

        for (int col = 0; col < columns; col++) {
            int row = find_lowest_row(col);
            if (row == -1) continue;

            board[row][col] = opp;

            int val = minimax(depth - 1, true, aiSym, alpha, beta);

            board[row][col] = '.';

            best = min(best, val);
            beta = min(beta, val);
            if (beta <= alpha) break;
        }
        return best;
    }
}


