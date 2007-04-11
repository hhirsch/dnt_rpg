#ifndef _aspectwindow_h
#define _aspectwindow_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "../gui/farso.h"
#include "../lang/lang.h"
#include "personagens.h"

#define ASPECTW_CANCEL  0 /**< Cancel return from Aspect Window */
#define ASPECTW_CONFIRM 1 /**< Confirm return from Aspect window */
#define ASPECTW_OTHER   2 /**< Other return from Aspect Window */

/*! Image to portrait */
class portraitImage
{
   public:
      portraitImage(){image = NULL;};
      ~portraitImage(){if(image){SDL_FreeSurface(image);}};
      SDL_Surface* image;         /**< Image to portrait */
      string imageFile;           /**< FileName of the Image */
};

/*! The Aspect Window Class */
class aspectWindow
{
   public:
      /*! Constructor
       * \param character -> pointer to the edited character
       * \param inter -> pointer to the GUI used interface */
      aspectWindow(personagem* character, interface* inter);
      /*! Destructor */
      ~aspectWindow();

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(Tobjeto* object, int eventInfo, interface* inter);

   private:
      /*! Load All Portrait Images */
      void loadImages();

      personagem* usedCharacter;    /**< Pointer to the used character */

      botao* buttonConfirm;         /**< Confirm Button */
      botao* buttonCancel;          /**< Cancel Button */
      botao* buttonNextImage;       /**< Next Image Button */
      botao* buttonPreviousImage;   /**< Previous Image Button */
      barraTexto* textName;         /**< Character Name Text */
      barraTexto* textAge;          /**< Character Age Text */
      cxSel* cxSelSexM;             /**< Feminine Sex SelBox */
      cxSel* cxSelSexF;             /**< Masculine Sex SelBox */
      cxSel* cxSelSexO;             /**< Other Sex SelBox */
      figura* figurePortrait;       /**< Portrait Figure */
      janela* window;               /**< Internal pointer to the window */

      lang language;                /**< Language internationalized */

      int totalImages;              /**< Number of total images */
      int curImage;                 /**< Nuber of the Current Image */
      portraitImage* images;        /**< All Portrait Images */
};

#endif

