#ifndef _modstate_h
#define _modstate_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../classes/inventory.h"
#include "../map/map.h"

#define MODSTATE_ACTION_OBJECT_REMOVE  0  /**< Removed object from map */
#define MODSTATE_ACTION_OBJECT_ADD     1  /**< Added object to the map */

#define MODSTATE_ACTION_CHARACTER_DEAD 3  /**< Killed Character on map */
#define MODSTATE_ACTION_CHARACTER_MOVE 4  /**< Character moved to positon */

#define MODSTATE_TALK_ENTER_VALUE      5  /*< New enter value for a talk */

#define MODSTATE_INVENTORY             6  /**< Inventory "Snapshot" */

/*! The Generic Modification Action Class */
class modAction
{
   public:
      /*! Constructor
       * \param act -> action type
       * \param tgt -> actor target fileName
       * \param mapFile - name of the map file where action occurs 
       * \param xPos -> x position
       * \param zPos -> z position */
      modAction(int act, string tgt, string mapFile,
                GLfloat xPos, GLfloat zPos);

      /*! Destructor */
      ~modAction();

      /*! Get the target of the action
       * \return pointer to object of the action */
      string getTarget();
      /*! Get the action type
       * \return number of the action type*/
      int getAction();
      /*! Get map filename
       * \return name of the map file */
      string getMapFileName();

      /*! Get the position where action occurs
       * \param posX -> X position
       * \param posZ -> Z position */
      void getPosition(GLfloat &posX, GLfloat& posZ);

      /*! Get next action on list 
       * \return pointer to next action on list */
      modAction* getNext();
      /*! Get previous action on list 
       * \return pointer to previous action on list */
      modAction* getPrevious();

      /*! Set next action on list
       * \param act -> pointer to next action */
      void setNext(modAction* act);
      /*! Set Previous action on list
       * \param act -> pointer to previous action */
      void setPrevious(modAction* act);

   protected:
      string mapFileName;       /**< name of the map file where action occurs */
      int action;               /**< type of the action */
      GLfloat x;                /**< X position on map */
      GLfloat z;                /**< Z position on map */
      string target;            /**< target of the action (fileName) */
      modAction* next;          /**< next action on list */
      modAction* previous;      /**< previous action on list */
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
      mapCharacterModAction(int act, string character, string mapFile,
                            GLfloat xPos, GLfloat zPos, GLfloat orientation,
                            GLfloat initialX, GLfloat initialZ);
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
      mapObjectModAction(int act, string obj, string mapFile,
                         GLfloat xPos, GLfloat zPos);
      /*! Destructor */
      ~mapObjectModAction();

   protected:

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
      mapTalkModAction(int act, string tgt, string mapFile,
                       int talkValue);

      /*! Destructor */
      ~mapTalkModAction();

      /*! Get the talk action value
       * \return -> talk action value */
      int getValue();

      /*! Set the talk action value
       * \param v -> new talk action value */
      void setValue(int v);

   protected:
       int value;   /**< Some talk Value Information */
};

/*! The modInvObj is just a structure to keep inventory objects
 * status. It is only used for modInventory. */
class modInvObj
{
   public:
      string fileName;      /**< The Object Filename */
      int x,                /**< X position on inventory */
          y;                /**< Y position on inventory */
      int invNumber;        /**< The inventory number */
      modInvObj* next;      /**< Next Object */
      modInvObj* previous;  /**< Previous Object */
};

/*! The modInventory class is a inventory state keeper,
 * basically used for populate inventories when back at some map.
 * \note -> basically, a modInventory is created (or modified, if 
 *          already exists) for each NPC when the PC exits the map.
 *          When the PC is back, each NPC's inventory is then populated
 *          by its previously modInventory. */
class modInventory: public modAction
{
   public:
      /*! Constructor
       * \param inv -> inventory to keep status
       * \param owner -> inventory owner
       * \param mapFile -> mapFileName where the owner is */
      modInventory(inventory* inv,string owner, string mapFile);
      /*! Destructor */
      ~modInventory();

      /*! Flush the objects list to the inventory,
       * loading needed models to the list
       * \param inv -> inventory to populate 
       * \param objs -> objects list
       * \param models -> models list
       * \param wType -> weapons type information */
      void flush(inventory* inv, lObject& objs, modelList& models, 
                 weaponTypes& wTypes);

      /*! Create the modInventory list based on an inventory
       * \param inv -> inventory to create the object list
       * \note -> this function will clear the previous created list */
      void create(inventory* inv);

      /*! Clear the Objects List */
      void clear();

   protected:
    
      /*! Insert a modInvObj on the list
       * \param obj -> pointer of object to insert */
      void insert(modInvObj* obj);

      modInvObj* objectsList;  /**< The objects on inventory list */
      int totalObjects;        /**< Total Objects on the list */
};

/*! modMap have all modifications done for a specified map */
class modMap
{
   public:
      /*! Constructor 
       * \param fileName -> map file name */
      modMap(string fileName);
      /*! Destructor */
      ~modMap();
      
