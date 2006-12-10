#ifndef _politic_h
#define _politc_h

#include "../../ia/potentAgent.h"
#include "../../ia/fuzzy.h"
#include "states.h"
#include "briefcase.h"

#define WORK_INTERVAL         60000 /**< Interval when politcs Work on congress*/
#define WORK_TIME              1000 /**< Time spent on Congress Work */
#define INTERROGATION_TIME     2000 /**< Time Spent on Interrogation */
#define PRETEND_TIME          40000 /**< Time Spent pretending */

#define BUSTED_VALUE           5000 /**< Value to spent when Busted! */


/*! The politic Agent Class */
class politic: public potentAgent
{
   public:
     /*! Constructor */
     politic(int* score);
     /*! Destructor */
     ~politic();

     /*! Actualize the Position and State machine of the agent
      * \param workTime -> true if is the congress work time */
     void actualizeMachineAndPosition(bool workTime);

     /*! Get the actual owned briefCase by the politic
      * \return the oned brief case, if has one, NULL otherwise */
     briefCase* currentBriefCase();

     /*! Set the actual owned briefCase
      * \param br -> briefCase to be owned */
     void setBriefCase(briefCase* br);

     /*! Define Rogue Group Area (where delivers briefcases) */
     void setBaseArea(int group, GLfloat x1,GLfloat z1, GLfloat x2, GLfloat z2);

     /*! Set Congress Center Point */
     void setCongress(GLfloat x, GLfloat z);

     /*! Set Federal Police Base center point. */
     void setFederal(GLfloat x, GLfloat z);

     /*! Gets the actual State of the politic
      * \return state of the politic */
     int getState();

     /*! Set The politic State
      * \param st -> state to be seted */
     void setState(int st);

     int getGroup(){return(rogueGroup);};

     void fuzzyInit();

     float getTimePercent();
     float getAvaricePercent();

     bool busted;             /**< If agent is busted or not */
     
   private:
     int rogueGroup;          /**< Number of the rogue group */
     int lastWork;            /**< Aditional Timer */
     int avarice;             /**< Avarice of the Politic */
     GLuint lastSteal;        /**< Last Time when he steals something */
     stateMachine* state;     /**< State Machine */
     briefCase* brief;        /**< BriefCase owned */

     GLfloat baseX1,          /**< Rogue group X1 coordinate */
             baseX2,          /**< Rogue group X2 coordinate */
             baseZ1,          /**< Rogue group Z1 coordinate */
             baseZ2;          /**< Rogue group Z2 coordinate */
     
     GLfloat congressX,       /**< Congress X center Coordinate */
             congressZ;       /**< Congress Z center Coordinate */
     
     GLfloat federalX,        /**< Federal Police X center coordinate */
             federalZ;        /**< Federal Police Z center coordinate */

     fuzzy fuzzyLogic;          /**< Fuzzy Logic Controler */
     fuzzyFunction* muchMoney;  /**< Much Money Function */
     fuzzyFunction* normalMoney;/**< Normal Money Function */
     fuzzyFunction* fewMoney;   /**< Few Money Function */
     fuzzyFunction* muchTime;   /**< Much Time Function */
     fuzzyFunction* normalTime; /**< Normal Time Function */
     fuzzyFunction* fewTime;    /**< Few Time Function */

     int* groupScore;         /**< Pointer to Group Score */


};

#endif

