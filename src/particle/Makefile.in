CC = g++
CFLAGS = -g -c -Wall -O2
OBJS = grass.o particle.o part1.o part2.o part3.o part4.o part5.o part6.o part7.o partSystem.o wave.o

all: $(OBJS)

grass.o: grass.cpp grass.h
	@printf "Making grass.o... "
	@$(CC) $(CFLAGS) grass.cpp -o grass.o
	@printf "\e[0;32mDone.\e[0;0m\n"

particle.o: particle.cpp particle.h
	@printf "Making particle.o... "
	@$(CC) $(CFLAGS) particle.cpp -o particle.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part1.o: part1.cpp part1.h
	@printf "Making part1.o (Waterfall)... "
	@$(CC) $(CFLAGS) part1.cpp -o part1.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part2.o: part2.cpp part2.h
	@printf "Making part2.o (Fire)... "
	@$(CC) $(CFLAGS) part2.cpp -o part2.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part3.o: part3.cpp part3.h
	@printf "Making part3.o (Water Surface)... "
	@$(CC) $(CFLAGS) part3.cpp -o part3.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part4.o: part4.cpp part4.h
	@printf "Making part4.o (Smoke)... "
	@$(CC) $(CFLAGS) part4.cpp -o part4.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part5.o: part5.cpp part5.h
	@printf "Making part5.o (Blood)... "
	@$(CC) $(CFLAGS) part5.cpp -o part5.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part6.o: part6.cpp part6.h
	@printf "Making part6.o (Lightning)... "
	@$(CC) $(CFLAGS) part6.cpp -o part6.o
	@printf "\e[0;32mDone.\e[0;0m\n"

part7.o: part7.cpp part7.h
	@printf "Making part7.o (Snow)... "
	@$(CC) $(CFLAGS) part7.cpp -o part7.o
	@printf "\e[0;32mDone.\e[0;0m\n"

wave.o: wave.cpp wave.h
	@printf "Making wave.o (Waves)... "
	@$(CC) $(CFLAGS) wave.cpp -o wave.o
	@printf "\e[0;32mDone.\e[0;0m\n"


partSystem.o: partSystem.cpp partSystem.h
	@printf "Making partSystem.o..."
	@$(CC) $(CFLAGS) partSystem.cpp -o partSystem.o
	@printf "\e[0;32mDone.\e[0;0m\n"

clean: 
	@printf "Cleaning Particle Bin Files... "
	@\rm -rf *.o *.e
	@printf "\e[0;32mDone.\e[0;0m\n"
