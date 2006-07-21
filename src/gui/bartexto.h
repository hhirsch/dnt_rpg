/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _bartexto_h
#define _bartexto_h

#include <stdlib.h>
#include <string.h>
#include "cores.h"
#include "desenho.h"
#include "fonte.h"
#include "mouse.h"
#include "objeto.h"

#include <string>
using namespace std;


/*! Text Bar Definition */
class barraTexto: public Tobjeto
{
   public:
      /*! Constructor */
      barraTexto();
      /*! Destructor */
      ~barraTexto();
      int x1,              /**< Coordinate on Window */
          y1,              /**< Coordinate on Window */
          x2,              /**< Coordinate on Window */
          y2;              /**< Coordinate on Window */
      int disp,            /**< Avaible? */
      cript;               /**< Pseudo-criptography? */
      int inic,            /**< Actual first visible character on string */
          fim;             /**< Actual last visible character on string */
      string texto;        /**< Text on the bar */
      unsigned int pos;    /**< Current cursor position */
      int ultEsc;          /**< Last time when write */
      string ultChar;      /**< Last Character writed */

                           /*! After Edition Called Procedure  */
      void (*procEditada)(barraTexto* bart,SDL_Surface *screen);
      cores Cores;

      /*!
       * Draw the text bar on a surface
       * \param Xjan -> window origin
       * \param Yjan -> window origin
       * \param salvar -> always 0.
       * \param screen > suface to draw */
      void Desenhar(int Xjan, int Yjan,int salvar,SDL_Surface *screen);

      /*!
       * Make the write thing on text bar
       * \param Xjan -> window origin
       * \param Yjan -> window origin
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate
       * \param screen -> surface to draw the text bar
       * \param Mbotao -> SDL mouse buttons state info
       * \param teclas -> SDL keyboard state info */
      int Escrever(int Xjan,int Yjan,int mouseX,int mouseY,
                    SDL_Surface *screen, Uint8 Mbotao, Uint8* teclas);

   private:
      void PoeTexto(int Xjan, int Yjan,int salvar,unsigned int pos, int marca,
                         SDL_Surface *screen);


};
 
#endif
