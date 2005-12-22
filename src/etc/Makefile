# DccNiTghtmare is public domain. Do whatever you want with this code.

CPP = g++
CC = gcc
CFLAGS = -g -Wall

all: glm.o fifo.o texture.o

texture.o: texture.h texture.cpp
	@printf "Making texture.o... "
	@$(CPP) $(CFLAGS) -c texture.cpp -o texture.o
	@printf "\e[0;32mDone.\e[0;0m\n"

glm.o: glm.h glm.c
	@printf "Making glm.o... "
	@$(CPP) $(CFLAGS) -c glm.c -o glm.o
	@printf "\e[0;32mDone.\e[0;0m\n"

fifo.o: fifo.c fifo.h
	@printf "Making fifo.o... "
	@$(CPP) $(CFLAGS) -c fifo.c -o fifo.o
	@printf "\e[0;32mDone.\e[0;0m\n"

fifo: fifo.c fifo.h
	$(CPP) $(CFLAGS) -DFIFO_TEST fifo.c -o fifo

clean:
	@printf "Cleaning Etc Bin Files... "
	@\rm -rf *.o fifo
	@printf "\e[0;32mDone.\e[0;0m\n"
