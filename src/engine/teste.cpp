/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "../gui/farso.h"
#include "dccnit.h"
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
   Farso_Iniciar(&screen,"DccNiTghtmare 0.0.2CVS");
   engine* Engine = new engine();
   

   Engine->Iniciar(screen);

   int result = Engine->TelaInicial(ON_INIT);

   while(result != EXIT_GAME)
   {
       if(result == NEW_GAME)
       {
          Engine->CarregaMapa("../data/mapas/quatro.map",1);
       }
       #ifdef REDE
          Engine->server = server;
       #endif
       //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

       Engine->Rodar(screen);
       result = Engine->TelaInicial(IN_GAME);
       #ifdef REDE
          free(Engine->server); 
       #endif
   }
 
       
   delete(Engine);

   return(0); 
}


