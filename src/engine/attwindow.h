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
       * \param sk -> skill List
       * \param inter -> GUI interface to be used */
      attWindow(skills* sk, interface* inter);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \param proj -> projection matrix
       * \param modl -> Model Viewn matrix
       * \param viewPort -> ViewPort Vector
       * \return 0 on close, 1 otherwise */
      int treat(Tobjeto* object, int eventInfo, interface* inter,
                GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      janela*      window;        /**< Pointer to the internal window */

   private:
      int points[6];              /**< Rolled Points */
      int attPointsIndex[6];      /**< Index of the selected points for att */
      bool used[6];               /**< Mark Used Rolled Points */

      skills*      externalSkill; /**< Pointer to the edited skillList */
      lang language;              /**< Language internationalization */

      quadroTexto* rolledPoints;  /**< Rolled Points Text */
      quadroTexto* attPoints[6];  /**< Attributes Points Texts */
      quadroTexto* attTotals[6];  /**< Attributes Totals Texts */
      quadroTexto* attMods[6];    /**< Attributes Totals Modifiers */
      quadroTexto* textDesc;      /**< Description Text */
      quadroTexto* textDescTitle; /**< Description Title Text */

      botao* attButtonNext[6];    /**< Next Buttons */
      botao* attButtonPrev[6];    /**< Previous Buttons */
      botao* rerollButton;        /**< ReRoll Button */
      botao* clearButton;         /**< Clear Button */
      botao* autoButton;          /**< Auto Assign Button */
      botao* buttonConfirm;       /**< Confirm Button */
      botao* buttonCancel;        /**< Cancel Button */

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
};

#endif

