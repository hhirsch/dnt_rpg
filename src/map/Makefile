# DccNiTghtmare is public domain. Do whatever you want with this code.
CPP = g++
CFLAGS = -g -Wall -c -DDEBUG_MAP
CLIBSDIR = -L/usr/X11/lib
CLIBS = -lGLU -lGL -lSDL -lSDL_image
BINDIR = ../../bin/
ENDGUI = ../gui/
DIRENGINE = ../engine/
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)nstring.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o
DEPENDENCIAS = map.o teste.o $(OBJSGUI) mapobjeto.o ../etc/glm.o

all: teste

map.o: map.cpp map.h
	$(CPP) $(CFLAGS) map.cpp -o map.o

mapobjeto.o: mapobjeto.cpp mapobjeto.h
	$(CC) $(CFLAGS) mapobjeto.cpp -o mapobjeto.o

teste.o: teste.c
	$(CPP) $(CFLAGS) teste.c -o teste.o

teste: $(DEPENDENCIAS)
	$(CPP) $(CLIBSDIR) $(CLIBS) -g -Wall $(DEPENDENCIAS) -o $(BINDIR)testeMapa

run: teste
	$(BINDIR)testeMapa

clean: 
	\rm -Rf ./*.o $(BINDIR)testeMapa 
