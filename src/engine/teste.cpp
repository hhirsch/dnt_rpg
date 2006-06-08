/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "../gui/farso.h"
#include "dccnit.h"
#include "../lang/lang.h"
#include <SDL/SDL_opengl.h>
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
   Farso_Iniciar(&screen,"DccNiTghtmare");
   engine* Engine = new engine();
   

   Engine->Init(screen);
 
   GLuint tituloID;
   int result = Engine->InitialScreen(ON_INIT,&tituloID,true);
   int estado = ON_INIT;
   int charCreation;
   bool reloadMusic;
   
   while(result != EXIT_GAME)
   {
       reloadMusic = false;
       if(result == NEW_GAME)
       {
          charCreation = Engine->CharacterScreen(&tituloID);
          if( charCreation == CHAR_CONFIRM)
          {
             Engine->LoadMap("../data/mapas/rua.map",1);
          }
       }

       if(result == OPTIONS)
       {
          Engine->OptionsScreen(&tituloID);
       }
       #ifdef REDE
          Engine->server = server;
       #endif
      //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glDeleteTextures(1,&tituloID);

      if( (result == CONTINUE_GAME) || 
          ( ((result == NEW_GAME)) && (charCreation == CHAR_CONFIRM) ) ) 
      {
         Engine->Run(screen);
         reloadMusic = true;
         estado = IN_GAME;
         result = Engine->InitialScreen(estado,&tituloID,reloadMusic);
      }
      else
      {
         result = Engine->InitialScreen(estado,&tituloID,reloadMusic);
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


