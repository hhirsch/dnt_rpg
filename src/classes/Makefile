# DccNiTghtmare is public domain. Do whatever you want with this code.
CPP = g++
CFLAGS = -g -Wall -c #-DDEBUG_MAP
CLIBSDIR = -L/usr/X11/lib
CLIBS = -lGLU -lGL -lSDL -lSDL_image
BINDIR = ../../bin/
ENDGUI = ../gui/
DIRENGINE = ../engine/

all: objetos

classes.o: classes.cpp classes.h actions.h skills.h feats.h dices.h
	@printf "Making classes.o... "
	@$(CPP) $(CFLAGS) classes.cpp -o classes.o
	@printf "\e[0;32mDone.\e[0;0m\n"

skills.o: skills.cpp skills.h
	@printf "Making skills.o... "
	@$(CC) $(CFLAGS) skills.cpp -o skills.o
	@printf "\e[0;32mDone.\e[0;0m\n"

feats.o: feats.cpp feats.h actions.h defs.h dices.h
	@printf "Making feats.o... "
	@$(CC) $(CFLAGS) feats.cpp -o feats.o
	@printf "\e[0;32mDone.\e[0;0m\n"

teste: feats.o teste.cpp
	g++ -g -Wall -lSDL -lSDL_image teste.cpp feats.o -o teste

objetos: skills.o feats.o 

clean: 
	@printf  "Cleaning Classes Bin Files... "
	@\rm -Rf ./*.o
	@printf "\e[0;32mDone.\e[0;0m\n" 
