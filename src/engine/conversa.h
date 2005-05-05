#ifndef _conversa_h
#define _conversa_h

#include "../gui/farso.h"

typedef struct {
   char Se[140];
   char Atributo[30];
   char Operador[3];
   int Limite;
   
   char Senao[140];  

   char SeAcao[30];
   char SenaoAcao[30];
    
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
       * numDialogo -> numero do dialogo a ser aberto */
      void abrirDialogo(int numDialogo);
};


#endif

