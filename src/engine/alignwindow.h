#ifndef _alignwindow_h
#define _alignwindow_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <libintl.h>
#include "../gui/farso.h"
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
       * \param inter -> GUI interface to use
       * \param actual -> pointer to the actual alignent */
      alignWindow(aligns* alg, interface* inter, align** actual);

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(guiObject* object, int eventInfo,interface* inter );

      window*      intWindow;        /**< Pointer to the internal window */

   private:
      aligns*      externalAligns;   /**< Pointer to the Aligns List */
      align*       actualAlign;      /**< Pointer to actual aligned */
      align**      choosedAlign;     /**< Pointer to the pointer of choosed */

      button* buttonConfirm;          /**< Confirm Button */
      button* buttonCancel;           /**< Cancel Button */

      button* buttonPrevious;         /**< Previous Button */
      button* buttonNext;             /**< Next Button */

      textBox* textName;             /**< Text Name */
      rolBar* textDesc;              /**< Text Description */

      picture* alignImage;           /**< Alignment Image */

};

#endif
