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
#define TALK_OPER_TRUE         5 /**< Operation True (always accepts) */
#define TALK_OPER_FALSE        6 /**< Operation False (always rejects) */

#define TALK_OPER_ZERO  0  /**< Operator Zero */
#define TALK_OPER_MINUS 1  /**< Operator Minus */
#define TALK_OPER_PLUS  2  /**< Operator Plus */

#define MAX_OPTIONS 5 /**< Max number of options per dialog */

#include "../gui/farso.h"
#include "personagens.h"
#include <string>
using namespace std;

/*! talkAction Struct */
class talkAction
{
  public:
   int id;   /**< Talk Action Identificator */
   int oper; /**< Operator used */
   int qty;  /**< Quantity */
   int att;  /**< Attribute to modify */
};

/*! If else struct on conversations */
class ifElse
{
  public:
   string ifText;         /**< If */
   int attribute;         /**< Attribute */
   int operation;         /**< Operator */
   int limit;             /**< Limit */
   
   string elseText;       /**< Else */ 

   talkAction ifAction;   /**< If action */
   talkAction elseAction; /**< Else Action */
    
};

/*! Dialog Struct */
class dialog
{
  public:
   ifElse npc;        /**< NPC ifElse */
   ifElse options[MAX_OPTIONS]; /**< PC options to talk */
   int id;            /**< Identificator */
   dialog* next;      /**< Next dialog on list */
   dialog* previous;  /**< Previous Dialog on struct */
};

/*! Conversation Class  */
class conversation
{
   public:
      /*! conversation Constructor */
      conversation();
      /*! conversation destructor */
      ~conversation();

      /*!
       * Load file that contents the dialog.
       * \param name -> file name to load
       * \return 1 if succed. */
      int loadFile(string name);  

      /*!
       * Save dialog file
       * \param name -> file name to save
       * \return 1 if succed. */
      int saveFile(string name);    

      /*!
       * Insert dialog on conversation
       * \return pointer to dialog */
      dialog* insertDialog();        

      /*!
       * Delete dialog from conversation
       * \param num -> number of the dialog to be removed */
      void removeDialog(int num);     

      /*!
       * Opens the conversation on window
       * \param numDialog -> dialog number to open 
       * \param gui -> window interface used
       * \param pers -> character to talk to */
      void openDialog(int numDialog, interface* gui, personagem* pers);

      /*! Treat Events on Window. 
       * \param guiObject -> active GUI object
       * \param eventInfo -> last GUI Event 
       * \param gui -> window interface used
       * \return true if event is threated, false otherwise. */
      bool treat(Tobjeto* guiObject, int eventInfo, interface* gui);

      /*! Verify if the dialog is open or not 
       * \return true if the window is opened */
      bool windowOpened();

      protected:
         janela* jan;    /**< Pointer to window used to show */
         dialog* first;  /**< Head Node */
         int total;      /**< Total Dialogs */
         int actual;     /**< Actual active Dialog */
         rolBar* npcText; /**< The NPC text quad */
         selTexto* pcSelText; /**< The PC selection text */

         /*!
          * Computates the action on dialog, based on selected option.
          * \param numDialog -> dialog number 
          * \param opcao -> option selected
          * \param gui -> window interface used
          * \param PC -> player's character
          * \param NPC -> non player character */
         void proccessAction(int numDialog, int opcao,interface* gui,
                            personagem* PC, personagem* NPC);

         /*! 
          * Change dialog
          * \param numDialog -> number of the new dialog to use */
         void changeDialog(int numDialog);

         string getString(int& initialPosition, char* buffer,
                          char& separator);
         int getActionID(string token, string fileName, int line);
         void printError(string fileName, string error, int lineNumber);
};


#endif

