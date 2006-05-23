#ifndef _tabbotao_h
#define _tabbotao_h

#include "figura.h"
#include "cores.h"

#define MAX_TABBOTOES 20

typedef struct 
{
   int x1, y1, x2, y2;
   int (*procPres)(void *jan,void *ljan,SDL_Surface *screen);
}quad;


class tabBotao: public figura
{
   public:
      tabBotao(int x,int y,const char* arquivo);
      ~tabBotao(){};
      void inserirBotao(int x1, int y1, int x2, int y2, 
                        int (*proc)(void *jan,void *ljan,SDL_Surface *screen));
      void Desenhar(int mouseX, int mouseY, int Xjan, int Yjan,
                    SDL_Surface *screen);
      bool VerificarSobrePosicao(int mouseX, int mouseY, int Xjan, int Yjan);

   private:
      int numBotoes;
      quad Botoes[MAX_TABBOTOES];
};


#endif

