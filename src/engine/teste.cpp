/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "../gui/farso.h"
#include "dccnit.h"
#include "../lang/lang.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <stdlib.h>


/*********************************************************************
 *                           Main Code                               *
 *********************************************************************/
int main(int argc, char **argv)
{
  
   SDL_Surface *screen;
   
   #ifdef REDE
      printf("Server IP or Name: ");
      char* server = (char*) malloc(55*sizeof(char));
      scanf("%s",server);
      printf("I'll conect to %s\n",server);
   #endif   
   Farso_Iniciar(&screen,"DccNiTghtmare 0.0.2");
   engine* Engine = new engine();
   

   Engine->Iniciar(screen);
 
   GLuint tituloID;
   int result = Engine->TelaInicial(ON_INIT,&tituloID);
   int charCreation = CHAR_OTHER;
   int tempo = SDL_GetTicks();
   int tempoAnterior = 0;
   Uint8* keys;
   int x,y;

   while(result != EXIT_GAME)
   {
       if(result == NEW_GAME)
       {
          skills* sk = new skills(SKILLS_DIR,"../data/skills/skills.skl"); 
          skillWindow* skWindow = new skillWindow(sk, 20, Engine->gui);
          while( (charCreation != CHAR_CANCEL) && 
                 (charCreation != CHAR_CONFIRM))
          {
             tempo = SDL_GetTicks();
             if(tempo - tempoAnterior > 20) 
             {
                SDL_PumpEvents();
                glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR);
                glClearColor(0,0,0,0);
                keys = SDL_GetKeyState(NULL);
                Uint8 Mbotao = SDL_GetMouseState(&x,&y);
                Engine->gui->ManipulaEventos(x,y,Mbotao,keys);
                AtualizaTela2D(tituloID,Engine->proj,Engine->modl,
                               Engine->viewPort,0,0,799,599,0.011);
                Engine->gui->Desenhar(Engine->proj,Engine->modl,
                                      Engine->viewPort);
                glFlush();
                SDL_GL_SwapBuffers();
             }
             charCreation = skWindow->treat(Engine->gui);
          }
          if( charCreation == CHAR_CONFIRM)
          {
             Engine->CarregaMapa("../data/mapas/casa.map",1);
          }
          delete(sk);
          delete(skWindow);
       }
       #ifdef REDE
          Engine->server = server;
       #endif
       //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glDeleteTextures(1,&tituloID);

      if(charCreation == CHAR_CONFIRM)
      {
         Engine->Rodar(screen);
         result = Engine->TelaInicial(IN_GAME,&tituloID);
      }
      else
      {
         result = Engine->TelaInicial(ON_INIT,&tituloID);
      }
      charCreation = CHAR_OTHER;
       
       #ifdef REDE
          free(Engine->server); 
       #endif
   }

   glDeleteTextures(1,&tituloID); 
       
   delete(Engine);

   return(0); 
}


