CC = g++
CFLAGS = -Wall --std=c++11

main.exe : main.o dices.o random.o
	$(CC) random.o dices.o main.o -o main.exe
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
dices.o : dices.cpp dices.h
	$(CC) -c dices.cpp -o dices.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
run : 
	./main.exe
clean :
	rm *.o main.exe seed.out
