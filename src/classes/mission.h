#ifndef _mission_h
#define _mission_h

#include "../ia/iascript.h"

#define MISSION_TEMP_FLAGS  5    /**< Number of temp flags on missions */

#define MISSION_COMPLETION_FINISHED -1
#define MISSION_COMPLETION_FALSE     0

/*! A mission definition. The mission is basically a main script
 * with temporary and a completed variables. */
class mission: public iaScript
{
   public:
      /*! Constructor
       * \param scriptFile -> fileName of the script
       * \param usedEngine -> the pointer to the engine used */
      mission(string scriptFile, void* usedEngine);

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

      /*! Define the controller as friend */
      friend class missionsController;

   protected:
      mission* next;       /**< Next mission list */
      mission* previous;   /**< Previous mission on list */

      int xpValue;         /**< XP received on complete the mission */

      int completed;       /**< 0 if isn't completed, type of completion, if
                                is completed by any way. */

      int tempFlag[MISSION_TEMP_FLAGS];  /**< Temporary flags to controll
                                               the mission at script. */
};

/*! The controller of missions */
class missionsController
{
   public:
      /*! Constructor 
       * \param usedEngine -> pointer to the used engine */
      missionsController(void* usedEngine);
      /*! Destructor */
      ~missionsController();

      /*! A a new mission to the current list
       * \param scriptFile -> fileName of the script */
      void addNewMission(string scriptFile);

      /*! Complete the mission (remove from current and put at completed)
       * \param m -> mission completed. */
      void completeMission(mission* m);

      /*! Search for a mission on the current list
       * \param scriptFile -> filename of mission to find
       * \return -> pointer to the mission */
      mission* getCurrentMission(string scriptFile);

      /*! Treat all current missions scripts
       * \param acMap -> pointer to current opened map */
      void treat(Map* acMap);

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

      mission* completed;  /**< The List of Completed Missions */
      int totalCompleted;  /**< Total number of Completed Missions */

      mission* current;    /**< The list of Current Missions */
      int totalCurrent;    /**< Total number of current missions */

      void* pEngine;       /**< Pointer to the current engine */
};

#endif

