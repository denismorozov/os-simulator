CC = g++
LFLAGS = -std=c++14 -Wall -pthread
CFLAGS = -std=c++14 -Wall -pthread -c
OBJS = Main.o Simulator.o Program.o

sim01: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o sim01

Main.o: Main.cpp Simulator.h Program.h
	$(CC) $(CFLAGS) Main.cpp

Simulator.o: Simulator.cpp Simulator.h Program.h
	$(CC) $(CFLAGS) Simulator.cpp

Program.o: Program.cpp Program.h
	$(CC) $(CFLAGS) Program.cpp

clean:
	\rm sim01 *.o
