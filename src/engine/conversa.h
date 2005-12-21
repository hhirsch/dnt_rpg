/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _conversa_h
#define _conversa_h


#define ACTION_GOTO   0
#define ACTION_FIGHT  1
#define ACTION_CLOSE  2
#define ACTION_MODPC  3
#define ACTION_MODNPC 4

#define OPER_EQUAL        0
#define OPER_GREATER      1
#define OPER_GREATEREQUAL 2
#define OPER_LESSER       3
#define OPER_LESSEREQUAL  4

#define OPER_ZERO  0
#define OPER_MINUS 1
#define OPER_PLUS  2      

#define ATT_AGILITY     0
#define ATT_BOVINICE    1
#define ATT_BRUTALITY   2
#define ATT_WISDOW      3
#define ATT_STRONG      4
#define ATT_GAMBIARRY   5
#define ATT_INTELIGENCY 6
#define ATT_HATE        7

#include "../gui/farso.h"
#include "personagens.h"
#include <string>
using namespace std;

typedef struct 
{
   int id;
   int oper;
   int qty;
   int att;
}action;

typedef struct {
   string Se;
   int Atributo;
   int Operador;
   int Limite;
   
   string Senao;  

   action SeAcao;
   action SenaoAcao;
    
}seSenao;

typedef struct dl{
   seSenao NPC;
   seSenao Opcoes[5];
   int id;
   struct dl* proximo;
   struct dl* anterior;
}dialogo;

class conversa
{
   public:
      dialogo* primeiro;                /*Nodo cabeca*/
      int total;                        /*Total de Dialogos*/
      int atual;                        /*Dialogo Atualmente Utilizado*/
      janela* jan;
 
      conversa();                       /*Construtor*/
      ~conversa();                      /*Destruidor*/

      /* Carrega arquivo de dialogo
       * nome -> nome do arquivo de dialogo a carregar */
      int carregarArquivo(char* nome);  

      /* Salva arquivo de dialogo
       * nome -> nome do arquivo a salvar */
      int salvarArquivo(char* nome);    

      /* Insere dialogo na conversa */
      dialogo* inserirDialogo();        

      /* Retira dialogo da conversa
       * num -> numero do dialogo a ser retirado */
      void retirarDialogo(int num);     

      /* Abre a conversa na janela
       * numDialogo -> numero do dialogo a ser aberto 
       * gui -> interface de jaelas utilizada */
      void abrirDialogo(int numDialogo, interface* gui, personagem* pers,
                        int (*procPres)(SDL_Surface *screen, int texto));

      int ProcessaAcao(int numDialogo, int opcao,interface* gui,
                           personagem* PC, personagem* NPC);



};


#endif

