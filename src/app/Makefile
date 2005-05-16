#  DccNiTghtmare is public domain. Do whatever you want with this code.
CC = g++
CFLAGS = -g -c -Wall
EFLAGS = -Wall -L/usr/X11R6/lib -lGLU -lGL -lSDL
PROGFLAGS = 
ENDGUI = ../gui/
ENDENGINE = ../engine/
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)nstring.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o
OBJSENGINE = $(ENDENGINE)conversa.o
OBJS = DlgEditor.o
BIN = ../../bin/

all: DlgEditor

DlgEditor.o: DlgEditor.cpp
	$(CC) $(CFLAGS) DlgEditor.cpp -o DlgEditor.o

DlgEditor: $(OBJSGUI) $(OBJSENGINE) $(OBJS)
	$(CC) $(EFLAGS) $(OBJSGUI) $(OBJS) -o $(BIN)DlgEditor

clean:
	\rm -rf *.o *.e $(BIN)DlgEditor

