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

#include "listtext.h"
#include "guilist.h"
#include "interface.h"


/**************************************************************
 *                        Constructor                         *
 **************************************************************/
listText::listText(int xa, int ya, int xb, int yb, SDL_Surface* surface,
                   guiList* list)
         : guiObject(surface), dntList(DNT_LIST_TYPE_ADD_AT_END)
{
   int i;
   /* Nullify elements */
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

   /* Alloc tabButtons pointers */
   maxButtons = ((yb-ya-2) / fnt.getHeight());
   listButtons = new oneTabButton*[maxButtons];


   intList = list;

   guiList* l = (guiList*)list;

   /* Create the rollbar */
   roll = l->insertRolBar(x1, y1, x2, y2, "");
   
   /* Create the tabButton */
   table = l->insertTabButton(x1, y1, x2-x1-12, y2-y1, NULL);
   table->setStyle(FARSO_TAB_BUTTON_STYLE_LIST_TEXT);
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
   /* Delete the pointers */
   delete [] listButtons;
   clearList();
}

/**************************************************************
 *                         freeElement                        *
 **************************************************************/
void listText::freeElement(dntListElement* obj)
{
   textElement* tx = (textElement*)obj;
   delete(tx);
}

/**************************************************************
 *                           clear                            *
 **************************************************************/
void listText::clear()
{
   clearList();
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
   insert(tel);
   if(r == -1)
   {
      roll->addText(text);
   }
   else
   {
      roll->addText(text, DNT_FONT_ARIAL, 10, DNT_FONT_STYLE_NORMAL,
                    DNT_FONT_ALIGN_LEFT, r,g,b);
   }
   roll->setFirstLine(0);
   defineTabButton();
}

/**************************************************************
 *                         removeText                         *
 **************************************************************/
void listText::removeText(string text)
{
   int i;
   int prevTotal = total;
   textElement* tel = (textElement*)first;
   textElement* taux;
   for(i=0; i < prevTotal; i++)
   {
      taux = (textElement*)tel->getNext();
      if(tel->text == text)
      {
         remove(tel);
      }
      tel = taux;
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
      listButtons[i]->setAvailable(i+curInit < total);
   }
}

/**************************************************************
 *                            draw                            *
 **************************************************************/
void listText::draw()
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
            if(pos < total)
            {
               int k;
               textElement* tel = (textElement*)first;
               for(k = 0; k < pos; k++)
               {
                  tel = (textElement*)tel->getNext();
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

