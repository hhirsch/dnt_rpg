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
   /* Abre o modelo3d */
   novo-> modelo3d = glmReadOBJ(arqmodelo,dirTexturas); 
   /* Define os grupos */
     novo->pe_d = _glmFindGroup(novo->modelo3d, "pe_d");
     novo->pe_e = _glmFindGroup(novo->modelo3d, "pe_e");
     novo->perna_d = _glmFindGroup(novo->modelo3d, "perna_d");
     novo->perna_e = _glmFindGroup(novo->modelo3d, "perna_e");
     novo->coxa_d = _glmFindGroup(novo->modelo3d, "coxa_d");
     novo->coxa_e = _glmFindGroup(novo->modelo3d, "coxa_e");
     novo->bacia_d = _glmFindGroup(novo->modelo3d, "bacia_d");
     novo->bacia_e = _glmFindGroup(novo->modelo3d, "bacia_e");
     novo->tronco = _glmFindGroup(novo->modelo3d, "tronco");
     novo->ante_d = _glmFindGroup(novo->modelo3d, "ante_d");
     novo->ante_e = _glmFindGroup(novo->modelo3d, "ante_e");
     novo->braco_d = _glmFindGroup(novo->modelo3d, "braco_d");
     novo->braco_e = _glmFindGroup(novo->modelo3d, "braco_e");
     novo->mao_d = _glmFindGroup(novo->modelo3d, "mao_d");
     novo->mao_e = _glmFindGroup(novo->modelo3d, "mao_e");
     novo->cabeca = _glmFindGroup(novo->modelo3d, "cabeca");
   novo->tipo = PERSONAGEM;
   InserirObj(novo);
   personagemAtivo = novo;
   /*novo->personagemDesenhar = glGenLists(1);
   glNewList(novo->personagemDesenhar,GL_COMPILE);
     glmDraw(novo->modelo3d, GLM_NONE | GLM_COLOR | GLM_SMOOTH | GLM_TEXTURE);
   glEndList();*/
   return(novo);
} 

/*********************************************************************
 *                   Retira Personagem da Lista                      *
 *********************************************************************/
void Lpersonagem::RetirarPersonagem(personagem* persona, int tiraMemoria)
{
   //glDeleteLists(persona->personagemDesenhar,1);
   glmDelete(persona->modelo3d);
   delete(persona->retrato);
   delete(persona->objetos);
   if(tiraMemoria)
     Retirar(persona);
}

