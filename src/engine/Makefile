# DccNiTghtmare is public domain. Do whatever you want with this code.

CC = g++
CFLAGS = -g -c -Wall
EFLAGS = -g -Wall -lm -L/usr/X11R6/lib -lGLU -lGL -lSDL -lSDL_image -lcal3d
PROGFLAGS = 
ENDGUI = ../gui/
ENDENGINE = ../engine/
ENDMAPA = ../map/
ENDNET = ../net/
ENDIA = ../ia/
OBJSMAPA = $(ENDMAPA)map.o $(ENDMAPA)mapobjeto.o
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)nstring.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o
OBJSNET = #$(ENDNET)client.o $(ENDNET)common.o
OBJSIA = $(ENDIA)ai.o
OBJS = conversa.o culling.o cursor.o dccnit.o ../etc/glm.o ../etc/texture.o personagens.o teste.o
BIN = ../../bin/
SUPREDE = #-DREDE #comente para nao ter suporte a rede na bagaca
FIFO = ../etc/fifo.o

all: teste

conversa.o: conversa.cpp conversa.h
	$(CC) $(CFLAGS) conversa.cpp -o conversa.o

culling.o: culling.cpp culling.h
	$(CC) $(CFLAGS) culling.cpp -o culling.o

cursor.o: cursor.cpp cursor.h
	$(CC) $(CFLAGS) cursor.cpp -o cursor.o

dccnit.o: dccnit.h dccnit.cpp
	$(CC) $(CFLAGS) $(SUPREDE) dccnit.cpp -o dccnit.o

personagens.o: personagens.cpp personagens.h
	$(CC) $(CFLAGS) personagens.cpp -o personagens.o

teste.o: teste.cpp
	$(CC) $(CFLAGS) $(SUPREDE) teste.cpp -o teste.o

teste: $(OBJSGUI) $(OBJSMAPA) $(OBJSNET) $(OBJSETC) $(OBJS) 
	$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSNET) $(OBJSIA) $(FIFO) $(OBJSMAPA) $(OBJS) $(SUPREDE) -o $(BIN)dccnitghtmare

clean:
	\rm -rf *.o *.e $(BIN)dccnitghtmare

