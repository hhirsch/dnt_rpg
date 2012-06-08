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

#include "minimapwindow.h"
#include "../lang/translate.h"
#include "../etc/dirs.h"

/***********************************************************************
 *                            constructor                              *
 ***********************************************************************/
miniMapWindow::miniMapWindow()
{
}

/***********************************************************************
 *                             destructor                              *
 ***********************************************************************/
miniMapWindow::~miniMapWindow()
{
}

/***********************************************************************
 *                               open                                  *
 ***********************************************************************/
void miniMapWindow::open(Farso::GuiInterface* gui, float posX, float posZ,
                         Map* openedMap) 
{
   /* Set the current map */
   curMap = openedMap;

   if( (mapWindow == NULL) && (curMap != NULL) )
   {
      int winX=0, winY=0;

      /* Align */
      winX = 0;
      winY = 161;

      width = curMap->getMiniMap()->w;
      height = curMap->getMiniMap()->h;

      /* Create the window */
      mapWindow = gui->insertWindow(winX, winY, winX+185, winY+128,
                                    gettext("Map"));

      charPosition = mapWindow->getObjectsList()->insertButton(0,0,1,1,
                                                               "",0);
      fig = mapWindow->getObjectsList()->insertPicture(6,17,width,height,NULL);
 
      /* Draw the minimap at the picture */
      SDL_BlitSurface(curMap->getMiniMap(), NULL, fig->get(), NULL);

      /* Set the visible, if needed */
      if( (width > 173) || (height > 106) )
      {
         fig->setVisibleArea(0,0,173,106);
      }

      /* Load connection image */
      dirs dir;
      connectionImage = IMG_Load(
            dir.getRealFile("texturas/general/travelmark.png").c_str());

      /* Set the connection labels */
      setConnections();

      /* Finally, open the window */
      mapWindow->setExternPointer(&mapWindow);
      gui->openWindow(mapWindow);

      /* And now, update the character position on miniMap */
      updateCharacterPosition(posX, posZ);
   }
}

/***********************************************************************
 *                         setConnections                              *
 ***********************************************************************/
void miniMapWindow::setConnections()
{   
   Square* sq;
   int i,j, aux;
   miniMapLabel* l;
   GLfloat ratio = (GLfloat)curMap->getSquareMiniSize() / 
                   (GLfloat)curMap->squareSize();

   /* Convert Character position to the MiniMap Coordinates */
   GLint x1,z1,x2,z2;
   
   for(i=0; i < curMap->getSizeX(); i++)
   {
      for(j=0; j < curMap->getSizeZ(); j++)
      {
         sq = curMap->relativeSquare(i, j);
         if( (sq) && (sq->mapConection.active) )
         {
            /* Convert to minimap coordinates */
            x1 = (int) (sq->mapConection.x1*ratio);
            z1 = (int) ((curMap->getSizeZ()*curMap->squareSize()) - 
                        sq->mapConection.z1)*ratio;

            /* Verify if label is already inserted */
            l = getLabel(x1, z1, sq->mapConection.mapName);
            if(!l)
            {
               /* Create and set label values */
               l = new miniMapLabel();
               l->labelText = sq->mapConection.mapName; 
               l->labelName = sq->mapConection.mapName; 
               l->position.x = x1;
               l->position.y = z1;

               /* Adjusts and calculations to draw on map */
               if(curMap->isOutdoor())
               {
                  x1 -= 1;
                  z1 -= 1;
                  x2 = x1 + 2;
                  z2 = z1 + 2;
               }
               else
               {
                  x2 = (int) (sq->mapConection.x2*ratio);
                  z2 = (int) ((curMap->getSizeZ()*curMap->squareSize()) - 
                           sq->mapConection.z2)*ratio;
               }

               /* Verify if inverted */
               if(x2 < x1)
               {
                  aux = x1;
                  x1 = x2;
                  x2 = aux;
               }
               if(z2 < z1)
               {
                  aux = z1;
                  z1 = z2;
                  z2 = aux;
               }

               /* Draw it on map picture */
               if(curMap->isOutdoor())
               {
                  Farso::color_Set(220, 20, 20, 255);
                  Farso::rectangle_Draw(fig->get(), x1, z1, x2, z2);
                  Farso::color_Set(235, 235, 25, 255);
                  Farso::rectangle_Fill(fig->get(), x1+1, z1+1, x2-1, z2-1);
               }
               else
               {
                  SDL_Rect rec;
                  rec.x = ((x2+x1)/2)-6;
                  rec.y = ((z2+z1)/2)-6;
                  SDL_BlitSurface(connectionImage, NULL, fig->get(), &rec);
               }

               /* Insert it on list */
               labels.push_front(l);
            }
         }
      }
   }
}

