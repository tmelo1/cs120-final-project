CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -g

chess: Chess.o Board.o
	$(CXX) $(CXXFLAGS) Chess.o Board.o -o chess

Board.o: Game.h
Chess.o: Game.h Chess.h Prompts.h

clean:
	rm *.o chess

