.PHONY: all

pawn-chess: main.cpp all_class.h all_func.h
	g++ -ftrack-macro-expansion=0 -fno-diagnostics-show-caret -O3 main.cpp -o pawn-chess

pawn-chess-pruned: main.cpp all_class.h all_func.h
	g++ -ftrack-macro-expansion=0 -fno-diagnostics-show-caret -O3 main.cpp -DALPH_BETA -o pawn-chess-pruned

all: pawn-chess pawn-chess-pruned

run: pawn-chess
	./pawn-chess

run-pruned: paen-chess-pruned
	./pawn-chess-pruned
