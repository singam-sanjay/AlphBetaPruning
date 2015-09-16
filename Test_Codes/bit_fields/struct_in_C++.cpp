#include<iostream>

struct BOARD
{
	unsigned char player : 2;
};

struct POS
{
	unsigned char x : 4;
	unsigned char y : 4;
};

struct CONFIG
{
	BOARD board[8][8]; // current config of board
	POS pos_of[2][8]; // pos_of[W/B][Nth pawn]
};

int main()
{
	std::cout  << sizeof(BOARD) << " " << sizeof(POS) << " " << sizeof(CONFIG) << '\n';
	return 0;
}
