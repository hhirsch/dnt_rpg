/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_dialog_h
#define _dnt_dialog_h

/*! \file dialog.h Define conversations options.*/

#define MAX_OPTIONS 5 /**< Max number of options per dialog */

#define MAX_ACTIONS 8 /**< Max number of actions per option */

#define MAX_PRE_TESTS 2 /**< Max number of preTests per option */

#include "../gui/farso.h"
#include "../etc/list.h"

#include "character.h"
#include <string>

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
     std::string satt; /**< String Attribute */
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
      bool set(std::string token, std::string t, std::string a);

      /*! Do the test with a character
       * \param pc -> pointer to the character to test
       * \param owner -> the owner of the dialog (object or character)
       * \return -> true if test pass */
      bool doTest(character* pc, thing* owner);

      /*! Get the test name (ie. if a skill, the skill name)
       * \param pc -> pointer to the character
       * \return -> the test name */
      std::string getTestName(character* pc);

   protected:

      /*! Get the against value (or skill value) */
      int getAgainstValue(thing* owner);

      int id;            /**< Talk test ID */
      std::string test;       /**< The modifier to test */
      std::string against;    /**< The against modifier to test (or value) */
};

/*! The dialog Option definition */
class dialogOption
{
   public:

      /*! Constructor */
      dialogOption();       

      talkTest preTest[MAX_PRE_TESTS]; /**< The pre test 
                                            (if all true, the option is show) */

      std::string text;    /**< The option text */

      talkTest postTest;   /**< The post test (if true run ifActions) */

      talkAction ifAction[MAX_ACTIONS];   /**< If postTest == true actions */
      int totalIfActions;                 /**< Number of if actions */
      talkAction elseAction[MAX_ACTIONS]; /**< If postTest == false actions */
      int totalElseActions;               /**< Number of else actions */
};

/*! Dialog Struct */
class dialog: public dntListElement
{
   public:
      std::string npcText;                /**< NPC text */
      dialogOption options[MAX_OPTIONS];  /**< PC options to talk */
      int id;                             /**< Identificator */
};

class engine;

/*! Conversation Class. A conversation is a set of dialogs, usually for
 * a NPC character or an object. */
class conversation: public dntList
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
      int loadFile(std::string name);  

      /*!
       * Save dialog file
       * \param name -> file name to save
       * \return 1 if succed. */
      int saveFile(std::string name);    

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
      void setOwner(thing* o, std::string mapFile);

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
      void proccessAction(int opcao, engine* curEngine);

      /*! Change dialog
       * \param numDialog -> number of the new dialog to use */
      void changeDialog(int numDialog);

      /*! Change dialog to the initial one */
      void changeDialog();

   protected:
      /*! Free the dialog's memory
       * \param obj -> pointer to the dialog to clean */
      void freeElement(dntListElement* obj);

      /*! Get the dialog pointer
       * \param id -> id of the dialog to get
       * \return pointer to the dialog or NULL */
      dialog* getDialog(int id);

      int actual;           /**< Actual active Dialog */
      int initialDialog;    /**< First dialog to show */
      character* actualPC;  /**< The Actual PC */
      thing* owner;         /**< The owner of the conversation */
      std::string ownerMap;      /**< The Map Owner */
      
      std::string getString(int& initialPosition, std::string buffer, 
                            char& separator);
      int getActionID(std::string token, std::string fileName, int line);
      void printError(std::string fileName, std::string error, int lineNumber);
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
      void open(Farso::GuiInterface* gui, character* PC, conversation* cv, 
                std::string pictureFile);

      /*! Treat Events on Window. 
       * \param guiObj -> active GUI object
       * \param eventInfo -> last GUI Event 
       * \param infoW -> pointer to the used itemWindow
       * \param curEngine ->pointer to the current engine
       * \param keys -> current keyboard state
       * \return true if event is threated, false otherwise. */
      bool treat(Farso::GuiObject* guiObj, int eventInfo,
                 itemWindow* infoW, engine* curEngine, Uint8 *keys);

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
      void setNPCText(std::string text);

      /*! Clear all options */
      void clearOptions();

      /*! Add a option 
       * \param optNumber -> the option number
       * \param text -> the option text 
       * \param info -> the option info */
      void addOption(int optNumber, std::string text, int info);

   protected:
      static conversation* conv;     /**< Pointer to the conversation used */
      static Farso::Window* jan;         /**< Pointer to window used to show */
      static Farso::GuiInterface* usedGui;/**< Pointer to the used interface */
      static Farso::RolBar* npcText;        /**< The NPC text quad */
      static Farso::SelText* pcSelText;     /**< The PC selection text */
      static Farso::Button* barterButton;   /**< The Barter Button */
      static int pressKey;              /**< Key pressed for dialog selection */
};

#endif

