CC = g++
CFLAGS = -g -c -Wall -O2

all: sound.o

sound.o: sound.cpp sound.h
	@printf "Making sound.o... "
	@$(CC) $(CFLAGS) sound.cpp -o sound.o
	@printf "\e[0;32mDone.\e[0;0m\n"

clean: 
	@printf "Cleaning Sound Bin Files... "
	@\rm -rf *.o *.e
	@printf "\e[0;32mDone.\e[0;0m\n"
