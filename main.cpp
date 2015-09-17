#include "all_func.h"

POS_elem pos[2][8];
int max_turns;
MOVE move;
unsigned char plyr = plyr_2, oppo = plyr_1;

void init_board();
void get_user_ip();
bool move_on_board();

int main()
{
	init_board();

	cout << "maximum #turns lookahed (max " << MAX_LVL/2 << ")" << endl;cin >> max_turns;
	
	do
	{
		swap(plyr,oppo);
		if( plyr==plyr_1 )
		{
			GAME(pos,max_turns,&move);
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
		pos[plyr_1][iter1].x = 0;pos[plyr_1][iter1].y = iter1;
		pos[plyr_2][iter1].x = 7;pos[plyr_2][iter1].y = iter1;
	}
}

void get_user_ip()
{
	unsigned int xold,xnew,yold,ynew;
	unsigned char iter1;
	do
	{
		cout << "xold:";cin >> xold;if( !(xold<0 && 7<xold) )goto invalid_coord;
		cout << "yold:";cin >> yold;if( !(yold<0 && 7<yold) )goto invalid_coord;
		cout << "xnew:";cin >> xnew;if( !(xnew<0 && 7<xnew && xnew<=xold) )goto invalid_coord;
		cout << "ynew:";cin >> ynew;if( !(ynew<0 && 7<ynew && ynew<yold) )goto invalid_coord;
		for( iter1=0 ; iter1<8 ; ++iter1 )
		{
			if( pos[oppo][iter1].x==move.xold && pos_of[oppo][iter1].y==move.yold )break;
		}
		if( iter1==8 )
		{
			cout << "Old coord do not exist\n";
			continue;
		}
		for( iter1=0 ; iter1<8 ; ++iter1)
		{
			if( pos[oppo][iter1].x==move.xnew && pos_of[oppo][iter1].y==move.ynew )break;
		}
		if( iter1!=8 )
		{
			cout << "New coord has your piece\n";
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
	cout << '[' << move.xold << ',' << move.yold << "]->[" << move.xnew << ',' << move.ynew << "]\n";
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
