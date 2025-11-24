#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <map>

using namespace std;

// فئة اللوحة للعبة SUS (شبكة 3x3 باستخدام رموز char)
class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';

public:
    // Constructor
    SUS_Board();

    // الدوال الأساسية (يجب عليك تطبيقها):
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;
};


// فئة واجهة المستخدم للعبة SUS
class SUS_UI : public UI<char> {
public:
    SUS_UI();
    ~SUS_UI() {};

    // إنشاء اللاعب (الرمز 'S' أو 'U' ثابت لكل لاعب)
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    // تخصيص رموز اللاعبين (S و U)
    Player<char>** setup_players() override;

    // الحصول على الحركة
    Move<char>* get_move(Player<char>* currentPlayer) override;
};

// دالة مساعدة لحساب تسلسلات "S-U-S" (سنستخدمها في نهاية اللعبة)
int calculate_all_sus_sequences(const vector<vector<char>>& board);


#endif // SUS_CLASSES_H