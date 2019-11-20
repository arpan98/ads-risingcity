CC=g++
DEPS = building.h myheap.cpp rbtree.cpp
OBJ = main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

risingCity: $(OBJ)
	$(CC) -o $@ $^