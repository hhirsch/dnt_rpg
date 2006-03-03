CC = g++
CFLAGS = -g -c -Wall

all: sound.o

sound.o: sound.cpp sound.h
	@printf "Making sound.o... "
	@$(CC) $(CFLAGS) sound.cpp -o sound.o
	@printf "\e[0;32mDone.\e[0;0m\n"
