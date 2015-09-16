#include"all_class.h"

/* functions operating on objects */

bool HVAL::operator<(HVAL other_hval)
{
	return val < other_hval.val;
}


void HVAL::update( unsigned char xold,unsigned char yold,unsigned char xnew,unsigned char ynew )
/*
 * Instead of computing the heuristic each time, we just calculate the value by looking at the move made
 */
{
	if( !(xold<=xnew && yold<=ynew) )
	{
		cerr << __func__ << " :" << '(' << xold << ',' << ynew << ")->(" << xnew << ',' << ynew << ") is not possible.\n";
		exit(11);
	}

	val += ( ynew-yold ); /* THIS IS delta(HEURISTIC) */
}

bool MOVE::operator<(MOVE other_move)
{
	return (hval < other_move.hval);
}

GAME::GAME(POS_elem pos[2][8], int max_turns) : hval(0)
{
	memcpy( pos_of, pos, sizeof(POS_elem)*2*8 );
	max_lvl = 2*max_turns;
	if( max_lvl>MAX_LVL )
	{
		cerr << __func__ << " :" << "#turns too much.\n";
		exit(11);
	}
	curr_lvl = 1;
	
	unsigned char iter1,iter2;
	for( iter1=0 ; iter1<8 ; ++iter1 )
	{
		for( iter2=0 ; iter1<8 ; ++iter1 )board[iter1][iter2].player = none;
	}
	for( iter1=0 ; iter1<8 ; ++iter1 )
	{
		board[ pos[plyr_1][iter1].x ][ pos[0][iter1].y ].player = plyr_1;
		board[ pos[plyr_2][iter1].x ][ pos[1][iter1].y ].player = plyr_2;
	}
}