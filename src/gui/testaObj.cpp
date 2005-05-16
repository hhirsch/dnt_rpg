/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "lista.h"
#include "objeto.h"
#include "botao.h"
#include "stdio.h"

int main()
{
   printf("Alocando Objetos\n");
   Tlista *lista = new Tlista;
   botao *obj1;// = new Tbotao;
   botao *obj2;// = new Tbotao;
   printf("Pronto\n");
   printf("Inserindo na Lista\n");
   //obj1->tipo = 1;
   //obj2->tipo = 2;
   //printf("epa\n");
   obj1 = lista->InserirBotao(10,10,30,30,0,0,123,"Botao1",1,NULL);
   obj2 = lista->InserirBotao(20,20,40,60,0,0,0,"Botao2",1,NULL);
   obj1->tipo = 1;
   obj2->tipo = 2;
   printf("Pronto\n");
   printf("Testando Insercao\n");
   printf(" %d  %d\n", lista->primeiro->proximo->proximo->tipo,
                     lista->primeiro->proximo->tipo);

   printf("teste finalizado.\n");
     

   return(0);

}


