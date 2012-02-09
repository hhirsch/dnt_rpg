/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_modstate_h
#define _dnt_modstate_h

#include "../classes/inventory.h"
#include "../etc/defparser.h"
#include "../etc/list.h"
#include "../map/map.h"

#define MODSTATE_ACTION_OBJECT_REMOVE  0  /**< Removed object from map */
#define MODSTATE_ACTION_OBJECT_ADD     1  /**< Added object to the map */
#define MODSTATE_ACTION_OBJECT_CHANGE_STATE 2 /**< Changed Object State */

#define MODSTATE_ACTION_CHARACTER_DEAD 3  /**< Killed Character on map */
#define MODSTATE_ACTION_CHARACTER_MOVE 4  /**< Character moved to positon */

#define MODSTATE_TALK_ENTER_VALUE      5  /*< New enter value for a talk */

#define MODSTATE_INVENTORY             6  /**< Inventory "Snapshot" */

#define MODSTATE_ACTION_CHARACTER_CHANGE_STATE 7 /**< Character Change State */

class characterList;

/*! The Generic Modification Action Class */
class modAction: public dntListElement
{
   public:
      /*! Constructor
       * \param act -> action type
       * \param tgt -> actor target fileName
       * \param mapFile - name of the map file where action occurs 
       * \param xPos -> x position
       * \param zPos -> z position */
      modAction(int act, std::string tgt, std::string mapFile,
                GLfloat xPos, GLfloat yPos, GLfloat zPos);

      /*! Destructor */
      virtual ~modAction();

      /*! Get the target of the action
       * \return pointer to object of the action */
      std::string getTarget();
      /*! Get the action type
       * \return number of the action type*/
      int getAction();
      /*! Get map filename
       * \return name of the map file */
      std::string getMapFileName();

      /*! Get the position where action occurs
       * \param posX -> X position
       * \param posY -> Y position
       * \param posZ -> Z position */
      void getPosition(GLfloat &posX, GLfloat& posY, GLfloat& posZ);

      /*! Set the position where action occurs
       * \param posX -> X position
       * \param posY -> Y position
       * \param posZ -> Z position */
      void setPosition(GLfloat posX, GLfloat posY, GLfloat posZ);

      /*! Convert the action to a string (usually to save)
       * \return string representing the action */
      virtual std::string toString()=0;
      /*! Define the action from a string (usually to load) 
       * \param s -> string to load from */
      virtual void fromString(std::string s)=0;

   protected:
      std::string mapFileName;  /**< name of the map file where action occurs */
      int action;               /**< type of the action */
      GLfloat x;                /**< X position on map */
      GLfloat y;                /**< Y Position on map */
      GLfloat z;                /**< Z position on map */
      std::string target;       /**< target of the action (fileName) */
};

/*! The list of modActions */
class modActionList: public dntList
{
   public:
      /*! Costructor */
      modActionList();
      /*! Destructor */
      ~modActionList();

   protected:
      /*! Free the modMap object memory
       * \param obj -> modMap to free */
      void freeElement(dntListElement* obj);
};

/*! A Modification Action made to (or by) a character on map */
class mapCharacterModAction : public modAction
{
   public:
      /*! Constructor
       * \param act -> action type
       * \param character -> character fileName
       * \param mapFile -> name of the map file where action occurs 
       * \param xPos -> x position
       * \param zPos -> z position
       * \param orientation -> character orientation angle
       * \param initialX -> initial X position when loaded the map
       * \param initialZ -> initial Z position when loaded the map */
      mapCharacterModAction(int act, std::string character, std::string mapFile,
                            GLfloat xPos, GLfloat yPos, GLfloat zPos, 
                            GLfloat orientation,
                            GLfloat initialX, GLfloat initialZ);

      /*! Constructor from string
       * \param s -> string with info to load */
      mapCharacterModAction(std::string s);

      /*! Destructor */                      
      ~mapCharacterModAction();

      /*! Get the initial X position of the character
       * \return inital X position of the character */
      GLfloat getInitialX();

      /*! Get the initial Z position of the character
       * \return inital Z position of the character */
      GLfloat getInitialZ();

