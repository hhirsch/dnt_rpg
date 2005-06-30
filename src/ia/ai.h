#include "../map/map.h"
#include "../engine/personagens.h"
#include <stdlib.h>
#include <math.h>

#define TIPOOBSTACULO 240 //constante associada ao campo feito por obstaculo
#define TIPOPC -100       //associada ao campo feito por pc, note q e' atracao  
#define TIPONPC 260 

#define COVELOCIDADE 100 //converte de agilidade para o numero de ciclos de espera      
#define PASSO 40 //distancia percorrida a cada passo dos personagens

typedef struct item {
    double x, z;       /* Coordenadas */
    int tipo;          /* Tipo do Item */
    item * prox;       /* Proximo na lista */
    int raio;          /* Raio de acao */
} item;    

/* As duas funcoes abaixo fornecem da engine o que e' necessario para a ai
 * campoInfluencia recebe a posicao inicial, e o raio de acao do campo
 * e retorna uma lista de itens para os objetos que influenciam o personagem
 * dentro do raio dado.
 * A funcao moverNpc passa o npc com a posicao atual e passa para onde ele
 * vai. A funcao deve realizar o movimento e checar se ele pode mover
 * para o destino, caso nao possa, simplesmente nao mova.
*/


class AI
{
   /* A funcao de interface entre a classe ai e a engine segue abaixo
    * MoverNpcs invoca todas as operacoes de ai para os npcs. Recebe como 
    * parametro o primeiro npc da lista.
    */
   
   public:
//    void destinoNpcs(Lpersonagem * npc);

	AI(){} //Construtor
	~AI(){} //DESTRUIDOR !!!
	void destinoNpc(personagem * npc);
      
// private:
       
//     int velocidade;
        
	/* A funcao iniciaListaCampos() zera uma lista de campos para uso */  
        void AI::iniciaListaCampos();
   

	/* A funcao campoInfluencia insere um novo campo de influencia na lista dos que estao influenciando */	 
	void AI:: campoInfluencia (double posX, double posZ, int tipo, double raio );
	void moverNpc (personagem *npc, double posX, double posZ);
		 
	void calculaCampo(double posXP, double posZP, double posXO, 
			double posZO,int tipoCampo, int brutalidade, 
			double * retorno);
                         
       /* NNormaliza vetor e' para garantir uma distancia de passo fixa.
        */                  
       void normalizarVetor (double * vetor);
       
       /* calculaVetorResultante calcula o vetor envolvendo todas as forcas
        * sobre o npc e a retorna. Nao normaliza.
        */
       void calculaVetorResultante (personagem npc, double * total);
       
       /* Para variar a velocidade dos personagens e tornar o movimento visivel
        * alguns dos ciclos os npcs nao terao a posicao modificada
        */
       int deveMover (int indice);
        
};                 
