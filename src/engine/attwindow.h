#ifndef _attwindow_h
#define _attwindow_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../gui/farso.h"
#include "../lang/lang.h"
#include "../classes/skills.h"

#define ATTW_CANCEL  0
#define ATTW_CONFIRM 1
#define ATTW_OTHER   2

/*! Attributes Roll Window */
class attWindow
{
   public:
      /*! Constructor 
       * \param sk -> skill List Descriptions
       * \param savSkill -> skill to be edited
       * \param inter -> GUI interface to be used
       * \param usePreviousValues -> if will use previous values or new ones */
      attWindow(skills* sk, skills* savSkill, interface* inter, 
                bool usePreviousValues);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \param proj -> projection matrix
       * \param modl -> Model Viewn matrix
       * \param viewPort -> ViewPort Vector
       * \return 0 on close, 1 otherwise */
      int treat(guiObject* object, int eventInfo, interface* inter,
                GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      janela*      window;        /**< Pointer to the internal window */

   private:
      int points[6];              /**< Rolled Points */
      int attPointsIndex[6];      /**< Index of the selected points for att */
      bool used[6];               /**< Mark Used Rolled Points */

      skills* externalSkill;      /**< Pointer to skill List with description */
      skills* saveSkill;          /**< Pointer to thje edited skill List */
      lang language;              /**< Language internationalization */

      textBox* rolledPoints;  /**< Rolled Points Text */
      textBox* attPoints[6];  /**< Attributes Points Texts */
      textBox* attTotals[6];  /**< Attributes Totals Texts */
      textBox* attMods[6];    /**< Attributes Totals Modifiers */
      textBox* textDesc;      /**< Description Text */
      textBox* textDescTitle; /**< Description Title Text */

      button* attButtonNext[6];    /**< Next Buttons */
      button* attButtonPrev[6];    /**< Previous Buttons */
      button* rerollButton;        /**< ReRoll Button */
      button* clearButton;         /**< Clear Button */
      button* autoButton;          /**< Auto Assign Button */
      button* buttonConfirm;       /**< Confirm Button */
      button* buttonCancel;        /**< Cancel Button */

      /*! Roll one Att Dices */
      int rollDices();
      /*! Roll All Att Dices */
      void rollAllDices();

      /*! Gets next (circular) not used rolled point 
       *  \param att -> attribute-1 number
       *  \return -> integer with the index of the point */
      int nextAvaiblePoints(int att);
      /*! Gets previous (circular) not used rolled point 
       *  \param att -> attribute-1 number
       *  \return -> integer with the index of the point */
      int previousAvaiblePoints(int att);
      /*! Clears the selected list of points. */
      void clear();
      /*! Gets the att bonus modifier and puts on screen 
       * \param att -> att-1 number
       * \return attribute bonus modifier */
      int assignAttMod(int att);
      /*! Verifies if all attributes are assigned
       * \return true if all are assigned, false otherwise*/
      bool allAssigned();
      /*! Auto Assign all values to Attributes */
      void autoAssign();
      /*! Assign Previous values to the dices */
      void assignPreviousToDices();
      /*! Assign Previous values to the attributes */
      void assignPreviousValues();

};

#endif

