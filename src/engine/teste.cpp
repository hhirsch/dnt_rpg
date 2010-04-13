/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../gui/farso.h"
#include "dccnit.h"
#include "../lang/translate.h"
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

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
#if ENABLE_NLS == 1
   setlocale (LC_CTYPE, "");
   setlocale(LC_MESSAGES, "");
   bindtextdomain(PACKAGE, LOCALEDIR);
   textdomain(PACKAGE);
#endif

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
      cout << "Server IP or Name: ";
      char* server = (char*) malloc(55*sizeof(char));
      scanf("%s",server);
      cout << "I'll conect to " << server << endl;
   #endif

   Farso_Init(&screen,"DccNiTghtmare", opt.getScreenWidth(), 
              opt.getScreenHeight(),  opt.getEnableFullScreen(), 
              opt.getAntiAliasing(), opt.getStencilBufferSize());

   engine* gameEngine = new engine();
   
   gameEngine->init(screen);

   SDL_ShowCursor(SDL_DISABLE);

   /* Call Splash Screen  */
   gameEngine->splashScreen();

   /* Call Initial Screen */
   glDisable(GL_LIGHTING);
   gameEngine->fadeInTexture(gameEngine->idTextura,
         0,0,SCREEN_X-1,SCREEN_Y-1,800,600);
   int result = gameEngine->menuScreen(ON_INIT,true);
   int estado = ON_INIT;
   int charCreation = 0;
   bool reloadMusic;
   
   while(result != EXIT_GAME)
   {
       reloadMusic = false;
       if(result == NEW_GAME)
       {
          charCreation = gameEngine->characterScreen();
          if( charCreation == CHAR_CONFIRM)
          {
             glDisable(GL_LIGHTING);
             glDisable(GL_FOG);
             gameEngine->mainScreenEffect();

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
          if(gameEngine->loadGame())
          {
             /* And continue to play it (as a new, to
              * open all initial windows)! */
             result = NEW_GAME;
             charCreation = CHAR_CONFIRM;
          }
       }
       
       if(result == OPTIONS)
       {
          gameEngine->optionsScreen();
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
            gameEngine->fadeOutTexture(gameEngine->idTextura,
                  0,0,SCREEN_X-1,SCREEN_Y-1,800,600);
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
         gameEngine->fadeInTexture(gameEngine->idTextura,
               0,0,SCREEN_X-1,SCREEN_Y-1, 800, 600);
         result = gameEngine->menuScreen(estado,reloadMusic);
      }
      else
      {
         result = gameEngine->menuScreen(estado,reloadMusic);
      }
      charCreation = CHAR_OTHER;
       
       #ifdef REDE
          free(gameEngine->server); 
       #endif
   }

   glDisable(GL_LIGHTING);
   gameEngine->fadeOutTexture(gameEngine->idTextura,
         0,0,SCREEN_X-1,SCREEN_Y-1,800,600);

   delete(gameEngine);

   Farso_End(screen);

   return(0); 
}


