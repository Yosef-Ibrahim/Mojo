#ifndef FOUR_IN_A_ROW_UI_H
#define FOUR_IN_A_ROW_UI_H

#include "BoardGame_Classes.h"

using namespace std;
//UI
class Four_In_A_Row_UI : public UI<char>
{
private:
	Board<char>* boardPtr;  

public:
	Four_In_A_Row_UI(Board<char>* board);
	virtual ~Four_In_A_Row_UI();

	Move<char>* get_move(Player<char>* player) override;
	Player<char>** setup_players() override;
	Player<char>* create_player(string& name, char symbol, PlayerType type) override;

	void display_board_matrix(const vector<vector<char>>& matrix) const;
};

//board 
class Four_In_A_Row_Board : public Board<char>
{
public:
	Four_In_A_Row_Board(int r = 6, int c = 7); 

	bool update_board(Move<char>* move) override;
	bool is_win(Player<char>* player) override;
	bool is_draw(Player<char>* player) override;
	bool game_is_over(Player<char>* player) override;

	int find_lowest_row(int column);
};

#endif // FOUR_IN_A_ROW_UI_H