/***********************************************************************
 *                            getLabel                                 *
 ***********************************************************************/
miniMapLabel* miniMapWindow::getLabel(float x, float y, std::string name)
{
   std::list<miniMapLabel*>::iterator it;

   for(it=labels.begin(); it != labels.end(); it++)
   {
      if( ((*it)->position.x == x) && ((*it)->position.y == y) &&
          ((*it)->labelName == name) )
      {
         return(*it);
      }
   }
   return(NULL);
}

/***********************************************************************
 *                            getLabel                                 *
 ***********************************************************************/
void miniMapWindow::clearLabels()
{
   /* Clear all labels from the list */
   std::list<miniMapLabel*>::iterator it;

   for(it=labels.begin(); it != labels.end(); it++)
   {
      delete(*it);
   }
   labels.clear();
}

/***********************************************************************
 *                              close                                  *
 ***********************************************************************/
void miniMapWindow::close(Farso::GuiInterface* gui)
{
   if(mapWindow != NULL)
   {
      /* Close the window */
      gui->closeWindow(mapWindow);
      clearLabels();
      mapWindow = NULL;
      if(connectionImage)
      {
         SDL_FreeSurface(connectionImage);
         connectionImage = NULL;
      }
   }
}

/***********************************************************************
 *                             isOpened                                *
 ***********************************************************************/
bool miniMapWindow::isOpened()
{
   return(mapWindow != NULL);
}

/***********************************************************************
 *                              reOpen                                 *
 ***********************************************************************/
void miniMapWindow::reOpen(Farso::GuiInterface* gui, Map* openedMap)
{
   int x1=0, y1=0, x2=0, y2=0;
   if(isOpened())
   {
      /* Retrieve current postion */
      x1 = mapWindow->getX1();
      y1 = mapWindow->getY1();
      x2 = mapWindow->getX2();
      y2 = mapWindow->getY2();
      
      /* Close the window */
      close(gui);
      /* Open the window */
      open(gui, 0,0, openedMap);

      /* Reset the position */
      mapWindow->setCoordinate(x1, y1, x2, y2);
   }
}

/***********************************************************************
 *                      updateCharacterPosition                        *
 ***********************************************************************/
void miniMapWindow::updateCharacterPosition(float posX, float posZ)
{
   if(mapWindow != NULL)
   {
      bool reDraw = false;
      int iX=0, iY=0;
      int pX=0, pY=0;
      GLfloat ratio = (GLfloat)curMap->getSquareMiniSize() / 
                      (GLfloat)curMap->squareSize();

      /* Convert Character position to the MiniMap Coordinates */
      GLint x = (int) (posX*ratio);
      GLint z = (int) ((curMap->getSizeZ()*curMap->squareSize()) - posZ)*ratio;

      /* Set the visible, if needed */
      iX = 0;
      iY = 0;
      if( (width > 173) || (height > 106) )
      {
         iX = x-85;
         iY = z-53;
         if( (iX < 0) || (width <= 173))
         {
            iX = 0;
         }
         else if( (iX + 173 > width) )
         {
            iX = width-173;
         }
         if( (iY < 0) || (height <= 106))
         {
            iY = 0;
         }
         else if( (iY + 106 > height) )
         {
            iY = height-106;
         }
         fig->setVisibleArea(iX,iY,iX+173,iY+106);
         reDraw = true;
      }

      /* Set the position, if needed */
      pX = x - iX - 1;
      pY = z - iY - 1;
      if( ((8+pX) != charPosition->getX1()) || 
            ((20+pY) != charPosition->getY1()) )
      {
         charPosition->setCoordinate(8+x-iX-1, 20+z-iY-1, 
               8+x-iX+1, 20+z-iY+1);
         reDraw = true;
      }

      /* Redraw the window, if needed */
      if(reDraw)
      {
         mapWindow->draw(-1, -1);
      }

   }
}

/***********************************************************************
 *                          Static Fields                              *
 ***********************************************************************/
int miniMapWindow::width = 0;
int miniMapWindow::height = 0;
Farso::Picture* miniMapWindow::fig = NULL;
Farso::Window* miniMapWindow::mapWindow = NULL;
Farso::Button* miniMapWindow::charPosition = NULL;
Map* miniMapWindow::curMap = NULL;
SDL_Surface* miniMapWindow::connectionImage = NULL;
std::list<miniMapLabel*> miniMapWindow::labels;

