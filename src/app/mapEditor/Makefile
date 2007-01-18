#  DccNiTghtmare is public domain. Do whatever you want with this code.
CC = g++
CFLAGS = -g -c -Wall -O2
EFLAGS = -Wall -O2 -L/usr/X11R6/lib -lGLU -lGL -lSDL -lSDL_image -lcal3d
PROGFLAGS = 
ENDGUI = ../../gui/
ENDENGINE = ../../engine/
ENDMAPA = ../../map/
ENDETC = ../../etc/
ENDIA = ../../ia/
ENDPARTICLE = ../../particle/
ENDCLASSES = ../../classes/
ENDLANG = ../../lang/
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)healthBar.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)rolbar.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o $(ENDGUI)tabbotao.o
OBJSIA = $(ENDIA)agent.o $(ENDIA)pattAgent.o $(ENDIA)potentAgent.o $(ENDIA)astar.o
OBJSENGINE = $(ENDENGINE)camera.o $(ENDENGINE)conversa.o $(ENDENGINE)collision.o $(ENDENGINE)culling.o $(ENDENGINE)sky.o $(ENDENGINE)sun.o $(ENDENGINE)personagens.o $(ENDENGINE)util.o 
OBJSMAPA = $(ENDETC)glm.o $(ENDMAPA)mapfog.o $(ENDMAPA)maplights.o $(ENDMAPA)mapobjeto.o $(ENDMAPA)map.o $(ENDMAPA)maproad.o
OBJSPARTICLE = $(ENDPARTICLE)particle.o $(ENDPARTICLE)part1.o $(ENDPARTICLE)part2.o $(ENDPARTICLE)part3.o $(ENDPARTICLE)part4.o $(ENDPARTICLE)part5.o $(ENDPARTICLE)part6.o $(ENDPARTICLE)part7.o $(ENDPARTICLE)partSystem.o $(ENDPARTICLE)grass.o
OBJSCLASSES = $(ENDCLASSES)skills.o $(ENDCLASSES)feats.o $(ENDCLASSES)defs.o $(ENDCLASSES)actions.o $(ENDCLASSES)thing.o
OBJSLANG = $(ENDLANG)lang.o
OBJS = area.o editor.o guiIO.o mapEditor.o message.o objects.o particles.o portal.o terrain.o wall.o

BIN = ../../../bin/

all: mapEditor

area.o: area.cpp area.h
	@printf "Making area.o... "
	@$(CC) $(CFLAGS) area.cpp -o area.o
	@printf "\e[0;32mDone.\e[0;0m\n"

editor.o: editor.cpp editor.h
	@printf "Making editor.o... "
	@$(CC) $(CFLAGS) editor.cpp -o editor.o
	@printf "\e[0;32mDone.\e[0;0m\n"

guiIO.o: guiIO.cpp guiIO.h
	@printf "Making guiIO.o... "
	@$(CC) $(CFLAGS) guiIO.cpp -o guiIO.o
	@printf "\e[0;32mDone.\e[0;0m\n"

mapEditor.o: mapEditor.cpp mapEditor.h
	@printf "Making mapEditor.o... "
	@$(CC) $(CFLAGS) mapEditor.cpp -o mapEditor.o
	@printf "\e[0;32mDone.\e[0;0m\n"

message.o: message.cpp message.h
	@printf "Making message.o... "
	@$(CC) $(CFLAGS) message.cpp -o message.o
	@printf "\e[0;32mDone.\e[0;0m\n"

particles.o: particles.cpp particles.h
	@printf "Making particles.o... "
	@$(CC) $(CFLAGS) particles.cpp -o particles.o
	@printf "\e[0;32mDone.\e[0;0m\n"

portal.o: portal.cpp portal.h
	@printf "Making portal.o... "
	@$(CC) $(CFLAGS) portal.cpp -o portal.o
	@printf "\e[0;32mDone.\e[0;0m\n"

objects.o: objects.cpp objects.h
	@printf "Making objects.o... "
	@$(CC) $(CFLAGS) objects.cpp -o objects.o
	@printf "\e[0;32mDone.\e[0;0m\n"

terrain.o: terrain.cpp terrain.h
	@printf "Making terrain.o... "
	@$(CC) $(CFLAGS) terrain.cpp -o terrain.o
	@printf "\e[0;32mDone.\e[0;0m\n"

wall.o: wall.cpp wall.h
	@printf "Making wall.o... "
	@$(CC) $(CFLAGS) wall.cpp -o wall.o
	@printf "\e[0;32mDone.\e[0;0m\n"

mapEditor: $(OBJSGUI) $(OBJSMAPA) $(OBJS)
	@printf "Linking mapEditor... "
	@$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSIA) $(OBJSMAPA) $(OBJSENGINE) $(OBJSPARTICLE) $(OBJSCLASSES) $(OBJSLANG) $(OBJS) -o $(BIN)mapEditor
	@printf "\e[0;32mDone.\e[0;0m\n"

clean:
	@printf "Cleaning mapEditor Bin Files... "
	@\rm -rf *.o *.e $(BIN)mapEditor
	@printf "\e[0;32mDone.\e[0;0m\n"
