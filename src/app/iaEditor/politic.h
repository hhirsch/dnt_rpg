#ifndef _politic_h
#define _politc_h

#include "../../ia/potentAgent.h"
#include "states.h"
#include "briefcase.h"

#define WORK_INTERVAL         60000 /**< Interval when politcs Work on congress*/
#define WORK_TIME              1000 /**< Time spent on Congress Work */
#define INTERROGATION_TIME     2000 /**< Time Spent on Interrogation */
#define PRETEND_TIME          40000 /**< Time Spent pretending */

/*! The politic Agent Class */
class politic: public potentAgent
{
   public:
     /*! Constructor */
     politic();
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
     void setBaseArea(int group, GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2);

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
     
   private:
     int rogueGroup;          /**< Number of the rogue group */
     int lastWork;            /**< Aditional Timer */
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

};

#endif

