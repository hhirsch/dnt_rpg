#ifndef _modstate_h
#define _modstate_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../classes/inventory.h"
#include "../map/map.h"

#define MODSTATE_ACTION_MAP_REMOVE  0  /**< Removed object from map */
#define MODSTATE_ACTION_MAP_ADD     1  /**< Added object to the map */

/*! An Modification Action made to objects in map */
class mapObjectModAction
{
   public:
      /*! Constructor
       * \param act -> action type
       * \param obj -> pointer to object
       * \param mapFile - name of the map file where action occurs 
       * \param xPos -> x position
       * \param zPos -> z position */
      mapObjectModAction(int act, string obj, string mapFile,
                         GLfloat xPos, GLfloat zPos);
      /*! Destructor */
      ~mapObjectModAction();

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
      mapObjectModAction* getNext();
      /*! Get previous action on list 
       * \return pointer to previous action on list */
      mapObjectModAction* getPrevious();

      /*! Set next action on list
       * \param act -> pointer to next action */
      void setNext(mapObjectModAction* act);
      /*! Set Previous action on list
       * \param act -> pointer to previous action */
      void setPrevious(mapObjectModAction* act);

   protected:
      string mapFileName;       /**< name of the map file where action occurs */
      int action;               /**< type of the action */
      GLfloat x;                /**< X position on map */
      GLfloat z;                /**< Z position on map */
      string target;           /**< object target of the action */
      mapObjectModAction* next;     /**< next action on list */
      mapObjectModAction* previous; /**< previous action on list */
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
       * \param target -> pointer to object
       * \param mapFileName - name of the map file where action occurs
       * \param xPos -> x position
       * \param zPos -> z position*/
      void mapObjectAddAction(int action, string target, string mapFileName,
                              GLfloat xPos, GLfloat zPos);

      /*! Do All saved modifications to the map (those that are for them,
       * usually when you return to the map and want it to appears exactly 
       * like when you left it). 
       * \param actualMap -> pointer to actual opened map */
      void doMapModifications(Map* actualMap);

   protected:
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
      
      mapObjectModAction* mapObjectsList; /**< List of mod action mapObjects */
      int totalMapObjects;               /**< Total mapObjects on list */
};

#endif

