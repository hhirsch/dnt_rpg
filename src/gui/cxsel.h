/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _cxsel_h
#define _cxsel_h

#include "desenho.h"
#include "objeto.h"
#include "cores.h"

class cxSel: public Tobjeto
{
   public:
      int x,y;               /* Coordenadas na tela */
      int selecionada;       /* != 0 se selecionada */
      cores Cores;   

      /* Desenha a CxSel na superficie
       * cx -> cxSel a ser desenhada
       * salvar -> != 0 se for para atualizar a superficie
       * screen -> superficie na qual sera desenhada a cxSel*/
      void Desenhar(SDL_Surface *screen);

};                  /* CAIXA DE SELECAO */

#endif

