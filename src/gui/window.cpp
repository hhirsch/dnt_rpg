/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "window.h"
#include "menu.h"
#include "../etc/dirs.h"

/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
windowList::windowList()
{
   activeWindow = NULL;
   total = 0;
   first = (window*) new window(10,10,20,20,"",this);
   first->type = GUI_WINDOW;
   first->next = first;
   first->previous = first;
   intMenu = NULL;
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
windowList::~windowList()
{
   window* jan;
   window* tmp;
   jan = (window*)first->next;
   while(jan != first )
   {
      tmp = jan;
      jan = (window*)jan->next;
      delete(tmp);
   }
} 
 
/*********************************************************************
 *                           insertWindow                            *
 *********************************************************************/
window* windowList::insertWindow(int xa,int ya,int xb,int yb,string text)
{
   window* novo;
   novo = new window(xa,ya,xb,yb,text,this);
   novo->next = first->next;
   novo->previous = first;
   first->next = novo;
   novo->next->previous = novo;
   total++;
   return(novo);
} 

/*********************************************************************
 *                           removeWindow                            *
 *********************************************************************/
void windowList::removeWindow(window *jan)
{
   jan->previous->next = jan->next;
   jan->next->previous = jan->previous;
   if (jan->getSurface() == NULL)
      printf("Fatal: jan->surface == NULL!!!\n");
   if(jan == activeWindow)
   {
      activeWindow = NULL;
      if((total > 1))
      {
         window* j = (window*)first->next;
         j->activate();
      }
   }
   delete(jan);
   jan = NULL;
   total--;
}

/**************************************************************
 *                            addMenu                         *
 **************************************************************/
guiObject* windowList::addMenu()
{
   if(intMenu)
   {
      removeMenu();
   }
   intMenu = (guiObject*)new menu(0,0);
   return(intMenu);
}

/**************************************************************
 *                           getMenu                          *
 **************************************************************/
guiObject* windowList::getMenu()
{
   return(intMenu);
}

/**************************************************************
 *                          removeMenu                        *
 **************************************************************/
void windowList::removeMenu()
{
   if(intMenu)
   {
      menu* men = (menu*)intMenu;
      delete(men);
   }
   intMenu = NULL;
}

/*********************************************************************
 *                             Constructor                           *
 *********************************************************************/
window::window(int xa, int ya, int xb, int yb, string title, void* list)
{
   dntFont fnt;
   dirs dir;

   /* Set Variables */
   intList = list;
   x1 = xa;
   x2 = xb;
   y1 = ya;
   y2 = yb;
   minY = 15;
   minX = 50;
   text = title;
   canMaximize = true;
   canScale = true;
   canClose = true;
   canMove = true;
   modal = false;
   externPointer = NULL;

   /* Define Machine Bit Order */
   Uint32 rmask, gmask, bmask, amask;
   #if SDL_BYTEORDER == SDL_BIG_ENDIAN
      rmask = 0xff000000;
      gmask = 0x00ff0000;
      bmask = 0x0000ff00;
      amask = 0x000000ff;
   #else
      rmask = 0x000000ff;
      gmask = 0x0000ff00;
      bmask = 0x00ff0000;
      amask = 0xff000000;
   #endif

   alpha = 0.75;
   /* Create Surface And generate texture */
   glGenTextures(1,&texture);
   surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                  smallestPowerOfTwo(xb-xa),
                                  smallestPowerOfTwo(yb-ya),32,
                                  rmask,gmask,bmask,amask);
   color_Set(255, 255, 255, 255);
   rectangle_Fill(surface, 0,0, smallestPowerOfTwo(xb-xa)-1,
                  smallestPowerOfTwo(yb-ya)-1);

   propX = (float)(xb-xa) / (float)smallestPowerOfTwo(xb-xa);
   propY = (float)(yb-ya) / (float)smallestPowerOfTwo(yb-ya);

   /* Create Objects List */
   objects = new guiList;
   objects->setSurface(surface);

   /* Create Menu Button */
   menuButton = objects->insertButton(3,3,13,12,"-",0);
   menuButton->men = new menu(0,0);
   menu* men = (menu*) menuButton->men;
   men->insertItem(gettext("Close"), dir.getRealFile("icons/close.png"), 1);
   men->insertItem("-",0);
   men->insertItem(gettext("Maximize"), 
                   dir.getRealFile("icons/maximize.png") ,0);

   /* Create Close Button */
   closeButton = objects->insertButton(14,3,24,12,fnt.createUnicode(0x25CF),0);
   closeButton->defineFont(DNT_FONT_ARIAL, 10);

   /* Create Minimize Maximize Button */
   minMaxButton = objects->insertButton(25,3,35,12,fnt.createUnicode(0x25B2),0);
   minMaxButton->defineFont(DNT_FONT_ARIAL, 8);

   /* Set the object type as WINDOW! */
   type = GUI_WINDOW;
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
window::~window()
{
   if(externPointer != NULL)
   {
      *externPointer = NULL;
   }
   glDeleteTextures(1,&texture);
   SDL_FreeSurface(surface);
   delete(objects);
}

/*********************************************************************
 *                                draw                               *
 *********************************************************************/
void window::draw(SDL_Surface* screen)
{
   draw(-1,-1, true);
}

/*********************************************************************
 *                                draw                               *
 *********************************************************************/
void window::draw(int mouseX, int mouseY, bool drawBar)
{
   int dx = x2 - x1;
   int dy = y2 - y1;

   color_Set(Colors.colorWindow.R, Colors.colorWindow.G,
             Colors.colorWindow.B, Colors.colorWindow.A);
   rectangle_Fill(surface, 3,3,dx-3,dy-3);
   color_Set(Colors.colorCont[0].R, Colors.colorCont[0].G,
             Colors.colorCont[0].B, Colors.colorCont[0].A);
   rectangle_Draw(surface,0,0,dx-1,dy-1);
   color_Set(Colors.colorButton.R, Colors.colorButton.G, 
             Colors.colorButton.B, Colors.colorButton.A);
   rectangle_Draw(surface,1,1,dx-2,dy-2);
   color_Set(Colors.colorCont[2].R, Colors.colorCont[2].G,
             Colors.colorCont[2].B, Colors.colorCont[2].A);
   line_Draw(surface,2,13,dx-4,13);
   color_Set(Colors.colorCont[0].R, Colors.colorCont[0].G,
             Colors.colorCont[0].B, Colors.colorCont[0].A);
   rectangle_2Colors(surface,2,2,dx-3,dy-3,Colors.colorCont[2].R,
                     Colors.colorCont[2].G,Colors.colorCont[2].B,
                     Colors.colorCont[2].A);


   /* Draw the bar */
   windowList* lst = (windowList*)intList;

   if(drawBar)
   {
      if(this == lst->getActiveWindow())
      {
         drawActiveBar();
      }
      else
      {
         drawInactiveBar();
      }
   }

   /* Objects Draw */
   guiObject *obj=objects->getFirst()->next;
   int aux;
   for(aux=0;aux<objects->getTotal();aux++)
   {
      switch(obj->type)
      {
         case GUI_BUTTON:
         {
              button *b = (button*) obj;   
              b->draw(surface);
              break;
         }
         case GUI_TEXT_BAR:
         {
              textBar *bart = (textBar*) obj; 
              bart->draw(surface);
              break;
         }
         case GUI_SEL_BOX:
         {
              cxSel *cx = (cxSel*) obj;
              cx->draw(surface);
              break;
         }
         case GUI_SEL_TEXT:
         {
              selText *st = (selText*) obj;
              st->draw(surface);
              break;
         }
         case GUI_PICTURE:
         {
              picture* fig = (picture*) obj;
              fig->draw(surface);
              break;
         }
         case GUI_TEXT_BOX:
         {
              textBox *quad = (textBox*) obj;
              quad->draw();
              break;
         }
         case GUI_TAB_BUTTON:
         {
              tabButton *bt = (tabButton*) obj; 
              bt->setCurrent(-1);
              bt->draw(surface);
              break;
         }
         case GUI_HEALTH_BAR:
         {
              healthBar* hb = (healthBar*) obj;
              hb->draw(surface);
              break;
         }
         default:break;
       
      } //case
      obj = obj->next;
   }
   setChanged();
}

/*********************************************************************
 *                         drawInactiveBar                           *
 *********************************************************************/
void window::drawInactiveBar()
{
   dntFont fnt;
   int dx = x2-x1;

   /* Redraw All window, removing the mouse from it */
   draw(-1,-1, false);

   /* Redraw the Inactive Bar */
   color_Set(Colors.colorWindow.R, Colors.colorWindow.G,
             Colors.colorWindow.B, Colors.colorWindow.A);
   rectangle_Fill(surface,36,3,dx-3,12);
   color_Set(Colors.colorBar.R+100, Colors.colorBar.G,
             Colors.colorBar.B, Colors.colorBar.A);
   fnt.defineFont(DNT_FONT_ARIAL,10);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
   fnt.write(surface,39,1,text);
   setChanged();
}

/*********************************************************************
 *                           drawActiveBar                           *
 *********************************************************************/
void window::drawActiveBar()
{
   dntFont fnt;
   int dx = x2-x1;
   color_Set(Colors.colorBar.R, Colors.colorBar.G,
             Colors.colorBar.B, Colors.colorBar.A);
   rectangle_Fill(surface,36,3,dx-3,12);
   color_Set(Colors.colorText.R, Colors.colorText.G,
             Colors.colorText.B, Colors.colorText.A);
   fnt.defineFont(DNT_FONT_ARIAL,10);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
   fnt.write(surface,39,1,text);
   setChanged();
}

/*********************************************************************
 *                               flush                               *
 *********************************************************************/
void window::flush()
{
   setTextureRGBA(surface, texture);
}

/*********************************************************************
 *                              render                               *
 *********************************************************************/
void window::render(float depth)
{
   /* Update the Texture if needed */
   if(changed())
   {
      flush();
   }

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture);

   glColor4f(1.0, 1.0, 1.0, /*alpha*/ 1.0);

   glPushMatrix();
      glBegin(GL_QUADS);
         glTexCoord2f(0.0, 0.0);
         glVertex3f(x1, SCREEN_Y-y1, depth);
         glTexCoord2f(0.0, propY);
         glVertex3f(x1, SCREEN_Y-y2, depth);
         glTexCoord2f(propX, propY);
         glVertex3f(x2, SCREEN_Y-y2, depth);
         glTexCoord2f(propX, 0.0);
         glVertex3f(x2, SCREEN_Y-y1, depth);
      glEnd();
   glPopMatrix();

   glColor4f(1.0,1.0,1.0,1.0);
   glDisable(GL_TEXTURE_2D);
}

