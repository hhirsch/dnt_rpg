/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "../gui/farso.h"
#include "dccnit.h"
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <stdlib.h>


/*********************************************************************
 *                           Corpo Principal                         *
 *********************************************************************/
int main(int argc, char **argv)
{
  
   SDL_Surface *screen;
   
   #ifdef REDE
      printf("IP ou Nome do servidor: ");
      char* server = (char*) malloc(55*sizeof(char));
      scanf("%s",server);
      printf("Vou conectar entao a %s\n",server);
   #endif   
   Farso_Iniciar(&screen,"DccNiTghtmare 0.0.2CVS");
   engine* Engine = new engine();
   

   Engine->Iniciar(screen);
   while(Engine->TelaInicial())
   {
       Engine->CarregaMapa("../data/mapas/teste.map",1);
       #ifdef REDE
          Engine->server = server;
       #endif
       //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

       Engine->Rodar(screen);
       #ifdef REDE
          free(Engine->server); 
       #endif
   }
 
       
   delete(Engine);

   return(0); 
}


