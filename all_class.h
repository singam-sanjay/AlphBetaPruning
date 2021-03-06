#include<bits/stdc++.h>
using namespace std;

/* Header file for class definitions */

#define plyr_1 ('\0') 
#define plyr_2 ('\1')
#define none   ('\2') // This is a state of a square in the board
#define invld  ('\010') // This is a position of a piece in the board, i.e. NOWHERE , octal 10 == decimal 8

class HVAL
{
	int val; // Replace this with actual heuristic
			 // h = sum(plyr_1,y) - sum(plyr_2,7-y)

public:
	static const int MAX_WIN = INT_MAX, MIN_WIN = INT_MIN;
	HVAL(int val)
	{
		(this->val) = val;
	}
	HVAL() {}
	bool operator<(HVAL hval);
	void update( unsigned char xold,unsigned char yold,unsigned char xnew,unsigned char ynew );
	char unsigned won();
	void print()
	{
		cout << "hval:" << val << endl;
	}
};

struct MOVE
{
	unsigned char xold : 4;
	unsigned char yold : 4;
	unsigned char xnew : 4;
	unsigned char ynew : 4;
	HVAL hval;
	MOVE(HVAL h_val) : hval(h_val) {}
	MOVE() : hval(0) {}
	void update();
	bool operator<(MOVE move);
};

struct POS_elem
{
	unsigned char x : 4;
	unsigned char y : 4;
};

struct MOVE_PIECE_elem
{
	unsigned char old_pos_pice : 2; // pice is faster type than piece ;)
	unsigned char new_pos_pice : 2;
	MOVE_PIECE_elem()
	{
		old_pos_pice = none;
		new_pos_pice = none;
	}
	MOVE_PIECE_elem(unsigned char old_p,unsigned char new_p)
	/* references since prev_cntxt( board[move.xold][move.yold].player,board[move.xnew][move.ynew].player )
	 * is ‘(MOVE_PIECE_elem) (unsigned char:2&, unsigned char:2&)’ */
	{
		old_pos_pice = old_p;
		new_pos_pice = new_p;
	}
};

struct BOARD_elem
{
	unsigned char player : 2;// 0,1 or 2
};

BOARD_elem board[8][8];
POS_elem	  pos_of[2][8];
int const MAX_LVL = 10;
int max_lvl, curr_lvl;
unsigned int count_moves;

class GAME
{
	list<MOVE> moves;
	HVAL hval;
	unsigned char plyr, oppo;

	MOVE_PIECE_elem prev_cntxt;
public:

	GAME(POS_elem pos[2][8], int max_turns, MOVE* ret_best_move); /*Game starts here*/ 
	GAME( MOVE move, HVAL* ret_best_hval );						/*Spawn a new game where the configuration differs by 'move'*/
	GAME(MOVE move, HVAL parnt_hval, HVAL* ret_hval );		/*Spawn a new game where the configuration differs by 'move',
											  knowing that the parent has minimax value of 'parent_hval'*/

	void save_prev_cntxt_and_update(MOVE move);
	void find_moves();
	void find_hval_of_moves();
	void sort_moves();
	void load_prev_cntxt(MOVE move);
};
