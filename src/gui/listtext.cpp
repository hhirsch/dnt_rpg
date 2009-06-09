/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#include "listtext.h"
#include "guilist.h"
#include "interface.h"


/**************************************************************
 *                        Constructor                         *
 **************************************************************/
listText::listText(int xa, int ya, int xb, int yb, SDL_Surface* surface,
                   void* list)
{
   int i;
   /* Nullify elements */
   first = NULL;
   totalElements = 0;
   selectedText = ""; 
   selectedPos = -1;  

   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL, 10);

   /* Init things */
   type = FARSO_OBJECT_LIST_TEXT;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   windowSurface = surface;

   /* Alloc tabButtons pointers */
   maxButtons = ((yb-ya-2) / fnt.getHeight());
   listButtons = new oneTabButton*[maxButtons];


   intList = list;

   guiList* l = (guiList*)list;

   /* Create the rollbar */
   roll = l->insertRolBar(x1, y1, x2, y2, "");
   
   /* Create the tabButton */
   table = l->insertTabButton(x1, y1, x2-x1-12, y2-y1, NULL);
   for(i = 0; i<maxButtons; i++)
   {
      listButtons[i] = table->insertButton(1, (i*fnt.getHeight())+3,
                                              (x2-x1)-14, 
                                              ((i+1)*fnt.getHeight())+2);
   }

   table->setObjectBelow(roll);

}

/**************************************************************
 *                         Destructor                         *
 **************************************************************/
listText::~listText()
{
   int i;
   textElement* tel = first;
   textElement* aux;

   /* Delete the pointers */
   delete [] listButtons;

   /* Delete All elements */
   for(i=0; i<totalElements; i++)
   {
      aux = tel;
      tel = tel->next;
      delete(aux);
   }
   totalElements = 0;
}

/**************************************************************
 *                           clear                            *
 **************************************************************/
void listText::clear()
{
   int i;
   textElement* tel = first;
   textElement* aux = NULL;

   
   /* Delete All elements */
   for(i=0; i<totalElements; i++)
   {
      aux = tel;
      tel = tel->next;
      delete(aux);
   }
   totalElements = 0;
   first = NULL;
   roll->setText("");

   defineTabButton();
}

/**************************************************************
 *                         insertText                         *
 **************************************************************/
void listText::insertText(string text)
{
   /* Insert with default color */
   insertText(text, -1, -1, -1);
}

/**************************************************************
 *                         insertText                         *
 **************************************************************/
void listText::insertText(string text, int r, int g, int b)
{
   textElement* tel = new textElement();
   tel->text = text;
   if(totalElements == 0)
   {
      first = tel;
      first->next = first;
      first->previous = first;
   }
   else
   {
      tel->next = first;
      tel->previous = first->previous;
      first->previous = tel;
      tel->previous->next = tel;
   }
   if(r == -1)
   {
      roll->addText(text);
   }
   else
   {
      roll->addText(text, DNT_FONT_ARIAL, 10, DNT_FONT_STYLE_NORMAL,
                    DNT_FONT_ALIGN_LEFT, r,g,b);
   }
   totalElements++;
   roll->setFirstLine(0);
   defineTabButton();
}

/**************************************************************
 *                         removeText                         *
 **************************************************************/
void listText::removeText(string text)
{
   int i;
   textElement* tel = first;
   for(i=0; i<totalElements; i++)
   {
      if(tel->text == text)
      {
         if(tel == first)
         {
            first = first->next;
         }
         tel->previous->next = tel->next;
         tel->next->previous = tel->previous;
         delete(tel);
         totalElements--;
         if(totalElements == 0)
         {
            first = NULL;
         }
         return;
      }
      tel = tel->next;
   }

   defineTabButton();
}

/**************************************************************
 *                      defineTabButton                       *
 **************************************************************/
void listText::defineTabButton()
{
   int curInit = roll->getFirstLine();
   int i;

   for(i = 0; i<maxButtons; i++)
   {
      listButtons[i]->setAvailable(i+curInit < totalElements);
   }
}

/**************************************************************
 *                            draw                            *
 **************************************************************/
void listText::draw(SDL_Surface* screen)
{
   //Actually nothing to do here
}

/**************************************************************
 *                         eventGot                           *
 **************************************************************/
bool listText::eventGot(int type, guiObject* object)
{
   int i;
   selectedText = "";
   selectedPos = -1;

   /* Verify Events */

   /* RolBar events */
   if(type == FARSO_EVENT_PRESSED_BUTTON)
   {
      if(roll->isOwner(object))
      {
         /* Must reconstruct tabbutton avaialability */
         defineTabButton();
      }
   }

   /* TabButton events */
   else if(type == FARSO_EVENT_PRESSED_TAB_BUTTON)
   {
      for(i = 0; i<maxButtons; i++)
      {
         if(object == (guiObject*)listButtons[i])
         {
            int pos = roll->getFirstLine() + i;
            if(pos < totalElements)
            {
               int k;
               textElement* tel = first;
               for(k = 0; k < pos; k++)
               {
                  tel = tel->next;
               }
               selectedText = tel->text;
               selectedPos = pos;
            }
            return(true);
         }
      }
   }
   return(false);
}

/**************************************************************
 *                      getSelectedText                       *
 **************************************************************/
string listText::getSelectedText()
{
   return(selectedText);
}

/**************************************************************
 *                      getSelectedPos                        *
 **************************************************************/
int listText::getSelectedPos()
{  
   return(selectedPos);
}

