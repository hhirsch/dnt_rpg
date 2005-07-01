# DccNiTghtmare is public domain. Do whatever you want with this code.

CPP = g++
CFLAGS = -g -c -Wall
EFLAGS = -g -Wall -lm
OBJS = ai.o


all: $(OBJS)

ai.o: ai.cpp ai.h
	$(CPP) $(CFLAGS) ai.cpp -o ai.o

teste: ai.cpp ai.h
	$(CPP) $(EFLAGS) -DTESTE ai.cpp -o testeai

clean:
	\rm -rf $(OBJS) ./testeai
