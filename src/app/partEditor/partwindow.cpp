/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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
#include "../../gui/farsoopts.h"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
partWindow::partWindow(Farso::GuiInterface* interf)
{
   curWindow = NULL;
   textureWindow = NULL;
   part = NULL;
   max = 0;
   gui = interf;
   elementWindow = new partElementWindow(interf);
   originWindow = new partOriginWindow(interf);
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
partWindow::~partWindow()
{
   delete(elementWindow);
   delete(originWindow);
   closeWindow();
}

/***********************************************************************
 *                           openTextureWindow                         *
 ***********************************************************************/
void partWindow::openTextureWindow()
{
   if(textureWindow)
   {
      /* Close the current one */
      gui->closeWindow(textureWindow);
   }

   textureWindow = gui->insertWindow(200,100,460,285,"File");
   fileSelector = textureWindow->getObjectsList()->insertFileSel(5,18, true,
         "../data/particles/");
   fileSelector->setFilter(".png");
   /*fileSelector->setFileName(part->getTextureFileName());*/
   textureWindow->setAttributes(false,true,false,false);
   textureWindow->setExternPointer(&textureWindow);
   gui->openWindow(textureWindow);
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
   max = 0;
   openWindow();
   /* Close element and origin windows to avoid edit previous particle */
   elementWindow->closeWindow();
   originWindow->closeWindow();
}

/***********************************************************************
 *                             openWindow                              *
 ***********************************************************************/
void partWindow::openWindow()
{
   Farso::Options opt;
   int posX=0, 
       posY = 64;
   int y;
   Farso::Font fnt;

   if(isOpen())
   {
      /* Close to reopen */
      posX = curWindow->getX1();
      posY = curWindow->getY1();
      closeWindow();
   }

   /* Create the window */
   curWindow = gui->insertWindow(posX, posY, posX+193, posY+321,
                                "Particle"); 
   curWindow->setExternPointer(&curWindow);

   y = 20;

   /* Current number of particles */
   curParticles = curWindow->getObjectsList()->insertTextBox(10,y,183,y+17,3,
         "Current Particles: ");
   y += 23;

   /* maxParticles */
   curWindow->getObjectsList()->insertTextBox(10,y,80,y+17,0,"MaxParticles:");
   maxParticles = curWindow->getObjectsList()->insertTextBar(81, y, 141, y+17, 
         "", 0);
   y += 23;

   /* maxLifeTime */
   curWindow->getObjectsList()->insertTextBox(10,y,90,y+17,0,"MaxLifeTime:");
   maxLifeTime = curWindow->getObjectsList()->insertTextBar(81, y, 141, y+17, 
         "", 0);
   y += 23;

   /* particleLifeTime */
   curWindow->getObjectsList()->insertTextBox(10,y,90,y+17,0,
         "ParticleLife:");
   particleLifeTime = curWindow->getObjectsList()->insertTextBar(81,y,141,y+17, 
         "", 0);
   y += 23;

   /* DrawMode things */
   curWindow->getObjectsList()->insertTextBox(10,y,80,y+17,0,"DrawMode:");
   previousDrawMode = curWindow->getObjectsList()->insertButton(81,y,91,y+17, 
         fnt.createUnicode(0x25C4),0);
   previousDrawMode->defineFont(opt.getDefaultFont(), 9);
   drawMode = curWindow->getObjectsList()->insertTextBox(92, y, 172, y+17, 
         1, "");
   drawMode->setFont(opt.getDefaultFont(), 10, Farso::Font::ALIGN_CENTER);
   nextDrawMode = curWindow->getObjectsList()->insertButton(173, y, 183, y+17, 
         fnt.createUnicode(0x25BA),0);
   nextDrawMode->defineFont(opt.getDefaultFont(), 9);
   y += 23;

   /* renderMode things */
   curWindow->getObjectsList()->insertTextBox(10,y,80,y+17,0,"RenderMode:");
   previousRenderMode = curWindow->getObjectsList()->insertButton(81,y,
         91,y+17, fnt.createUnicode(0x25C4),0);
   previousRenderMode->defineFont(opt.getDefaultFont(), 9);
   renderMode = curWindow->getObjectsList()->insertTextBox(92,y,172,y+17, 
         1, "");
   renderMode->setFont(opt.getDefaultFont(), 10, Farso::Font::ALIGN_CENTER);
   nextRenderMode = curWindow->getObjectsList()->insertButton(173,112,183,129, 
         fnt.createUnicode(0x25BA),0);
   nextRenderMode->defineFont(opt.getDefaultFont(), 9);
   y += 23;

   /* pointSize */
   curWindow->getObjectsList()->insertTextBox(10,y,80,y+17,0,"PointSize:");
   pointSize = curWindow->getObjectsList()->insertTextBar(81, y, 141, y+17, 
         "", 0);
   y += 23;

   /* Wind Affect */
   windAffect = curWindow->getObjectsList()->insertCxSel(10, y+4, false);
   curWindow->getObjectsList()->insertTextBox(21,y,91,y+17,0,"Wind Affect");
   y += 23;

   /* Floor Collision */
   floorCollision = curWindow->getObjectsList()->insertCxSel(10, y+4, false);
   curWindow->getObjectsList()->insertTextBox(21,y,121,y+17,0,
         "Floor Collision");
   y += 23;

   /* Elements */
   element = curWindow->getObjectsList()->insertButton(20, y, 97, y+17, 
         "Elements", true);
   Farso::Menu* men = new Farso::Menu(80,204,192,318,curWindow->getSurface());
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
   element->men = men;

   origin = curWindow->getObjectsList()->insertButton(103, y, 173, y+17, 
         "Origin", true);
   y += 23;

   /* Gravity */
   curWindow->getObjectsList()->insertTextBox(10,y,90,y+17,0,
         "Gravity:");
   gravity = curWindow->getObjectsList()->insertTextBar(81,y,141,y+17,
         "", 0);
   y += 23;

   /* Texture select */
   texture = curWindow->getObjectsList()->insertButton(60, y, 133, y+17, 
         "Texture", true);
   y += 23;

   /* Restart the system */
   restart = curWindow->getObjectsList()->insertButton(60, y, 133, y+17,
         "Restart", true);
   y += 23;

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

   /* Gravity */
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
 *                          writeCurParticles                          *
 ***********************************************************************/
void partWindow::writeCurParticles()
{
   char buf[256];

   /* Rewrite number of current particles */
   if( (part) && (isOpen()))
   {
      if(part->numParticles() > max)
      {
         max = part->numParticles();
      }
      sprintf(buf, "Current: %d Max: %d", part->numParticles(), max); 
      curParticles->setText(buf);
   }
}


/***********************************************************************
 *                               treat                                 *
 ***********************************************************************/
bool partWindow::treat(Farso::GuiObject* object, int eventInfo)
{
   if(elementWindow->isOpen())
   {
      /* Treat element window */
      if(elementWindow->treat(object, eventInfo))
      {
         return(true);
      }
   }

   if(originWindow->isOpen())
   {
      /* Treat origin window */
      if(originWindow->treat(object, eventInfo))
      {
         return(true);
      }
   }

   if(eventInfo == Farso::EVENT_SELECTED_MENU)
   {
      Farso::Menu* men = (Farso::Menu*)element->men;
      Farso::GuiObject* actualItem = men->getItem(men->getActualItem());
      if(actualItem != NULL)
      {
         dntPartElement* e = part->getElement(actualItem->getText());
         elementWindow->setElement(e, actualItem->getText(), part);
      }
   }
   else if(eventInfo == Farso::EVENT_WROTE_TEXT_BAR)
   {
      int i=0;
      if(object == (Farso::GuiObject*)maxParticles)
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
      else if(object == (Farso::GuiObject*)maxLifeTime)
      {
         sscanf(maxLifeTime->getText().c_str(), "%i", &i);
         part->setInt(DNT_PART_AUX_MAX_LIFE_TIME, i);
         return(true);
      }
      else if(object == (Farso::GuiObject*)particleLifeTime)
      {
         sscanf(particleLifeTime->getText().c_str(), "%i", &i);
         part->setInt(DNT_PART_AUX_MAX_PARTICLE_LIFE_TIME, i);
         return(true);
      }
      else if(object == (Farso::GuiObject*)pointSize)
      {
         sscanf(pointSize->getText().c_str(), "%i", &i);
         part->setInt(DNT_PART_AUX_POINT_SIZE, i);
         return(true);
      }
      else if(object == (Farso::GuiObject*)gravity)
      {
         float g=0.0f;
         sscanf(gravity->getText().c_str(), "%f", &g);
         part->setGravity(g);
         return(true);
      }
   }
   else if(eventInfo == Farso::EVENT_PRESSED_BUTTON)
   {
      if(object == (Farso::GuiObject*)nextRenderMode)
      {
         if(part->getInt(DNT_PART_AUX_RENDER_MODE) < DNT_PARTICLE_RENDER_GLOW)
         {
            part->setInt(DNT_PART_AUX_RENDER_MODE,
                  part->getInt(DNT_PART_AUX_RENDER_MODE)+1);
            setModeNames();
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)previousRenderMode)
      {
         if(part->getInt(DNT_PART_AUX_RENDER_MODE) > 0)
         {
            part->setInt(DNT_PART_AUX_RENDER_MODE,
                  part->getInt(DNT_PART_AUX_RENDER_MODE)-1);
            setModeNames();
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)nextDrawMode)
      {
         if(part->getInt(DNT_PART_AUX_DRAW_MODE) < DNT_PARTICLE_DRAW_INDIVIDUAL)
         {
            part->setInt(DNT_PART_AUX_DRAW_MODE,
                  part->getInt(DNT_PART_AUX_DRAW_MODE)+1);
            setModeNames();
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)previousDrawMode)
      {
         if(part->getInt(DNT_PART_AUX_DRAW_MODE) > 0)
         {
            part->setInt(DNT_PART_AUX_DRAW_MODE,
                  part->getInt(DNT_PART_AUX_DRAW_MODE)-1);
            setModeNames();
         }
         return(true);
      }
      else if(object == (Farso::GuiObject*)restart)
      {
         part->reset();
      }
      else if(object == (Farso::GuiObject*)origin)
      {
         originWindow->setParticle(part);
      }
      else if(object == (Farso::GuiObject*)texture)
      {
         openTextureWindow();
      }
   }
   /* Texture Load things */
   else if(eventInfo == Farso::EVENT_FILE_SEL_ACCEPT)
   {
      if( (textureWindow) && (object == (Farso::GuiObject*)fileSelector) )
      {
         part->setTextureFileName(fileSelector->getFileName());
         gui->closeWindow(textureWindow);
      }
   }
   else if(eventInfo == Farso::EVENT_FILE_SEL_CANCEL)
   {
      if((textureWindow) && (object == (Farso::GuiObject*)fileSelector))
      {
         /* Just close the window */
         gui->closeWindow(textureWindow);
      }
   }

   return(false);
}

