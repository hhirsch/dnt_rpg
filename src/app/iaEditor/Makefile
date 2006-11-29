#  DccNiTghtmare is public domain. Do whatever you want with this code.
CC = g++
CFLAGS = -g -c -Wall -O2
EFLAGS = -Wall -O2 -L/usr/X11R6/lib -lGLU -lGL -lSDL -lSDL_image #-lcal3d
PROGFLAGS = 
ENDGUI = ../../gui/
ENDENGINE = ../../engine/
ENDMAPA = ../../map/
ENDETC = ../../etc/
ENDPARTICLE = ../../particle/
ENDCLASSES = ../../classes/
ENDLANG = ../../lang/
ENDIA = ../../ia/
OBJSGUI = $(ENDGUI)bartexto.o $(ENDGUI)botao.o $(ENDGUI)cores.o $(ENDGUI)cxsel.o $(ENDGUI)desenho.o $(ENDGUI)erro.o $(ENDGUI)eventos.o $(ENDGUI)farso.o $(ENDGUI)figura.o $(ENDGUI)fonte.o $(ENDGUI)healthBar.o $(ENDGUI)janela.o $(ENDGUI)lista.o $(ENDGUI)menu.o $(ENDGUI)mouse.o $(ENDGUI)quadtexto.o $(ENDGUI)seltexto.o $(ENDGUI)tabbotao.o
OBJSENGINE = $(ENDENGINE)culling.o $(ENDENGINE)sun.o $(ENDENGINE)util.o
OBJSMAPA = $(ENDETC)distance.o $(ENDETC)glm.o $(ENDMAPA)mapfog.o $(ENDMAPA)maplights.o $(ENDMAPA)mapobjeto.o $(ENDMAPA)map.o
OBJSPARTICLE = $(ENDPARTICLE)particle.o $(ENDPARTICLE)part1.o $(ENDPARTICLE)part2.o $(ENDPARTICLE)part3.o $(ENDPARTICLE)part4.o $(ENDPARTICLE)part5.o $(ENDPARTICLE)part6.o $(ENDPARTICLE)part7.o $(ENDPARTICLE)partSystem.o $(ENDPARTICLE)grass.o
OBJSCLASSES = #$(ENDCLASSES)skills.o $(ENDCLASSES)feats.o $(ENDCLASSES)defs.o $(ENDCLASSES)actions.o $(ENDCLASSES)thing.o
OBJSLANG = $(ENDLANG)lang.o
OBJSIA = $(ENDIA)agent.o $(ENDIA)potentAgent.o $(ENDIA)pattAgent.o
OBJS = agents.o briefcase.o editor.o guiIO.o iaEditor.o message.o pf.o politic.o states.o

BIN = ../../../bin/

all: iaEditor

agents.o: agents.cpp agents.h
	@printf "Making agents.o... "
	@$(CC) $(CFLAGS) agents.cpp -o agents.o
	@printf "\e[0;32mDone.\e[0;0m\n"

briefcase.o: briefcase.cpp briefcase.h
	@printf "Making briefcase.o... "
	@$(CC) $(CFLAGS) briefcase.cpp -o briefcase.o
	@printf "\e[0;32mDone.\e[0;0m\n"

editor.o: editor.cpp editor.h
	@printf "Making editor.o... "
	@$(CC) $(CFLAGS) editor.cpp -o editor.o
	@printf "\e[0;32mDone.\e[0;0m\n"

guiIO.o: guiIO.cpp guiIO.h
	@printf "Making guiIO.o... "
	@$(CC) $(CFLAGS) guiIO.cpp -o guiIO.o
	@printf "\e[0;32mDone.\e[0;0m\n"

iaEditor.o: iaEditor.cpp iaEditor.h
	@printf "Making iaEditor.o... "
	@$(CC) $(CFLAGS) iaEditor.cpp -o iaEditor.o
	@printf "\e[0;32mDone.\e[0;0m\n"

message.o: message.cpp message.h
	@printf "Making message.o... "
	@$(CC) $(CFLAGS) message.cpp -o message.o
	@printf "\e[0;32mDone.\e[0;0m\n"

pf.o: pf.cpp pf.h
	@printf "Making pf.o... "
	@$(CC) $(CFLAGS) pf.cpp -o pf.o
	@printf "\e[0;32mDone.\e[0;0m\n"

politic.o: politic.cpp politic.h
	@printf "Making politic.o... "
	@$(CC) $(CFLAGS) politic.cpp -o politic.o
	@printf "\e[0;32mDone.\e[0;0m\n"

states.o: states.cpp states.h
	@printf "Making states.o... "
	@$(CC) $(CFLAGS) states.cpp -o states.o
	@printf "\e[0;32mDone.\e[0;0m\n"


iaEditor: $(OBJSGUI) $(OBJSMAPA) $(OBJS)
	@printf "Linking iaEditor... "
	@$(CC) $(EFLAGS) $(OBJSGUI) $(OBJSMAPA) $(OBJSENGINE) $(OBJSPARTICLE) $(OBJSCLASSES) $(OBJSLANG) $(OBJS) $(OBJSIA) -o $(BIN)iaEditor
	@printf "\e[0;32mDone.\e[0;0m\n"

clean:
	@printf "Cleaning iaEditor Bin Files... "
	@\rm -rf *.o *.e $(BIN)iaEditor
	@printf "\e[0;32mDone.\e[0;0m\n"
