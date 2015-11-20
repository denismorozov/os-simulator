CC = g++
LFLAGS = -std=c++11 -Wall -pthread
CFLAGS = -I. -std=c++11 -Wall -pthread -c
OBJS = main.o simulator.o program.o operation.o

sim02: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o sim02

main.o: src/main.cpp src/simulator.h src/program.h src/operation.h
	$(CC) $(CFLAGS) src/main.cpp

simulator.o: src/simulator.cpp src/simulator.h src/program.h src/operation.h
	$(CC) $(CFLAGS) src/simulator.cpp

program.o: src/program.cpp src/program.h src/operation.h 
	$(CC) $(CFLAGS) src/program.cpp

operation.o: src/operation.cpp src/operation.h
	$(CC) $(CFLAGS) src/operation.cpp

generator:
	$(CC) src/program_generator.cpp -o generator

clean:
	\rm sim02 generator *.o

