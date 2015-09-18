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
			if( yold>ynew )
			{		
				cerr << __func__ << " :" << '(' << (int)xold << ',' << (int)yold << ")->(" << (int)xnew << ',' << (int)ynew << ") is not possible for plyr_1 @LVL " << curr_lvl << ".\n";
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
			if( yold<ynew )
			{		
				cerr << __func__ << " :" << '(' << (int)xold << ',' << (int)yold << ")->(" << (int)xnew << ',' << (int)ynew << ") is not possible for plyr_2 @LVL " << curr_lvl << ".\n";
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
		case none:{
			cerr << __func__ << ": got none @ " << (int)xold << ',' << (int)yold << " @LVL " << curr_lvl << " \n";exit(11);
		}
	}

}

unsigned char HVAL::won()
{
	switch( val )
	{
		case HVAL::MAX_WIN:return plyr_1;
		case HVAL::MIN_WIN:return plyr_2;
		default:		   return none;
	}
}
bool MOVE::operator<(MOVE other_move)
{
	return (hval < other_move.hval);
}

GAME::GAME(POS_elem pos[2][8], int max_turns, MOVE* ret_best_move) : hval(0)
{	cout << "GAME1\n";
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
		for( iter2=0 ; iter2<8 ; ++iter2 )board[iter1][iter2].player = none;
	}
	for( iter1=0 ; iter1<8 ; ++iter1 )
	{
		if( pos[plyr_1][iter1].x!=invld && pos[plyr_1][iter1].y!=invld )
		{
			board[ pos[plyr_1][iter1].x ][ pos[plyr_1][iter1].y ].player = plyr_1;
		}
		if( pos[plyr_2][iter1].x!=invld && pos[plyr_2][iter1].y!=invld )
		{
			board[ pos[plyr_2][iter1].x ][ pos[plyr_2][iter1].y ].player = plyr_2;
		}
	}

	plyr = plyr_1 ; oppo = plyr_2;
	find_moves();
	find_hval_of_moves();
	sort_moves();

	HVAL ret_hval;
	MOVE best_move;

	// since the largest will be behind, move = moves.back()
	best_move = MOVE(moves.back());
	if( best_move.hval.won()==plyr_1 )
	{
		*(ret_best_move) = best_move;
		return;
	}
	GAME(best_move,&ret_hval);
	hval = ret_hval;
	moves.pop_back();
	ret_hval.print();

	if( hval.won()==plyr_1 )
	{
		*(ret_best_move) = best_move;
		return;
	}

	while( moves.size()!=0 )
	{
		GAME(MOVE(moves.back()),hval,&ret_hval);
		ret_hval.print();
		if( hval<ret_hval ) // Since the first player (playr_1) is MAX
		{
			hval = ret_hval;
			best_move = MOVE(moves.back()); // Remembering the best move till now
			if( hval.won()==plyr_1 )
			{
				*(ret_best_move) = best_move;
				return;
			}
		}
		moves.pop_back();
	}

	*(ret_best_move) = best_move;
}

GAME::GAME( MOVE move, HVAL* ret_best_hval )
{	cout << "GAME2\n";
	HVAL ret_hval;

	save_prev_cntxt_and_update(move);

	//Now it starts
	find_moves();
	find_hval_of_moves();
	if( curr_lvl==max_lvl )
	{
		hval = (*( plyr==plyr_1 ? max_element( moves.begin(),moves.end() ) : min_element( moves.begin(),moves.end() ))).hval;
		goto wrap_up_1;
	}
	sort_moves();

	if( plyr==plyr_1 )
	{
		if( MOVE(moves.back()).hval.won()==plyr )
		{
			MOVE(moves.back()).hval.print();
			hval = MOVE(moves.back()).hval;
			goto wrap_up_1;
		}
		GAME(MOVE(moves.back()),&ret_hval);
		ret_hval.print();
		hval = ret_hval;
		moves.pop_back();
		if( hval.won()==plyr_1 )
		{
			goto wrap_up_1;
		}
		while( moves.size()!=0 )
		{
			GAME(MOVE(moves.back()),hval,&ret_hval);
			ret_hval.print();
			if( hval<ret_hval )
			{
				hval = ret_hval;
				if( hval.won()==plyr_1 )
				{
					goto wrap_up_1;
				}
			}
			moves.pop_back();
		}
	}
	else//plyr is plyr_2
	{
		if( MOVE(moves.front()).hval.won()==plyr )
		{
			MOVE(moves.front()).hval.print();
			hval = MOVE(moves.front()).hval;
			goto wrap_up_1;
		}
		GAME(MOVE(moves.front()),&ret_hval);
		ret_hval.print();
		hval = ret_hval;
		moves.pop_front();
		if( hval.won()==plyr_2 )
		{
			goto wrap_up_1;
		}
		while( moves.size()!=0 )
		{
			GAME(MOVE(moves.front()),hval,&ret_hval);
			ret_hval.print();
			if( ret_hval<hval )
			{
				hval = ret_hval;
				if( hval.won()==plyr_2 )
				{
					goto wrap_up_1;
				}
			}
			moves.pop_front();
		}
	}
	
wrap_up_1:*(ret_best_hval) = hval;

	load_prev_cntxt(move);
}

