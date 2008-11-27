/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#include "../gui/farso.h"
#include "dccnit.h"
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <libintl.h>

#include "../etc/userinfo.h"
#include "../etc/dirs.h"
#include "../cbook/comicbook.h"

#ifdef _MSC_VER
   #include "../config_win.h"
#else
   #include "../config.h"
#endif

/*********************************************************************
 *                           Main Code                               *
 *********************************************************************/
int main(int argc, char **argv)
{
   /* Set locale */
   setlocale (LC_CTYPE, "");
   setlocale(LC_MESSAGES, "");
   bindtextdomain (PACKAGE, LOCALEDIR);
   textdomain (PACKAGE);

   SDL_Surface *screen;

   /* Load User Info */
   userInfo user;
   user.getValuesFromSystem();

   /* Get current data directorie */
   dirs dir;
   dir.findDataDirectories();

   /* Load Options */
   options opt;
   opt.load();
   
   #ifdef REDE
      printf("Server IP or Name: ");
      char* server = (char*) malloc(55*sizeof(char));
      scanf("%s",server);
      printf("I'll conect to %s\n",server);
   #endif

   Farso_Init(&screen,"DccNiTghtmare", opt.getScreenWidth(), 
              opt.getScreenHeight(),  opt.getEnableFullScreen(), 
              opt.getAntiAliasing());

   engine* gameEngine = new engine();
   
   gameEngine->init(screen);

   SDL_ShowCursor(SDL_DISABLE);

   GLuint tituloID;
   /* Call Splash Screen  */
   gameEngine->splashScreen();

   /* Load backImage */
   SDL_Surface* img = IMG_Load(
                        dir.getRealFile("texturas/general/inicio.png").c_str());
   glGenTextures(1,&tituloID);
   setTexture(img,tituloID);
   SDL_FreeSurface(img);

   /* Call Initial Screen */
   glDisable(GL_LIGHTING);
   gameEngine->fadeInTexture(tituloID,0,0,SCREEN_X-1,SCREEN_Y-1,800,600);
   int result = gameEngine->menuScreen(ON_INIT,tituloID,true);
   int estado = ON_INIT;
   int charCreation = 0;
   bool reloadMusic;
   
   while(result != EXIT_GAME)
   {
       reloadMusic = false;
       if(result == NEW_GAME)
       {
          charCreation = gameEngine->characterScreen(tituloID);
          if( charCreation == CHAR_CONFIRM)
          {
             glDisable(GL_LIGHTING);
             gameEngine->fadeOutTexture(tituloID,0,0,SCREEN_X-1,SCREEN_Y-1, 
                                    800, 600);

             comicBook* cBook = new comicBook();
             cBook->load("comics/intro/intro.dcb");
             cBook->run();

             gameEngine->loadMap("mapas/tutorial/house1.map");
          }
       }
       
       if( (result == SAVE_GAME) && (estado == IN_GAME))
       {
          gameEngine->saveGame();
       }

       if(result == LOAD_GAME)
       {
          /* Load the game */
          gameEngine->loadGame();
          /* And continue to play it (as a new, to
           * open all initial windows)! */
          result = NEW_GAME;
          charCreation = CHAR_CONFIRM;
       }
       
       if(result == OPTIONS)
       {
          gameEngine->optionsScreen(tituloID);
       }
       #ifdef REDE
          gameEngine->server = server;
       #endif
      //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

      if( (result == CONTINUE_GAME) || 
          ( ((result == NEW_GAME)) && (charCreation == CHAR_CONFIRM) ) ) 
      {
         if(result == CONTINUE_GAME)
         {
            glDisable(GL_LIGHTING);
            gameEngine->fadeOutTexture(tituloID,0,0,SCREEN_X-1,SCREEN_Y-1,
                                       800,600);
         }
         if(gameEngine->run(screen, (result == CONTINUE_GAME)) == 1)
         {
            estado = IN_GAME;
         }
         else
         {
            estado = ON_INIT;
         }
         reloadMusic = true;
         glDisable(GL_LIGHTING);
	      glDisable(GL_FOG);
         gameEngine->fadeInTexture(tituloID,0,0,SCREEN_X-1,SCREEN_Y-1, 
                                   800, 600);
         result = gameEngine->menuScreen(estado,tituloID,reloadMusic);
      }
      else
      {
         result = gameEngine->menuScreen(estado,tituloID,reloadMusic);
      }
      charCreation = CHAR_OTHER;
       
       #ifdef REDE
          free(gameEngine->server); 
       #endif
   }

   glDisable(GL_LIGHTING);
   gameEngine->fadeOutTexture(tituloID,0,0,SCREEN_X-1,SCREEN_Y-1,800,600);

   glDeleteTextures(1,&tituloID); 

   delete(gameEngine);

   Farso_End(screen);

   return(0); 
}


