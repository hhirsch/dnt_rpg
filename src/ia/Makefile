# DccNiTghtmare is public domain. Do whatever you want with this code.

CPP = g++
CFLAGS = -g -c -Wall
EFLAGS = -g -Wall -lm
OBJS = ai.o


all: $(OBJS)

ai.o: ai.cpp ai.h
	@printf "Making ai.o... "
	@$(CPP) $(CFLAGS) ai.cpp -o ai.o
	@printf "\e[0;32mDone.\e[0;0m\n"

teste: ai.cpp ai.h
	@printf "Making & Linking TesteAi... "
	@$(CPP) $(EFLAGS) -DTESTE ai.cpp -o testeai
	@printf "\e[0;32mDone.\e[0;0m\n"

clean:
	@printf "Cleaning AI Bin Files... "
	@\rm -rf $(OBJS) ./testeai
	@printf "\e[0;32mDone.\e[0;0m\n"
