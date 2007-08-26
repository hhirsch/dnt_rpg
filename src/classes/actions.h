#ifndef _actions_h
#define _actions_h

/** \file actions.h
    The actions are define in two manners: by internal number and by string
   name. The string is usually to be loaded from a file, and the number is
   the internal representation on game structs.
*/

#include <string>
using namespace std;

#include "../engine/character.h"
#include "thing.h"

#define ACT_OPEN                  0
#define ACT_ATTACK                1
#define ACT_DRINK                 2
#define ACT_EAT                   3
#define ACT_DEFEND                4
#define ACT_TALK                  5
#define ACT_CLOSE                 6
#define ACT_MOVE                  7
#define ACT_TAKE                  8
#define ACT_FREE                  9
#define ACT_MODIFY_OWN_CONCEPT   10
#define ACT_MODIFY_OTHER_CONCEPT 11
#define ACT_CONJURE              12
#define ACT_MODIFY_OBJECT        13
#define ACT_CURE                 14
#define ACT_FIX                  15
#define ACT_BREAK                16

#define ACT_STR_OPEN                 "ACT_OPEN"
#define ACT_STR_ATTACK               "ACT_ATTACK"
#define ACT_STR_DRINK                "ACT_DRINK"
#define ACT_STR_EAT                  "ACT_EAT"
#define ACT_STR_DEFEND               "ACT_DEFEND"
#define ACT_STR_TALK                 "ACT_TALK"
#define ACT_STR_CLOSE                "ACT_CLOSE"
#define ACT_STR_MOVE                 "ACT_MOVE"
#define ACT_STR_TAKE                 "ACT_TAKE"
#define ACT_STR_FREE                 "ACT_FREE"
#define ACT_STR_MODIFY_OWN_CONCEPT   "ACT_MODIFY_OWN_CONCEPT"
#define ACT_STR_MODIFY_OTHER_CONCEPT "ACT_MODIFY_OTHER_CONCEPT"
#define ACT_STR_CONJURE              "ACT_CONJURE"
#define ACT_STR_MODIFY_OBJECT        "ACT_MODIFY_OBJECT"
#define ACT_STR_CURE                 "ACT_CURE"
#define ACT_STR_FIX                  "ACT_FIX"
#define ACT_STR_BREAK                "ACT_BREAK"


#define ACT_TYPE_FREE_ACTION       0
#define ACT_TYPE_NORMAL_ACTION     1
#define ACT_TYPE_SECUNDARY_ACTION  2
#define ACT_TYPE_NO_ACTION         3

#define ACT_TYPE_STR_FREE_ACTION       "ACT_TYPE_FREE_ACTION"
#define ACT_TYPE_STR_NORMAL_ACTION     "ACT_TYPE_NORMAL_ACTION"
#define ACT_TYPE_STR_SECUNDARY_ACTION  "ACT_TYPE_SECUNDARY_ACTION"
#define ACT_TYPE_STR_NO_ACTION         "ACT_TYPE_NO_ACTION"

/*!
 ************************************************************* 
 * Return the number of the action with name buffer  
 * \param buffer -> string with the name of the concept
 * \return correspondent action Type number.
 ***************************************************************/
int numberAction(string buffer);

/*!
 ************************************************************** 
 * Return the number of the Action Type with name buffer  
 * \param buffer -> string with the name of the concept
 * \return correspondent action Type number.
 ***************************************************************/
int numberActionType(string buffer);

/*! A action. Usually used with a script. When the script got a more than
 * one cycle function */
class action
{
   public:
      /*! Constructor
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgt -> the target thing */
      action(string strLine, int type, character* act, thing* tgt);
      /*! Constructor
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor 
       * \param tgtX -> target X position
       * \param tgtZ -> target Z position */
      action(string strLine, int type, character* act, 
             GLfloat tgtX, GLfloat tgtZ);
      /*! Constructor
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgt -> the target thing
       * \param tgtX -> target X position
       * \param tgtZ -> target Z position */
      action(string strLine, int type, character* act, thing* tgt,
             GLfloat tgtX, GLfloat tgtZ);
      /*! Destructor */
      ~action();

      /*! Constructor
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgt -> the target thing
       * \param tgtX -> target X position
       * \param tgtZ -> target Z position */
      void init(string strLine, int type, character* act, thing* tgt,
                GLfloat tgtX, GLfloat tgtZ);

      /*! Verify if the action is still running
       * \return true if is still running */
      bool isRunning();

      /*! Set the action as if it ended
       * \param result -> result of the action (if is successfull, or not)*/
      void setAsEnded(bool result);

      /*! Get the type of the action
       * \return -> type of the action */
      int getType();

      /*! Get the actior of the action
       * \return -> pointer to the character actor */
      character* getActor();

      /*! Get the target thing
       * \return -> pointer to the target thing */
      thing* getTargetThing();

      /*! Get the target position
       * \param x -> the X position
       * \param z -> the Z position */
      void getTargetPosition(GLfloat& x, GLfloat& z);

      /*! Get the return value of the action
       * \return -> boolean if the action was succesfull, false otherwise */
      bool getReturnValue();

      /*! Define the controller as friend class */
      friend class actionController;

      /*! Return the toggle state. the toggle is used to set an internal
       * state, like the false while pending for the result of the astar
       * \return toggle state. */
      bool getToggle();

      /*! set the toggle state */
      void setToggle(bool b);

      /*! Get the script line where the action was called
       * \return -> the line, with the action's function changed to the
       *            temporary variable. */
      string getScriptLine();

      action* next;       /**< Next action on the list */
      action* previous;   /**< Previous action on the list */

   private:
      string scriptLine;   /**< The line of the script that calls the more than
                                one cycle function. */

      character* actor;    /**< The actor of the action */
      thing* target;       /**< The target of the action. Can be NULL if
                                no THING target is defined. */

      GLfloat targetX;     /**< The target X position. Usually used when 
                                target thing is NULL */
      GLfloat targetZ;     /**< The target Z position. Usually used when
                                target thing is NULL */

      bool done;          /**< Will be true when the action is done */
      bool returnValue;   /**< All pending actions always return a boolean
                               value. */
      int actionType;     /**< The type of the action */

      bool toggle;        /**< The state of the action */
};

/*! the controller of actions taked by characters on the game */
class actionController
{
   public:
      /*! Constructor */
      actionController();
      /*! Destructor */
      ~actionController();

      /*! Add action to the controller
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgt -> the target thing
       * \return pointer to the action added */
      action* addAction(string strLine, int type, character* act, thing* tgt);
      /*! Add action to the controller
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgtX -> target X position
       * \param tgtZ -> target Z position
       * \return pointer to the action added */
      action* addAction(string strLine, int type, character* act, 
                        GLfloat tgtX, GLfloat tgtZ);
      /*! Add action to the controller
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgt -> the target thing
       * \param tgtX -> target X position
       * \param tgtZ -> target Z position
       * \return pointer to the action added */
      action* addAction(string strLine, int type, character* act, thing* tgt,
                        GLfloat tgtX, GLfloat tgtZ);
      /*! add action to the controller
       * \param act -> pointer to the action to add
       * \return -> action pointer. the same of the parameter. */
      action* addAction(action* act);

      /*! Remove Action from the controller
       * \param act -> pointer to the action to remove */
      void removeAction(action* act);

      /*! Get the total actions on list
       * \return number of actions on the list */
      int getTotal();

      /*! Get the first action on the list
       * \return pointer to the first action on the list */
      action* getFirst();
   private:
      action* first;    /**< First action on the list */
      int total;        /**< Total actions on the list */
};

#endif

