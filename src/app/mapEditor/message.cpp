/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "message.h"

#include "../../engine/util.h"
#include "../../engine/cursor.h"
#include "../../gui/farsoopts.h"

#include <stdio.h>
#include <stdlib.h>

using namespace std;

namespace dntMapEditor
{

/******************************************************************
 *                        messageForTool                          *
 ******************************************************************/
string messageForTool(int tool)
{
   switch(tool)
   {
      case TOOL_TERRAIN_UP: 
         return("Select Square Vertex to Up"); 
      break;
      case TOOL_TERRAIN_NIVELATE:
         return("Select Area to Nivelate");
      break;
      case TOOL_TERRAIN_DOWN:
         return("Select Square Vertex to Down");
      break;
      case TOOL_TERRAIN_TEXTURE:
         return("Select Square to Texturize");
      break;
      case TOOL_PORTAL_ADD:
         return("Select Area to Add Portal (Max One portal per Square)");
      break;
      case TOOL_PORTAL_TAG:
         return("Select Portal to Define Destiny");
      break;
      case TOOL_PORTAL_DOOR:
         return("Put Door on Wall");
      break;
      case TOOL_PORTAL_DOOR_LOCK:
         return("Select Door to Lock/Unlock");
      break;
      case TOOL_WALL_ADD:
         return("Draw The Wall (press B to Add only on Square "
                "Borders)");
      break;
      case TOOL_WALL_DESTROY:
         return("Click on an wall to delete it");
      break;
      case TOOL_WALL_EDIT:
         return("Select an wall to edit");
      break;
      case TOOL_WALL_CUT:
         return("Click on the position to cut wall");
      break;
      case TOOL_WALL_EDIT_FACE:
         return("Select a Wall face to edit");
      break;
      case TOOL_WALL_TEXTURE:
         return("Select a Wall to Change Texture");
      break;
      case TOOL_WALL_LESS_Y_TEXTURE:
         return("Select a Wall to Less Y Texture Repeat");
      break;
      case TOOL_WALL_MORE_Y_TEXTURE:
         return("Select a Wall to More Y Texture Repeat");
      break;
      case TOOL_WALL_LESS_X_TEXTURE:
         return("Select a Wall to Less X Texture Repeat");
      break;
      case TOOL_WALL_MORE_X_TEXTURE:
         return("Select a Wall to More X Texture Repeat");
      break;
      case TOOL_WALL_LESS_Z_TEXTURE:
         return("Select a Wall to Less Z Texture Repeat");
      break;
      case TOOL_WALL_MORE_Z_TEXTURE:
         return("Select a Wall to More Z Texture Repeat");
      break;
      case TOOL_OBSTACLE_ADD:
         return("Put Object on Map with Left Mouse Button. "
                "(x y z) + Right or Middle Buttons to rotate");
      break;
      case TOOL_NODE_EDITOR:
         return("Select an sceneNode to edit "
                "and do changes at the window ");
      break;
      case TOOL_NPC_ADD:
         return("Put NPC on Map with left mouse Button.");
      break;
      case TOOL_TILE_WALL_REMOVE:
         return("Click on a tile to remove it.");
      break;
   }
   return("DNT!");
}

/******************************************************************
 *                        getStringForUser                        *
 ******************************************************************/
string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   cursor cursors;
   cursors.hide();
   SDL_ShowCursor(SDL_ENABLE);

   Farso::GuiInterface* gui = new Farso::GuiInterface("");
   Farso::Window* getWindow;
   Farso::Button* okButton;
   Farso::Button* cancelButton;
   Farso::TextBar* getText = NULL;
   bool quit = false;
   string returnStr;
   Uint8 mButton;
   Uint8* keys;
   int mouseX, mouseY;
   int sX = Farso::SCREEN_X / 2;
   int sY = Farso::SCREEN_Y / 2;

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
      int eventInfo = Farso::EVENT_NONE;
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      mButton = SDL_GetMouseState(&mouseX,&mouseY);

      Farso::GuiObject* object;
      object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, eventInfo);

      if(eventInfo == Farso::EVENT_PRESSED_BUTTON)
      {
         if(object == (Farso::GuiObject*) okButton)
         {
            returnStr = getText->getText();
            quit =true;
         }
         else if(object == (Farso::GuiObject*) cancelButton)
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

   SDL_ShowCursor(SDL_DISABLE);
   cursors.set(CURSOR_WALK);
   delete(gui);

   return(returnStr);

}

/******************************************************************
 *                            showMessage                         *
 ******************************************************************/
int getOptionFromUser(string title, string message, string opt1, string opt2,
                      GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   Farso::Options fopts;
   cursor cursors;
   cursors.hide();
   SDL_ShowCursor(SDL_ENABLE);

   Farso::GuiInterface* gui = new Farso::GuiInterface("");
   Farso::Window* getWindow;
   Farso::Button* opt1Button;
   Farso::Button* opt2Button;
   Farso::TextBox* quadText = NULL;
   bool quit = false;
   Uint8 mButton;
   Uint8* keys;
   Farso::Font fnt;
   fnt.defineFont(fopts.getDefaultFont(), 10);
   int mouseX, mouseY;
   int sizeX = fnt.getStringWidth(message);
   if(sizeX < 180)
   {
      sizeX = 180;
   }
   int med = sizeX / 2;
   int sX = Farso::SCREEN_X / 2;
   int sY = Farso::SCREEN_Y / 2;
   int ret = -1;

   getWindow=gui->insertWindow(sX-med-10,sY-31,sX+med+10,sY+31, title.c_str());
   opt1Button = getWindow->getObjectsList()->insertButton(med-80,37,med-10,55,
                                                 opt1.c_str(),1);
   opt2Button = getWindow->getObjectsList()->insertButton(med+10,37,med+80,55,
                                                 opt2.c_str(),1);
                                                 
   quadText = getWindow->getObjectsList()->insertTextBox(5,17,sizeX+10,33,0,
                                                      message.c_str());
   quadText->setFont(fopts.getDefaultFont(), 10, Farso::Font::ALIGN_CENTER);
   getWindow->setAttributes(true, false, false, false);
   getWindow->setExternPointer(&getWindow);
   gui->openWindow(getWindow);

   while(!quit)
   {
      int eventInfo = Farso::EVENT_NONE;
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      mButton = SDL_GetMouseState(&mouseX,&mouseY);

      Farso::GuiObject* object;
      object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, eventInfo);

      if(eventInfo == Farso::EVENT_PRESSED_BUTTON)
      {
         if(object == (Farso::GuiObject*) opt1Button)
         {
            quit =true;
            ret = 1;
         }
         else if(object == (Farso::GuiObject*) opt2Button)
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

   SDL_ShowCursor(SDL_DISABLE);
   cursors.set(CURSOR_WALK);
   delete(gui);
   
   return(ret);
}

}

