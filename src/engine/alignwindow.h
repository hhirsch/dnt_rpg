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
      /*! Constructor
       * \param alg -> alignmnet list
       * \param inter -> GUI interface to use */
      alignWindow(aligns* alg, interface* inter);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \param actual -> pointer to the actual alignent
       * \return 0 on close, 1 otherwise */
      int treat(Tobjeto* object, int eventInfo,interface* inter, align** actual);

      janela*      window;           /**< Pointer to the internal window */

   private:
      aligns*      externalAligns;   /**< Pointer to the Aligns List */
      align*       actualAlign;      /**< Pointer to actual aligned */

      botao* buttonConfirm;          /**< Confirm Button */
      botao* buttonCancel;           /**< Cancel Button */

      botao* buttonPrevious;         /**< Previous Button */
      botao* buttonNext;             /**< Next Button */

      quadroTexto* textName;         /**< Text Name */
      rolBar* textDesc;              /**< Text Description */

      figura* alignImage;            /**< Alignment Image */

      lang language;                 /**< Language internationalization */

};

#endif
