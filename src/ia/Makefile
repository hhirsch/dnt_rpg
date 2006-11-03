# DccNiTghtmare is public domain. Do whatever you want with this code.

CPP = g++
CFLAGS = -g -c -Wall -O2
EFLAGS = -g -Wall -O2 -lm
OBJS = agent.o astar.o pattAgent.o potentAgent.o


all: $(OBJS)

agent.o: agent.cpp agent.h
	@printf "Making agent.o... "
	@$(CPP) $(CFLAGS) agent.cpp -o agent.o
	@printf "\e[0;32mDone.\e[0;0m\n"

astar.o: astar.cpp astar.h
	@printf "Making astar.o... "
	@$(CPP) $(CFLAGS) astar.cpp -o astar.o
	@printf "\e[0;32mDone.\e[0;0m\n"

teste: ai.cpp ai.h
	@printf "Making & Linking TesteAi... "
	@$(CPP) $(EFLAGS) -DTESTE ai.cpp -o testeai
	@printf "\e[0;32mDone.\e[0;0m\n"

pattAgent.o: pattAgent.cpp pattAgent.h
	@printf "Making pattAgent.o... "
	@$(CPP) $(CFLAGS) pattAgent.cpp -o pattAgent.o
	@printf "\e[0;32mDone.\e[0;0m\n"

potentAgent.o: potentAgent.cpp potentAgent.h
	@printf "Making potentAgent.o... "
	@$(CPP) $(CFLAGS) potentAgent.cpp -o potentAgent.o
	@printf "\e[0;32mDone.\e[0;0m\n"


clean:
	@printf "Cleaning AI Bin Files... "
	@\rm -rf $(OBJS) ./testeai
	@printf "\e[0;32mDone.\e[0;0m\n"
