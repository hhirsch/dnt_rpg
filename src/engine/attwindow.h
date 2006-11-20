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
      /*! Constructor */
      attWindow(skills* sk, interface* inter);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(Tobjeto* object, int eventInfo, interface* inter);

      janela*      window;           /**< Pointer to the internal window */

   private:
      int points[6];              /**< Rolled Points */
      int attPointsIndex[6];      /**< Index of the selected points for att */
      bool used[6];               /**< Mark Used Rolled Points */

      skills*      externalSkill; /**< Pointer to the edited skillList */
      lang language;              /**< Language internationalization */

      quadroTexto* rolledPoints;
      quadroTexto* attPoints[6];
      quadroTexto* attTotals[6];
      quadroTexto* attMods[6];
      quadroTexto* textDesc;
      quadroTexto* textDescTitle;

      botao* attButtonNext[6];
      botao* attButtonPrev[6];
      botao* rerollButton;
      botao* clearButton;
      botao* buttonConfirm;
      botao* buttonCancel;

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
};

#endif

