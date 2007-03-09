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
   /* Call Splash Screen  */
   Engine->SplashScreen();

   /* Load backImage */
   SDL_Surface* img = IMG_Load("../data/texturas/inicio.png");
   carregaTextura(img,&tituloID);
   SDL_FreeSurface(img);

   /* Call Initial Screen */
   glDisable(GL_LIGHTING);
   Engine->fadeInTexture(tituloID);
   int result = Engine->InitialScreen(ON_INIT,tituloID,true);
   int estado = ON_INIT;
   int charCreation = 0;
   bool reloadMusic;
   
   while(result != EXIT_GAME)
   {
       reloadMusic = false;
       if(result == NEW_GAME)
       {
          charCreation = Engine->CharacterScreen(tituloID);
          if( charCreation == CHAR_CONFIRM)
          {
             glDisable(GL_LIGHTING);
             Engine->fadeOutTexture(tituloID);
             Engine->LoadMap("../data/mapas/poc2.map",1);
          }
       }
       
       if( (result == SAVE_GAME) && (estado == IN_GAME))
       {
          Engine->save();
       }

       if(result == LOAD_GAME)
       {
          Engine->load();
       }
       
       if(result == OPTIONS)
       {
          Engine->OptionsScreen(tituloID);
       }
       #ifdef REDE
          Engine->server = server;
       #endif
      //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

      if( (result == CONTINUE_GAME) || 
          ( ((result == NEW_GAME)) && (charCreation == CHAR_CONFIRM) ) ) 
      {
         if(result == CONTINUE_GAME)
         {
            glDisable(GL_LIGHTING);
            Engine->fadeOutTexture(tituloID);
         }
         if(Engine->Run(screen) == 1)
         {
            estado = IN_GAME;
         }
         else
         {
            estado = ON_INIT;
         }
         reloadMusic = true;
         glDisable(GL_LIGHTING);
         Engine->fadeInTexture(tituloID);
         result = Engine->InitialScreen(estado,tituloID,reloadMusic);
      }
      else
      {
         result = Engine->InitialScreen(estado,tituloID,reloadMusic);
      }
      charCreation = CHAR_OTHER;
       
       #ifdef REDE
          free(Engine->server); 
       #endif
   }

   glDisable(GL_LIGHTING);
   Engine->fadeOutTexture(tituloID);

   glDeleteTextures(1,&tituloID); 
       
   delete(Engine);

   return(0); 
}


