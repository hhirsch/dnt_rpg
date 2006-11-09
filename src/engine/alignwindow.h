#ifndef _alignwindow_h
#define _alignwindow_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../gui/farso.h"
#include "../lang/lang.h"
#include "../classes/align.h"

#define ALIGNW_CANCEL  0
#define ALIGNW_CONFIRM 1
#define ALIGNW_OTHER   2

/*! Alignments Window */
class alignWindow
{
   public:
      /*! Constructor */
      alignWindow(aligns* alg, interface* inter);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(Tobjeto* object, int eventInfo,interface* inter, align** actual);

      janela*      window;           /**< Pointer to the internal window */

   private:
      aligns*      externalAligns;   /**< Pointer to the Aligns List */
      align*       actualAlign;      /**< Pointer to actual aligned */

      botao* buttonConfirm;
      botao* buttonCancel;

      botao* buttonPrevious;
      botao* buttonNext;

      quadroTexto* textName;
      quadroTexto* textDesc;

      figura* alignImage;

      lang language;                 /**< Language internationalization */

};

#endif
