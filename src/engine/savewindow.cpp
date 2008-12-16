#include "savewindow.h"

#include <SDL/SDL_image.h>

#include "cursor.h"
#include "options.h"

#include "../etc/userinfo.h"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
saveWindow::saveWindow()
{
   curFileName = "";
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
saveWindow::~saveWindow()
{
}

/***********************************************************************
 *                          getSelectedFileName                        *
 ***********************************************************************/
string saveWindow::getSelectedFileName()
{
   return(curFileName);
}

/***********************************************************************
 *                                open                                 *
 ***********************************************************************/
void saveWindow::open()
{
   int midX=0, midY=0;
   int dX=0, dY=0;
   userInfo info;

   /* Define Sizes and positions */
   midX = (SCREEN_X / 2);
   midY = (SCREEN_Y / 2);
   if(windowIsLoad)
   {
      dX = 132;
      dY = 140;
   }
   else
   {
      dX = 132;
      dY = 95;
   }

   /* Create the window */
   fileWindow = gui->insertWindow(midX-dX, midY-dY, midX+dX, midY+dY,
                                  windowIsLoad?gettext("Load"):gettext("Save"));

   /* Set the File Selector and Image/Text, if needed */
   if(windowIsLoad)
   {
      fileImage = fileWindow->getObjectsList()->insertPicture(8,18,0,0,NULL);
      fileWindow->getObjectsList()->insertTextBox(6,16,137,115,2,"");
      fileTitle = fileWindow->getObjectsList()->insertTextBox(138,16,256,115,
                                                              2,"");
      fileSelector = fileWindow->getObjectsList()->insertFileSel(6,120,true,
                            info.getSavesDirectory(), false);
   }
   else
   {
      fileSelector = fileWindow->getObjectsList()->insertFileSel(6,18,false,
                            info.getSavesDirectory(), false);
   }

   /* Define the filter */
   fileSelector->setFilter(".sav");

   /* Open the window */
   fileWindow->setAttributes(false,true,false,false);
   fileWindow->setExternPointer(&fileWindow);
   gui->openWindow(fileWindow);

}

/***********************************************************************
 *                           changeInfo                                *
 ***********************************************************************/
void saveWindow::changeInfo()
{
   saveFile* sav;
   warning warn;

   /* Free any previous */
   if(windowIsLoad)
   {
      if(fileImage->get() != NULL)
      {
         SDL_FreeSurface(fileImage->get());
         fileImage->set(NULL);
      }
      fileTitle->setText("");

      /* Reload a new, if defined */
      if(!fileSelector->getFileName().empty())
      {
         sav = new saveFile;
         if(sav->loadHeader(fileSelector->getFileName()))
         {
            fileImage->set(IMG_Load(sav->getImageFile().c_str()));
            fileTitle->setText(sav->getTitle());
         }
         else
         {
            warn.show(gettext("Error"), gettext("Invalid File!"), gui);
         }
         delete(sav);
      }

      fileWindow->draw(0,0);
   }
}

/***********************************************************************
 *                                 run                                 *
 ***********************************************************************/
int saveWindow::run(bool load, GLdouble proj[16],GLdouble modl[16],
                    GLint viewPort[4])
{
   int state = -1;
   guiObject* obj;
   Uint8 mButton;
   Uint8* keys;
   int x,y;
   int time=0, lastTime=0;
   int eventInfo;
   dirs dir;
   options option;
   cursor cursors;

   /* Define type */
   windowIsLoad = load;

   /* Create the GUI */
   gui = new guiInterface(NULL);

   /* Create the background */
   GLuint tituloId;
   SDL_Surface* img = IMG_Load(
         dir.getRealFile("texturas/general/inicio.png").c_str());
   glGenTextures(1,&tituloId);
   setTexture(img,tituloId);
   SDL_FreeSurface(img);

   /* Open the window */
   open();

   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);
   while(state == -1)
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

         /* Get Events  */
         switch(eventInfo)
         {
            case FILE_SEL_CHANGED:
               changeInfo();
            break;
            case FILE_SEL_CANCEL:
               state = DNT_SAVE_WINDOW_CANCEL;
            break;
            case FILE_SEL_ACCEPT:
               curFileName = fileSelector->getFileName();
               state = DNT_SAVE_WINDOW_CONFIRM; 
            break;
         }
      }
      else if((UPDATE_RATE-1) - (time - lastTime) > 0 )
      {
         SDL_Delay((UPDATE_RATE-1) - (time - lastTime) );
      }
   }

   delete(gui);

   glEnable(GL_LIGHTING);
   glEnable(GL_FOG);

   return(state);

}

