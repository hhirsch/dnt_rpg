CC = g++
CFLAGS = -g -c -Wall -O2
OBJS = lang.o

all: $(OBJS)

lang.o: lang.cpp lang.h
	@printf "Making lang.o... "
	@$(CC) $(CFLAGS) lang.cpp -o lang.o
	@printf "\e[0;32mDone.\e[0;0m\n"

clean: 
	@printf "Cleaning Lang Bin Files... "
	@\rm -rf *.o *.e
	@printf "\e[0;32mDone.\e[0;0m\n"
