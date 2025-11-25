#include "Infinity_TicTacToe.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


template <typename T>
Infinity_TicTacToe_Board<T>::Infinity_TicTacToe_Board() : Board<T>(3, 3) {
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            this->board[i][j] = static_cast<T>(' ');
        }
    }
}

template <typename T>
bool Infinity_TicTacToe_Board<T>::update_board(Move<T>* move) {
    int x = move->get_x();
    int y = move->get_y();
    T symbol = move->get_symbol();

   
    if (x < 0 || x >= 3 || y < 0 || y >= 3) {
        return false;
    }


    if (this->board[x][y] != static_cast<T>(' ')) {
        return false;
    }

    
    this->board[x][y] = symbol;
    this->n_moves++;

    
    move_history.push_back(make_pair(x, y));

 
    if (move_history.size() > 3) {
        pair<int, int> oldest = move_history.front();
        move_history.pop_front();

        
        this->board[oldest.first][oldest.second] = static_cast<T>(' ');
        this->n_moves--;
    }

    return true;
}

template <typename T>
bool Infinity_TicTacToe_Board<T>::check_win_for_symbol(T symbol) {
  
    for (int i = 0; i < 3; ++i) {
        if (this->board[i][0] == symbol &&
            this->board[i][1] == symbol &&
            this->board[i][2] == symbol) {
            return true;
        }
    }

    
    for (int j = 0; j < 3; ++j) {
        if (this->board[0][j] == symbol &&
            this->board[1][j] == symbol &&
            this->board[2][j] == symbol) {
            return true;
        }
    }


    if (this->board[0][0] == symbol &&
        this->board[1][1] == symbol &&
        this->board[2][2] == symbol) {
        return true;
    }

  
    if (this->board[0][2] == symbol &&
        this->board[1][1] == symbol &&
        this->board[2][0] == symbol) {
        return true;
    }

    return false;
}

template <typename T>
bool Infinity_TicTacToe_Board<T>::is_win(Player<T>* player) {
    return check_win_for_symbol(player->get_symbol());
}

template <typename T>
bool Infinity_TicTacToe_Board<T>::is_draw(Player<T>* player) {

    return false;
}

template <typename T>
bool Infinity_TicTacToe_Board<T>::game_is_over(Player<T>* player) {
    return is_win(player);
}


template <typename T>
Infinity_TicTacToe_Player<T>::Infinity_TicTacToe_Player(string name, T symbol, PlayerType type)
    : Player<T>(name, symbol, type) {}


template <typename T>
Infinity_TicTacToe_RandomPlayer<T>::Infinity_TicTacToe_RandomPlayer(string name, T symbol)
    : Infinity_TicTacToe_Player<T>(name, symbol, PlayerType::RANDOM) {}

template <typename T>
pair<int, int> Infinity_TicTacToe_RandomPlayer<T>::get_random_move(Board<T>* board) {
    vector<pair<int, int>> available_moves;
    auto board_matrix = board->get_board_matrix();

   
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board_matrix[i][j] == static_cast<T>(' ')) {
                available_moves.push_back(make_pair(i, j));
            }
        }
    }

  
    if (!available_moves.empty()) {
        int random_index = rand() % available_moves.size();
        return available_moves[random_index];
    }

    return make_pair(-1, -1); 
}


template <typename T>
Infinity_TicTacToe_UI<T>::Infinity_TicTacToe_UI()
    : UI<T>("Welcome to Infinity Tic-Tac-Toe!", 3), board_ptr(nullptr) {}

template <typename T>
Player<T>* Infinity_TicTacToe_UI<T>::create_player(string& name, T symbol, PlayerType type) {
    if (type == PlayerType::HUMAN) {
        return new Infinity_TicTacToe_Player<T>(name, symbol, PlayerType::HUMAN);
    }
    else {
        return new Infinity_TicTacToe_RandomPlayer<T>(name, symbol);
    }
}

template <typename T>
Move<T>* Infinity_TicTacToe_UI<T>::get_move(Player<T>* currentPlayer) {
    int x, y;

    if (currentPlayer->get_type() == PlayerType::HUMAN) {
        cout << "\n" << currentPlayer->get_name()
            << " (" << currentPlayer->get_symbol() << "), enter your move (row column): ";
        cin >> x >> y;
    }
    else {
        // اللاعب العشوائي
        auto* random_player = dynamic_cast<Infinity_TicTacToe_RandomPlayer<T>*>(currentPlayer);
        if (random_player && board_ptr) {
            pair<int, int> random_move = random_player->get_random_move(board_ptr);
            x = random_move.first;
            y = random_move.second;
            cout << "\n" << currentPlayer->get_name()
                << " (Computer) chose: Row " << x << ", Column " << y << endl;
        }
        else {
            x = y = -1;
        }
    }

    return new Move<T>(x, y, currentPlayer->get_symbol());
}


template class Infinity_TicTacToe_Board<char>;
template class Infinity_TicTacToe_Player<char>;
template class Infinity_TicTacToe_RandomPlayer<char>;
template class Infinity_TicTacToe_UI<char>;