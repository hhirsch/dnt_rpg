CC = gcc
CPP = g++
CFLAGS = -g -c -Wall
EFLAGS = -g -Wall -lm -lSDL
OBJS = bartexto.o botao.o cores.o cxsel.o desenho.o erro.o eventos.o farso.o figura.o fonte.o janela.o menu.o mouse.o nstring.o quadtexto.o seltexto.o 
OBJSPP = bartexto.o botao.o cores.o cxsel.o desenho.o erro.o eventos.o farso.o figura.o fonte.o janela.o lista.o menu.o mouse.o nstring.o quadtexto.o seltexto.o
EFLAGSGL = -L/usr/X11R6/lib -lGLU -lGL -lSDL
PROGFLAGs = 
BIN = ../../bin/


# Comente a linha abaixo se quiser movimento transparente das janelas
# POR ENQUANTO, MOVIMENTO TRANSPARENTE NAO FUNCIONA!!!!!
MOVOPACO =  -DMOVIMENTO_OPACO

all: objeto teste fntEditor tp2

teste: $(OBJSPP) teste.o 
	$(CPP) $(EFLAGSGL) $(OBJSPP) teste.o -o $(BIN)teste

tp2: $(OBJSPP) SDLtp2.o
	$(CPP) $(EFLAGSGL) $(OBJSPP) SDLtp2.o -o $(BIN)tp2

fntEditor: $(OBJSPP) fntEditor.o
	$(CPP) $(EFLAGSGL) $(OBJSPP) fntEditor.o -o $(BIN)fntEditor

SDLtp2.o: SDLtp2.cpp
	$(CPP) $(CFLAGS) SDLtp2.cpp -o SDLtp2.o

fntEditor.o: fntEditor.cpp
	$(CPP) $(CFLAGS) fntEditor.cpp -o fntEditor.o

teste.o: teste.cpp
	$(CPP) $(CFLAGS) teste.cpp -o teste.o

bartexto.o: bartexto.h bartexto.cpp
	$(CPP) $(CFLAGS) bartexto.cpp -o bartexto.o

botao.o: botao.h botao.cpp
	$(CPP) $(CFLAGS) botao.cpp -o botao.o

cores.o: cores.h cores.cpp
	$(CPP) $(CFLAGS) cores.cpp -o cores.o

cxsel.o: cxsel.h cxsel.cpp
	$(CPP) $(CFLAGS) cxsel.cpp -o cxsel.o

desenho.o: desenho.h desenho.c
	$(CPP) $(CFLAGS) desenho.c -o desenho.o

erro.o: erro.c erro.h
	$(CPP) $(CFLAGS) erro.c -o erro.o

eventos.o: eventos.h eventos.cpp
	$(CPP) $(CFLAGS) eventos.cpp -o eventos.o

farso.o: farso.h farso.c
	$(CPP) $(CFLAGS) farso.c -o farso.o

figura.o: figura.h figura.cpp
	$(CPP) $(CFLAGS) figura.cpp -o figura.o

fonte.o: fonte.h fonte.c
	$(CPP) $(CFLAGS) fonte.c -o fonte.o

janela.o: janela.h janela.cpp
	$(CPP) $(CFLAGS) $(MOVOPACO) janela.cpp -o janela.o

lista.o: lista.h lista.cpp objeto.h
	$(CPP) $(CFLAGS) lista.cpp -o lista.o

menu.o: menu.h menu.cpp
	$(CPP) $(CFLAGS) menu.cpp -o menu.o

mouse.o: mouse.h mouse.c
	$(CPP) $(CFLAGS) mouse.c -o mouse.o

nstring.o: nstring.h nstring.c
	$(CPP) $(CFLAGS) nstring.c -o nstring.o

quadtexto.o: quadtexto.h quadtexto.cpp
	$(CPP) $(CFLAGS) quadtexto.cpp -o quadtexto.o

seltexto.o: seltexto.h seltexto.cpp
	$(CPP) $(CFLAGS) seltexto.cpp -o seltexto.o

run: teste
	./teste $(PROGFLAGs)

testaObj.o: testaObj.cpp
	$(CPP) $(CFLAGS) testaObj.cpp -o testaObj.o

objeto: $(OBJSPP) testaObj.o 
	$(CPP) $(EFLAGSGL) $(OBJSPP) testaObj.o -o $(BIN)testaObj

clean:
	\rm -rf *.o *.e teste $(BIN)fntEditor $(BIN)testaObj $(BIN)tp2
