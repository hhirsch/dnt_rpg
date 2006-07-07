# DccNiTghtmare is public domain. Do whatever you want with this code.
CPP = g++
CFLAGS = -g -Wall -c #-DDEBUG_MAP
CLIBSDIR = -L/usr/X11/lib
CLIBS = -lGLU -lGL -lSDL -lSDL_image
BINDIR = ../../bin/
ENDGUI = ../gui/
DIRENGINE = ../engine/

objetos = fightSystem.o


all: $(objetos)

fightSystem.o: fightSystem.cpp fightSystem.h
	@printf "Making fightSystem.o... "
	@$(CC) $(CFLAGS) fightSystem.cpp -o fightSystem.o
	@printf "\e[0;32mDone.\e[0;0m\n"

clean: 
	@printf  "Cleaning Classes Bin Files... "
	@\rm -Rf ./*.o
	@printf "\e[0;32mDone.\e[0;0m\n"

