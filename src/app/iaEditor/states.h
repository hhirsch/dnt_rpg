#ifndef _states_h
#define _states_h

#include <SDL/SDL_opengl.h>

#define STATE_NONE            0

/* Politic States */
#define STATE_LOOK_OBJECT     1
#define STATE_GET_OBJECT      2
#define STATE_BRING_OBJECT    3
#define STATE_BUSTED          4
#define STATE_INTERROGATION   5
#define STATE_PRETEND         6
#define STATE_WORK            7
#define STATE_GOING_WORK      8

/* Federal Police States */
#define STATE_PATROL          10
#define STATE_CATCH           11
#define STATE_SEND_PRISON     12
#define STATE_GO_PATROL       13

/*! The Mensalao State Machine */
class stateMachine
{
   public:
      /*! Constructor
       * \param st -> initial State */
      stateMachine(int st);
      /*! Destructor */
      ~stateMachine();

      /*! Gets the Actual Machine Status 
       * \return actual state */
      int getActualState();
      /*! Gets the Time when init the actual status
       * \return time*/
      GLuint getTime();
      /*! Set to Next State
       * \param time -> current time */
      void nextState(GLuint time);
      /*! Set State to specific state 
       * \param st -> state
       * \param time -> current time*/
      void setState(int st, GLuint time);
      void setTime(GLuint time);

      
   protected:
      int state;                 /**< Current State */
      GLuint initialStateTime;   /**< Time on the State */
};

#endif


