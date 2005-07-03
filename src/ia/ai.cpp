#include "ai.h"

/* Inicia a lista */
void AI::iniciaListaCampos()
{
	listaCampos.tamanhoLista = 0;   //coloca o item atual no 0;
}

/* Insere Campo */
void AI:: campoInfluencia (double posX, double posZ, int tipo, double raio )
{
        listaCampos.campos[listaCampos.tamanhoLista].x = posX;
        listaCampos.campos[listaCampos.tamanhoLista].z = posZ;
        listaCampos.campos[listaCampos.tamanhoLista].tipo = tipo;
        listaCampos.campos[listaCampos.tamanhoLista].raio = raio;
        listaCampos.tamanhoLista++;
}

/* Move o NPC */    
void AI:: moverNpc (personagem *npc, double posX, double posZ)
{
	if(posX>=0)
          npc->posicaoLadoX = posX;
        else
          npc->posicaoLadoX = 0;
	if(posZ>=0)
          npc->posicaoLadoZ = posZ;
        else
          npc->posicaoLadoZ = 0;
}

/* Normaliza o vetor */
void AI:: normalizarVetor (double * vetor)
{
	double x = vetor[0]*vetor[0];
	double z = vetor[1]*vetor[1];
	double norma = sqrt((x+z));
      
        vetor[0]/= norma;
	vetor[1]/= norma;
	vetor[0]*=PASSO;
	vetor[1]*=PASSO;
}

void AI:: calculaVetorResultante (personagem npc, double * total)
{
      double vetorParcial [2];
      total[0] = 0;
      total[1] = 0;
      int i;

      for(i = 0; i < listaCampos.tamanhoLista; i++){
         vetorParcial[0] = npc.posicaoLadoX - listaCampos.campos[i].x;
         vetorParcial[1] = npc.posicaoLadoZ - listaCampos.campos[i].z;

         /* Se for PC eh atracao, por isso inverte o sinal */
         if(listaCampos.campos[i].tipo == TIPOPC)
         {
             vetorParcial[0] = -1*vetorParcial[0];
             vetorParcial[1] = -1*vetorParcial[1];
         }
            
         total[0]+= vetorParcial[0];
         total[1]+= vetorParcial[1];
      }
}

/* Acha o destino do NPC */  
void AI:: destinoNpc(personagem * npc)
{
	double posX, posZ;
	double total[2];

	calculaVetorResultante (*npc, total);
	normalizarVetor(total);
              
	posX= npc->posicaoLadoX + total[0];
	posZ= npc->posicaoLadoZ + total[1];
              
	moverNpc (npc, posX, posZ);
}


