# DccNiTghtmare is public domain. Do whatever you want with this code.

CC = g++
CFLAGS = -g -c -Wall
EFLAGS = -g -Wall -lm -L/usr/X11R6/lib -lGLU -lGL -lSDL -lSDL_image
PROGFLAGS = 
ENDGUI = ../gui/
ENDENGINE = ../engine/
ENDMAPA = ../map/
OBJSMAPA = $(ENDMAPA)map.o $(ENDMAPA)mapobjeto.o
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)nstring.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o
OBJS = conversa.o dccnit.o ../etc/glm.o personagens.o teste.o
BIN = ../../bin/

all: teste

conversa.o: conversa.cpp conversa.h
	$(CC) $(CFLAGS) conversa.cpp -o conversa.o

dccnit.o: dccnit.h dccnit.cpp
	$(CC) $(CFLAGS) dccnit.cpp -o dccnit.o

personagens.o: personagens.cpp personagens.h
	$(CC) $(CFLAGS) personagens.cpp -o personagens.o

teste.o: teste.cpp
	$(CC) $(CFLAGS) teste.cpp -o teste.o

teste: $(OBJSGUI) $(OBJSMAPA) $(OBJS) 
	$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSMAPA) $(OBJS) -o $(BIN)testeEngine

clean:
	\rm -rf *.o *.e $(BIN)testeEngine

