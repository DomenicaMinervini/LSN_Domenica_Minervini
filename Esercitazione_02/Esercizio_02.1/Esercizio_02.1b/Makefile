CC = g++
CFLAGS = -Wall --std=c++11

main.exe : main.o media_blocchi.o random.o
	$(CC) random.o media_blocchi.o main.o -o main.exe
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
media_blocchi.o : media_blocchi.cpp media_blocchi.h
	$(CC) -c media_blocchi.cpp -o media_blocchi.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
run :
	./main.exe
clean :
	rm *.o main.exe seed.out
