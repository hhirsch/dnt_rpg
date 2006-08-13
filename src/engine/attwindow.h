#ifndef _attwindow_h
#define _attwindow_h

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
      bool used[6];               /**< Mark Used Rolled Points */

      skills*      externalSkill; /**< Pointer to the edited skillList */

      lang language;              /**< Language internationalization */

      quadroTexto* rolledPoints;
      quadroTexto* attPoints[6];
      int attPointsIndex[6];
      quadroTexto* attMods[6];
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

      int nextAvaiblePoints(int att);
      int previousAvaiblePoints(int att);

      void clear();
};

#endif

