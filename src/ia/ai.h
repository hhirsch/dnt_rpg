#include "../map/map.h"
#include "../engine/personagens.h"
#include <stdlib.h>
#include <math.h>

#define TIPOOBSTACULO -240 
#define TIPOPC 100           
#define TIPONPC -260 

#define PASSO 1         // distancia percorrida a cada passo dos personagens

#define TLISTACAMPOS 20 // maximo total de campos de influencia

typedef struct item {
    double x, z;       /* Coordenadas */
    int tipo;          /* Tipo do Item */
    item * prox;       /* Proximo na lista */
    double raio;       /* Raio de acao */
} item;    

typedef struct Litem {
	item campos[TLISTACAMPOS];
	int tamanhoLista;
} Litem;


class AI
{
   /* A funcao de interface entre a classe ai e a engine segue abaixo
    * MoverNpcs invoca todas as operacoes de ai para os npcs. Recebe como 
    * parametro o primeiro npc da lista.
    */
   
   public:

	AI(){} //Construtor
	~AI(){} //DESTRUIDOR !!!
      
	/* A funcao iniciaListaCampos() zera uma lista de campos para uso */  
	void AI::iniciaListaCampos();

	/* A funcao campoInfluencia insere um novo campo de influencia na 
	 * lista dos que estao influenciando */	 
	void AI:: campoInfluencia (double posX, double posZ, int tipo, 
                                   double raio );

	/* Para onde vai o NPC ? */
	void destinoNpc(personagem * npc);

	private:
	/* Normaliza vetor e' para garantir uma distancia de passo fixa.
	 */                  
	void normalizarVetor (double * vetor);

	/* calculaVetorResultante calcula o vetor envolvendo todas as forcas
	 * sobre o npc e a retorna. Nao normaliza.
	 */
	void calculaVetorResultante (personagem npc, double * total);

        /* Atualiza as coordenadas do NPC */
        void moverNpc (personagem *npc, double posX, double posZ);
 
        Litem listaCampos;
        
};                 
