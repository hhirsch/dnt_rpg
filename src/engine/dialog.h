#ifndef _dialog_h
#define _dialog_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

/*! \file dialog.h Define conversations options.*/

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

     /*! Constructor */
     talkAction();

     int id;   /**< Talk Action Identificator */
     int oper; /**< Operator used */
     int qty;  /**< Quantity */
     int att;  /**< Attribute to modify */
     string satt; /**< String Attribute */
};

/* a talk test */
class talkTest
{
   public:

      /*! Constructor  */
      talkTest();

      /*! Set the Talk test function
       * \param token -> function token string
       * \param t -> the test string
       * \param a -> the against string */
      bool set(string token, string t, string a);

      /*! Do the test with a character
       * \param pc -> pointer to the character to test 
       * \return -> true if test pass */
      bool doTest(character* pc);

   protected:
      int id;            /**< Talk test ID */
      string test;       /**< The modifier to test */
      string against;    /**< The against modifier to test (or value) */
};

/*! The dialog Option definition */
class dialogOption
{
   public:

      /*! Constructor */
      dialogOption();       

      talkTest preTest;    /**< The pre test (if true, the option is show) */

      string text;         /**< The option text */

      talkTest postTest;   /**< The post test (if true run ifActions) */

      talkAction ifAction[MAX_ACTIONS];   /**< If postTest == true actions */
      int totalIfActions;                 /**< Number of if actions */
      talkAction elseAction[MAX_ACTIONS]; /**< If postTest == false actions */
      int totalElseActions;               /**< Number of else actions */
};

/*! Dialog Struct */
class dialog
{
   public:
      string npcText;                     /**< NPC text */
      dialogOption options[MAX_OPTIONS];  /**< PC options to talk */
      int id;                             /**< Identificator */

      dialog* next;                       /**< Next dialog on chain list */
      dialog* previous;                   /**< Previous Dialog on chain list */
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

