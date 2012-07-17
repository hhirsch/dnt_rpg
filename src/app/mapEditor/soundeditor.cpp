/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#include "soundeditor.h"
#include "message.h"
using namespace dntMapEditor;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
SoundEditor::SoundEditor(Farso::GuiInterface* g)
{
   gui = g;
   sndWindow = NULL;
   fileWindow = NULL;
   curSound = NULL;
   curNode = NULL;
   tmpNode = NULL;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
SoundEditor::~SoundEditor()
{
   clear();
}

/***********************************************************************
 *                                 clear                               *
 ***********************************************************************/
void SoundEditor::clear()
{
   if(fileWindow)
   {
      gui->closeWindow(fileWindow);
   }
   if(sndWindow)
   {
      gui->closeWindow(sndWindow);
   }
}

/***********************************************************************
 *                           openFileWindow                            *
 ***********************************************************************/
void SoundEditor::openFileWindow()
{
   dirs dir;
   if(fileWindow)
   {
      /* Close the current opened */
      gui->closeWindow(fileWindow);
   }

   fileWindow = gui->insertWindow(200,100,460,285,"Select Sound File");
   fileSelector = fileWindow->getObjectsList()->insertFileSel(5,18,true,
         dir.getRealFile("sndfx/"));
   fileSelector->setFilter(".ogg");
   fileWindow->setAttributes(false,true,false,false);
   fileWindow->setExternPointer(&fileWindow);
   gui->openWindow(fileWindow);
}

/***********************************************************************
 *                             openWindow                              *
 ***********************************************************************/
void SoundEditor::openWindow()
{
   dirs dir;
   if(!sndWindow)
   {
      /* create the window and its elements */
      sndWindow = gui->insertWindow(437, 37, 677, 158, "Sound Effect");

      /* the tab with position edit and delete. */
      sndTab = sndWindow->getObjectsList()->insertTabButton(5,17,0,0, 
            dir.getRealFile("mapEditor/sound_edit.png").c_str());
      moveX[0] = sndTab->insertButton(23,4,37,19);
      moveX[1] = sndTab->insertButton(23,39,37,55);
      moveY[0] = sndTab->insertButton(3,59,19,95);
      moveY[1] = sndTab->insertButton(22,58,39,94);
      moveZ[0] = sndTab->insertButton(4,22,19,39);
      moveZ[1] = sndTab->insertButton(40,22,55,39);
      deleteSnd = sndTab->insertButton(43,47,61,80);
     
      sndFileText = sndWindow->getObjectsList()->insertTextBar(70,18,168,32,"",
            0);
      sndFileSelButton = sndWindow->getObjectsList()->insertButton(169,17,
         235,33,"Select",1);

      cxSelSndLoop = sndWindow->getObjectsList()->insertCxSel(70, 38, true);
      sndWindow->getObjectsList()->insertTextBox(80, 35, 190, 49, 0, 
            "Enable loop at end");
      sndWindow->getObjectsList()->insertTextBox(70, 52, 184, 66, 0,
            "Loop interval (seconds)");
      loopIntervalText = sndWindow->getObjectsList()->insertTextBar(185,53,
            235,67, "0", 0);

      /* Finally, open it! */
      sndWindow->setExternPointer(&sndWindow);
      gui->openWindow(sndWindow);
   }

   /* Set values */
   sndFileText->setText(curSound->fileName);
   cxSelSndLoop->setSelection(curSound->loopInterval != SOUND_NO_LOOP);

   char buf[32];
   sprintf(buf, "%d", curSound->loopInterval);
         loopIntervalText->setText(buf);
   loopIntervalText->setAvailable(cxSelSndLoop->isSelected());
   sndWindow->draw();
}

/***********************************************************************
 *                               setMap                                *
 ***********************************************************************/
void SoundEditor::setMap(Map* m)
{
   /* Set map pointer */
   curMap = m;

   /* Add a scene node for each sound defined on it */
   int i;
   scene sc;
   mapSound* s = curMap->getSounds();
   soundInfo* sInfo = (soundInfo*)s->getFirst();
   for(i=0; i < s->getTotal(); i++)
   {
      sc.createSceneNode(true, "models/objetos/geral/corchea/model.cfg", 
            sInfo->x, sInfo->y, sInfo->z, 0.0f, 0.0f, 0.0f);

      sInfo = (soundInfo*)sInfo->getNext();
   }
}

/***********************************************************************
 *                             verifyAction                            *
 ***********************************************************************/
int SoundEditor::verifyAction(Uint8* keys, GLfloat mouseX, GLfloat mouseY, 
      GLfloat mouseZ, GLfloat xFloor, GLfloat zFloor, Uint8 mButton, int tool)
{
   scene sc;
   mapSound* s = curMap->getSounds();

   if(tool == TOOL_SOUND_ADD)
   {
      if(!tmpNode)
      {
         /* create the temporary node representing it */
         tmpNode = sc.createSceneNode(true, 
               "models/objetos/geral/corchea/model.cfg", 
               mouseX, mouseY, mouseZ,
               0.0f, 0.0f, 0.0f);
      }
      else 
      {
         tmpNode->setPosition(xFloor, mouseY, zFloor);
         tmpNode->setAngle(0.0f, tmpNode->getAngleY()+5.0f, 0.0f);

         if(mButton & SDL_BUTTON(1))
         {
            /* Add the sound to the map */
            curSound = s->newSound();
            curSound->x = tmpNode->getPosX();
            curSound->y = tmpNode->getPosY();
            curSound->z = tmpNode->getPosZ();

            /* Hard-wait mouse button release */
            int x=0,y=0;
            while(mButton & SDL_BUTTON(1))
            {
               SDL_PumpEvents();
               mButton = SDL_GetMouseState(&x, &y);
            }

            /* Open the window to edit its values */
            curNode = tmpNode;
            tmpNode = NULL;
            openWindow();
            return(SOUND_EDITOR_ACTION_ADDED);
         }
      }
      return(SOUND_EDITOR_ACTION_OTHER);
   }
   else if(tool == TOOL_SOUND_EDIT)
   {
      if(mButton & SDL_BUTTON(1))
      {
         /* Create a bounding box for the mouse position */
         boundingBox mouseBox;
         mouseBox.setMin(mouseX-4, mouseY-4.0, mouseZ-4);
         mouseBox.setMax(mouseX+4, mouseY+4.0, mouseZ+4);

         sceneNode* scNode = sc.getSceneNode(mouseBox);
         if((scNode) && 
            (scNode->getModel()->getFileName() ==
             "models/objetos/geral/corchea/model.cfg") )
         {
            /* Node is a corchea, select it! */
            curSound = s->getSound(scNode->getPosX(), scNode->getPosY(),
                  scNode->getPosZ());
            if(curSound)
            {
               openWindow();
               curNode = scNode;
            }
         }
      }
      return(SOUND_EDITOR_ACTION_OTHER);
   }

   return(SOUND_EDITOR_ACTION_NONE);
}

/***********************************************************************
 *                            eventGot                                 *
 ***********************************************************************/
bool SoundEditor::eventGot(int eventInfo, Farso::GuiObject* obj)
{
   /* Close window if no sound is defined to edit */
   if( (!curNode) && (!curSound) && (sndWindow) )
   {
      gui->closeWindow(sndWindow);
      return(true);
   }

   /* Close file window if no sound window is opened */
   if((!sndWindow) && (fileWindow))
   {
      gui->closeWindow(fileWindow);
      return(true);
   }

   if(eventInfo == Farso::EVENT_ON_PRESS_TAB_BUTTON)
   {
      if(obj == (Farso::GuiObject*)moveX[0])
      {
         curNode->setPosition(curNode->getPosX()-1, 
               curNode->getPosY(), curNode->getPosZ() );
         curSound->x -= 1;
         return(true);
      }
      else if(obj == (Farso::GuiObject*)moveX[1])
      {
         curNode->setPosition(curNode->getPosX()+1, 
               curNode->getPosY(), curNode->getPosZ() );
         curSound->x += 1;
         return(true);
      }
      else if(obj == (Farso::GuiObject*)moveY[0])
      {
         curNode->setPosition(curNode->getPosX(), 
               curNode->getPosY()+1, curNode->getPosZ() );
         curSound->y += 1;
         return(true);
      }
      else if(obj == (Farso::GuiObject*)moveY[1])
      {
         curNode->setPosition(curNode->getPosX(), 
               curNode->getPosY()-1, curNode->getPosZ() );
         curSound->y -= 1;
         return(true);
      }
      else if(obj == (Farso::GuiObject*)moveZ[0])
      {
         curNode->setPosition(curNode->getPosX(), 
               curNode->getPosY(), curNode->getPosZ()-1 );
         curSound->z -= 1;
         return(true);
      }
      else if(obj == (Farso::GuiObject*)moveZ[1])
      {
         curNode->setPosition(curNode->getPosX(), 
               curNode->getPosY(), curNode->getPosZ()+1 );
         curSound->z += 1;
         return(true);
      }
      else if(obj == (Farso::GuiObject*)deleteSnd)
      {
         scene sc;
         sc.deleteSceneNode(curNode);
         curMap->getSounds()->remove(curSound);
         curNode = NULL;
         curSound = NULL;
         gui->closeWindow(sndWindow);
         return(true);
      }
   }
   else if(eventInfo == Farso::EVENT_WROTE_TEXT_BAR)
   {
      if(obj == (Farso::GuiObject*)sndFileText)
      {
         curSound->fileName = sndFileText->getText();
         return(true);
      }
      else if(obj == (Farso::GuiObject*)loopIntervalText)
      {
         char buf[32];

         /* update from text bar */
         sscanf(loopIntervalText->getText().c_str(), "%d", 
               &curSound->loopInterval);

         /* update text bar, to keep them equal */
         sprintf(buf, "%d", curSound->loopInterval);
         loopIntervalText->setText(buf);
         return(true);
      }
   }
   else if(eventInfo == Farso::EVENT_MODIFIED_CX_SEL)
   {
      if(obj == (Farso::GuiObject*)cxSelSndLoop)
      {
         if(cxSelSndLoop->isSelected())
         {
            /* update loop interval from text bar */
            sscanf(loopIntervalText->getText().c_str(), "%d", 
                  &curSound->loopInterval);
            loopIntervalText->setAvailable(true);
         }
         else
         {
            /* Disable loop and edit */
            loopIntervalText->setAvailable(false);
            curSound->loopInterval = SOUND_NO_LOOP;
         }

         sndWindow->draw();
         return(true);
      }
   }
   else if(eventInfo == Farso::EVENT_PRESSED_BUTTON)
   {
      if(obj == (Farso::GuiObject*)sndFileSelButton)
      {
         openFileWindow();
         return(true);
      }
   }
   else if(eventInfo == Farso::EVENT_FILE_SEL_ACCEPT)
   {
      if(fileWindow)
      {
         if(obj == (Farso::GuiObject*)fileSelector)
         {
            std::string f = fileSelector->getFileName();
            size_t it = f.find("../data/");
            if(it != std::string::npos)
            {
               f = f.replace(it, it+8,"");
            }

            curSound->fileName = f;
            sndFileText->setText(curSound->fileName);
            gui->closeWindow(fileWindow);
            return(true);
         }
      }
   }
   else if(eventInfo == Farso::EVENT_FILE_SEL_CANCEL)
   {
      if(fileWindow)
      {
         if(obj == (Farso::GuiObject*)fileSelector)
         {
            gui->closeWindow(fileWindow);
            return(true);
         }
      }
   }
   return(false);
}

/***********************************************************************
 *                            drawTemporary                            *
 ***********************************************************************/
void SoundEditor::drawTemporary()
{
   if(curNode)
   {
      curNode->getBoundingBox().render();
   }
}

/***********************************************************************
 *                           clearTemporary                            *
 ***********************************************************************/
void SoundEditor::clearTemporary()
{
   if(tmpNode)
   {
      scene sc;
      sc.deleteSceneNode(tmpNode);
      tmpNode = NULL;
   }
}


