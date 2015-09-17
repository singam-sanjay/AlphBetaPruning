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
	switch( board[xold][yold].player )
	{
		case plyr_1:{
			if( !(xold<=xnew && yold<ynew) )
			{		
				cerr << __func__ << " :" << '(' << xold << ',' << ynew << ")->(" << xnew << ',' << ynew << ") is not possible for plyr_1.\n";
				exit(11);
			}
			if( ynew == 7 )
			{
				val = HVAL::MAX_WIN;
				break;
			}
			if( board[xnew][ynew].player == plyr_2 )
			{
				val -= -( 7-ynew ); /* REMOVING the (-ve) EFFECT OF one of the PLYR_2'S PAWNS ON the HEURISTIC */
			}
			val += 1; // Since one pawn has moved "forward"
			break;
		}
		case plyr_2:{
			if( !(xold>=xnew && yold>ynew) )
			{		
				cerr << __func__ << " :" << '(' << xold << ',' << ynew << ")->(" << xnew << ',' << ynew << ") is not possible for plyr_2.\n";
				exit(11);
			}
			if( ynew==0 )
			{
				val = HVAL::MIN_WIN;
				break;
			}
			if( board[xnew][ynew].player == plyr_1 )
			{
				val -= ( ynew-0 ); /* REMOVING the (+ve) EFFECT OF one of the PLYR_2'S PAWNS ON the HEURISTIC */
			}
			val += -1; // Since onw pawn has moved "backward"
			break;
		}
		case invld:{
			cerr << __func__ << ": got invld @ " << xold << ',' << yold << ".\n";break;
		}
		case none:{
			cerr << __func__ << ": got none @ " << xold << ',' << yold << ".\n";break;
		}
	}

}

bool MOVE::operator<(MOVE other_move)
{
	return (hval < other_move.hval);
}

GAME::GAME(POS_elem pos[2][8], int max_turns, MOVE* ret_move) : hval(0)
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
	
	plyr = plyr_1 ; oppo = plyr_2;
	find_moves();
	find_hval_of_moves();
	sort_moves();

	HVAL ret_hval;
	MOVE best_move;
	
	// since the largest will be behind, move = moves.back()
	best_move = MOVE(moves.back());
	GAME(best_move,&ret_hval);
	hval = ret_hval;
	moves.pop_back();

	while( moves.size()!=0 )
	{
		GAME(MOVE(moves.back()),&ret_hval);
		if( hval<ret_hval ) // Since playr_1 is MAX
		{
			hval = ret_hval;
			best_move = MOVE(moves.back()); // Remembering the best move till now
		}
		moves.pop_back();
	}

	*ret_move = best_move;
}

GAME::GAME( MOVE move, HVAL* ret_hval )
{
	++curr_lvl;
	return;
}


void GAME::find_moves()
{
	unsigned char iter1,iter2;
	MOVE move(hval); 
	
	switch( plyr )
	{
		case plyr_1:{
			for(iter1=0 ; iter1<8 ; ++iter1)
			{
				if( pos_of[plyr][iter1].x == invld || pos_of[plyr][iter1].y == invld )continue;

				move.xold = pos_of[plyr][iter1].x;
				move.yold = pos_of[plyr][iter1].y;
				move.ynew = pos_of[plyr][iter1].y+1;
				if( 1 <= pos_of[plyr][iter1].x && board[ pos_of[plyr][iter1].x-1 ][ pos_of[plyr][iter1].y+1 ].player!=plyr )
				{
					move.xnew = pos_of[plyr][iter1].x-1;
					moves.push_front(move);
				}
				if( board[ pos_of[plyr][iter1].x ][ pos_of[plyr][iter1].y+1 ].player!=plyr )
				{
					move.xnew = pos_of[plyr][iter1].x;
					moves.push_front(move);
				}
				if( pos_of[plyr][iter1].x <= 6 && board[ pos_of[plyr][iter1].x+1 ][ pos_of[plyr][iter1].y+1 ].player!=plyr )
				{
					move.xnew = pos_of[plyr][iter1].x+1;
					moves.push_front(move);
				}
			}break;
		}
		case plyr_2:{
			for( iter1=0 ; iter1<8 ; ++iter1)
			{
				if( pos_of[plyr][iter1].x == invld || pos_of[plyr][iter1].y == invld )continue;

				move.xold = pos_of[plyr][iter1].x;
				move.yold = pos_of[plyr][iter1].y;
				move.ynew = pos_of[plyr][iter1].y-1;
				if( 1 <= pos_of[plyr][iter1].x && board[ pos_of[plyr][iter1].x-1 ][ pos_of[plyr][iter1].y-1 ].player!=plyr )
				{
					move.xnew = pos_of[plyr][iter1].x-1;
					moves.push_front(move);
				}
				if( board[ pos_of[plyr][iter1].x ][ pos_of[plyr][iter1].y-1 ].player!=plyr )
				{
					move.xnew = pos_of[plyr][iter1].x;
					moves.push_front(move);
				}
				if( pos_of[plyr][iter1].x <= 6 && board[ pos_of[plyr][iter1].x+1 ][ pos_of[plyr][iter1].y-1 ].player!=plyr )
				{
					move.xnew = pos_of[plyr][iter1].x+1;
					moves.push_front(move);
				}
			}
		}break;
		
	}
}

void MOVE::update()
{
	hval.update(xold,yold,xnew,ynew);
}

void GAME::find_hval_of_moves()
{
	static int N;
	static MOVE move;

	N = moves.size();
	while(N--)
	{
		move = MOVE(moves.back());moves.pop_back();
		move.update();
		moves.push_front(move);
	}
}

void GAME::sort_moves()
{
	moves.sort();
}