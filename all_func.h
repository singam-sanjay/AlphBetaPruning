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
		cerr << '(' << xold << ',' << ynew << ")->(" << xnew << ',' << ynew << ") is not possible.\n";
		exit(11);
	}

	val += ( ynew-yold ); /* THIS IS delta(HEURISTIC) */
}

bool MOVE::operator<(MOVE other_move)
{
	return (hval < other_move.hval);
}

