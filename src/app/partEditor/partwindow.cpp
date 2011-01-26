/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "partwindow.h"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
partWindow::partWindow(guiInterface* interf)
{
   curWindow = NULL;
   part = NULL;
   gui = interf;
   elementWindow = new partElementWindow(interf);
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
partWindow::~partWindow()
{
   delete(elementWindow);
   closeWindow();
}

/***********************************************************************
 *                              isOpened                               *
 ***********************************************************************/
bool partWindow::isOpen()
{
   return(curWindow != NULL);
}

/***********************************************************************
 *                                 set                                 *
 ***********************************************************************/
void partWindow::set(partAux* p)
{
   part = p;
   openWindow();
}

/***********************************************************************
 *                             openWindow                              *
 ***********************************************************************/
void partWindow::openWindow()
{
   int posX=0, 
       posY = 64;
   dntFont fnt;

   if(isOpen())
   {
      /* Clsoe to reopen */
      posX = curWindow->getX1();
      posY = curWindow->getY1();
      closeWindow();
   }

   /* Create the window */
   curWindow = gui->insertWindow(posX, posY, posX+193, posY+241,
                                "Particle"); 
   curWindow->setExternPointer(&curWindow);

   /* maxParticles */
   curWindow->getObjectsList()->insertTextBox(10,20,80,37,0,"MaxParticles:");
   maxParticles = curWindow->getObjectsList()->insertTextBar(81, 20, 141, 37, 
         "", 0);

   /* maxLifeTime */
   curWindow->getObjectsList()->insertTextBox(10,43,90,60,0,"MaxLifeTime:");
   maxLifeTime = curWindow->getObjectsList()->insertTextBar(81, 43, 141, 60, 
         "", 0);

   /* particleLifeTime */
   curWindow->getObjectsList()->insertTextBox(10,66,90,83,0,
         "ParticleLife:");
   particleLifeTime = curWindow->getObjectsList()->insertTextBar(81,66,141,83, 
         "", 0);

   /* DrawMode things */
   curWindow->getObjectsList()->insertTextBox(10,89,80,106,0,"DrawMode:");
   previousDrawMode = curWindow->getObjectsList()->insertButton(81,89,91,106, 
         fnt.createUnicode(0x25C4),0);
   previousDrawMode->defineFont(DNT_FONT_ARIAL, 9);
   drawMode = curWindow->getObjectsList()->insertTextBox(92, 89, 172, 106, 
         1, "");
   nextDrawMode = curWindow->getObjectsList()->insertButton(173, 89, 183, 106, 
         fnt.createUnicode(0x25BA),0);
   nextDrawMode->defineFont(DNT_FONT_ARIAL, 9);

   /* renderMode things */
   curWindow->getObjectsList()->insertTextBox(10,112,80,129,0,"RenderMode:");
   previousRenderMode = curWindow->getObjectsList()->insertButton(81,112,
         91,129, fnt.createUnicode(0x25C4),0);
   previousRenderMode->defineFont(DNT_FONT_ARIAL, 9);
   renderMode = curWindow->getObjectsList()->insertTextBox(92,112,172,129, 
         1, "");
   nextRenderMode = curWindow->getObjectsList()->insertButton(173,112,183,129, 
         fnt.createUnicode(0x25BA),0);
   nextRenderMode->defineFont(DNT_FONT_ARIAL, 9);

   /* pointSize */
   curWindow->getObjectsList()->insertTextBox(10,135,80,152,0,"PointSize:");
   pointSize = curWindow->getObjectsList()->insertTextBar(81, 135, 141, 152, 
         "", 0);

   /* Wind Affect */
   windAffect = curWindow->getObjectsList()->insertCxSel(10, 162, false);
   curWindow->getObjectsList()->insertTextBox(21,158,91,175,0,"Wind Affect");

   /* Floor Collision */
   floorCollision = curWindow->getObjectsList()->insertCxSel(10, 185, false);
   curWindow->getObjectsList()->insertTextBox(21,181,121,198,0,
         "Floor Collision");

   element = curWindow->getObjectsList()->insertButton(10, 204, 80, 221, 
         "Elements", true);
   menu* men = new menu(80, 204, 193, 241, curWindow->getSurface());
   men->insertItem(DNT_PART_AUX_PARTICLES_TO_CREATE, true);
   men->insertItem("-", false);
   men->insertItem(DNT_PART_AUX_RED, true);
   men->insertItem(DNT_PART_AUX_GREEN, true);
   men->insertItem(DNT_PART_AUX_BLUE, true);
   men->insertItem(DNT_PART_AUX_ALPHA, true);
   men->insertItem("-", false);
   men->insertItem(DNT_PART_AUX_VELX, true);
   men->insertItem(DNT_PART_AUX_VELY, true);
   men->insertItem(DNT_PART_AUX_VELZ, true);
   men->insertItem("-", false);
   men->insertItem(DNT_PART_AUX_POSX, true);
   men->insertItem(DNT_PART_AUX_POSY, true);
   men->insertItem(DNT_PART_AUX_POSZ, true);
   men->insertItem("-", false);
   men->insertItem(DNT_PART_AUX_SCALE, true);
   element->men = (void*)men;

   origin = curWindow->getObjectsList()->insertButton(90, 204, 160, 221, 
         "Origin", true);


   /* Finally, open the window */
   gui->openWindow(curWindow);
   setTextValues();
}

/***********************************************************************
 *                            closeWindow                              *
 ***********************************************************************/
void partWindow::closeWindow()
{
   if(isOpen())
   {
      gui->closeWindow(curWindow);
   }
}

/***********************************************************************
 *                           setTextValues                             *
 ***********************************************************************/
void partWindow::setTextValues()
{
   char buf[128];
   if(!isOpen())
   {
      return;
   }

   /* Max Particles */
   sprintf(buf, "%d", part->getMaxParticles());
   maxParticles->setText(buf);

   /* Max Life Time */
   sprintf(buf, "%d", part->getInt(DNT_PART_AUX_MAX_LIFE_TIME));
   maxLifeTime->setText(buf);

   /* Particle Life Time */
   sprintf(buf, "%d", part->getInt(DNT_PART_AUX_MAX_PARTICLE_LIFE_TIME));
   particleLifeTime->setText(buf);

   /* PointSize */
   sprintf(buf, "%d", part->getInt(DNT_PART_AUX_POINT_SIZE));
   pointSize->setText(buf);

   /* Wind Affect */
   windAffect->setSelection(part->getBool(DNT_PART_AUX_WIND_AFFECT));

   /* Floor Collision */
   floorCollision->setSelection(part->getBool(DNT_PART_AUX_FLOOR_COLLISION));

   curWindow->draw();
}

/***********************************************************************
 *                               treat                                 *
 ***********************************************************************/
bool partWindow::treat(guiObject* object, int eventInfo)
{
   if(elementWindow->isOpen())
   {
      /* Treat element window */
      if(elementWindow->treat(object, eventInfo))
      {
         return(true);
      }
   }

   if(eventInfo == FARSO_EVENT_SELECTED_MENU)
   {
      menu* men = (menu*)element->men;
      guiObject* actualItem = men->getItem(men->getActualItem());
      if(actualItem != NULL)
      {
         dntPartElement* e = part->getElement(actualItem->getText());
         elementWindow->setElement(e, actualItem->getText(), part);
      }
   }
   else if(eventInfo == FARSO_EVENT_WROTE_TEXT_BAR)
   {
      int i;
      if(object == (guiObject*)maxParticles)
      {
         sscanf(maxParticles->getText().c_str(), "%i", &i);
         part->setInt(DNT_PART_AUX_MAX_PARTICLES, i);
      }
      else if(object == (guiObject*)maxLifeTime)
      {
         sscanf(maxLifeTime->getText().c_str(), "%i", &i);
         part->setInt(DNT_PART_AUX_MAX_LIFE_TIME, i);
      }
      else if(object == (guiObject*)particleLifeTime)
      {
         sscanf(particleLifeTime->getText().c_str(), "%i", &i);
         part->setInt(DNT_PART_AUX_MAX_PARTICLE_LIFE_TIME, i);
      }
      else if(object == (guiObject*)pointSize)
      {
         sscanf(pointSize->getText().c_str(), "%i", &i);
         part->setInt(DNT_PART_AUX_POINT_SIZE, i);
      }
   }

   return(false);
}

