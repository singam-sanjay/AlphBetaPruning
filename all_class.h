#include<bits/stdc++.h>
using namespace std;

/* Header file for class definitions */

#define plyr_1 ('\0')
#define plyr_2 ('\1')
#define none   ('\2')
#define invld  ('\3')

class HVAL
{
	int val; // Replace this with actual heuristic
			 // h = sum(plyr_1,y) - sum(plyr_2,7-y)

public:
	HVAL(int val)
	{
		(this->val) = val;
	}
	bool operator<(HVAL hval);
	void update( unsigned char xold,unsigned char yold,unsigned char xnew,unsigned char ynew );
};

struct MOVE
{
	unsigned char xold : 4;
	unsigned char yold : 4;
	unsigned char xnew : 4;
	unsigned char ynew : 4;
	HVAL hval;
	MOVE(HVAL h_val) : hval(h_val) {}
	bool operator<(MOVE move);
};

struct POS_elem
{
	unsigned char x : 4;
	unsigned char y : 4;
};

struct BOARD_elem
{
	unsigned char player : 2;// 0,1 or 2
};

BOARD_elem board[8][8];
POS_elem	  pos_of[2][8];
int const MAX_LVL = 10;
int max_lvl, curr_lvl;

class GAME
{
	list<MOVE> moves;
	HVAL hval;
	unsigned char plyr, oppo;

public:

	GAME(POS_elem pos[2][8], int max_turns); /*Game starts here*/ 
	GAME(MOVE move);						/*Spawn a new game where the configuration differs by 'move'*/
	GAME(MOVE move, int parnt_hval );		/*Spawn a new game where the configuration differs by 'move',
											  knowing that the parent has minimax value of 'parent_hval'*/

	void find_moves();
	void find_hval_of_moves();
	void sort_moves();
};