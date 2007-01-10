/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _fonte_h
#define _fonte_h

#include "desenho.h"
#include <string.h>

#include <string>
using namespace std;

/* Alinhamentos  da Fonte */
#define ESQUERDA         0
#define DIREITA          1
#define CENTRALIZADO     2
#define TEMPOREPETICAO 100

#define FFARSO     "../data/fnt/farso.fnt"
#define FHELVETICA "../data/fnt/Helve.fnt"
#define FMINI      "../data/fnt/mini.fnt"
#define FNORMAL    "../data/fnt/normal.fnt"
#define FNEGRITO   "../data/fnt/Neg.fnt"
#define FPLATINA   "../data/fnt/Plat.fnt"
#define FTIMES     "../data/fnt/times.fnt"

/* Onde e definida a escrita de texto na superficie */
typedef struct
{
   unsigned short int letra[257][16]; /* Cada Letra em HexaDecimal */
   unsigned short int incCP;          /* O Incremento da Fonte */
}fnt;                  /* Fonte do Tipo FNT */

/* Escreve na superficie o texto
 * screen      -> Superficie na qual sera escrito o texto
 * x,y         -> Coordenada de base do Texto
 * texto       -> o texto a ser escrito
 * OPCIONAIS
 * inic        -> posicao inicial
 * fim         -> posicao final
 * x1,y1,x2,y2 -> coordenadas limitrofes */
void escxy(SDL_Surface *screen,int x,int y, const char* texto);
void escxy_Def(SDL_Surface *screen,int x,int y, const char* texto,int inic,int fim);
/*Retorna o y */
int escxy_Area(SDL_Surface *screen,int x, int y, const char* texto,int x1,int y1,
                int x2,int y2);

int fonte_incCP();

/* Define a Fonte Ativa, retorna 0 caso nao encontre o arquivo
 * nome   -> nome do arquivo contendo a fonte
 * alinha -> alinhamento da fonte na tela
 * tam    -> tamanho da fonte na tela */
int selFonte(const char* nome, int alinha, int tam);

/*! Copies lines from source, based on | dividers
 * \param source -> Source String
 * \param firstLine -> first line to be copied
 * \param lastLine -> last line to be copied
 * \return -> copied string */
string copyLines(string source, int firstLine, int lastLine);

/*! Count total lines of the source, based on | dividers
 * \param source -> Source String
 * \return total lines of the source string */
int getTotalLines(string source);

#endif