/*********************************************************************
 *                             activate                              *
 *********************************************************************/
void window::activate()
{
  windowList *ljan = (windowList*) intList;
  if (ljan->getActiveWindow() != NULL)
  {
     if(ljan->getActiveWindow()->isModal())
     {
        /* The current active window is a modal one,
         * so it continue as the active window! */
        return;
     }

     /* Draw the Inactive TitleBar to the current active window */
     ljan->getActiveWindow()->drawInactiveBar();
  }

  /* Set this window as the new active one */
  ljan->setActiveWindow(this);

  /* And obviously, draw its active bar */
  drawActiveBar();
}

/*********************************************************************
 *                               open                                *
 *********************************************************************/
void window::open()
{
   activate();
   draw(0,0);
   setChanged();
}

/*********************************************************************
 *                          setAttributes                            *
 *********************************************************************/
void window::setAttributes(bool close, bool move, bool scale, bool maximize)
{
   menu* men = (menu*)menuButton->men;
   canClose = close;
   men->setItemAvaible(WINDOW_MENU_CLOSE, canClose);
   canMove = move;
   canScale = scale;
   canMaximize = maximize;
}

/*********************************************************************
 *                              isModal                              *
 *********************************************************************/
bool window::isModal()
{
   return(modal);
}

/*********************************************************************
 *                              setModal                             *
 *********************************************************************/
