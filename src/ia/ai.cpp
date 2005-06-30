#include "ai.h"

int main()
{
	double total[2];
	personagem npc;
	AI npc_ai;	
	int i;

	npc.posicaoLadoX = 0;
	npc.posicaoLadoZ = 0;

	for (i = 0 ; i < 5 ; i++)
	{
		npc_ai.calculaVetorResultante(npc, total);
		npc_ai.destinoNpc(&npc);
	}

	return(EXIT_SUCCESS);
}

void AI:: campoInfluencia (double posY, double posZ, item *itens)
{
	item item2;
	item2.x = 10;
	item2.z = 10;
	item2.tipo = TIPOOBSTACULO;
	item2.prox = NULL;
	itens->prox = &item2; 
}
    
void AI:: moverNpc (personagem *npc, double posX, double posZ)
{
	printf("Posicao do elemento:%.3lf,%.3lf\n", posX, posZ);
}

 void AI:: calculaCampo(double posXP, double posZP,double posXO, double posZO,
                        int tipoCampo, int brutalidade, double * retorno)
 {
     double quadradoDifX= ((posXP - posXO)*(posXP - posXO));
     double quadradoDifZ= ((posZP - posZO)*(posZP - posZO));
     
     if (tipoCampo== TIPOOBSTACULO || tipoCampo== TIPONPC)
     {
         /* o campo nestes casos varia com d a quarta */
         quadradoDifX*= quadradoDifX; 
         quadradoDifZ*= quadradoDifZ;
     }                               
     
     if (quadradoDifX) 
       retorno[0]= (tipoCampo * brutalidade) /quadradoDifX;
     else retorno[0]=0;  
     
     if (quadradoDifZ)
       retorno[1]= (tipoCampo * brutalidade) /quadradoDifZ;
     else retorno[1]=0;
  }               
        
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
      item itens; //nodo cabeca da lista
      item * aux;
      item * desalocar;
      double vetorParcial [2];
      
      campoInfluencia (npc.posicaoLadoX,npc.posicaoLadoZ, &itens); 
      aux= itens.prox; 
      int x;
      while(x != 1)
      {
        calculaCampo(npc.posicaoLadoX, npc.posicaoLadoZ,aux->x, aux->z,
                     aux->tipo, npc.brutalidade, vetorParcial);
                     
         total[0]+= vetorParcial[0];
         total[1]+= vetorParcial[1];
			if (aux->prox == NULL)
				break;
	      aux= aux->prox;
			x = 1;
      }
      /* nada de memory leak :-) */
      aux= itens.prox;
      while (x!=1)
      {
          desalocar= aux->prox;
          free(aux);
          aux= desalocar;
			 x = 1;
      }                  
  }
  
/*              
// Necessário na Função do cesão de múltiplos NPCs
int AI:: deveMover (int indice)
{
	return !(--(velocidade[indice]));
}      
*/   

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

 /*

// Função do Cesão pra múltiplos NPCs
 
  void AI:: destinoNpcs(Lpersonagem * npc)
  {
      static int contagemAnterior=0;
      
      int contagem=0;
      double posX, posZ;
      personagem * atual;
      double total[2];
      
      // Incialmente descobre qtos npcs ha... para aumentar o
      // dinamismo e facilitar a insercao de npcs durante o jogo 
      atual= npc->primeiro->proximo;
      
      while (atual)
      {
          atual= atual->proximo;
          contagem++;
      }
      if (contagemAnterior!= contagem)
      {
          if (!velocidades) //primeira chamada da funcao
             velocidades= (int *) malloc (sizeof(int)*contagem);
          else velocidades= (int *) realloc (sizeof(int)* contagem);
          contagemAnterior= contagem;
          
          atual= npc->primeiro->proximo;
          contagem=0;
          
          while (atual) //atualizara as velocidades
          {
            velocidades[contagem++]= (atual->agilidade)* COVELOCIDADE;
            atual= atual->proximo;
          }      
            
      }
      atual= npc->primeiro->proximo;
      
      //Inicio do calculo do movimento em si... 
      
      for (contagem=0; contagem < contagemAnterior; contagem++)
      {
          // Importante notar que caso mais de um npc se movam no mesmo
          // ciclo, o que vier primeiro na fila npc se move primeiro.
          // E a forca sobre os proximos a se moverem e' calculada baseando-se
          // na nova posicao do primeiro npc, ou de qualquer npc que se mova e
          // preceda o npc cuja resultante esta sendo calculada.          
          if (deveMover(contagem))
          {
              calculaVetorResultante (*atual, &total);
              normalizarVetor(&total);
              
              posXP= atual->posicaoLadoX + total[0];
              posZP= atual->posicaoLadoZ + total[1];
              
              moverNpc (atual, posXP, posZP);
              velocidades[contagem]= (atual->agilidade)* COVELOCIDADE;
          }
          atual= atual->proximo;                 
              
      }           
            
  }          
               
        
} 
*/
