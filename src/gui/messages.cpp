#include "messages.h"
#include "../engine/util.h"
#include "../engine/cursor.h"

/******************************************************************
 *                        getStringForUser                        *
 ******************************************************************/
string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   cursor cursors;
   cursors.set(CURSOR_WALK);

   guiInterface* gui = new guiInterface(NULL);
   window* getWindow;
   button* okButton;
   button* cancelButton;
   textBar* getText = NULL;
   bool quit = false;
   string returnStr;
   Uint8 mButton;
   Uint8* keys;
   int mouseX, mouseY;
   int sX = SCREEN_X / 2;
   int sY = SCREEN_Y / 2;

   getWindow = gui->insertWindow(sX-100,sY-31,sX+100,sY+31,title.c_str());
   okButton = getWindow->getObjectsList()->insertButton(40,37,95,55,"Ok",1);
   cancelButton = getWindow->getObjectsList()->insertButton(100,37,155,55,
                                                            "Cancel",1);
   getText = getWindow->getObjectsList()->insertTextBar(10,17,190,33,
                                                        previous.c_str(),0);
   getWindow->setAttributes(true, false, false, false);
   getWindow->setExternPointer(&getWindow);
   gui->openWindow(getWindow);
   

   while(!quit)
   {
      int eventInfo = NOTHING;
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      mButton = SDL_GetMouseState(&mouseX,&mouseY);

      guiObject* object;
      object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, eventInfo);

      if(eventInfo == PRESSED_BUTTON)
      {
         if(object == (guiObject*) okButton)
         {
            returnStr = getText->getText();
            quit =true;
         }
         else if(object == (guiObject*) cancelButton)
         {
            returnStr = previous;
            quit = true;
         }
      }

      if(getWindow == NULL)
      {
         quit = true;
      }

      /* Draw */
      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      glDisable(GL_FOG);
      glPushMatrix();
        draw2DMode();
        gui->draw(proj,modl,viewPort);
        cursors.draw(mouseX, mouseY);
        draw3DMode(OUTDOOR_FARVIEW);
      glPopMatrix();
      glEnable(GL_FOG);
      glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);

      glFlush();
      SDL_GL_SwapBuffers();

      SDL_Delay(20);

   }

   delete(gui);

   return(returnStr);

}

/******************************************************************
 *                            showMessage                         *
 ******************************************************************/
int getOptionFromUser(string title, string message, string opt1, string opt2,
                      GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   cursor cursors;
   cursors.set(CURSOR_WALK);

   guiInterface* gui = new guiInterface(NULL);
   window* getWindow;
   button* opt1Button;
   button* opt2Button;
   textBox* quadText = NULL;
   bool quit = false;
   Uint8 mButton;
   Uint8* keys;
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL, 10);
   int mouseX, mouseY;
   int sizeX = fnt.getStringWidth(message);
   if(sizeX < 180)
   {
      sizeX = 180;
   }
   int med = sizeX / 2;
   int sX = SCREEN_X / 2;
   int sY = SCREEN_Y / 2;
   int ret = -1;

   getWindow=gui->insertWindow(sX-med-10,sY-31,sX+med+10,sY+31, title.c_str());
   opt1Button = getWindow->getObjectsList()->insertButton(med-80,37,med-10,55,
                                                 opt1.c_str(),1);
   opt2Button = getWindow->getObjectsList()->insertButton(med+10,37,med+80,55,
                                                 opt2.c_str(),1);
                                                 
   quadText = getWindow->getObjectsList()->insertTextBox(5,17,sizeX+10,33,0,
                                                      message.c_str());
   quadText->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   getWindow->setAttributes(true, false, false, false);
   getWindow->setExternPointer(&getWindow);
   gui->openWindow(getWindow);

   while(!quit)
   {
      int eventInfo = NOTHING;
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      mButton = SDL_GetMouseState(&mouseX,&mouseY);

      guiObject* object;
      object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, eventInfo);

      if(eventInfo == PRESSED_BUTTON)
      {
         if(object == (guiObject*) opt1Button)
         {
            quit =true;
            ret = 1;
         }
         else if(object == (guiObject*) opt2Button)
         {
            quit =true;
            ret = 2;
         }
      }

      if(getWindow == NULL)
      {
         quit = true;
      }

      /* Draw */
      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      glDisable(GL_FOG);
      glPushMatrix();
        draw2DMode();
        gui->draw(proj,modl,viewPort);
        cursors.draw(mouseX, mouseY);
        draw3DMode(OUTDOOR_FARVIEW);
      glPopMatrix();
      glEnable(GL_FOG);
      glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);

      glFlush();
      SDL_GL_SwapBuffers();

      SDL_Delay(20);

   }

   delete(gui);
   
   return(ret);
}

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
warning::warning()
{
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
warning::~warning()
{
}

/***********************************************************************
 *                               Show                                  *
 ***********************************************************************/
void warning::show(string title, string message, guiInterface* gui)
{ 
   dntFont fnt;

   /* Close the window if it is opened */
   close();

   /* Define the gui */
   lastGui = gui;

   /* Calculate Window Position And size */
   fnt.defineFont(DNT_FONT_ARIAL, 10);
   int sizeX = fnt.getStringWidth(message);
   int sX = SCREEN_X / 2;
   int sY = SCREEN_Y / 2;
   int med = sizeX / 2;

   warnWindow = gui->insertWindow(sX-med-10,sY-31,sX+med+10,sY+31,title);
   okButton = warnWindow->getObjectsList()->insertButton(med-28+10,37,
                                                         med+28+10,55,
                                                         "Ok",1);
   textBox* quadText = warnWindow->getObjectsList()->insertTextBox(5,17,
                                                      sizeX+10,33,0,
                                                      message.c_str());
   quadText->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   warnWindow->setModal();
   warnWindow->setExternPointer(&warnWindow);
   gui->openWindow(warnWindow);
}

/***********************************************************************
 *                            getOkButton                              *
 ***********************************************************************/
button* warning::getOkButton()
{
   return(okButton);
}

/***********************************************************************
 *                             isOpened                                *
 ***********************************************************************/
bool warning::isOpened()
{
   return(warnWindow != NULL);
}

/***********************************************************************
 *                               close                                 *
 ***********************************************************************/
void warning::close()
{
   if(warnWindow != NULL)
   {
      lastGui->closeWindow(warnWindow);
   }
}

/***********************************************************************
 *                           static members                            *
 ***********************************************************************/
window* warning::warnWindow = NULL;
guiInterface* warning::lastGui = NULL;
button* warning::okButton = NULL;

