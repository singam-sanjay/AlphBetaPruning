#include "all_func.h"

POS_elem pos[2][8];
int max_turns;
MOVE move;
unsigned char plyr = plyr_2, oppo = plyr_1;

void init_board();
void custom_init_board();
void get_user_ip();
bool move_on_board();

int main()
{
	//custom_init_board();
	init_board();

	cout << "maximum #turns lookahed (max " << MAX_LVL/2 << ") :" ;cin >> max_turns;

	do
	{
		swap(plyr,oppo);
		if( plyr==plyr_1 )
		{
			count_moves = 0;
			GAME(pos,max_turns,&move);
			cout << count_moves << " cumulative moves made to lookahead.\n";
		}
		else
		{
			get_user_ip();
		}
	}while(move_on_board()!=true);//true, then winning move
	if( plyr==plyr_1 )
	{
		cout << "I won!\n";
	}
	else
	{
		cout << "You won!\n";
	}
	return 0;
}

void init_board()
{
	unsigned char iter1;
	for( iter1=0 ; iter1<8 ; ++iter1 )
	{
		pos[plyr_1][iter1].y = 0;pos[plyr_1][iter1].x = iter1;
		pos[plyr_2][iter1].y = 7;pos[plyr_2][iter1].x = iter1;
	}
}

void custom_init_board()
{
	//pos[plyr_1][0].x = 0;pos[plyr_1][0].y = 6;
	//pos[plyr_1][0].x = 1;pos[plyr_1][0].y = 5;
	pos[plyr_1][0].x = invld;pos[plyr_1][0].y = invld;
	//pos[plyr_1][1].x = invld;pos[plyr_1][1].y = invld;
	//pos[plyr_1][1].x = 2;pos[plyr_1][1].y = 5;
	pos[plyr_1][1].x = invld;pos[plyr_1][1].y = invld;
	pos[plyr_1][2].x = invld;pos[plyr_1][2].y = invld;
	pos[plyr_1][3].x = invld;pos[plyr_1][3].y = invld;
	pos[plyr_1][4].x = invld;pos[plyr_1][4].y = invld;
	pos[plyr_1][5].x = 5;pos[plyr_1][5].y = 1;
	pos[plyr_1][6].x = 6;pos[plyr_1][6].y = 5;
	pos[plyr_1][7].x = invld;pos[plyr_1][7].y = invld;

	//pos[plyr_2][0].x = 0;pos[plyr_2][0].y = 1;
	pos[plyr_2][0].x = invld ;pos[plyr_2][0].y = invld;
	//pos[plyr_2][1].x = 1;pos[plyr_2][1].y = 7;
	//pos[plyr_2][1].x = 0;pos[plyr_2][1].y = 7;
	//pos[plyr_2][1].x = 1;pos[plyr_2][1].y = 6;
	pos[plyr_2][1].x = invld;pos[plyr_2][1].y = invld;
	pos[plyr_2][2].x = invld;pos[plyr_2][2].y = invld;
	pos[plyr_2][3].x = 3;pos[plyr_2][3].y = 5;
	pos[plyr_2][4].x = invld;pos[plyr_2][4].y = invld;
	pos[plyr_2][5].x = invld;pos[plyr_2][5].y = invld;
	pos[plyr_2][6].x = invld;pos[plyr_2][6].y = invld;
	pos[plyr_2][7].x = invld;pos[plyr_2][7].y = invld;

}
void get_user_ip()
{
	int xold,xnew,yold,ynew;
	cout << "Your move\n";
	unsigned char iter1;
	do
	{
		cout << "xold:";cin >> xold;if( xold<0 || 7<xold )goto invalid_coord;move.xold = xold;
		cout << "yold:";cin >> yold;if( yold<0 || 7<yold )goto invalid_coord;move.yold = yold;
		cout << "xnew:";cin >> xnew;if( xnew<0 || 7<xnew || xnew<(xold-1) || (xold+1)<xnew )goto invalid_coord;move.xnew = xnew;
		cout << "ynew:";cin >> ynew;if( ynew<0 || 7<ynew || ynew!=(yold-1) )goto invalid_coord;move.ynew = ynew;
		for( iter1=0 ; iter1<8 ; ++iter1 )
		{
			if( pos[plyr_2][iter1].x==move.xold && pos[plyr_2][iter1].y==move.yold )break;
		}
		if( iter1==8 )
		{
			cout << "Old coord do not exist\n";
			continue;
		}
		for( iter1=0 ; iter1<8 ; ++iter1)
		{
			if( pos[plyr_2][iter1].x==move.xnew && pos[plyr_2][iter1].y==move.ynew )break;
		}
		if( iter1!=8 )
		{
			cout << "New coord has your piece\n";
			continue;
		}
		if( move.xnew==move.xold )//It's moving forward
		for( iter1=0 ; iter1<8 ; ++iter1 )
		{//Need to check if computer's piece is in front
			if( pos[plyr_1][iter1].x==move.xnew && pos[plyr_1][iter1].y==move.ynew )break;
		}
		if( iter1!=8 )
		{
			cout << "My piece is in front of the piece you're moving.";
			continue;
		}
		break;
invalid_coord:cout << "Invalid coordinates\n";
		continue;
	}while(1);
	move.xold = xold;
	move.yold = yold;
	move.xnew = xnew;
	move.ynew = ynew;
}
bool move_on_board()
{
	cout << '[' << (int)move.xold << ',' << (int)move.yold << "]->[" << (int)move.xnew << ',' << (int)move.ynew << "]\n";
	if( move.ynew==7 || move.ynew==0 )
	{
		return true;// Someone just won
	}
	unsigned char iter1=0;
	for( iter1=0 ; iter1<8 ; ++iter1 )
	{
		if( pos[plyr][iter1].x==move.xold && pos[plyr][iter1].y==move.yold )
		{
			pos[plyr][iter1].x = move.xnew;
			pos[plyr][iter1].y = move.ynew;
		}
		if( pos[oppo][iter1].x==move.xnew && pos[oppo][iter1].y==move.ynew )
		{
			pos[oppo][iter1].x = invld;
			pos[oppo][iter1].y = invld;
		}
	}
	return false;
}
