/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_dialog_h
#define _dnt_dialog_h

/*! \file dialog.h Define conversations options.*/

#define MAX_OPTIONS 5 /**< Max number of options per dialog */

#define MAX_ACTIONS 5 /**< Max number of actions per option */

#define MAX_PRE_TESTS 2 /**< Max number of preTests per option */

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
       * \param owner -> the owner of the dialog (object or character)
       * \return -> true if test pass */
      bool doTest(character* pc, thing* owner);

      /*! Get the test name (ie. if a skill, the skill name)
       * \param pc -> pointer to the character
       * \return -> the test name */
      string getTestName(character* pc);

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

      talkTest preTest[MAX_PRE_TESTS]; /**< The pre test 
                                            (if all true, the option is show) */

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

/*! Conversation Class. A conversation is a set of dialogs, usually for
 * a NPC character or an object. */
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

      /*! Set the initial dialog (the one displayed at a click on character)
       * \param numDialog -> new init dialog number */
      void setInitialDialog(int numDialog);

      /*! Set the owner of the conversation
       * \param thing -> pointer to thing owner
       * \param mapFile -> fileName where the NPC is */
      void setOwner(thing* o, string mapFile);

      /*! Set the playable character pointer
       * \param PC -> pointer to the playable character */
      void setPC(character* PC);

      /*! Get the owner of the dialog (if one)
       * \return -> pointer to the owner or NULL */
      thing* getOwner();

      /*! Get the current PC talking to the NPC
       * \return -> pointer to the PC character */
      character* getPC();

      /*! Computates the action on dialog, based on selected option.
       * \param opcao -> option selected 
       * \param curEngine -> pointer to the current engine */
      void proccessAction(int opcao, void* curEngine);

      /*! Change dialog
       * \param numDialog -> number of the new dialog to use */
      void changeDialog(int numDialog);

      /*! Change dialog to the initial one */
      void changeDialog();

   protected:
      dialog* first;        /**< Head Node */
      int total;            /**< Total Dialogs */
      int actual;           /**< Actual active Dialog */
      int initialDialog;    /**< First dialog to show */
      character* actualPC;  /**< The Actual PC */
      thing* owner;         /**< The owner of the conversation */
      string ownerMap;      /**< The Map Owner */
      
      string getString(int& initialPosition, string buffer, char& separator);
      int getActionID(string token, string fileName, int line);
      void printError(string fileName, string error, int lineNumber);
};

/*! The window where will show a dialog */
class dialogWindow
{
   public:
      /*! Open the dialog window
       * \param gui -> window interface used
       * \param PC -> player's character 
       * \param cv -> pointer to the conversation to show 
       * \param pictureFile -> filename of the picture to use */
      void open(guiInterface* gui, character* PC, conversation* cv, 
                string pictureFile);

      /*! Treat Events on Window. 
       * \param guiObj -> active GUI object
       * \param eventInfo -> last GUI Event 
       * \param infoW -> pointer to the used itemWindow
       * \param curEngine ->pointer to the current engine
       * \return true if event is threated, false otherwise. */
      bool treat(guiObject* guiObj, int eventInfo,
                 itemWindow* infoW, void* curEngine);

      /*! Verify if the dialog is open or not 
       * \return true if the window is opened */
      bool isOpened();

      /*! Verify if the dialog window is opened for an specific conversation
       * \param cv -> pointer to the conversation to verify if the 
       *              dialog window is opened for
       * \return -> true if the dialog window is opened for this conversation
       *            false if the dialog window is closed or opened for another 
       *            conversation. */
      bool isOpened(conversation* cv);

      /*! Close, if opened, the dialog window */
      void close();

      /*! Redraw the window */
      void redraw();

      /*! Set the current NPC Text
       * \param text -> current npc text to display */
      void setNPCText(string text);

      /*! Clear all options */
      void clearOptions();

      /*! Add a option 
       * \param optNumber -> the option number
       * \param text -> the option text 
       * \param info -> the option info */
      void addOption(int optNumber, string text, int info);

   protected:
      static conversation* conv;     /**< Pointer to the conversation used */
      static window* jan;            /**< Pointer to window used to show */
      static guiInterface* usedGui;  /**< Pointer to the used interface */
      static rolBar* npcText;        /**< The NPC text quad */
      static selText* pcSelText;     /**< The PC selection text */
      static button* barterButton;   /**< The Barter Button */
};

#endif