      /*! Get the orientation of the character at action's instant
       * \return character orientation */
      GLfloat getOrientation();
      /*! Set the orientation of the character at action's instant
       * \param o -> character orientation */
      void setOrientation(float o);

      /*! Convert the action to a string (usually to save)
       * \return string representing the action */
      std::string toString();
      /*! Define the action from a string (usually to load)
       * \param s -> string to load from */
      void fromString(std::string s);

   protected:
      GLfloat initX;                   /**< X initial position on map */
      GLfloat initZ;                   /**< Z initial position on map */
      GLfloat oriAngle;                /**< Orientation Angle */
};

/*! A Modification Action made to objects in map */
class mapObjectModAction: public modAction
{
   public:
      /*! Constructor
       * \param act -> action type
       * \param obj -> object fileName
       * \param mapFile - name of the map file where action occurs 
       * \param xPos -> x position
       * \param zPos -> z position */
      mapObjectModAction(int act, std::string obj, std::string mapFile,
                         GLfloat xPos, GLfloat yPos, GLfloat zPos);

      /*! Constructor from string
       * \param s -> string with info to load */
      mapObjectModAction(std::string s);

      /*! Destructor */
      ~mapObjectModAction();

      /*! Get the action value
       * \return -> action value */
      int getValue();

      /*! Set the action value
       * \param v -> new action value */
      void setValue(int v);

      /*! Convert the action to a string (usually to save)
       * \return string representing the action */
      std::string toString();
      /*! Define the action from a string (usually to load)
       * \param s -> string to load from */
      void fromString(std::string s);

   protected:
       int value;   /**< Some Value Information (usually related to state)  */

};

/*! A modification to a talk on map */
class mapTalkModAction: public modAction
{
   public:
      /*! Constructor
       * \param act -> action type
       * \param tgt -> actor target fileName
       * \param mapFile - name of the map file where action occurs 
       * \param talkValue -> value position */
      mapTalkModAction(int act, std::string tgt, std::string mapFile,
                       int talkValue);

      /*! Constructor from string
       * \param s -> string with info to load */
      mapTalkModAction(std::string s);

      /*! Destructor */
      ~mapTalkModAction();

      /*! Get the talk action value
       * \return -> talk action value */
      int getValue();

      /*! Set the talk action value
       * \param v -> new talk action value */
      void setValue(int v);

      /*! Convert the action to a string (usually to save)
       * \return string representing the action */
      std::string toString();
      /*! Define the action from a string (usually to load)
       * \param s -> string to load from */
      void fromString(std::string s);

   protected:
       int value;   /**< Some talk Value Information */
};

/*! The modInvObj is just a structure to keep inventory objects
 * status. It is only used for modInventory. */
class modInvObj: public dntListElement
{
   public:
      std::string fileName; /**< The Object Filename */
      int x,                /**< X position on inventory */
          y;                /**< Y position on inventory */
      int invNumber;        /**< The inventory number */
      int state;            /**< The current object state */
};

/*! The modInventory class is a inventory state keeper,
 * basically used for populate inventories when back at some map.
 * \note -> basically, a modInventory is created (or modified, if 
 *          already exists) for each NPC when the PC exits the map.
 *          When the PC is back, each NPC's inventory is then populated
 *          by its previously modInventory. */
class modInventory: public modAction, public dntList
{
   public:
      /*! Constructor
       * \param inv -> inventory to keep status
       * \param owner -> inventory owner
       * \param mapFile -> mapFileName where the owner is */
      modInventory(inventory* inv, std::string owner, std::string mapFile);

      /*! Destructor */
      ~modInventory();

      /*! Flush the objects list to the inventory,
       * loading needed models to the list
       * \param curMap -> current opened map
       * \param inv -> inventory to populate */ 
      void flush(Map* map, inventory* inv);

      /*! Create the modInventory list based on an inventory
       * \param inv -> inventory to create the object list
       * \note -> this function will clear the previous created list */
      void create(inventory* inv);

      /*! Clear the Objects List */
      void clear();

