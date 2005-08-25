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
OBJSMAPA = $(ENDETC)glm.o $(ENDMAPA)mapobjeto.o $(ENDMAPA)map.o $(ENDENGINE)culling.o
OBJS = DlgEditor.o
BIN = ../../bin/

all: DlgEditor MapEditor

MapEditor.o: MapEditor.cpp
	$(CC) $(CFLAGS) MapEditor.cpp -o MapEditor.o

MapEditor: $(OBJSGUI) $(OBJSMAPA) MapEditor.o
	$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSMAPA) MapEditor.o -o $(BIN)MapEditor

DlgEditor.o: DlgEditor.cpp
	$(CC) $(CFLAGS) DlgEditor.cpp -o DlgEditor.o

DlgEditor: $(OBJSGUI) $(OBJSENGINE) $(OBJS)
	$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSENGINE) $(OBJS) -o $(BIN)DlgEditor

clean:
	\rm -rf *.o *.e $(BIN)DlgEditor

