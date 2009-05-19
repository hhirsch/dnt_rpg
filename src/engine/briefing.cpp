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
 *                           Static Members                            *
 ***********************************************************************/
rolBar* briefing::briefTxt = NULL;
window* briefing::briefWindow = NULL;
Uint32 briefing::timeLastAdd = 0;

