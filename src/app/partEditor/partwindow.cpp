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
   curWindow = gui->insertWindow(posX, posY, posX+193, posY+298,
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
   drawMode->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
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
   renderMode->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
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

   /* Elements */
   element = curWindow->getObjectsList()->insertButton(20, 204, 97, 221, 
         "Elements", true);
   menu* men = new menu(80, 204, 192, 295, curWindow->getSurface());
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

   origin = curWindow->getObjectsList()->insertButton(103, 204, 173, 221, 
         "Origin", true);

   /* Gravity */
   curWindow->getObjectsList()->insertTextBox(10,227,90,244,0,
         "Gravity:");
   gravity = curWindow->getObjectsList()->insertTextBar(81,227,141,244,
         "", 0);

   /* Texture select */
   texture = curWindow->getObjectsList()->insertButton(60, 250, 133, 267, 
         "Texture", true);

   /* Restart the system */
   restart = curWindow->getObjectsList()->insertButton(60, 273, 133, 290,
         "Restart", true);


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

   setModeNames();

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

   /* Gravtiy */
   sprintf(buf, "%f", part->getGravity());
   gravity->setText(buf);

   curWindow->draw();
}

/***********************************************************************
 *                           setModeNames                              *
 ***********************************************************************/
void partWindow::setModeNames()
{
   /* drawMode */
   switch(part->getInt(DNT_PART_AUX_DRAW_MODE))
   {
      case DNT_PARTICLE_DRAW_GROUP:
      {
         drawMode->setText("Group");
      }
      break;
      case DNT_PARTICLE_DRAW_INDIVIDUAL:
      {
         drawMode->setText("Individual");
      }
   }

   /* renderMode */
   switch(part->getInt(DNT_PART_AUX_RENDER_MODE))
   {
      case DNT_PARTICLE_RENDER_DEFAULT:
      {
         renderMode->setText("Default");
      }
      break;
      case DNT_PARTICLE_RENDER_GLOW:
      {
         renderMode->setText("Glow");
      }
   }
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
      int i=0;
      if(object == (guiObject*)maxParticles)
      {
         if(!maxParticles->getText().empty())
         {
            sscanf(maxParticles->getText().c_str(), "%i", &i);
         }
         if(i > 0)
         {
            part->setInt(DNT_PART_AUX_MAX_PARTICLES, i);
         }
         return(true);
      }
      else if(object == (guiObject*)maxLifeTime)
      {
         sscanf(maxLifeTime->getText().c_str(), "%i", &i);
         part->setInt(DNT_PART_AUX_MAX_LIFE_TIME, i);
         return(true);
      }
      else if(object == (guiObject*)particleLifeTime)
      {
         sscanf(particleLifeTime->getText().c_str(), "%i", &i);
         part->setInt(DNT_PART_AUX_MAX_PARTICLE_LIFE_TIME, i);
         return(true);
      }
      else if(object == (guiObject*)pointSize)
      {
         sscanf(pointSize->getText().c_str(), "%i", &i);
         part->setInt(DNT_PART_AUX_POINT_SIZE, i);
         return(true);
      }
      else if(object == (guiObject*)gravity)
      {
         float g=0.0f;
         sscanf(gravity->getText().c_str(), "%f", &g);
         part->setGravity(g);
         return(true);
      }
   }
   else if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
   {
      if(object == (guiObject*)nextRenderMode)
      {
         if(part->getInt(DNT_PART_AUX_RENDER_MODE) < DNT_PARTICLE_RENDER_GLOW)
         {
            part->setInt(DNT_PART_AUX_RENDER_MODE,
                  part->getInt(DNT_PART_AUX_RENDER_MODE)+1);
            setModeNames();
         }
         return(true);
      }
      else if(object == (guiObject*)previousRenderMode)
      {
         if(part->getInt(DNT_PART_AUX_RENDER_MODE) > 0)
         {
            part->setInt(DNT_PART_AUX_RENDER_MODE,
                  part->getInt(DNT_PART_AUX_RENDER_MODE)-1);
            setModeNames();
         }
         return(true);
      }
      else if(object == (guiObject*)nextDrawMode)
      {
         if(part->getInt(DNT_PART_AUX_DRAW_MODE) < DNT_PARTICLE_DRAW_INDIVIDUAL)
         {
            part->setInt(DNT_PART_AUX_DRAW_MODE,
                  part->getInt(DNT_PART_AUX_DRAW_MODE)+1);
            setModeNames();
         }
         return(true);
      }
      else if(object == (guiObject*)previousDrawMode)
      {
         if(part->getInt(DNT_PART_AUX_DRAW_MODE) > 0)
         {
            part->setInt(DNT_PART_AUX_DRAW_MODE,
                  part->getInt(DNT_PART_AUX_DRAW_MODE)-1);
            setModeNames();
         }
         return(true);
      }
      else if(object == (guiObject*)restart)
      {
         part->reset();
      }

   }

   return(false);
}

