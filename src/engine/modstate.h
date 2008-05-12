#ifndef _modstate_h
#define _modstate_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../classes/inventory.h"
#include "../map/map.h"

#define MODSTATE_ACTION_OBJECT_REMOVE  0  /**< Removed object from map */
#define MODSTATE_ACTION_OBJECT_ADD     1  /**< Added object to the map */

#define MODSTATE_ACTION_CHARACTER_DEAD   3  /**< Killed Character on map */
#define MODSTATE_ACTION_CHARACTER_MOVE   4  /**< The character moved to a new positon */

#define MODSTATE_TALK_ENTER_VALUE        5  /*< New enter value for a talk */

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

      /*! Get thje position where action occurs
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

/*! The Modifications made by users on game are armazened here. It is also
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

      /*! Do All saved modifications to the map (those that are for them,
       * usually when you return to the map and want it to appears exactly 
       * like when you left it). 
       * \param actualMap -> pointer to actual opened map
       * \param NPCs -> current NPCs list */
      void doMapModifications(Map* actualMap, void* NPCs);

      /*! Clear All the modifications states (usually called after death) */
      void clear();

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
      
      static modAction* modActionsList;   /**< List of modification actions */
      static int totalModActions;         /**< Total modActions on list */
};

#endif

