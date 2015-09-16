/* Header file for class definitions */

class HVAL
{
	int val; // Replace this with actual heuristic
	public:
	HVAL(int val)
	{
		this->val = val;
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

class GAME
{
	static BOARD_elem board[8][8];
	static POS_elem	  pos_of[2][8];
	static int const MAX_LVL = 10;
	static int max_lvl, curr_lvl=0;

	list<move> moves;

public:

	GAME(int board[2][8], int max_turns); /*Game starts here*/ 
	GAME(MOVE move);					  /*Spawn a new game where the configuration differs by 'move'*/
	GAME(MOVE move, int parnt_hval );	  /*Spawn a new game where the configuration differs by 'move',
											knowing that the parent has minimax value of 'parent_hval'*/

	void find_moves();
	void find_hval_of_moves();
	void sort_moves();
}