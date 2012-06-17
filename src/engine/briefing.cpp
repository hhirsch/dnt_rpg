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

#include "briefing.h"
#include "../etc/dirs.h"
#include "../lang/translate.h"
#include "util.h"

using namespace std;

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
void briefing::openWindow(Farso::GuiInterface* gui)
{
   dirs dir;

   if(briefWindow == NULL)
   {
      briefWindow = gui->insertWindow(0,0, 320, 160,
                                      gettext("Briefing"), true);
      briefTxt = briefWindow->getObjectsList()->insertRolBar(7,7,310,154,
                                     gettext("Welcome to DNT!"));
      briefWindow->getObjectsList()->insertPicture(0,0,0,0,
            dir.getRealFile("texturas/general/brief.png").c_str());
      briefWindow->setExternPointer(&briefWindow);
      gui->openWindow(briefWindow);
   }
}

/***********************************************************************
 *                            closeWindow                              *
 ***********************************************************************/
void briefing::closeWindow(Farso::GuiInterface* gui)
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
void briefing::reOpen(Farso::GuiInterface* gui)
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
          ((time - timeLastAdd) > DNT_BRIEFING_RATE) )
      {
         timeLastAdd = time;
         briefTxt->addText(text);
      }
   }
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
void briefing::addText(string text, int R, int G, int B, bool forceRep)
{
   dirs dir;
   addText(text, dir.getRealFile(DNT_FONT_ARIAL), 10, Farso::Font::STYLE_NORMAL,
           Farso::Font::ALIGN_LEFT, R, G, B, forceRep); 
}

/***********************************************************************
 *                              addText                                *
 ***********************************************************************/
void briefing::addText(string text, string font, int size, int style,
                       int align, int R, int G, int B, bool forceRep)
{
   if(briefWindow != NULL)
   {
      /* To avoid same last line */
      if( (briefTxt->getLastLine() != text) || (forceRep))
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
Farso::RolBar* briefing::briefTxt = NULL;
Farso::Window* briefing::briefWindow = NULL;
Uint32 briefing::timeLastAdd = 0;

