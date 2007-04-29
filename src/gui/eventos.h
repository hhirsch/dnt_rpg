/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _eventos_h
#define _eventos_h

#include <string.h>
#include "janela.h"
#include "botao.h"
#include "bartexto.h"
#include "cxsel.h"
#include "mouse.h"
#include "seltexto.h"
#include "menu.h"
#include "lista.h"

/* Constantes de Retorno */
#define NADA                -1
#define SAIR                 0
#define SELTEXTOMODIFICADA   1
#define MENUSELECIONADO      2
#define BOTAOPRESSIONADO     3
#define BOTAONAOPRESSIONADO  4
#define BARRATEXTOESCRITA    5
#define CXSELMODIFICADA      6
#define JANELAFECHADA        7
#define JANELAATIVADA        8
#define JANELAMOVIMENTADA    9
#define JANELACLICADA       10
#define TABBOTAOPRESSIONADO 11
#define TABBOTAOEMPRESSAO   12
#define BOTAOEMPRESSAO      13
#define MENUMODIFICADO      14
#define SELTEXTOSELECIONADA 15

/* Constantes de Foco */
#define FOCO_JANELA       1
#define FOCO_BARRATEXTO   2
#define FOCO_BOTAO        3
#define FOCO_CXSEL        4
#define FOCO_MENU         5
#define FOCO_SELTEXTO     6
#define FOCO_JANELAMOVER  7
#define FOCO_MENUJANELA   8
#define FOCO_TABBUTTON    9
#define FOCO_JOGO         100

/*! Interface is the GUI upper level class. Is from it that all GUI events
 * are treated, and all actions maybe be takken. */
class interface
{
   public:
      /*! Constructor
       * \param arqfundo -> name of a image file to be the background. 
       *                    NULL if no background is used*/
      interface(char* arqFundo);
      /*! Destructor  */
      ~interface();

      /*! Manipulate Events on Gui.
       * \param x -> mouseX position
       * \param y -> mouseY position
       * \param Mbotao -> mous button state
       * \param tecla -> keyboard state
       * \param eventInfo -> variable that will receive the information 
       *                     on the event that occurs
       * \return -> pointer to the active objet of the action */
      Tobjeto* manipulateEvents(int x, int y, Uint8 Mbotao, Uint8* tecla,
                                int* eventInfo);
      /*! Draw all the interface (all windows and objects)
       * \param proj -> project view matrix
       * \param modl -> model view matrix
       * \param viewPort -> the viewport vector*/
      void draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);
      /*! Clear the internal pointer to the actived object. 
       *  Use it when close by force some window */
      void clearActiveObject();
      /*! Close a window
       * \param jan -> pointer to the window */
      void closeWindow(janela *jan);
      /*! Close all interface openned windows */
      void closeAllWindows();
      /*! Verify if the mouse is on some window or not
       * \param mouseX -> mouse x position
       * \param mouseZ -> mouse z position
       * \return true if mouse is in some window, false otherwise. */
      bool mouseOnGui(int mouseX, int mouseY);
      /*! Insert a window on the interface
       * \param xa -> x1 window position
       * \param ya -> y1 window position
       * \param xb -> x2 window posiiton 
       * \param yb -> y2 window position
       * \param text -> title of the window 
       * \param maximiz -> if can be maximized
       * \param redmens -> if can redmensionate (not used anymore)
       * \return -> pointer to the inserted window */
      janela* insertWindow(int xa,int ya,int xb,int yb,const char *text,
                           int maximiz,int redmens);
      /*! Open a Window
       * \param jan -> pointer to the window opened */
      void openWindow(janela* jan);
      
      int foco;    /**< Current GUI focus (on some element of on GAME) */

   private:
      SDL_Surface* fundo;     /**< background surface, if has one */
      Tlista* objects;        /**< some no window objects */
      Ljanela* ljan;          /**< windows list */
      Tobjeto* objAtivo;      /**< pointer to the actived object */
      Tobjeto* chamador;      /**< pointer to the caller object */

};



#endif

