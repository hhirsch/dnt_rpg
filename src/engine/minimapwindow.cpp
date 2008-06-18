
#include "minimapwindow.h"

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
void miniMapWindow::open(guiInterface* gui, float posX, float posZ,
                         Map* openedMap)
{
   /* Set the current map */
   curMap = openedMap;

   if( (mapWindow == NULL) && (curMap != NULL) )
   {
      int winX=0, winY=0;

      /* Align Up of Shortcuts */
      winX = 0;
      winY = SCREEN_Y-257;

      /* Create the window */
      mapWindow = gui->insertWindow(winX, winY, winX+185, winY+128,
                                    gettext("Map"));

      charPosition = mapWindow->getObjectsList()->insertButton(0,0,1,1,
                                                               "",0);
      picture* fig = mapWindow->getObjectsList()->insertPicture(8,20,240,95,
                                                                NULL);
 
      /* Draw the minimap at the picture */
      curMap->drawMinimap(fig->get());

      /* Finally, open the window */
      mapWindow->setExternPointer(&mapWindow);
      gui->openWindow(mapWindow);

      /* And now, update the character position on miniMap */
      updateCharacterPosition(posX, posZ);
   }
}

/***********************************************************************
 *                              close                                  *
 ***********************************************************************/
void miniMapWindow::close(guiInterface* gui)
{
   if(mapWindow != NULL)
   {
      /* Close the window */
      gui->closeWindow(mapWindow);
      mapWindow = NULL;
   }
}

/***********************************************************************
 *                      updateCharacterPosition                        *
 ***********************************************************************/
void miniMapWindow::updateCharacterPosition(float posX, float posZ)
{
   if(mapWindow != NULL)
   {
      int div2;

      /* Convert Character position to the MiniMap Coordinates */
      GLint x = (int) (posX / curMap->squareSize());
      GLint z = (int) (posZ / curMap->squareSize());
      if(x > curMap->getSizeX()-1)
      {
         x = curMap->getSizeX()-1;
      }
      if( z > curMap->getSizeZ()-1)
      {
         z = curMap->getSizeZ()-1;
      }

      /* If Outdoor, must ignore the no walkable squares */
      if(curMap->isOutdoor())
      {
         x -= 7;
         z -= 7;
      }

      /* The incredible scale */
      x = 8 + (x*curMap->SQUAREMINISIZE);
      z = 20 + (z*curMap->SQUAREMINISIZE);
      div2 = curMap->SQUAREMINISIZE / 2;

      charPosition->setCoordinate(x+div2, z+div2, x+div2+1, z+div2+1);
      mapWindow->draw(-1, -1);
   }
}

/***********************************************************************
 *                          Static Fields                              *
 ***********************************************************************/
window* miniMapWindow::mapWindow = NULL;
button* miniMapWindow::charPosition = NULL;
Map* miniMapWindow::curMap = NULL;

