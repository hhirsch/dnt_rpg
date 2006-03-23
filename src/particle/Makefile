CC = g++
CFLAGS = -g -c -Wall
OBJS = particle.o part1.o part2.o part3.o part4.o

all: $(OBJS)

particle.o: particle.cpp particle.h
	@printf "Making particle.o... "
	@$(CC) $(CFLAGS) particle.cpp -o particle.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part1.o: part1.cpp part1.h
	@printf "Making part1.o... "
	@$(CC) $(CFLAGS) part1.cpp -o part1.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part2.o: part2.cpp part2.h
	@printf "Making part2.o... "
	@$(CC) $(CFLAGS) part2.cpp -o part2.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part3.o: part3.cpp part3.h
	@printf "Making part3.o... "
	@$(CC) $(CFLAGS) part3.cpp -o part3.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part4.o: part4.cpp part4.h
	@printf "Making part4.o... "
	@$(CC) $(CFLAGS) part4.cpp -o part4.o
	@printf "\e[0;32mDone.\e[0;0m\n"

clean: 
	@printf "Cleaning Particle Bin Files... "
	@\rm -rf *.o *.e
	@printf "\e[0;32mDone.\e[0;0m\n"
