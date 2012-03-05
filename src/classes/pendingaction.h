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

#ifndef _dnt_pending_actions_h
#define _dnt_pending_actions_h

#include <string>

#include "../etc/list.h"
#include "../engine/character.h"

#include "thing.h"
#include "actions.h"

/*! A pending action. Usually used with a script. When the script got a more
 * than one cycle function */
class pendingAction: public dntListElement
{
   public:
      /*! Constructor
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgt -> the target thing */
      pendingAction(std::string strLine, int type, character* act, thing* tgt);
      /*! Constructor
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor 
       * \param tgtX -> target X position
       * \param tgtZ -> target Z position */
      pendingAction(std::string strLine, int type, character* act, 
                    GLfloat tgtX, GLfloat tgtZ);
      /*! Constructor
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgt -> the target thing
       * \param tgtX -> target X position
       * \param tgtZ -> target Z position */
      pendingAction(std::string strLine, int type, character* act, thing* tgt,
                    GLfloat tgtX, GLfloat tgtZ);
      /*! Constructor
       * \param strLine -> the line of the script the action was
       * \param type -> type of the action
       * \param v -> integer value to pass */
      pendingAction(std::string strLine, int type, int v);
      /*! Destructor */
      ~pendingAction();

      /*! Constructor
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgt -> the target thing
       * \param tgtX -> target X position
       * \param tgtZ -> target Z position
       * \param v -> integer value */
      void init(std::string strLine, int type, character* act, thing* tgt,
                GLfloat tgtX, GLfloat tgtZ, int v);

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
      friend class pendingActionController;

      /*! Return the toggle state. the toggle is used to set an internal
       * state, like the false while pending for the result of the astar
       * \return toggle state. */
      bool getToggle();

      /*! set the toggle state */
      void setToggle(bool b);

      /*! Set the action as a mission one
       * \note -> mission actions aren't aborted */
      void setAsMissionAction();

      /*! Verify if the action is a mission one
       * \return -> true if is a mission action */
      bool isMissionAction();

      /*! Get the script line where the action was called
       * \return -> the line, with the action's function changed to the
       *            temporary variable. */
      std::string getScriptLine();

   private:
      std::string scriptLine; /**< The line of the script that calls the more 
                                   than one cycle function. */

      character* actor;    /**< The actor of the action */
      thing* target;       /**< The target of the action. Can be NULL if
                                no THING target is defined. */

      GLfloat targetX;     /**< The target X position. Usually used when 
                                target thing is NULL */
      GLfloat targetZ;     /**< The target Z position. Usually used when
                                target thing is NULL */

      int value;           /**< Another value */

      bool done;          /**< Will be true when the action is done */
      bool returnValue;   /**< All pending actions always return a boolean
                               value. */
      int actionType;     /**< The type of the action */

      Uint32 initedTime;  /**< Time when inited */

      bool toggle;        /**< The state of the action */

      bool missionAction; /**< True if is a mission action (that can't
                               be stopped) */
};

/*! List of pending actions */
class pendingActionList: public dntList
{
   public:
      /*! Constructor */
      pendingActionList();
      /*! Destructor */
      ~pendingActionList();

   protected:
      /*! Delete a pending action memory
       * \param obj -> pointer to the pendingAction to delete */
      void freeElement(dntListElement* obj);
};

/*! the controller of actions taked by characters on the game */
class pendingActionController
{
   public:
      /*! Constructor */
      pendingActionController();
      /*! Destructor */
      ~pendingActionController();

      /*! Add action to the controller
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgt -> the target thing
       * \param mission -> if is a mission action
       * \return pointer to the pending action added */
      pendingAction* addAction(std::string strLine, int type, character* act, 
                               thing* tgt, bool mission);
      /*! Add action to the controller
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgtX -> target X position
       * \param tgtZ -> target Z position
       * \param mission -> if is a mission action
       * \return pointer to the pnding action added */
      pendingAction* addAction(std::string strLine, int type, character* act, 
                               GLfloat tgtX, GLfloat tgtZ, bool mission);
      /*! Add action to the controller
       * \param strLine -> the line of the script the action was 
       * \param type -> the type of the action
       * \param act -> the character actor
       * \param tgt -> the target thing
       * \param tgtX -> target X position
       * \param tgtZ -> target Z position
       * \param mission -> if is a mission action
       * \return pointer to the pending action added */
      pendingAction* addAction(std::string strLine, int type, character* act, 
                               thing* tgt, GLfloat tgtX, GLfloat tgtZ, 
                               bool mission);
      /*! Constructor
       * \param strLine -> the line of the script the action was
       * \param type -> type of the action
       * \param mission -> if is a mission action
       * \param v -> integer value to pass */
      pendingAction* addAction(std::string strLine, int type, int v, 
                               bool mission);
      /*! add action to the controller
       * \param act -> pointer to the action to add
       * \param mission -> if is a mission action
       * \return -> action pointer. the same of the parameter. */
      pendingAction* addAction(pendingAction* act, bool mission);

      /*! Remove Action from the controller
       * \param act -> pointer to the pendingAction to remove */
      void removeAction(pendingAction* act);

      /*! Remove all Actions from the controller
       * \param missions -> true if will remove missions actions too */
      void removeAllActions(bool missions=false);

      /*! Abort All actions at the controller (usefull when changing from
       *  battle mode to real time mode). */
      void abortAllActions();

      /*! Get the total actions on list
       * \return number of actions on the list */
      int getTotal();

      /*! Get the first action on the list
       * \return pointer to the first action on the list */
      pendingAction* getFirst();

      /*! Treat the actions on the controller */
      void treatActions(Map* actualMap, bool fightMode);

      /*! Set the character Lists of the action controller */
      void setCharacterLists(characterList* npcs, characterList* pcs);

   private:
      pendingActionList actions; /**< Pending Actions List */

      characterList* NPCs;    /**< Current PCs List */
      characterList* PCs;     /**< Current NPCs List */
};

#endif

