
#include "tabbox.h"

#include "guilist.h"

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                                  tabObj                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
tabObj::tabObj()
{
   list = new guiList();
   title = "";
   next = NULL;
   previous = NULL;
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
tabObj::~tabObj()
{
   delete(list);
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                                  tabBox                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
tabBox::tabBox(int xa, int ya, int xb, int yb, SDL_Surface* screen)
{
   /* Define guiObject type */
   type = GUI_TAB_BOX;
   /* Define Position */
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   wSurface = screen;

   /* And nullify the list! */
   first = NULL;
   total = 0;
   active = NULL;
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
tabBox::~tabBox()
{
   int i;
   tabObj* obj = first;

   /* Delete all elements on the list! */
   for(i = 0; i < total; i++)
   {
      active = obj;
      obj = obj->next;
      delete(active);
   }
   total = 0;
}

/***********************************************************************
 *                             getActiveList                           *
 ***********************************************************************/
guiList* tabBox::getActiveList()
{
   if(active)
   {
      return(active->list);
   }

   return(NULL);
}

/***********************************************************************
 *                            getActiveTitle                           *
 ***********************************************************************/
string tabBox::getActiveTitle()
{
   if(active)
   {
      return(active->title);
   }
   return("");
}

/***********************************************************************
 *                               getObject                             *
 ***********************************************************************/
tabObj* tabBox::getObject(int opt)
{
   int i;
   tabObj* obj = first;

   /* Search for the element */
   if( (opt < total) && (opt >= 0))
   {
      for(i = 0; i < opt; i++)
      {
         obj = obj->next;
      }
      return(obj);
   }

   return(NULL);
}

/***********************************************************************
 *                                getList                              *
 ***********************************************************************/
guiList* tabBox::getList(int opt)
{
   tabObj* obj = getObject(opt);

   if(obj != NULL)
   {
      return(obj->list);
   }

   return(NULL);
}

/***********************************************************************
 *                                getList                              *
 ***********************************************************************/
guiList* tabBox::getList(string title)
{
   int i;
   tabObj* obj = first;

   /* Search for the element */
   for(i = 0; i < total; i++)
   {
      if(obj->title == title)
      {
         /* Found it! */
         return(obj->list);
      }
      obj = obj->next;
   }

   return(NULL);
}

/***********************************************************************
 *                              getTotal                               *
 ***********************************************************************/
int tabBox::getTotal()
{
   return(total);
}
   

/***********************************************************************
 *                              insertOption                           *
 ***********************************************************************/
guiList* tabBox::insertOption(string title)
{
   tabObj* obj;

   /* Only insert if not found! */
   if(getList(title) == NULL)
   {
      obj = new tabObj;
      obj->title = title;
      obj->list->setSurface(wSurface);

      if(first == NULL)
      {
         obj->next = obj;
         obj->previous = obj;
      }
      else
      {
         obj->next = first;
         obj->previous = first->previous;
         obj->next->previous = obj;
         obj->previous->next = obj;
      }

      first = obj;

      /* Define the active one */
      active = obj;

      total++;

      return(obj->list);
   }

   return(getList(title));
}

/***********************************************************************
 *                                 draw                                *
 ***********************************************************************/
void tabBox::draw(SDL_Surface* screen)
{
   if(total == 0)
   {
     return; //nothing to draw
   }

   int i;
   int incX = (x2-x1) / total;
   int posX = 0;
   int endPos = 0;
   tabObj* obj;
   farso_colors colors;

   /* Set the font */
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL, 10);
   fnt.defineFontAlign(DNT_FONT_ALIGN_CENTER);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);

   /* Draw Limitators */
   color_Set(colors.colorButton.R, colors.colorButton.G,
             colors.colorButton.B, colors.colorButton.A);
   rectangle_Fill(screen, x1+1, y1+19, x2-1, y2-1);
   color_Set(colors.colorCont[0].R, colors.colorCont[0].G,
             colors.colorCont[0].B, colors.colorCont[0].A);
   rectangle_2Colors(screen, x1, y1+18, x2, y2,
                     colors.colorCont[1].R, colors.colorCont[1].G,
                     colors.colorCont[1].B, colors.colorCont[1].A);

   /* Draw All Titles */
   obj = first;
   posX = x1;
   for(i = 0; i < total; i++)
   {
      /* Define Final position */
      if(i != total-1)
      {
         endPos = posX+incX-1;
      }
      else
      {
         endPos = x2;
      }

      if(obj == active)
      {
         /* Draw some contorn */
         color_Set(colors.colorCont[0].R, colors.colorCont[0].G,
                   colors.colorCont[0].B, colors.colorCont[0].A);
         line_Draw(screen, posX, y1, posX, y1+18);
         line_Draw(screen, posX, y1, endPos, y1);
         color_Set(colors.colorCont[1].R, colors.colorCont[1].G,
                   colors.colorCont[1].B, colors.colorCont[1].A);
         line_Draw(screen, endPos, y1, endPos, y1+18);

         /* Without Line separator bellow */
         color_Set(colors.colorButton.R, colors.colorButton.G,
                   colors.colorButton.B, colors.colorButton.A);
         rectangle_Fill(screen, posX+1, y1+17, endPos-1, y1+19);
      }
      else
      {
         /* Draw some pressed contorn */
         color_Set(colors.colorCont[1].R, colors.colorCont[1].G,
                   colors.colorCont[1].B, colors.colorCont[1].A);
         rectangle_2Colors(screen, posX, y1, endPos, y1+17,
                           colors.colorCont[0].R, colors.colorCont[0].G,
                           colors.colorCont[0].B, colors.colorCont[0].A);
      }
      

      /* Draw the title text */
      color_Set(colors.colorText.R, colors.colorText.G,
                colors.colorText.B, colors.colorText.A);
      fnt.write(screen, posX+2, y1+2, obj->title, posX, y1, endPos, y1+17);

      /* Next! */
      posX += incX;
      obj = obj->next;
   }

   /* Draw all active objects */
   if(active)
   {
      active->list->draw(screen);
   }
}

/***********************************************************************
 *                              verifyChanges                          *
 ***********************************************************************/
bool tabBox::verifyChanges(int mouseX, int mouseY)
{
   if(total == 0)
   {
      return(false);//nothing to verify
   }
   int incX = (x2-x1) / total;
   int cur = 0;
   tabObj* obj = NULL;

   if(isMouseAt(x1, y1, x2, y1+18, mouseX, mouseY))
   {
      /* Clicked at the title bar! */
      cur = mouseX / incX;
      obj = getObject(cur);
      if( (obj != NULL) && (obj != active))
      {
         active = obj;
         draw(wSurface);
         setChanged();
         return(true);
      }
   }
   return(false);
}


