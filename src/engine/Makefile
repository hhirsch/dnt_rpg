CC = g++
CFLAGS = -g -c -Wall
EFLAGS = -g -Wall -lm -L/usr/X11R6/lib -lGLU -lGL -lSDL -lSDL_image
PROGFLAGS = 
ENDGUI = ../GUI/
ENDENGINE = ../ENGINE/
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)nstring.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o
OBJS = conversa.o dccnit.o glm.o personagens.o teste.o
BIN = ../BIN/

all: teste

conversa.o: conversa.cpp conversa.h
	$(CC) $(CFLAGS) conversa.cpp -o conversa.o

dccnit.o: dccnit.h dccnit.cpp
	$(CC) $(CFLAGS) dccnit.cpp -o dccnit.o

glm.o: glm.h glm.c
	$(CC) $(CFLAGS) glm.c -o glm.o

personagens.o: personagens.cpp personagens.h
	$(CC) $(CFLAGS) personagens.cpp -o personagens.o

teste.o: teste.cpp
	$(CC) $(CFLAGS) teste.cpp -o teste.o

teste: $(OBJSGUI) $(OBJS)
	$(CC) $(EFLAGS) $(OBJSGUI) $(OBJS) -o $(BIN)testeEngine

clean:
	\rm -rf *.o *.e $(BIN)testeEngine