GAME::GAME(MOVE move, HVAL parnt_hval, HVAL* ret_best_hval )
{	cout << "GAME3\n";
	HVAL ret_hval;

	save_prev_cntxt_and_update(move);

	//Now it starts
	find_moves();
	find_hval_of_moves();
	if( curr_lvl==max_lvl )
	{
		hval = (*( plyr==plyr_1 ? max_element( moves.begin(),moves.end() ) : min_element( moves.begin(),moves.end() ))).hval;
		goto wrap_up_2;
	}
	sort_moves();

	if( plyr==plyr_1 )/* MAX plyr_1 => my parent's plyr_2, MIN */
	{
		if( MOVE(moves.back()).hval.won()==plyr )
		{
			hval = MOVE(moves.back()).hval;
			goto wrap_up_2;
		}
		GAME(MOVE(moves.back()),&ret_hval);
		hval = ret_hval;
		moves.pop_back();
		while( hval<parnt_hval && moves.size()!=0 )
		{
			GAME(MOVE(moves.back()),hval,&ret_hval);
			if( hval<ret_hval )
			{
				hval = ret_hval;
			}
			moves.pop_back();
		}
	}
	else/*plyr is MIN plyr_2 => my parent's plyr_1, MAX */
	{
		if( MOVE(moves.front()).hval.won()==plyr )
		{
			hval = MOVE(moves.front()).hval;
			goto wrap_up_2;
		}
		GAME(MOVE(moves.front()),&ret_hval);
		hval = ret_hval;
		moves.pop_front();
		while( parnt_hval<hval && moves.size()!=0 )
		{
			GAME(MOVE(moves.front()),hval,&ret_hval);
			if( ret_hval<hval )
			{
				hval = ret_hval;
			}
			moves.pop_front();
		}
	}

wrap_up_2:*(ret_best_hval) = hval;

	load_prev_cntxt(move);
}