      /*! Add action to the list (or remove some inverse action from)
       * \param action -> action type
       * \param target -> fileName of the object
       * \param mapFileName - name of the map file where action occurs
       * \param xPos -> x position
       * \param zPos -> z position*/
      void mapObjectAddAction(int action, string target, string mapFileName,
                              GLfloat xPos, GLfloat zPos);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> character fileName
       * \param mapFile -> name of the map file where action occurs 
       * \param xPos -> x position
       * \param zPos -> z position
       * \param orientation -> character orientation angle
       * \param initialX -> initial X position when loaded the map
       * \param initialZ -> initial Z position when loaded the map */
      void mapCharacterAddAction(int act, string character, string mapFile,
                                 GLfloat xPos, GLfloat zPos, 
                                 GLfloat orientation,
                                 GLfloat initialX, GLfloat initialZ);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> character fileName
       * \param mapFile -> fileName of the map
       * \param value -> the action talk value */
      void mapTalkAddAction(int act, string character, string mapFile,
                            int value);

      /*! Add Inventory to the modifications list (or update respective 
       *  modInv if one exists)
       *  \param inv -> pointer to the inventory to update
       *  \param owner -> inventory owner filename */ 
      void mapInventoryAdd(inventory* inv, string owner);

      /*! Do All saved modifications to the map (those that are for them,
       * usually when you return to the map and want it to appears exactly 
       * like when you left it). 
       * \param actualMap -> pointer to actual opened map
       * \param NPCs -> current NPCs list
       * \param objList -> current map objects list
       * \param mdlList -> current model list
       * \param wTypes -> current weapon types */
      void doMapModifications(Map* actualMap, void* NPCs, lObject& objs,
                              modelList& mdlList, weaponTypes& wTypes);

      /*! Clear All the modifications states (usually called after death) */
      void clear();

      /*! Get the next modMap pointer
       * \return -> next modMap */
      modMap* getNext();
      /*! Get the previous modMap pointer
       * \return -> previous modMap */
      modMap* getPrevious();
      /*! Set the next modMap pointer
       * \param n -> pointer to the next modMap */
      void setNext(modMap* n);
      /*! Set the previous modMap pointer
       * \param p -> pointer to the previous modMap */
      void setPrevious(modMap* p);
      
      /*! Get the mapFileName */
      string getMapFileName();

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
      bool removeInverseObjectAction(int action, string target, 
                                     string mapFileName, GLfloat xPos, 
                                     GLfloat zPos);

      /*! Search for a modAction on the list
       * \param action -> action number constant
       * \param target -> target name
       * \return -> pointer to the modAction found, or NULL if none */
      modAction* search(int action, string target, 
                        GLfloat xPos=-1, GLfloat zPos=-1);

      string mapFileName;          /**< The map file name */
      modMap* next;                /**< Next modMap on list */
      modMap* previous;            /**< Previous modMap on list */
      modAction* modActionsList;   /**< List of map modification actions */
      int totalModActions;         /**< Total Actions on the list */
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
      bool loadState(string file);

      /*! Save File of State 
       * \param file -> filename to save */
      bool saveState(string file);

      /*! Add action to the list (or remove some inverse action from)
       * \param action -> action type
       * \param target -> fileName of the object
       * \param mapFileName - name of the map file where action occurs
       * \param xPos -> x position
       * \param zPos -> z position*/
      void mapObjectAddAction(int action, string target, string mapFileName,
                              GLfloat xPos, GLfloat zPos);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> character fileName
       * \param mapFile -> name of the map file where action occurs 
       * \param xPos -> x position
       * \param zPos -> z position
       * \param orientation -> character orientation angle
       * \param initialX -> initial X position when loaded the map
       * \param initialZ -> initial Z position when loaded the map */
      void mapCharacterAddAction(int act, string character, string mapFile,
                                 GLfloat xPos, GLfloat zPos, 
                                 GLfloat orientation,
                                 GLfloat initialX, GLfloat initialZ);

      /*! Add action to the list (or remove inverse action from)
       * \param act -> action type
       * \param character -> character fileName
       * \param mapFile -> fileName of the map
       * \param value -> the action talk value */
      void mapTalkAddAction(int act, string character, string mapFile,
                            int value);
      
      /*! Add Inventory to the modifications list (or update respective 
       *  modInv if one exists)
       *  \param inv -> pointer to the inventory to update
       *  \param owner -> inventory owner filename 
       *  \param mapFile -> map filename where the character is */
      void mapInventoryAdd(inventory* inv, string owner, string mapFile);

      /*! Do All saved modifications to the map (those that are for them,
       * usually when you return to the map and want it to appears exactly 
       * like when you left it). 
       * \param actualMap -> pointer to actual opened map
       * \param NPCs -> current NPCs list
       * \param objList -> current map objects list
       * \param mdlList -> current model list
       * \param wTypes -> current weapon types */
      void doMapModifications(Map* actualMap, void* NPCs, lObject& objs,
                              modelList& mdlList, weaponTypes& wTypes);

      /*! Clear All the modifications states (usually called after death) */
      void clear();

   protected:

      /*! Create the ModMap related to the fileName 
       * \param fileName -> map file name
       * \return -> pointer to the created modMap */
      modMap* createModMap(string fileName);

      /*! Find the modMap on the list related to the fileName
       * \param fileName -> map filename to search
       * \return -> modMap pointer if found or NULL */
      modMap* findModMap(string fileName);
      
      static modMap* modMapList;   /**< List of maps */
      static int totalModMaps;     /**< Total modMaps on list */
};

#endif