void window::setModal()
{
   modal = true;
}

/*********************************************************************
 *                              changed                              *
 *********************************************************************/
bool window::changed()
{
   bool result = false;

   /* Verify the Window */
   result |= hadChanged;

   /* Verify some Object Change. Must verify all to avoid
    * not needed redraws at next frame.  */
   guiObject *obj=objects->getFirst()->next;
   int aux;
   for(aux=0; (aux < objects->getTotal()) ;aux++)
   {
      result |= obj->changed();
      obj = obj->next;
   }

   /* Reset the Flag */
   hadChanged = false;
   return(result);
}

/*********************************************************************
 *                               doMove                              *
 *********************************************************************/
int window::doMove(SDL_Surface* backGround, int xinic, int yinic, int Mbotao)
{
   int dx = x2 - x1;        /* Width */
   int dy = y2 - y1;        /* Heigh */
   
   int x,y;         /* Mouse Coordinates */
   
      if (Mbotao & SDL_BUTTON(1))
      {
         x = xinic - difx; 
         y = yinic - dify;
         if(x + dx > SCREEN_X-1)
         { 
             x -= (x + dx) - (SCREEN_X-1);
         }
         if(y + dy > SCREEN_Y-1) 
         {
             y -= (y+dy) - (SCREEN_Y-1);
         }
         if(x < 0)
            x = 0;
         if(y <0)
            y = 0;
         x1 = x;
         x2 = x + dx;
         y1 = y;
         y2 = y+ dy;
      }
      else 
      {
         //No more pressed the mouse button. Finished
         return(0);
      }
      //Button pressed, continue to move
      return(1);
}


