/**
 * @file Infinity_TicTacToe.h
 * @brief ملف الهيدر للعبة Infinity Tic-Tac-Toe.
 * @details اللعبة تسمح بعدد محدود من الحركات لكل لاعب، وعند تجاوز الحد تختفي أقدم حركة.
 * @author Youssef Ibrahim , Shahd Ayman , Marwan Medhat and Mohamed Rami
 */

#ifndef INFINITY_CLASSES_H
#define INFINITY_CLASSES_H

#include "BoardGame_Classes.h"
#include <deque>

 /**
  * @class Infinity_Board
  * @brief بورد يدير تاريخ الحركات لحذف القديم منها.
  */
class Infinity_Board : public Board<char> {
private:
    char blank_symbol = '.';
    std::deque<Move<char>*> moveHistory; ///< طابور لتخزين الحركات بالترتيب.
    int moveCounter = 0;

public:
    Infinity_Board();
    bool update_board(Move<char>* move) override;

    /**
     * @brief حذف أقدم حركة عندما يتجاوز العدد المسموح.
     */
    void removeOldestMove();

    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override { return false; }
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class Infinity_UI
 * @brief واجهة المستخدم للعبة Infinity.
 */
class Infinity_UI : public UI<char> {
public:
    Infinity_UI();
    ~Infinity_UI() {}

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;
    virtual Move<char>* get_move(Player<char>* player) override;
};

#endif