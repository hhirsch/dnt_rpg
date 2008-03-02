/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "window.h"
#include "menu.h"

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

   alpha = 1.0;
   /* Create Surface And generate texture */
   glGenTextures(1,&texture);
   surface = SDL_CreateRGBSurface(SDL_HWSURFACE,
                                  /*smallestPowerOfTwo*/(xb-xa+1),
                                  /*smallestPowerOfTwo*/(yb-ya+1),32,
                                  rmask,gmask,bmask,amask);

   objects = new guiList;

   button* tmp;
   tmp = objects->insertButton(3,3,13,12,"-",0);
   tmp->men = new menu(0,0);
   menu* men = (menu*) tmp->men;
   men->insertItem(gettext("Close"),1);
   men->insertItem("-",0);
   men->insertItem(gettext("Maximize"),0);
   men->insertItem(gettext("Move"),0); 
   button* b;
   b = objects->insertButton(14,3,24,12,fnt.createUnicode(0x25CF),0);
   b->defineFont(DNT_FONT_ARIAL, 10);
   b = objects->insertButton(25,3,35,12,fnt.createUnicode(0x25B2),0);
   b->defineFont(DNT_FONT_ARIAL, 8);
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
void window::draw(int mouseX, int mouseY)
{
   int dx = x2 - x1 + 1;
   int dy = y2 - y1 + 1;

   color_Set(Colors.colorWindow.R,Colors.colorWindow.G,
             Colors.colorWindow.B);
   rectangle_Fill(surface, 3,3,dx-3,dy-3);
   color_Set(Colors.colorCont[0].R,Colors.colorCont[0].G,Colors.colorCont[0].B);
   rectangle_Draw(surface,0,0,dx-1,dy-1);
   color_Set(Colors.colorButton.R,Colors.colorButton.G,Colors.colorButton.B);
   rectangle_Draw(surface,1,1,dx-2,dy-2);
   color_Set(Colors.colorCont[2].R,Colors.colorCont[2].G,Colors.colorCont[2].B);
   line_Draw(surface,2,13,dx-4,13);
   color_Set(Colors.colorCont[0].R,Colors.colorCont[0].G,Colors.colorCont[0].B);
   rectangle_2Colors(surface,2,2,dx-3,dy-3,Colors.colorCont[2].R,
                     Colors.colorCont[2].G,Colors.colorCont[2].B);


   /* Draw the bar */
   windowList* lst = (windowList*)intList;
   if(this == lst->getActiveWindow())
   {
      drawActiveBar();
   }
   else
   {
      drawInactiveBar();
   }

   /* Objects Draw */
   guiObject *obj=objects->getFirst()->next;
   int aux;
   for(aux=0;aux<objects->getTotal();aux++)
   {
      switch(obj->type)
      {
         case GUI_BUTTON:{
              button *b = (button*) obj;   
              b->draw(0,surface);
              break;
         }
         case GUI_TEXT_BAR:{
              textBar *bart = (textBar*) obj; 
              bart->draw(surface);
              break;
         }
         case GUI_SEL_BOX:{
              cxSel *cx = (cxSel*) obj;
              cx->draw(surface);
              break;
         }
         case GUI_SEL_TEXT:{
              selText *st = (selText*) obj;
              st->draw(surface);
              break;
         }
         case GUI_PICTURE:{
              picture* fig = (picture*) obj;
              fig->draw(surface);
              break;
         }
         case GUI_TEXT_BOX:{
              textBox *quad = (textBox*) obj;
              quad->draw(surface);
              break;
         }
         case GUI_TAB_BUTTON:{
              tabButton *bt = (tabButton*) obj; 
              bt->draw(mouseX, mouseY, x1, y1, surface);
              break;
         }
         default:break;
       
      } //case
      obj = obj->next;
   }
   setTextureRGBA(surface, texture);
}

/*********************************************************************
 *                         drawInactiveBar                           *
 *********************************************************************/
void window::drawInactiveBar()
{
   dntFont fnt;
   int dx = x2-x1;
   color_Set(Colors.colorWindow.R,Colors.colorWindow.G,Colors.colorWindow.B);
   rectangle_Fill(surface,36,3,dx-3,12);
   color_Set(0,0,0);
   fnt.defineFont(DNT_FONT_ARIAL,10);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.write(surface,39,1,text);
}

/*********************************************************************
 *                          drawInactiveBar                          *
 *********************************************************************/
void window::drawActiveBar()
{
   dntFont fnt;
   int dx = x2-x1;
   color_Set(Colors.colorBar.R,Colors.colorBar.G,Colors.colorBar.B);
   rectangle_Fill(surface,36,3,dx-3,12);
   color_Set(Colors.colorText.R,Colors.colorText.G,Colors.colorText.B);
   fnt.defineFont(DNT_FONT_ARIAL,10);
   fnt.defineFontAlign(DNT_FONT_ALIGN_LEFT);
   fnt.write(surface,39,1,text);
}

/*********************************************************************
 *                             activate                              *
 *********************************************************************/
void window::activate()
{
  windowList *ljan = (windowList*) intList;
  if (ljan->getActiveWindow() != NULL)
  {
     ljan->getActiveWindow()->drawInactiveBar();
  }
  ljan->setActiveWindow(this);
  drawActiveBar();
}

/*********************************************************************
 *                               open                                *
 *********************************************************************/
void window::open()
{
   activate();
   draw(0,0);
}

/*********************************************************************
 *                          setAttributes                            *
 *********************************************************************/
void window::setAttributes(bool close, bool move, bool scale, bool maximize)
{
   canClose = close;
   canMove = move;
   canScale = scale;
   canMaximize = maximize;
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


