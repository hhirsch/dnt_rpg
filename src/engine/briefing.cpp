/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#include "briefing.h"

#define DNT_BRIEFING_RATE 500

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
briefing::briefing()
{
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
briefing::~briefing()
{
}

/***********************************************************************
 *                              isOpened                               *
 ***********************************************************************/
bool briefing::isOpened()
{
   return(briefWindow != NULL);
}

/***********************************************************************
 *                             openWindow                              *
 ***********************************************************************/
void briefing::openWindow(guiInterface* gui)
{
   if(briefWindow == NULL)
   {
      briefWindow = gui->insertWindow(186,SCREEN_Y-257, 512, SCREEN_Y-129,
                                      gettext("Briefing"));
      briefTxt = briefWindow->getObjectsList()->insertRolBar(8,18,318,121,
                                     gettext("Welcome to DNT!"));
      briefWindow->setExternPointer(&briefWindow);
      gui->openWindow(briefWindow);
   }
}

/***********************************************************************
 *                            closeWindow                              *
 ***********************************************************************/
void briefing::closeWindow(guiInterface* gui)
{
   if(briefWindow != NULL)
   {
      gui->closeWindow(briefWindow);
      briefWindow = NULL;
   }
}

/***********************************************************************
 *                              reOpen                                 *
 ***********************************************************************/
void briefing::reOpen(guiInterface* gui)
{
   int x1=0, y1=0, x2=0, y2=0;
   if(isOpened())
   {
      /* Retrieve current postion */
      x1 = briefWindow->getX1();
      y1 = briefWindow->getY1();
      x2 = briefWindow->getX2();
      y2 = briefWindow->getY2();
      
      /* Close the window */
      closeWindow(gui);
      /* Open the window */
      openWindow(gui);

      /* Reset the position */
      briefWindow->setCoordinate(x1, y1, x2, y2);
   }
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
void briefing::addText(string text)
{
   if(briefWindow != NULL)
   {
      /* To avoid same last line for too much mouse press, for example */
      Uint32 time = SDL_GetTicks(); 
      if( (briefTxt->getLastLine() != text) ||
          (time - timeLastAdd > DNT_BRIEFING_RATE) )
      {
         timeLastAdd = time;
         briefTxt->addText(text);
      }
   }
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
void briefing::addText(string text, int R, int G, int B)
{
   addText(text, DNT_FONT_ARIAL, 10, DNT_FONT_STYLE_NORMAL, 
           DNT_FONT_ALIGN_LEFT, R, G, B); 
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
void briefing::addText(string text, string font, int size, int style,
                       int align, int R, int G, int B)
{
   if(briefWindow != NULL)
   {
      /* To avoid same last line */
      if(briefTxt->getLastLine() != text)
      {
         briefTxt->addText(text, font, size, style, align, R, G, B);
      }
   }
}

/***********************************************************************
 *                           addCheckText                              *
 ***********************************************************************/
void briefing::addCheckText(string testName, int value, int difficulty)
{
   char buffer[512];
   bool res = (value >= difficulty);

   /* Buffer it */
   sprintf(&buffer[0], "%s: %d x %d: %s.",
         testName.c_str(), value, difficulty,
         res?gettext("Success"):gettext("Failure"));

   if(res)
   {
      /* With blue color */
      addText(buffer, 27, 169, 245);
   }
   else
   {
      /* With red color */
      addText(buffer, 233, 0, 5);
   }

}

/***********************************************************************
 *                           Static Members                            *
 ***********************************************************************/
rolBar* briefing::briefTxt = NULL;
window* briefing::briefWindow = NULL;
Uint32 briefing::timeLastAdd = 0;

