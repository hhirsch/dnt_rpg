# DccNiTghtmare is public domain. Do whatever you want with this code.

CC = g++
CFLAGS = -g -c -Wall -O2
EFLAGS = -g -Wall -O2 -lm -lGLU -lGL -lSDL -lSDL_image -lcal3d -logg -lvorbis -lvorbisfile -lopenal
PROGFLAGS = 
ENDGUI = ../gui/
ENDENGINE = ../engine/
ENDMAPA = ../map/
ENDNET = ../net/
ENDIA = ../ia/
ENDCLASSES = ../classes/
ENDSOUND = ../sound/
ENDLANG = ../lang/
ENDPART = ../particle/
ENDFIGHT = ../fight/
OBJSMAPA = $(ENDMAPA)map.o $(ENDMAPA)mapfog.o $(ENDMAPA)maplights.o $(ENDMAPA)mapobjeto.o
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)messages.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)healthBar.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o $(ENDGUI)tabbotao.o
OBJSNET = #$(ENDNET)client.o $(ENDNET)common.o
OBJSIA = $(ENDIA)agent.o $(ENDIA)pattAgent.o $(ENDIA)potentAgent.o $(ENDIA)astar.o $(ENDIA)fuzzy.o $(ENDIA)fuzzyvariable.o $(ENDIA)fuzzyfunction.o $(ENDIA)fuzzyrule.o
OBJSCLASSES = $(ENDCLASSES)race.o $(ENDCLASSES)skills.o $(ENDCLASSES)align.o $(ENDCLASSES)feats.o $(ENDCLASSES)defs.o $(ENDCLASSES)actions.o $(ENDCLASSES)thing.o
OBJSSOUND = $(ENDSOUND)ogg_stream.o  $(ENDSOUND)sound.o $(ENDSOUND)sndfx.o
OBJSLANG = $(ENDLANG)lang.o
OBJSPART = $(ENDPART)part1.o $(ENDPART)part2.o $(ENDPART)part3.o $(ENDPART)part4.o $(ENDPART)part5.o $(ENDPART)part6.o $(ENDPART)part7.o $(ENDPART)particle.o $(ENDPART)partSystem.o $(ENDPART)wave.o $(ENDPART)grass.o
OBJSFIGHT = $(ENDFIGHT)fightGroup.o $(ENDFIGHT)initiative.o $(ENDFIGHT)fightSystem.o
OBJS = alignwindow.o attwindow.o camera.o collision.o conversa.o culling.o cursor.o dccnit.o ../etc/glm.o initial.o options.o ../etc/texture.o personagens.o racewindow.o skillwindow.o sun.o teste.o util.o
BIN = ../../bin/
SUPREDE = #-DVIDEO_MODE #-DREDE #comente para nao ter suporte a rede na bagaca
FIFO = ../etc/fifo.o

all: teste

alignwindow.o: alignwindow.h alignwindow.cpp
	@printf "Making alignwindow.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) alignwindow.cpp -o alignwindow.o
	@printf "\e[0;32mDone.\e[0;0m\n"

attwindow.o: attwindow.h attwindow.cpp
	@printf "Making attwindow.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) attwindow.cpp -o attwindow.o
	@printf "\e[0;32mDone.\e[0;0m\n"

camera.o: camera.cpp camera.h
	@printf "Making camera.o... "
	@$(CC) $(CFLAGS) camera.cpp -o camera.o
	@printf "\e[0;32mDone.\e[0;0m\n"

conversa.o: conversa.cpp conversa.h
	@printf "Making conversa.o... "
	@$(CC) $(CFLAGS) conversa.cpp -o conversa.o
	@printf "\e[0;32mDone.\e[0;0m\n"

collision.o: collision.cpp collision.h
	@printf "Making collision.o... "
	@$(CC) $(CFLAGS) collision.cpp -o collision.o
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

options.o: options.h options.cpp
	@printf "Making options.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) options.cpp -o options.o
	@printf "\e[0;32mDone.\e[0;0m\n"

personagens.o: personagens.cpp personagens.h
	@printf "Making personagens.o... "
	@$(CC) $(CFLAGS) personagens.cpp -o personagens.o
	@printf "\e[0;32mDone.\e[0;0m\n"

racewindow.o: racewindow.h racewindow.cpp
	@printf "Making racewindow.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) racewindow.cpp -o racewindow.o
	@printf "\e[0;32mDone.\e[0;0m\n"

skillwindow.o: skillwindow.h skillwindow.cpp
	@printf "Making skillwindow.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) skillwindow.cpp -o skillwindow.o
	@printf "\e[0;32mDone.\e[0;0m\n"

sun.o: sun.h sun.cpp
	@printf "Making sun.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) sun.cpp -o sun.o
	@printf "\e[0;32mDone.\e[0;0m\n"

teste.o: teste.cpp
	@printf "Making teste.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) teste.cpp -o teste.o
	@printf "\e[0;32mDone.\e[0;0m\n"

util.o: util.cpp util.h
	@printf "Making util.o... "
	@$(CC) $(CFLAGS) $(SUPREDE) util.cpp -o util.o
	@printf "\e[0;32mDone.\e[0;0m\n"

teste: $(OBJSGUI) $(OBJSMAPA) $(OBJSNET) $(OBJSETC) $(OBJSCLASSES) $(OBJSLANG) $(OBJSSOUND) $(OBJSPART) $(OBJSFIGHT) $(OBJS) 
	@printf "Linking dccnitghtmare... "
	@$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSCLASSES) $(OBJSLANG) $(OBJSNET) $(OBJSIA) $(FIFO) $(OBJSMAPA) $(OBJSSOUND) $(OBJSPART) $(OBJSFIGHT) $(OBJS) $(SUPREDE) -o $(BIN)dccnitghtmare
	@printf "\e[0;32mDone.\e[0;0m\n"

clean:
	@printf "Cleaning Engine Bin Files... "
	@\rm -rf *.o *.e $(BIN)dccnitghtmare
	@printf "\e[0;32mDone.\e[0;0m\n"