      /*! Convert the action to a string (usually to save)
       * \return string representing the action */
      std::string toString();
      /*! Define the action from a string (usually to load)
       * \param s -> string to load from */
      void fromString(std::string s);

      /*! Save the modInventory to the file
       * \param file -> pointer to the file to save */
      void save(std::ofstream* file);

      /*! Load the modInventory from a file
       * \param file -> fileName to load
       * \return -> true if ok  */
      bool load(std::string file);

      /*! Insert a item from a string representing the modInvObj
       * \param s -> string representing modInvObj */
      void insert(std::string s);

   protected:
    
      /*! Insert a modInvObj on the list
       * \param obj -> pointer of object to insert */
      void insert(modInvObj* obj);

      /*! Free modInvObj memory
       * \param obj -> modInvObj to delete */
      void freeElement(dntListElement* obj);
};

/*! modMap have all modifications done for a specified map */
class modMap: public dntListElement
{
   public:
      /*! Constructor 
       * \param fileName -> map file name */
      modMap(std::string fileName);
      /*! Destructor */
      ~modMap();

      /*! Save the modMap to the file
       * \param file -> pointer to the file to save */
      void save(std::ofstream* file);

      /*! Load the modMap from the definitions parser
       * \param def -> pointer to the defParser with info for the modMap */
      void load(defParser* def);
      
      /*! Add action to the list (or remove some inverse action from)
       * \param action -> action type
       * \param target -> fileName of the object
       * \param mapFileName - name of the map file where action occurs
       * \param xPos -> x position
       * \param yPos -> y position
       * \param zPos -> z position
       * \param value -> some value related to the action */
      void mapObjectAddAction(int action, std::string target, 
                              std::string mapFileName,
                              GLfloat xPos, GLfloat yPos, GLfloat zPos,
                              int value);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> character fileName
       * \param mapFile -> name of the map file where action occurs 
       * \param xPos -> x position
       * \param zPos -> z position
       * \param orientation -> character orientation angle
       * \param initialX -> initial X position when loaded the map
       * \param initialZ -> initial Z position when loaded the map */
      void mapCharacterAddAction(int act, std::string character,
                                 std::string mapFile,
                                 GLfloat xPos, GLfloat yPos, GLfloat zPos, 
                                 GLfloat orientation,
                                 GLfloat initialX, GLfloat initialZ);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> character fileName
       * \param mapFile -> fileName of the map
       * \param value -> the action talk value */
      void mapTalkAddAction(int act, std::string character, 
                            std::string mapFile, int value);

      /*! Add Inventory to the modifications list (or update respective 
       *  modInv if one exists)
       *  \param inv -> pointer to the inventory to update
       *  \param owner -> inventory owner filename */ 
      void mapInventoryAdd(inventory* inv, std::string owner);

      /*! Do All saved modifications to the map (those that are for them,
       * usually when you return to the map and want it to appears exactly 
       * like when you left it). 
       * \param actualMap -> pointer to actual opened map
       * \param NPCs -> current NPCs list */
      void doMapModifications(Map* actualMap, characterList* NPCs);

      /*! Clear All the modifications states (usually called after death) */
      void clear();      

      /*! Get the mapFileName */
      std::string getMapFileName();

      /*! Verify if all Characters defined at a NPC file (relative,
       * obviously, to a map) are dead. 
       * \return -> true if all are dead*/
      bool allCharactersDead(std::string npcFileName);

      /*! Verify if all Characters defined at a NPC file (relative,
       * obviously, to a map) are alive 
       * \return -> true if all are alive */
      bool allCharactersAlive(std::string npcFileName);

   protected:

      /*! Remove Action from the list
       * \param act -> action to remove */
      void removeAction(modAction* act);

      /*! Add Action to the list
       * \param act -> pointer to the action to add */
      void addAction(modAction* act);

      /*! remove inverse action, if it exists in list. 
       * \param action -> action type
       * \param target -> pointer to object
       * \param mapFileName - name of the map file where action occurs
       * \param xPos -> x position
       * \param zPos -> z position
       * \return true if the inverse action is found and removed. */
      bool removeInverseObjectAction(int action, std::string target, 
                                     std::string mapFileName, GLfloat xPos, 
                                     GLfloat yPos, GLfloat zPos);

