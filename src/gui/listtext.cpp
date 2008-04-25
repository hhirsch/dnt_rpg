/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
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

   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL, 10);

   /* Init things */
   type = GUI_LIST_TEXT;
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

   /* Remove GUI elements */
   /*Tlista* l = (Tlista*)intList;
   l->Retirar(roll);
   l->Retirar(table);*/
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
   if(type == PRESSED_TAB_BUTTON)
   {
      for(i = 0; i<maxButtons; i++)
      {
         if(object == (guiObject*)listButtons[i])
         {
            int pos = roll->getActualInit() + i;
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

