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
   int estado = ON_INIT;
   int charCreation;
   
   while(result != EXIT_GAME)
   {
       if(result == NEW_GAME)
       {
          charCreation = Engine->TelaPersonagens(&tituloID);
          if( charCreation == CHAR_CONFIRM)
          {
             Engine->CarregaMapa("../data/mapas/casa.map",1);
          }
       }

       if(result == OPTIONS)
       {
          Engine->TelaOpcoes(&tituloID);
       }
       #ifdef REDE
          Engine->server = server;
       #endif
      //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glDeleteTextures(1,&tituloID);

      if( (result == CONTINUE_GAME) || 
          ( ((result == NEW_GAME)) && (charCreation == CHAR_CONFIRM) ) ) 
      {
         Engine->Rodar(screen);
         estado = IN_GAME;
         result = Engine->TelaInicial(estado,&tituloID);
      }
      else
      {
         result = Engine->TelaInicial(estado,&tituloID);
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


