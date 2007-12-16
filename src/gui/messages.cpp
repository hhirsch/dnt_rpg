#include "messages.h"
#include "../engine/util.h"

/******************************************************************
 *                        getStringForUser                        *
 ******************************************************************/
string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   interface* gui = new interface(NULL);
   window* getWindow;
   button* okButton;
   button* cancelButton;
   textBar* getText = NULL;
   bool quit = false;
   string returnStr;
   Uint8 mButton;
   Uint8* keys;
   int mouseX, mouseY;

   getWindow = gui->insertWindow(300,200,500,262,title.c_str());
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
      int eventInfo;
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      mButton = SDL_GetMouseState(&mouseX,&mouseY);

      guiObject* object;
      object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, &eventInfo);

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
        draw3DMode(FARVIEW);
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
   interface* gui = new interface(NULL);
   window* getWindow;
   button* opt1Button;
   button* opt2Button;
   textBox* quadText = NULL;
   bool quit = false;
   Uint8 mButton;
   Uint8* keys;
   int mouseX, mouseY;
   int sizeX = message.length()*7;
   if(sizeX < 180)
   {
      sizeX = 180;
   }
   int med = sizeX / 2;
   int ret = -1;

   getWindow = gui->insertWindow(300,200,300+sizeX,262, title.c_str());
   opt1Button = getWindow->getObjectsList()->insertButton(med-80,37,med-10,55,
                                                 opt1.c_str(),1);
   opt2Button = getWindow->getObjectsList()->insertButton(med+10,37,med+80,55,
                                                 opt2.c_str(),1);                  
                                                 
   quadText = getWindow->getObjectsList()->insertTextBox(10,17,sizeX-10,33,0,
                                                      message.c_str());
   quadText->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   getWindow->setAttributes(true, false, false, false);
   getWindow->setExternPointer(&getWindow);
   gui->openWindow(getWindow);

   while(!quit)
   {
      int eventInfo;
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      mButton = SDL_GetMouseState(&mouseX,&mouseY);

      guiObject* object;
      object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, &eventInfo);

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
	draw3DMode(FARVIEW);
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

/******************************************************************
 *                            showMessage                         *
 ******************************************************************/
void showMessage(string title, string message,
                 GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   interface* gui = new interface(NULL);
   window* getWindow;
   button* okButton;
   textBox* quadText = NULL;
   bool quit = false;
   Uint8 mButton;
   Uint8* keys;
   int mouseX, mouseY;
   int sizeX = message.length()*7;
   int med = sizeX / 2;

   getWindow = gui->insertWindow(300,200,300+sizeX,262,title.c_str());
   okButton = getWindow->getObjectsList()->insertButton(med-28,37,med+28,55,
                                                        "Ok",1);
   quadText = getWindow->getObjectsList()->insertTextBox(10,17,sizeX-10,33,0,
                                                      message.c_str());
   quadText->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   getWindow->setAttributes(true, false, false, false);
   getWindow->setExternPointer(&getWindow);
   gui->openWindow(getWindow);

   while(!quit)
   {
      int eventInfo;
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      mButton = SDL_GetMouseState(&mouseX,&mouseY);

      guiObject* object;
      object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, &eventInfo);

      if(eventInfo == PRESSED_BUTTON)
      {
         if(object == (guiObject*) okButton)
         {
            quit =true;
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
        draw3DMode(FARVIEW);
      glPopMatrix();
      glEnable(GL_FOG);
      glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);

      glFlush();
      SDL_GL_SwapBuffers();

      SDL_Delay(20);

   }

   delete(gui);

}


