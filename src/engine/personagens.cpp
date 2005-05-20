/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "personagens.h"

/*********************************************************************
 *                      Destruidor da Lista                          *
 *********************************************************************/
Lpersonagem::~Lpersonagem()
{
   personagem* per = (personagem*) primeiro->proximo;
   int aux;
   for(aux=0;aux<total;aux++)
   {
      RetirarPersonagem(per,0);
      per = (personagem*) per->proximo;
   }
} 
 
/*********************************************************************
 *                    Insere Personagem na Lista                     *
 *********************************************************************/
personagem* Lpersonagem::InserirPersonagem(int forca,int agilidade,int inteligencia,
                                int esperteza,char* retrato,int x, int y,
                                char *nome, char* arqmodelo, char* dirTexturas)
{
   personagem* novo;
   novo = new(personagem);
   novo->retrato = new(Tlista);
   novo->objetos = new(Tlista);
   novo->x = x;
   novo->y = y;
   novo->tipo = PERSONAGEM;
   novo->forca        = forca;
   novo->agilidade    = agilidade;
   novo->inteligencia = inteligencia;
   novo->esperteza    = esperteza;
   novo->armaAtual = NULL;
   novo->orientacao = 0.0;
   novo->posicaoFrente = 0.0;
   novo->posicaoLadoX = 0.0;
   novo->posicaoLadoZ = 0.0;
   novo->nome = nome;
   novo->retrato->InserirFigura(POSRETX,POSRETY,retrato);
   novo-> modelo3d = glmReadOBJ(arqmodelo,dirTexturas); 
   novo->tipo = PERSONAGEM;
   InserirObj(novo);
   personagemAtivo = novo;
   return(novo);
} 

/*********************************************************************
 *                   Retira Personagem da Lista                      *
 *********************************************************************/
void Lpersonagem::RetirarPersonagem(personagem* persona, int tiraMemoria)
{
   glmDelete(persona->modelo3d);
   delete(persona->retrato);
   delete(persona->objetos);
   if(tiraMemoria)
     Retirar(persona);
}

