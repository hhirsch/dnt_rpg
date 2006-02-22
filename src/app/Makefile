#  DccNiTghtmare is public domain. Do whatever you want with this code.
CC = g++
CFLAGS = -g -c -Wall
EFLAGS = -Wall -L/usr/X11R6/lib -lGLU -lGL -lSDL -lSDL_image
PROGFLAGS = 
ENDGUI = ../gui/
ENDENGINE = ../engine/
ENDMAPA = ../map/
ENDETC = ../etc/
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)nstring.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o
OBJSENGINE = $(ENDENGINE)conversa.o $(ENDENGINE)culling.o
OBJSMAPA = $(ENDETC)glm.o $(ENDMAPA)mapfog.o $(ENDMAPA)maplights.o $(ENDMAPA)mapobjeto.o $(ENDMAPA)map.o $(ENDENGINE)culling.o
OBJS = DlgEditor.o
BIN = ../../bin/

all: DlgEditor MapEditor dlgCompiler

MapEditor.o: MapEditor.cpp
	@printf "Making MapEditor.o... "
	@$(CC) $(CFLAGS) MapEditor.cpp -o MapEditor.o
	@printf "\e[0;32mDone.\e[0;0m\n"

MapEditor: $(OBJSGUI) $(OBJSMAPA) MapEditor.o
	@printf "Linking MapEditor... "
	@$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSMAPA) MapEditor.o -o $(BIN)MapEditor
	@printf "\e[0;32mDone.\e[0;0m\n"

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

clean:
	@printf "Cleaning App Bin Files... "
	@\rm -rf *.o *.e $(BIN)DlgEditor $(BIN)MapEditor
	@printf "\e[0;32mDone.\e[0;0m\n"
