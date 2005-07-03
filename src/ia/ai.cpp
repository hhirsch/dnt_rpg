#include "ai.h"

Litem listaCampos;

#ifdef TESTE	
int main()
{
	
	double total[2];
	personagem npc;
	AI npc_ai;
	int i;


	npc.posicaoLadoX = 2;
	npc.posicaoLadoZ = 3;
	npc_ai.iniciaListaCampos();

	for (i = 0 ; i < 5 ; i++)
	{
		npc_ai.calculaVetorResultante(npc, total);
		npc_ai.destinoNpc(&npc);
	}

	return(EXIT_SUCCESS);
}

#endif

void AI::iniciaListaCampos()
{
	int i;

	listaCampos.tamanhoLista = 0;   //coloca o item atual no 0;

	/*for (i = 0; i < TLISTACAMPOS; i++){
		listaCampos.campos[i].x = 0;
		listaCampos.campos[i].z = 0;
		listaCampos.campos[i].tipo = TIPOOBSTACULO;
		listaCampos.campos[i].raio = 0;
	}*/
	
}

void AI:: campoInfluencia (double posX, double posZ, int tipo, double raio )
{
        listaCampos.campos[listaCampos.tamanhoLista].x = posX;
        listaCampos.campos[listaCampos.tamanhoLista].z = posZ;
        listaCampos.campos[listaCampos.tamanhoLista].tipo = tipo;
        listaCampos.campos[listaCampos.tamanhoLista].raio = raio;
        listaCampos.tamanhoLista++;
}
    
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
	//printf("Posicao do elemento: %.3lf , %.3lf\n", posX, posZ);
}

void AI:: calculaCampo(double posXP, double posZP,double posXO, double posZO,
                        int tipoCampo, int brutalidade, double * retorno)
{
#if 0
   if(tipoCampo == TIPOPC)
   {
      retorno[0] = -(posXP - posXO);
      retorno[1] = -(posZP - posZO);
   }
   else
   {
      retorno[0] = (posXP - posXO);
      retorno[1] = (posZP - posZO);
   }
#endif

//#if 0
     double quadradoDifX= ((posXP - posXO)*(posXP - posXO));
     double quadradoDifZ= ((posZP - posZO)*(posZP - posZO));

     if (tipoCampo== TIPOOBSTACULO /*|| tipoCampo== TIPONPC*/)
     {
         /* o campo nestes casos varia com d a quarta */
         quadradoDifX*= (quadradoDifX * quadradoDifX); 
         quadradoDifZ*= (quadradoDifZ * quadradoDifZ);
     }                               
     
     if (quadradoDifX) 
       retorno[0]= (tipoCampo * brutalidade) /quadradoDifX;
     else retorno[0]=0;  
     
     if (quadradoDifZ)
       retorno[1]= (tipoCampo * brutalidade) /quadradoDifZ;
     else retorno[1]=0;

//#endif
	  
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
      double vetorParcial [2];
		//int tipo = TIPONPC;
		//double raio = 20.0;
		int i;
      total[0] = 0;
      total[1] = 0;
      //printf("lista: %d\n",listaCampos.tamanhoLista);
      for(i = 0; i < listaCampos.tamanhoLista; i++){
		
      	//campoInfluencia (listaCampos.campos[i].x, listaCampos.campos[i].z,
        //             listaCampos.campos[i].tipo, listaCampos.campos[i].raio); 
			
         /*calculaCampo(npc.posicaoLadoX, npc.posicaoLadoZ, 
                      listaCampos.campos[i].x, 
                      listaCampos.campos[i].z, listaCampos.campos[i].tipo, 
                      npc.brutalidade,vetorParcial);*/

         vetorParcial[0] = npc.posicaoLadoX - listaCampos.campos[i].x;
         vetorParcial[1] = npc.posicaoLadoZ - listaCampos.campos[i].z;
         //printf("%f %f\n",vetorParcial[0],vetorParcial[1]);
         if(listaCampos.campos[i].tipo == TIPOPC)
         {
             vetorParcial[0] = -1*vetorParcial[0];
             vetorParcial[1] = -1*vetorParcial[1];
         }            
         total[0]+= vetorParcial[0];
         total[1]+= vetorParcial[1];
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
