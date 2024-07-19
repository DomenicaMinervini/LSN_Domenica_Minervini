CC = mpicxx
CFLAGS = -Wall --std=c++11

TSP.exe : TSP.o trip.o city.o random.o
	$(CC) random.o city.o trip.o TSP.o -o TSP.exe
TSP.o : TSP.cpp TSP.h
	$(CC) -c TSP.cpp -o TSP.o $(CFLAGS)
trip.o : trip.cpp trip.h
	$(CC) -c trip.cpp -o trip.o $(CFLAGS)
city.o : city.cpp city.h
	$(CC) -c city.cpp -o city.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
run :
	mpiexec -np 10 ./TSP.exe
clean :
	rm *.o TSP.exe seed.out
