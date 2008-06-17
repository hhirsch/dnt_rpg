#include "briefing.h"

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
 *                             openWindow                              *
 ***********************************************************************/
void briefing::openWindow(guiInterface* gui)
{
   if(briefWindow == NULL)
   {
      briefWindow = gui->insertWindow(0,SCREEN_Y-258, 326, SCREEN_Y-130,
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
      briefTxt->addText(text);
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
      briefTxt->addText(text, font, size, style, align, R, G, B);
   }
}

/***********************************************************************
 *                           Static Members                            *
 ***********************************************************************/
rolBar* briefing::briefTxt = NULL;
window* briefing::briefWindow = NULL;

