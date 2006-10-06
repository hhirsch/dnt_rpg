#  DccNiTghtmare is public domain. Do whatever you want with this code.
CC = g++
CFLAGS = -g -c -Wall -O2
EFLAGS = -Wall -O2 -L/usr/X11R6/lib -lGLU -lGL -lSDL -lSDL_image
PROGFLAGS = 
ENDGUI = ../gui/
ENDENGINE = ../engine/
ENDMAPA = ../map/
ENDETC = ../etc/
ENDPARTICLE = ../particle/
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o $(ENDGUI)tabbotao.o
OBJSENGINE = $(ENDENGINE)conversa.o $(ENDENGINE)culling.o
OBJSMAPA = $(ENDETC)glm.o $(ENDMAPA)mapfog.o $(ENDMAPA)maplights.o $(ENDMAPA)mapobjeto.o $(ENDMAPA)map.o $(ENDENGINE)culling.o
OBJSPARTICLE = $(ENDPARTICLE)particle.o $(ENDPARTICLE)part1.o $(ENDPARTICLE)part2.o $(ENDPARTICLE)part3.o $(ENDPARTICLE)part4.o $(ENDPARTICLE)part5.o $(ENDPARTICLE)part6.o $(ENDPARTICLE)part7.o
SUBSYSTEMS = iaEditor/ mapEditor/

OBJS = DlgEditor.o
BIN = ../../bin/

all: DlgEditor fntEditor dlgCompiler partEditor iaEditor MapEditor

fntEditor.o: fntEditor.cpp
	@printf "Making fntEditor.o... "
	@$(CC) $(CFLAGS) fntEditor.cpp -o fntEditor.o
	@printf "\e[0;32mDone.\e[0;0m\n"

fntEditor: $(OBJSGUI) $(OBJSENGINE) fntEditor.o
	@printf "Linking fntEditor... "
	@$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSENGINE) fntEditor.o -o $(BIN)fntEditor
	@printf "\e[0;32mDone.\e[0;0m\n"

MapEditor.o: MapEditor.cpp
	@printf "Making MapEditor.o... "
	@$(CC) $(CFLAGS) MapEditor.cpp -o MapEditor.o
	@printf "\e[0;32mDone.\e[0;0m\n"

iaEditor: 
	@cd ./iaEditor; make all;

MapEditor: 
	@cd ./mapEditor; make all;
#$(OBJSGUI) $(OBJSMAPA) MapEditor.o
#	@printf "Linking MapEditor... "
#	@$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSMAPA) MapEditor.o -o $(BIN)MapEditor
#	@printf "\e[0;32mDone.\e[0;0m\n"

DlgEditor.o: DlgEditor.cpp
	@printf "Making DlgEditor.o... "
	@$(CC) $(CFLAGS) DlgEditor.cpp -o DlgEditor.o
	@printf "\e[0;32mDone.\e[0;0m\n"

DlgEditor: $(OBJSGUI) $(OBJSENGINE) $(OBJS) 
	@printf "Linking DlgEditor... "
	@$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSENGINE) $(OBJS) -o $(BIN)DlgEditor
	@printf "\e[0;32mDone.\e[0;0m\n"

dlgCompiler.o: dlgCompiler.cpp
	@printf "Making dlgCompiler.o... "
	@$(CC) $(CFLAGS) dlgCompiler.cpp -o dlgCompiler.o
	@printf "\e[0;32mDone.\e[0;0m\n"

dlgCompiler: $(OBJSGUI) $(OBJSENGINE) dlgCompiler.o
	@printf "Linking dlgCompiler... "
	@$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSENGINE) dlgCompiler.o -o $(BIN)dlgCompiler
	@printf "\e[0;32mDone.\e[0;0m\n"


partEditor.o: partEditor.cpp
	@printf "Making partEditor.o... "
	@$(CC) $(CFLAGS) partEditor.cpp -o partEditor.o
	@printf "\e[0;32mDone.\e[0;0m\n"

partEditor: $(OBJSGUI) $(OBJSENGINE) partEditor.o
	@printf "Linking partEditor... "
	@$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSENGINE) $(OBJSPARTICLE) partEditor.o -o $(BIN)partEditor
	@printf "\e[0;32mDone.\e[0;0m\n"

clean:
	@printf "Cleaning App Bin Files... "
	@\rm -rf *.o *.e ./mapEditor/*.o ./iaEditor/*.o $(BIN)DlgEditor $(BIN)mapEditor $(BIN)fntEditor $(BIN)partEditor $(BIN)iaEditor
	@printf "\e[0;32mDone.\e[0;0m\n"
