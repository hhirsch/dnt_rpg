#ifndef _conversa_h
#define _conversa_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

/*! \file conversa.h Define conversations options.*/

#define TALK_ACTION_GOTO   0 /**< Go To some conversation point */
#define TALK_ACTION_FIGHT  1 /**< End talk and initiate a fight */
#define TALK_ACTION_CLOSE  2 /**< End Talk */
#define TALK_ACTION_MODPC  3 /**< Modify PC attribute */
#define TALK_ACTION_MODNPC 4 /**< Modify NPC attribute */

#define TALK_OPER_EQUAL        0 /**< Operation Equal */
#define TALK_OPER_GREATER      1 /**< Operation Greater */
#define TALK_OPER_GREATEREQUAL 2 /**< Operation Greater or Equal */
#define TALK_OPER_LESSER       3 /**< Operation Lesser */
#define TALK_OPER_LESSEREQUAL  4 /**< Operation Lesser or Equal */

#define TALK_OPER_ZERO  0  /**< Operator Zero */
#define TALK_OPER_MINUS 1  /**< Operator Minus */
#define TALK_OPER_PLUS  2  /**< Operator Plus */

#include "../gui/farso.h"
#include "personagens.h"
#include <string>
using namespace std;

/*! talkAction Struct */
typedef struct 
{
   int id;   /**< Talk Action Identificator */
   int oper; /**< Operator used */
   int qty;  /**< Quantity */
   int att;  /**< Attribute to modify */
}talkAction;

/*! If else struct on conversations */
typedef struct
{
   string Se;    /**< If */
   int Atributo; /**< Attribute */
   int Operador; /**< Operator */
   int Limite;   /**< Limit */
   
   string Senao; /**< Else */ 

   talkAction SeAcao;   /**< If action */
   talkAction SenaoAcao; /**< Else Action */
    
}seSenao;

/*! Dialog Struct */
typedef struct dl
{
   seSenao NPC;        /**< NPC ifElse */
   seSenao Opcoes[5];  /**< PC options to talk */
   int id;             /**< Identificator */
   struct dl* proximo; /**< Next dialog on list */
   struct dl* anterior;/**< Previous Dialog on struct */
}dialogo;

/*! Conversation Class  */
class conversa
{
   public:
      dialogo* primeiro;                /**< Head Node */
      int total;                        /**< Total Dialogs */
      int atual;                        /**< Actual active Dialog */
      janela* jan;                      /**< Pointer to window used to show */
 
      /*! conversation Constructor */
      conversa();
      /*! conversation destructor */
      ~conversa();

      /*!
       * Load file that contents the dialog.
       * \param nome -> file name to load
       * \return 1 if succed. */
      int carregarArquivo(char* nome);  

      /*!
       * Save dialog file
       * \param nome -> file name to save
       * \return 1 if succed. */
      int salvarArquivo(char* nome);    

      /*!
       * Insert dialog on conversation
       * \return pointer to dialog */
      dialogo* inserirDialogo();        

      /*!
       * Delete dialog from conversation
       * \param num -> numero do dialogo a ser retirado */
      void retirarDialogo(int num);     

      /*!
       * Opens the conversation on window
       * \param numDialogo -> dialog number to open 
       * \param gui -> window interface used
       * \param pers -> character to talk to
       * \param procPres -> procedure to invoke after press */
      void abrirDialogo(int numDialogo, interface* gui, personagem* pers,
                        int (*procPres)(SDL_Surface *screen, int texto));

      /*!
       * Computates the action on dialog, based on selected option.
       * \param numDialogo -> dialog number 
       * \param opcao -> option selected
       * \param gui -> window interface used
       * \param PC -> player's character
       * \param NPC -> non player character 
       * \return next dialog number .*/
      int ProcessaAcao(int numDialogo, int opcao,interface* gui,
                           personagem* PC, personagem* NPC);
};


#endif

