pawn-chess: main.cpp all_class.h all_func.h
	g++ -ftrack-macro-expansion=0 -fno-diagnostics-show-caret -O3 main.cpp -o pawn-chess

run: pawn-chess
	./pawn-chess
