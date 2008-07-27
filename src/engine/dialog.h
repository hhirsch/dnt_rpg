#ifndef _dialog_h
#define _dialog_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

/*! \file dialog.h Define conversations options.*/

#define TALK_ACTION_GO_TO_DIALOG   0 /**< Go To some conversation point */
#define TALK_ACTION_INIT_FIGHT     1 /**< End talk and initiate a fight */
#define TALK_ACTION_FINISH_DIALOG  2 /**< End Talk */
#define TALK_ACTION_MOD_PC         3 /**< Modify PC attribute */
#define TALK_ACTION_MOD_NPC        4 /**< Modify NPC attribute */
#define TALK_ACTION_DIALOG_INIT    5 /**< Set the new initial dialog */
#define TALK_ACTION_ADD_MISSION    6 /**< Add a mission */
#define TALK_ACTION_COMPLETE_MISSION 7 /**< Complete Mission */
#define TALK_ACTION_GIVE_ITEM      8 /**< Give an item */
#define TALK_ACTION_RECEIVE_MONEY  9 /**< Receive some money */

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

#define MAX_ACTIONS 5 /**< Max number of actions per option */

#include "../gui/farso.h"
#include "character.h"
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
     string satt; /**< String Attribute */
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

   talkAction ifAction[MAX_ACTIONS];   /**< If action */
   int totalIfActions;                 /**< Number of if actions */
   talkAction elseAction[MAX_ACTIONS]; /**< Else Action */
   int totalElseActions;               /**< Number of else actions */
    
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
      /*! conversation Constructor
       * \param pEngine -> pointer to current engine*/
      conversation(void* pEngine);
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

      /*! Set the initial dialog (the one displayed at a click on character)
       * \param numDialog -> new init dialog number */
      void setInitialDialog(int numDialog);
      
      /*! Set the owner of the conversation
       * \param pers -> pointer to NPC owner
       * \param mapFile -> fileName where the NPC is */
      void setOwner(character* pers, string mapFile);

      /*!
       * Opens the conversation on window
       * \param gui -> window interface used
       * \param pers -> character to talk to
       * \param PC -> player's character */
      void openDialog(guiInterface* gui, character* PC);

      /*! Treat Events on Window. 
       * \param guiObj -> active GUI object
       * \param eventInfo -> last GUI Event 
       * \param infoW -> pointer to the used itemWindow
       * \return true if event is threated, false otherwise. */
      bool treat(guiObject* guiObj, int eventInfo,
                 itemWindow* infoW);

      /*! Verify if the dialog is open or not 
       * \return true if the window is opened */
      bool windowOpened();

      /*! Close, if opened, the dialog window */
      void closeWindow();

      protected:
         window* jan;          /**< Pointer to window used to show */
         guiInterface* usedGui;/**< Pointer to the used interface */
         dialog* first;        /**< Head Node */
         int total;            /**< Total Dialogs */
         int actual;           /**< Actual active Dialog */
         int initialDialog;    /**< First dialog to show */
         rolBar* npcText;      /**< The NPC text quad */
         selText* pcSelText;   /**< The PC selection text */
         button* barterButton; /**< The Barter Button */
         character* actualPC;  /**< The Actual PC */
         character* ownerNPC;  /**< The NPC owner of the conversation */
         string ownerMap;      /**< The Map Owner */
         void* actualEngine;   /**< The actual Engine */

         /*!
          * Computates the action on dialog, based on selected option.
          * \param numDialog -> dialog number 
          * \param opcao -> option selected */
         void proccessAction(int numDialog, int opcao);

         /*! 
          * Change dialog
          * \param numDialog -> number of the new dialog to use */
         void changeDialog(int numDialog);

         string getString(int& initialPosition, string buffer,
                          char& separator);
         int getActionID(string token, string fileName, int line);
         void printError(string fileName, string error, int lineNumber);
};


#endif

