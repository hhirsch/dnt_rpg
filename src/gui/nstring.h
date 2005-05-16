/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _nstring_h
#define _nstring_h

#include <stdlib.h>

/* Procedimentos de String, extremamente cambiarrados com o malloc
 * a concertar isto depois. */

/* Coloca um caractere na posicao pos da string
 * st  -> string
 * c   -> caractere
 * pos -> posicao desejada*/
char* string_Colocar(char* st,char c,int pos);

/* Deleta n caracteres a partir da posicao pos da string
 * st  -> string
 * pos -> posicao de deletar
 * n   -> numero de caracteres a deletar*/
char* string_Deletar(char *st,int pos, int n);


#endif

