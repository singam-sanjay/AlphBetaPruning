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

	POS pos;

	unsigned char values[] = "\0\1\2\3\4\5\6\7\14\9";
	for( int i=0 ; i<10 ; ++i )
	{
		pos.x=values[i];
		std::cout << (unsigned int)pos.x << ' ';
	}
	std::cout << 8 << ' ' << (unsigned char)(8) << '\n';
	std::cout << std::endl;
	return 0;
}
