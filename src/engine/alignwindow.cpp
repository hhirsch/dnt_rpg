/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "alignwindow.h"

/************************************************************************
 *                           Constructor                                *
 ************************************************************************/
alignWindow::alignWindow(aligns* alg, interface* inter, align** actual)
{
   dntFont fnt;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);
   
   externalAligns = alg;
   choosedAlign = actual;
   if(*choosedAlign != NULL)
   {
      actualAlign = *choosedAlign;
   }
   else
   {
      actualAlign = externalAligns->getAlignByInteger(0);
   }
   
   /* create intWindow */
   intWindow = inter->insertWindow(centerX-134,centerY-129,
                                   centerX+134,centerY+129,
                                   gettext("Tendency & Alignment"));

   /* Align Image */
   alignImage = intWindow->getObjectsList()->insertPicture(103,185,0,0,NULL);   
   alignImage->set(actualAlign->image);

   /* Align Description */
   textDesc = intWindow->getObjectsList()->insertRolBar(5,38,262,180, 
                                                        "",
                                                     intWindow->getSurface());
   textDesc->addText(actualAlign->description.c_str(),
                     DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                     DNT_FONT_STYLE_NORMAL, 33, 65, 10); 
   textDesc->setFirstLine(0);
      
   /* Name and Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(5,19,19,37,
                                                 fnt.createUnicode(0x25C4),0);
   buttonNext = intWindow->getObjectsList()->insertButton(248,19,262,37,
                                                 fnt.createUnicode(0x25BA),0);
   textName = intWindow->getObjectsList()->insertTextBox(20,19,247,37,1, 
                                                  actualAlign->name.c_str());
   textName->setFont(DNT_FONT_ARIAL,12,DNT_FONT_ALIGN_CENTER,
                     DNT_FONT_STYLE_BOLD);

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(188,229,258,248, 
                                              gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(11,229,81,248,
                                               gettext("Cancel"),1);

   /* Contorns */
   intWindow->getObjectsList()->insertTextBox(5,181,87,224,2,"");
   intWindow->getObjectsList()->insertTextBox(5,225,87,252,2,"");
   intWindow->getObjectsList()->insertTextBox(88,181,181,252,2,"");
   intWindow->getObjectsList()->insertTextBox(182,181,262,224,2,"");
   intWindow->getObjectsList()->insertTextBox(182,225,262,252,2,"");

   /* Open Skill Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false, false, false, false);
   inter->openWindow(intWindow);
}

/************************************************************************
 *                              treat                                   *
 ************************************************************************/
int alignWindow::treat(guiObject* object, int eventInfo, interface* inter)
{
   if(eventInfo == PRESSED_BUTTON)
   {
      if( (object == (guiObject*) buttonNext) || 
          (object == (guiObject*) buttonPrevious))
      {
         if(object == (guiObject*) buttonNext)
         {
            actualAlign = actualAlign->next;
         }
         else
         {
            actualAlign = actualAlign->previous;
         }
         textName->setText(actualAlign->name);
         textDesc->setText("");
         textDesc->addText(actualAlign->description.c_str(),
                     DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                     DNT_FONT_STYLE_NORMAL, 33, 65, 10);
         textDesc->setFirstLine(0);
         alignImage->set(actualAlign->image);
         intWindow->draw(0,0);
      }
      else if(object == (guiObject*) buttonConfirm)
      {
         alignImage->set(NULL);
         *choosedAlign = actualAlign;
         inter->closeWindow(intWindow);
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ALIGNW_CONFIRM);
      }
      else if(object == (guiObject*) buttonCancel) 
      {
         alignImage->set(NULL); //to not delete align images
         inter->closeWindow(intWindow);
         *choosedAlign = NULL;
         intWindow = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ALIGNW_CANCEL);
      }
   }
   return(ALIGNW_OTHER);
}

