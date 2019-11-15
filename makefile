CC=g++
DEPS = building.h myheap.cpp
OBJ = main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

risingCity: $(OBJ)
	$(CC) -o $@ $^