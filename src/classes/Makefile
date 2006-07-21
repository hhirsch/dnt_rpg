# DccNiTghtmare is public domain. Do whatever you want with this code.
CPP = g++
CFLAGS = -g -Wall -c -O2 #-DDEBUG_MAP
CLIBSDIR = -L/usr/X11/lib
CLIBS = -L/usr/X11R6/lib -lGLU -lGL -lSDL -lSDL_image -lcal3d
BINDIR = ../../bin/
ENDGUI = ../gui/
ENDENGINE = ../engine/
ENDFIGHT = ../fight/
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o $(ENDGUI)tabbotao.o
OBJSFIGHT = $(ENDFIGHT)fightGroup.o $(ENDFIGHT)initiative.o $(ENDFIGHT)fightSystem.o $(ENDENGINE)personagens.o $(OBJSGUI)
objetos = skills.o feats.o defs.o actions.o thing.o


all: $(objetos)

classes.o: classes.cpp classes.h actions.h skills.h feats.h dices.h defs.h
	@printf "Making classes.o... "
	@$(CPP) $(CFLAGS) classes.cpp -o classes.o
	@printf "\e[0;32mDone.\e[0;0m\n"

defs.o: defs.h defs.cpp
	@printf "Making defs.o... "
	@$(CPP) $(CFLAGS) defs.cpp -o defs.o
	@printf "\e[0;32mDone.\e[0;0m\n"

skills.o: skills.cpp skills.h
	@printf "Making skills.o... "
	@$(CC) $(CFLAGS) skills.cpp -o skills.o
	@printf "\e[0;32mDone.\e[0;0m\n"

feats.o: feats.cpp feats.h actions.h defs.h dices.h
	@printf "Making feats.o... "
	@$(CC) $(CFLAGS) feats.cpp -o feats.o
	@printf "\e[0;32mDone.\e[0;0m\n"

actions.o: actions.cpp actions.h
	@printf "Making actions.o... "
	@$(CC) $(CFLAGS) actions.cpp -o actions.o
	@printf "\e[0;32mDone.\e[0;0m\n"

thing.o: thing.cpp thing.h
	@printf "Making thing.o... "
	@$(CC) $(CFLAGS) thing.cpp -o thing.o
	@printf "\e[0;32mDone.\e[0;0m\n"

teste: $(objetos) $(OBJSFIGHT) teste.cpp
	g++ -g -Wall $(CLIBS) teste.cpp $(objetos) $(OBJSFIGHT) ../gui/healthBar.o ../lang/lang.o -o teste

clean: 
	@printf  "Cleaning Classes Bin Files... "
	@\rm -Rf ./*.o
	@printf "\e[0;32mDone.\e[0;0m\n" 