void GAME::save_prev_cntxt_and_update(MOVE move)
{
	cout << "load [" << (int)move.xold << ',' << (int)move.yold << "]->[" << (int)move.xnew << ',' << (int)move.ynew << "]\n";
	register unsigned char iter1;
	/*Save the old context, pieces in 'prev_cntxt', position already in 'move'*/
	prev_cntxt.old_pos_pice = (board[move.xold][move.yold].player);
	prev_cntxt.new_pos_pice = (board[move.xnew][move.ynew].player);

	/*updates for the current level : need to move oppo's piece into position */
	++curr_lvl;
	plyr = ( curr_lvl%2 ?  plyr_1 : plyr_2 ); // ODD(curr_lvl) => expr evals to true => plyr_1 or EVEN(curr_lvl) => expr eval to false => plyr_2
	oppo = ( curr_lvl%2 ?  plyr_2 : plyr_1 ); // 'oppo' should be the other guy, that's why the options are reversed
	/*Updating pos_of */
	{//need to change the position of oppo's piece in pos_of
		//cout << "plyr_" << (int)(plyr+1) << " @LVL" << curr_lvl << '\n';
		for( iter1=0 ; iter1<8 ; ++iter1)
		{
			//cout<<(int)iter1<<" "<<(int)move.xold<<" "<<(int)pos_of[oppo][iter1].x<<" "<<(int)move.yold<<" "<<(int)pos_of[oppo][iter1].y<<" "<<"New points"<<(int)move.xnew<<" "<<(int)move.ynew<<endl;
			if( (pos_of[oppo][iter1].x==move.xold) && (pos_of[oppo][iter1].y==move.yold) )
			{
				pos_of[oppo][iter1].x = move.xnew;
				pos_of[oppo][iter1].y = move.ynew;
				break;
			}
		}
		if( iter1==8 )
		{
			cerr << __func__ << ':' << __LINE__ << ": Wasn't able to find the opponent 'plyr_" << (char)('1'+oppo) << "'s piece in pos_of at level " << curr_lvl << '.' << endl;
			exit(11);
		}
	}
	if( prev_cntxt.new_pos_pice==plyr )
	{//need to invalidate my piece in pos_of, incase oppo takes my piece
		for( iter1=0 ; iter1<8 ; ++iter1 )
		{
			if( (pos_of[plyr][iter1].x==move.xnew) && (pos_of[plyr][iter1].y==move.ynew) )
			{
				pos_of[plyr][iter1].x = invld;
				pos_of[plyr][iter1].y = invld;
				break;
			}
		}
		if( iter1==8 )
		{
			cerr << __func__ << ':' << __LINE__ << ": Wasn't able to find the current player 'plyr_" << (char)('1'+plyr) << "'s piece in pos_of at level " << curr_lvl << '.' << endl;
			exit(11);
		}
	}
	board[move.xold][move.yold].player = none; // moved plyr out of this
	board[move.xnew][move.ynew].player = oppo; // moved oppo into new position
	hval = move.hval; // hval for this move already computed while selecting moves in the previous 'GAME'
}
void GAME::find_moves()
{
	unsigned char iter1;
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
				if( 1 <= pos_of[plyr][iter1].x && board[ pos_of[plyr][iter1].x-1 ][ pos_of[plyr][iter1].y+1 ].player==oppo )
				{
					move.xnew = pos_of[plyr][iter1].x-1;
					moves.push_front(move);
				}
				if( board[ pos_of[plyr][iter1].x ][ pos_of[plyr][iter1].y+1 ].player==none )
				{
					move.xnew = pos_of[plyr][iter1].x;
					moves.push_front(move);
				}
				if( pos_of[plyr][iter1].x <= 6 && board[ pos_of[plyr][iter1].x+1 ][ pos_of[plyr][iter1].y+1 ].player==oppo )
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
				if( 1 <= pos_of[plyr][iter1].x && board[ pos_of[plyr][iter1].x-1 ][ pos_of[plyr][iter1].y-1 ].player==oppo )
				{
					move.xnew = pos_of[plyr][iter1].x-1;
					moves.push_front(move);
				}
				if( board[ pos_of[plyr][iter1].x ][ pos_of[plyr][iter1].y-1 ].player==none )
				{
					move.xnew = pos_of[plyr][iter1].x;
					moves.push_front(move);
				}
				if( pos_of[plyr][iter1].x <= 6 && board[ pos_of[plyr][iter1].x+1 ][ pos_of[plyr][iter1].y-1 ].player==oppo )
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

void GAME::load_prev_cntxt(MOVE move)
{
	cout << "restore [" << (int)move.xold << ',' << (int)move.yold << "]<-[" << (int)move.xnew << ',' << (int)move.ynew << "]\n";
	register unsigned char iter1;
	/* Restore previous state */
	board[move.xnew][move.ynew].player = prev_cntxt.new_pos_pice;
	board[move.xold][move.yold].player = prev_cntxt.old_pos_pice;//the player
	if( prev_cntxt.new_pos_pice==plyr )
	{//Need to validate my piece corresponding to board[move.xnew][move.ynew].player in pos_of
		for( iter1=0 ; iter1<8 ; ++iter1)
		{
			if( (pos_of[plyr][iter1].x==invld) && (pos_of[plyr][iter1].y==invld) )
			{
				pos_of[plyr][iter1].x = move.xnew;
				pos_of[plyr][iter1].y = move.ynew;
				break;
			}
		}
		if( iter1==8 )
		{
			cerr << __func__ << ':' << __LINE__ << ": Wasn't able to find the current player 'plyr_" << (char)('1'+plyr) << "'s piece in pos_of at level " << curr_lvl << '.' << endl;
			exit(11);
		}
	}
	{//need to revert the position of oppo's piece in pos_of
		for( iter1=0 ; iter1<8 ; ++iter1)
		{
			if( (pos_of[oppo][iter1].x==move.xnew) && (pos_of[oppo][iter1].y==move.ynew) )
			{
				pos_of[oppo][iter1].x = move.xold;
				pos_of[oppo][iter1].y = move.yold;
				break;
			}
		}
		if( iter1==8 )
		{
			cerr << __func__ << ':' << __LINE__ << ": Wasn't able to find the opponent 'plyr_" << (char)('1'+oppo) << "'s piece in pos_of at level " << curr_lvl << '.' << endl;
			exit(11);
		}
	}
	--curr_lvl;
}