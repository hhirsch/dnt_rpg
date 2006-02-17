# DccNiTghtmare is public domain. Do whatever you want with this code.
CPP = g++
CFLAGS = -g -Wall -c #-DDEBUG_MAP
CLIBSDIR = -L/usr/X11/lib
CLIBS = -lGLU -lGL -lSDL -lSDL_image
BINDIR = ../../bin/
ENDGUI = ../gui/
DIRENGINE = ../engine/
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)nstring.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o
#DEPENDENCIAS = map.o $(OBJSGUI) mapobjeto.o ../etc/glm.o ../etc/texture.o $(DIRENGINE)culling.o

all: objetos

map.o: map.cpp map.h
	@printf "Making map.o... "
	@$(CPP) $(CFLAGS) map.cpp -o map.o
	@printf "\e[0;32mDone.\e[0;0m\n"

mapfog.o: mapfog.cpp mapfog.h
	@printf "Making mapfog.o... "
	@$(CC) $(CFLAGS) mapfog.cpp -o mapfog.o
	@printf "\e[0;32mDone.\e[0;0m\n"

maplights.o: maplights.cpp maplights.h
	@printf "Making maplights.o... "
	@$(CC) $(CFLAGS) maplights.cpp -o maplights.o
	@printf "\e[0;32mDone.\e[0;0m\n"

mapobjeto.o: mapobjeto.cpp mapobjeto.h
	@printf "Making mapobjeto.o... "
	@$(CC) $(CFLAGS) mapobjeto.cpp -o mapobjeto.o
	@printf "\e[0;32mDone.\e[0;0m\n"

objetos: map.o mapfog.o maplights.o mapobjeto.o 

clean: 
	@printf  "Cleaning Map Bin Files... "
	@\rm -Rf ./*.o $(BINDIR)testeMapa
	@printf "\e[0;32mDone.\e[0;0m\n" 
