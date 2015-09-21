pawn-chess: main.cpp all_class.h all_func.h
	g++ main.cpp -O3 -o pawn-chess

run: pawn-chess
	./pawn-chess
