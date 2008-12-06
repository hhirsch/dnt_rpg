
#include "levelup.h"
#include "classwindow.h"
#include "skillwindow.h"
#include "culling.h"
#include "cursor.h"
#include "options.h"

/* Some State constants */
#define LEVEL_UP_CANCEL         0
#define LEVEL_UP_CLASS_WINDOW   1
#define LEVEL_UP_SKILL_WINDOW   2
#define LEVEL_UP_FINISHED       3

//TODO new feats choose/get!

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
levelUp::levelUp(character* c)
{
   current = c;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
levelUp::~levelUp()
{
   current = NULL;
}

/***********************************************************************
 *                            doLevelUp                                *
 ***********************************************************************/
void levelUp::doLevelUp(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   guiObject* obj;
   Uint8 mButton;
   Uint8* keys;
   options option;
   dirs dir;
   int x,y;
   int time=0, lastTime=0;
   int eventInfo, res;
   cursor cursors;

   classWindow *clWindow = NULL;
   skillWindow *skWindow = NULL;
   classe* selClass = current->actualClass[0];

   /* Create the new GUI */
   guiInterface* gui = new guiInterface(NULL);

   /* Create the background */
   GLuint tituloId;
   SDL_Surface* img = IMG_Load(
         dir.getRealFile("texturas/general/inicio.png").c_str());
   glGenTextures(1,&tituloId);
   setTexture(img,tituloId);
   SDL_FreeSurface(img);

   /* Open the class window */
   clWindow = new classWindow(gui, &selClass);
   state = LEVEL_UP_CLASS_WINDOW;

   /* Run, until canceled or finished */
   while((state != LEVEL_UP_CANCEL) && (state != LEVEL_UP_FINISHED))
   {
      time = SDL_GetTicks();
      if(time - lastTime >= UPDATE_RATE)
      {
         lastTime = time;
         /* Clear things */
         glClearColor(0,0,0,1);
         glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

         /* Get Events */
         SDL_PumpEvents();
         keys = SDL_GetKeyState(NULL);
         mButton = SDL_GetMouseState(&x,&y);
         obj = gui->manipulateEvents(x,y,mButton,keys, eventInfo);

         /* Render Things */
         glPushMatrix();
            draw2DMode();
            textureToScreen(tituloId,0,0,SCREEN_X-1,SCREEN_Y-1,800,600);
            gui->draw(proj,modl,viewPort);
            glPushMatrix();
               cursors.draw(x, y);
            glPopMatrix();
            draw3DMode(option.getFarViewFactor()*OUTDOOR_FARVIEW);
         glPopMatrix();
         glFlush();
         SDL_GL_SwapBuffers();

         /* Treat current opened window events */
         switch(state)
         {
            /* Treat CLass Windows Events */
            case LEVEL_UP_CLASS_WINDOW:
            {
               res = clWindow->treat(obj, eventInfo, gui);
               if(res == CLASSW_CONFIRM)
               {
                  /* Delete the current class window */
                  delete(clWindow);

                  /* Verify if can take a level at the class */
                  if(current->canClass(selClass))
                  {
                     /* Can, so, we must go to the skill window */
                     int points = current->getOtherLevelSkillPoints(selClass);
                     current->sk.setAvaiblePoints(points + 
                                                current->sk.getAvaiblePoints());
                     skWindow = new skillWindow(&current->sk, gui, 
                                                current->getLevel(selClass)+1);
                     state = LEVEL_UP_SKILL_WINDOW;
                  }
                  else
                  {
                     /* Can't, so must alert user and keep at the 
                      * class window */                     
                     warning warn;
                     clWindow = new classWindow(gui, &selClass);
                     state = LEVEL_UP_CLASS_WINDOW;
                     warn.show(gettext("Warning"), 
                               gettext("You can't multiclass more!"), gui);

                  }
               }
               else if(res == CLASSW_CANCEL)
               {
                  /* Must cancel the level up */
                  state = LEVEL_UP_CANCEL;
                  delete(clWindow);
               }

            }
            break;

            /* Treat skill window */
            case LEVEL_UP_SKILL_WINDOW:
            {
               res = skWindow->treat(obj, eventInfo, gui);
               if(res == SKILLW_CONFIRM)
               {
                  delete(skWindow);
                  state = LEVEL_UP_FINISHED;
               }
               else if(res == SKILLW_CANCEL)
               {
                  /* Must undo the previous class free skills points */
                  int points = current->getOtherLevelSkillPoints(selClass);
                  current->sk.setAvaiblePoints(current->sk.getAvaiblePoints() -
                                               points);
                  /* Must go back to the class window */
                  delete(skWindow);
                  state = LEVEL_UP_CLASS_WINDOW;
                  clWindow = new classWindow(gui, &selClass);
               }
            }
            break;
         }
      }
      else
      {
         SDL_Delay(UPDATE_RATE-2);
      }
   }

   if(state == LEVEL_UP_FINISHED)
   {
      /* Must apply the class choosed */
      current->getNewClassLevel(selClass);
   }

   /* Clear the temporary gui, and done! */
   delete(gui);
   glDeleteTextures(1,&tituloId);
}