      /*! Search for a modAction on the list
       * \param action -> action number constant
       * \param target -> target name
       * \return -> pointer to the modAction found, or NULL if none */
      modAction* search(int action, std::string target, 
                        GLfloat xPos=-1, GLfloat yPos=-1, GLfloat zPos=-1);

      std::string mapFileName;     /**< The map file name */
      modActionList modList;       /**< List of map modification actions */
};

/*! The list of modMaps */
class modMapList: public dntList
{
   public:
      /*! Constructor */
      modMapList();
      /*! Destructor */
      ~modMapList();

   protected:
      /*! Free modMap memory
       * \param obj -> modMap to delete */
      void freeElement(dntListElement* obj);
};

/*! All modifications made by users on game are armazened here. It is also
 * the class to load/save states. */
class modState
{
   public:
      /*! Constructor */
      modState();
      /*! Destructor */
      ~modState();

      /*! Load File of state 
       * \param file -> filename to load */
      bool loadState(std::string file);

      /*! Save File of State 
       * \param file -> filename to save */
      bool saveState(std::string file);

      /*! Add action to the list (or remove some inverse action from)
       * \param action -> action type
       * \param target -> fileName of the object
       * \param mapFileName - name of the map file where action occurs
       * \param xPos -> x position
       * \param yPos -> y position 
       * \param zPos -> z position
       * \param value -> some value related to the action */
      void mapObjectAddAction(int action, std::string target,
                              std::string mapFileName,
                              GLfloat xPos, GLfloat yPos, GLfloat zPos,
                              int value=0);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> character fileName
       * \param mapFile -> name of the map file where action occurs 
       * \param xPos -> x position
       * \param yPos -> y position
       * \param zPos -> z position
       * \param orientation -> character orientation angle
       * \param initialX -> initial X position when loaded the map
       * \param initialZ -> initial Z position when loaded the map */
      void mapCharacterAddAction(int act, std::string character,
                                 std::string mapFile,
                                 GLfloat xPos, GLfloat yPos, GLfloat zPos, 
                                 GLfloat orientation,
                                 GLfloat initialX, GLfloat initialZ);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> character fileName
       * \param mapFile -> fileName of the map
       * \param value -> the action talk value */
      void mapTalkAddAction(int act, std::string character, std::string mapFile,
                            int value);
      
      /*! Add Inventory to the modifications list (or update respective 
       *  modInv if one exists)
       *  \param inv -> pointer to the inventory to update
       *  \param owner -> inventory owner filename 
       *  \param mapFile -> map filename where the character is */
      void mapInventoryAdd(inventory* inv, std::string owner,
                           std::string mapFile);

      /*! Do All saved modifications to the map (those that are for them,
       * usually when you return to the map and want it to appears exactly 
       * like when you left it). 
       * \param actualMap -> pointer to actual opened map
       * \param NPCs -> current NPCs list */
      void doMapModifications(Map* actualMap, characterList* NPCs);

      /*! Clear All the modifications states (usually called after death) */
      void clear();

      /*! Verify if all Characters defined at a NPC file (relative,
       * obviously, to a map) are dead. 
       * \return -> true if all are dead*/
      bool allCharactersDead(std::string npcFileName);

      /*! Verify if all Characters defined at a NPC file (relative,
       * obviously, to a map) are alive 
       * \return -> true if all are alive */
      bool allCharactersAlive(std::string npcFileName);

      /*! Verify if the modState have informations for the map
       * \param fileName -> map filename to verify
       * \return -> true if have info, false if not */
      bool haveMap(std::string fileName);

   protected:

      /*! Create the ModMap related to the fileName 
       * \param fileName -> map file name
       * \return -> pointer to the created modMap */
      modMap* createModMap(std::string fileName);

      /*! Find the modMap on the list related to the fileName
       * \param fileName -> map filename to search
       * \return -> modMap pointer if found or created
       * \note -> if not found a modMap this function create a new one */
      modMap* findModMap(std::string fileName);
      
      static modMapList modList;   /**< List of mod maps */
};

#endif

