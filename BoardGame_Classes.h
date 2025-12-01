#ifndef _BOARDGAME_CLASSES_H
#define _BOARDGAME_CLASSES_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib> 

using namespace std;

template <typename T> class Player;
template <typename T> class Move;

// أنواع اللاعبين (تم إضافة AI و Random)
enum class PlayerType {
    HUMAN,
    COMPUTER,
    AI,
    RANDOM
};

// -----------------------------------------------------------------------------
// Class: Board
// -----------------------------------------------------------------------------
template <typename T>
class Board {
protected:
    int rows;
    int columns;
    vector<vector<T>> board;
    int n_moves = 0;

public:
    Board(int r, int c) : rows(r), columns(c), board(r, vector<T>(c)) {}
    virtual ~Board() {}

    int get_rows() const { return rows; }
    int get_columns() const { return columns; }
    vector<vector<T>>& get_board_matrix() { return board; }

    virtual bool update_board(Move<T>* move) = 0;
    virtual bool is_win(Player<T>* player) = 0;
    virtual bool is_draw(Player<T>* player) = 0;
    virtual bool is_lose(Player<T>* player) { return false; };
    virtual bool game_is_over(Player<T>* player) = 0;
};

// -----------------------------------------------------------------------------
// Class: Move
// -----------------------------------------------------------------------------
template <typename T>
class Move {
protected:
    int x, y;
    T symbol;

public:
    Move(int r, int c, T s) : x(r), y(c), symbol(s) {}
    virtual ~Move() {}

    int get_x() const { return x; }
    int get_y() const { return y; }
    T get_symbol() const { return symbol; }
};

// -----------------------------------------------------------------------------
// Class: Player
// -----------------------------------------------------------------------------
template <typename T>
class Player {
protected:
    string name;
    T symbol;
    PlayerType type;

public:
    Player(string n, T s, PlayerType t) : name(n), symbol(s), type(t) {}
    virtual ~Player() {}

    string get_name() const { return name; }
    T get_symbol() const { return symbol; }
    PlayerType get_type() const { return type; }
};

// -----------------------------------------------------------------------------
// Class: UI
// -----------------------------------------------------------------------------
template <typename T>
class UI {
protected:
    string welcome_message;
    int board_size;

public:
    UI(string msg, int size) : welcome_message(msg), board_size(size) {}
    virtual ~UI() {}

    virtual Player<T>** setup_players();
    virtual Player<T>* create_player(string& name, T symbol, PlayerType type) = 0;
    virtual Move<T>* get_move(Player<T>* currentPlayer) = 0;

    void display_welcome() {
        cout << "\n========================================\n";
        cout << "  " << welcome_message << "\n";
        cout << "========================================\n";
    }

    // دالة العرض المحسنة (New Visuals)
    void display_board_matrix(const vector<vector<T>>& matrix) {
        int rows = matrix.size();
        int cols = (rows > 0) ? matrix[0].size() : 0;

        // 1. رسم الهرم بتصميم الصناديق المتراصة (أنظف وأوضح)
        if (rows == 3 && cols == 5) {
            cout << "\n";
            // دليل الأعمدة
            cout << "                      0   1   2   3   4" << endl;
            cout << "                          +---+" << endl;
            // الصف 0 (القمة - خانة واحدة في العمود 2)
            cout << " 0                        | " << matrix[0][2] << " |" << endl;

            // الصف 1 (الوسط - 3 خانات في الأعمدة 1, 2, 3)
            cout << "                      +---+---+---+" << endl;
            cout << " 1                    | " << matrix[1][1] << " | " << matrix[1][2] << " | " << matrix[1][3] << " |" << endl;

            // الصف 2 (القاعدة - 5 خانات كاملة)
            cout << "                  +---+---+---+---+---+" << endl;
            cout << " 2                | " << matrix[2][0] << " | " << matrix[2][1] << " | " << matrix[2][2] << " | " << matrix[2][3] << " | " << matrix[2][4] << " |" << endl;
            cout << "                  +---+---+---+---+---+" << endl;
            cout << endl;
            return;
        }

        // 2. العرض العادي لباقي الألعاب (Grid) مع ترقيم الصفوف والأعمدة
        cout << "\n     ";
        for (int j = 0; j < cols; ++j) {
            cout << " " << j << "  ";
        }
        cout << "\n    +";
        for (int j = 0; j < cols; ++j) {
            cout << "---+";
        }
        cout << "\n";

        for (int i = 0; i < rows; ++i) {
            cout << "  " << i << " |"; // رقم الصف
            for (int j = 0; j < cols; ++j) {
                cout << " " << matrix[i][j] << " |";
            }
            cout << "\n    +";
            for (int j = 0; j < cols; ++j) {
                cout << "---+";
            }
            cout << "\n";
        }
        cout << endl;
    }

    void display_message(const string& msg) {
        cout << msg << endl;
    }

    string get_player_name(const string& player_label) {
        string name;
        cout << "Enter " << player_label << " name: ";
        cin >> name;
        return name;
    }

    PlayerType get_player_type_choice(const string& player_label, const vector<string>& options) {
        int choice;
        cout << player_label << " type (";
        for (size_t i = 0; i < options.size(); ++i) {
            cout << i + 1 << ":" << options[i] << (i == options.size() - 1 ? "" : ", ");
        }
        cout << "): ";
        cin >> choice;
        return static_cast<PlayerType>(choice - 1);
    }
};

// -----------------------------------------------------------------------------
// Class: GameManager
// -----------------------------------------------------------------------------
template <typename T>
class GameManager {
private:
    Board<T>* boardPtr;
    Player<T>** players;
    UI<T>* ui;

public:
    GameManager(Board<T>* b, Player<T>** p, UI<T>* u) : boardPtr(b), players(p), ui(u) {}

    void run() {
        ui->display_welcome();
        ui->display_board_matrix(boardPtr->get_board_matrix());

        while (true) {
            for (int i = 0; i < 2; ++i) {
                Player<T>* currentPlayer = players[i];
                Move<T>* move = ui->get_move(currentPlayer);

                while (!boardPtr->update_board(move)) {
                    ui->display_message("\n!!! Invalid Move. Spot is taken or out of bounds. Try again. !!!");
                    move = ui->get_move(currentPlayer);
                }

                // عرض اللوحة الجديدة (بدون مسح القديم عشان التاريخ يفضل موجود)
                ui->display_board_matrix(boardPtr->get_board_matrix());

                if (boardPtr->is_win(currentPlayer)) {
                    ui->display_message(currentPlayer->get_name() + " wins!");
                    return;
                }
                if (boardPtr->is_lose(currentPlayer)) {
                    ui->display_message(players[1 - i]->get_name() + " wins!");
                    return;
                }
                if (boardPtr->is_draw(currentPlayer)) {
                    ui->display_message("Draw!");
                    return;
                }
            }
        }
    }
};

template <typename T>
Player<T>** UI<T>::setup_players() {
    Player<T>** players = new Player<T>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string nameX = get_player_name("Player 1");
    PlayerType typeX = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(nameX, static_cast<T>('X'), typeX);

    string nameO = get_player_name("Player 2");
    PlayerType typeO = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(nameO, static_cast<T>('O'), typeO);

    return players;
}

#endif