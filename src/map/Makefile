# DccNiTghtmare is public domain. Do whatever you want with this code.
CPP = g++
CFLAGS = -g -Wall -c #-DDEBUG_MAP
CLIBSDIR = -L/usr/X11/lib
CLIBS = -lGLU -lGL -lSDL -lSDL_image
BINDIR = ../../bin/
ENDGUI = ../gui/
DIRENGINE = ../engine/
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)nstring.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o
DEPENDENCIAS = map.o $(OBJSGUI) mapobjeto.o ../etc/glm.o ../etc/texture.o $(DIRENGINE)culling.o

all: objetos

map.o: map.cpp map.h
	$(CPP) $(CFLAGS) map.cpp -o map.o

mapobjeto.o: mapobjeto.cpp mapobjeto.h
	$(CC) $(CFLAGS) mapobjeto.cpp -o mapobjeto.o

objetos: $(DEPENDENCIAS)

clean: 
	\rm -Rf ./*.o $(BINDIR)testeMapa 
