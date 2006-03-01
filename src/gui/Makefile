#  DccNiTghtmare is public domain. Do whatever you want with this code.

CC = gcc
CPP = g++
CFLAGS = -g -c -Wall
EFLAGS = -g -Wall -lm -lSDL -lSDL_image
OBJS = bartexto.o botao.o cores.o cxsel.o desenho.o erro.o eventos.o farso.o figura.o fonte.o janela.o menu.o mouse.o quadtexto.o seltexto.o 
OBJSPP = bartexto.o botao.o cores.o cxsel.o desenho.o erro.o eventos.o farso.o figura.o fonte.o janela.o lista.o menu.o mouse.o quadtexto.o seltexto.o
EFLAGSGL = -L/usr/X11R6/lib -lGLU -lGL -lSDL -lSDL_image
PROGFLAGs = 
BIN = ../../bin/


# Comente a linha abaixo se quiser movimento transparente das janelas
# POR ENQUANTO, MOVIMENTO TRANSPARENTE NAO FUNCIONA!!!!! (e provavelmente, nunca funcionara!!!)
MOVOPACO =  -DMOVIMENTO_OPACO

all: objeto  

bartexto.o: bartexto.h bartexto.cpp
	@printf "Making bartexto.o... "
	@$(CPP) $(CFLAGS) bartexto.cpp -o bartexto.o
	@printf "\e[0;32mDone.\e[0;0m\n"

botao.o: botao.h botao.cpp
	@printf "Making botao.o... "
	@$(CPP) $(CFLAGS) botao.cpp -o botao.o
	@printf "\e[0;32mDone.\e[0;0m\n"

cores.o: cores.h cores.cpp
	@printf "Making cores.o... "
	@$(CPP) $(CFLAGS) cores.cpp -o cores.o
	@printf "\e[0;32mDone.\e[0;0m\n"

cxsel.o: cxsel.h cxsel.cpp
	@printf "Making cxsel.o... "
	@$(CPP) $(CFLAGS) cxsel.cpp -o cxsel.o
	@printf "\e[0;32mDone.\e[0;0m\n"

desenho.o: desenho.h desenho.c
	@printf "Making desenho.o... "
	@$(CPP) $(CFLAGS) desenho.c -o desenho.o
	@printf "\e[0;32mDone.\e[0;0m\n"

erro.o: erro.c erro.h
	@printf "Making erro.o... "
	@$(CPP) $(CFLAGS) erro.c -o erro.o
	@printf "\e[0;32mDone.\e[0;0m\n"

eventos.o: eventos.h eventos.cpp
	@printf "Making eventos.o... "
	@$(CPP) $(CFLAGS) eventos.cpp -o eventos.o
	@printf "\e[0;32mDone.\e[0;0m\n"

farso.o: farso.h farso.c
	@printf "Making farso.o... "
	@$(CPP) $(CFLAGS) farso.c -o farso.o
	@printf "\e[0;32mDone.\e[0;0m\n"

figura.o: figura.h figura.cpp
	@printf "Making figura.o... "
	@$(CPP) $(CFLAGS) figura.cpp -o figura.o
	@printf "\e[0;32mDone.\e[0;0m\n"

fonte.o: fonte.h fonte.c
	@printf "Making fonte.o... "
	@$(CPP) $(CFLAGS) fonte.c -o fonte.o
	@printf "\e[0;32mDone.\e[0;0m\n"

janela.o: janela.h janela.cpp
	@printf "Making janela.o... "
	@$(CPP) $(CFLAGS) $(MOVOPACO) janela.cpp -o janela.o
	@printf "\e[0;32mDone.\e[0;0m\n"

lista.o: lista.h lista.cpp objeto.h
	@printf "Making lista.o... "
	@$(CPP) $(CFLAGS) lista.cpp -o lista.o
	@printf "\e[0;32mDone.\e[0;0m\n"

menu.o: menu.h menu.cpp
	@printf "Making menu.o... "
	@$(CPP) $(CFLAGS) menu.cpp -o menu.o
	@printf "\e[0;32mDone.\e[0;0m\n"

mouse.o: mouse.h mouse.c
	@printf "Making mouse.o... "
	@$(CPP) $(CFLAGS) mouse.c -o mouse.o
	@printf "\e[0;32mDone.\e[0;0m\n"

quadtexto.o: quadtexto.h quadtexto.cpp
	@printf "Making quadtexto.o... "
	@$(CPP) $(CFLAGS) quadtexto.cpp -o quadtexto.o
	@printf "\e[0;32mDone.\e[0;0m\n"

seltexto.o: seltexto.h seltexto.cpp
	@printf "Making seltexto.o... "
	@$(CPP) $(CFLAGS) seltexto.cpp -o seltexto.o
	@printf "\e[0;32mDone.\e[0;0m\n"

objeto: $(OBJSPP)

clean:
	@printf "Cleaning Gui Bin Files... "
	@\rm -rf *.o *.e $(BIN)teste $(BIN)fntEditor $(BIN)testaObj $(BIN)tp2
	@printf "\e[0;32mDone.\e[0;0m\n"
