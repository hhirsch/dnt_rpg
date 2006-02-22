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
OBJSMAPA = $(ENDMAPA)map.o $(ENDMAPA)mapfog.o $(ENDMAPA)maplights.o $(ENDMAPA)mapobjeto.o
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)nstring.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o
OBJSNET = #$(ENDNET)client.o $(ENDNET)common.o
OBJSIA = $(ENDIA)ai.o
OBJS = conversa.o culling.o cursor.o dccnit.o ../etc/glm.o initial.o ../etc/texture.o personagens.o teste.o
BIN = ../../bin/
SUPREDE = #-DREDE #comente para nao ter suporte a rede na bagaca
FIFO = ../etc/fifo.o

all: teste

conversa.o: conversa.cpp conversa.h
	@printf "Making conversa.o... "
	@$(CC) $(CFLAGS) conversa.cpp -o conversa.o
	@printf "\e[0;32mDone.\e[0;0m\n"

culling.o: culling.cpp culling.h
	@printf "Making culling.o... "
	@$(CC) $(CFLAGS) culling.cpp -o culling.o
	@printf "\e[0;32mDone.\e[0;0m\n"

cursor.o: cursor.cpp cursor.h
	@printf "Making cursor.o... "
	@$(CC) $(CFLAGS) cursor.cpp -o cursor.o
	@printf "\e[0;32mDone.\e[0;0m\n"

dccnit.o: dccnit.h dccnit.cpp
	@printf "Making dccnit.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) dccnit.cpp -o dccnit.o
	@printf "\e[0;32mDone.\e[0;0m\n"

initial.o: initial.h initial.cpp
	@printf "Making initial.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) initial.cpp -o initial.o
	@printf "\e[0;32mDone.\e[0;0m\n"

personagens.o: personagens.cpp personagens.h
	@printf "Making personagens.o... "
	@$(CC) $(CFLAGS) personagens.cpp -o personagens.o
	@printf "\e[0;32mDone.\e[0;0m\n"

teste.o: teste.cpp
	@printf "Making teste.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) teste.cpp -o teste.o
	@printf "\e[0;32mDone.\e[0;0m\n"

teste: $(OBJSGUI) $(OBJSMAPA) $(OBJSNET) $(OBJSETC) $(OBJS) 
	@printf "Linking dccnitghtmare... "
	@$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSNET) $(OBJSIA) $(FIFO) $(OBJSMAPA) $(OBJS) $(SUPREDE) -o $(BIN)dccnitghtmare
	@printf "\e[0;32mDone.\e[0;0m\n"

clean:
	@printf "Cleaning Engine Bin Files... "
	@\rm -rf *.o *.e $(BIN)dccnitghtmare
	@printf "\e[0;32mDone.\e[0;0m\n"
