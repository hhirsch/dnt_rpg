#ifndef _dccnit_h
#define _dccnit_h

#include "../GUI/farso.h"
//#include "../MAPA/mapa.h"
#include "personagens.h"
#include "luta.h"

class engine
{
   public:
 
      /* Construtor da Engine
       * map -> ponteiro para o mapa utilizado pela engine */
      engine(/*Tmapa* map*/);
      /* Destruidor da engine */
      ~engine();
 
      //Tmapa mapa;        // Mapa
      Lpersonagem *NPCs; // NPCs presentes na tela carregada atual
      Lpersonagem *PCs;  // PCs na engine
      int x,y;           // Coordenada Central do mapa atualmente visto

      int RotacaoX,RotacaoY,RotacaoZ; //variaveis de testeatual

      /* Inicia a estrutura da Engine para utilizacao
       * screen -> ponteiro para a tela3d atual */
      void Iniciar(SDL_Surface *screen);

      /* Faz a rodagem principal da engine
       * surface -> ponteiro para a tela3d atual */
      int  Rodar(SDL_Surface *surface);    

      /* Faz o desenho da engine na tela atualmente
       * É necessario utilizar SDL_GL_SwapBuffers();
       * apos o termino da funcao. */
      void Desenhar();

   private:

      /* Redefine a tela para o tamanho da tela atual
       * screen -> ponteiro para a tela3d atual */
      void Redmensiona(SDL_Surface *screen);

      /* Trata as opcoes de Entrada e Saida na Engine
       * screen -> ponteiro para a tela3d atual */
      int  TrataES(SDL_Surface *screen);

 
};

#endif

