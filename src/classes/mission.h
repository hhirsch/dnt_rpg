#ifndef _mission_h
#define _mission_h

#include "../ia/iascript.h"
#include "../etc/defparser.h"

#include <iostream>
using namespace std;

#define MISSION_TEMP_FLAGS  5    /**< Number of temp flags on missions */

#define MISSION_COMPLETION_FAILED   -2
#define MISSION_COMPLETION_FINISHED -1
#define MISSION_COMPLETION_FALSE     0

/*! A mission definition. The mission is basically a main script
 * with temporary and a completed variables. */
class mission: public iaScript
{
   public:
      /*! Constructor
       * \param missionFile -> fileName of the mission's definition
       *                       or of the mission's script file
       * \param usedEngine -> the pointer to the engine used
       * \param loadDefinition -> true if is to load the definition file
       *                          false if the missionFile is already 
       *                          the script itself. */
      mission(string missionFile, void* usedEngine, bool loadDefinition);

      /*! Destructor */
      ~mission();

      /*! Get the completion type of the mission
       * \return -> 0 if is not completed, a integer with the completion
       *            type if is completed. */
      int getCompletion();

      /*! Get a temporary flag value
       * \param flagNumber -> the number of the desired flag
       * \return -> flagValue */
      int getTempFlag(int flagNumber);

      /*! Set a temporary flag value
       * \param flagNumber -> the number of the desired flag
       * \param flagValue -> the new value of the flag */
      void setTempFlag(int flagNumber, int flagValue);

      /*! get XP value on completing the mission
       * \return xp */
      int getXp();

      /*! Set the xp value of the function
       * \param xp -> new xp value */
      void setXp(int xp);

      /*! Get the mission's description
       * \return -> translated mission description string */
      string getDescription();

      /*! Set the mission's description
       * \param desc -> translated mission description string */
      void setDescription(string desc);

      /*! Get the Area where the mission occurs
       * \return -> Area Name */
      string getArea();

      /*! Set the Area where the mission occurs
       * \param a -> Area Name */
      void setArea(string a);

      /*! Save the current mission status to a file
       * \param file -> pointer to the file to use
       * \note -> this function will save all the stack and script 
       *          status relative to the mission */
      void saveAsCurrent(ofstream* file);

      /*! Save the completed mission to a file
       * \param file -> pointer to the file to use */
      void saveAsCompleted(ofstream* file);

      /*! Load the completed mission from a definitions file
       * \param def -> pointer to the defintions file to load from */
      void loadAsCompleted(defParser* def);

      /*! Load the current mission state from a definitions file
       * \param def -> pointer to the defintions file to load from */
      void loadAsCurrent(defParser* def);

      /*! Define the controller as friend */
      friend class missionsController;

   protected:
      mission* next;    /**< Next mission list */
      mission* previous;/**< Previous mission on list */

      string area;        /**< Area where mission occurs (ex: Tyrol) */
      string description; /**< Mission's Description */

      int xpValue;      /**< XP received on complete the mission */

      int completed; /**< 0 if isn't completed. < 0 means failed. > 0 success */

      int tempFlag[MISSION_TEMP_FLAGS];  /**< Temporary flags to controll
                                               the mission at script. */
};

/*! The controller of missions */
class missionsController
{
   public:
      /*! Constructor */
      missionsController();
      /*! Destructor */
      ~missionsController();

      /*! Init the mission controller state to use
       * \param usedEngine -> pointer to the used engine */
      void init(void* usedEngine);

      /*! finish the use of the mission controller (cleaning it) */
      void finish();

      /*! A a new mission to the current list
       * \param missionFile -> fileName of the definition's file */
      void addNewMission(string missionFile);

      /*! Complete the mission (remove from current and put at completed)
       * \param m -> mission completed.
       * \param type -> type of completion 
       *                (<= 0 for failure, > 0 for success) */
      void completeMission(mission* m, int type);

      /*! Search for a mission on the current list
       * \param scriptFile -> filename of mission to find
       * \return -> pointer to the mission */
      mission* getCurrentMission(string scriptFile);

      /*! Treat all current missions scripts
       * \param acMap -> pointer to current opened map
       * \param NPCs -> pointer to the current NPCs list*/
      void treat(Map* acMap, characterList* NPCs);

      /*! Save all missions (completed and current) to a file
       * \param fName -> fileName to save 
       * \return -> true if save. */
      bool save(string fName);

      /*! Load all mission
       * \param fName -> name of the file to load
       * \return -> true if loaded. */
      bool load(string fName);

      /*! Get first mission at the current missions list
       * \return -> pointer to the first mission at the current list */
      mission* getFirstCurrentMission();

      /*! Get the next mission on the current list 
       * \return -> pointer to the next mission or NULL */
      mission* getNextCurrentMission();

      /*! Get first mission at the completed missions list
       * \return -> pointer to the first mission at the current list */
      mission* getFirstCompletedMission();
      
      /*! Get the next mission on the completed list 
       * \return -> pointer to the next mission or NULL */
      mission* getNextCompletedMission();

   protected:
      /*! Remove misssion from current
       * \param m-> mission to remove
       * \param del -> true if is to delete the mission. */
      void removeFromCurrent(mission* m, bool del);

      /*! Remove mission from completed
       * \param m -> mission to remove.*/
      void removeFromCompleted(mission* m);

      /*! Add mission to completed list
       * \param m -> new completed mission */
      void addCompleted(mission* m);

      /*! Add mission to current list
       * \param m -> pointer to the mission to add */
      void addCurrent(mission* m);

      static mission* completed;  /**< The List of Completed Missions */
      static mission* curComp;    /**< Current completed navigation pointer */
      static int totalCompleted;  /**< Total number of Completed Missions */

      static mission* current;    /**< The list of Current Missions */
      static mission* curCur;     /**< Current current navigation pointer */
      static mission* curTreat;   /**< Pointer to the mission to treat next  */
      static int totalCurrent;    /**< Total number of current missions */

      static void* pEngine;       /**< Pointer to the current engine */
};

#endif

