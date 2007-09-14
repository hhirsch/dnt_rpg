/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "alignwindow.h"

/************************************************************************
 *                           Constructor                                *
 ************************************************************************/
alignWindow::alignWindow(aligns* alg, interface* inter, align** actual)
{
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
   intWindow = inter->insertWindow(270,186,537,441,
                                  gettext("Tendency & Alignment"));
   /* Align Image */
   alignImage = intWindow->getObjectsList()->insertPicture(111,185,0,0,NULL);   
   alignImage->set(actualAlign->image);

   /* Align Description */
   textDesc = intWindow->getObjectsList()->insertRolBar(5,38,262,180, 
                                            actualAlign->description.c_str(),
                                            intWindow->getSurface());
      
   /* Name and Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(5,19,19,37,"<",0);
   buttonNext = intWindow->getObjectsList()->insertButton(248,19,262,37,">",0);
   textName = intWindow->getObjectsList()->insertTextBox(20,19,247,37,1, 
                                                  actualAlign->name.c_str());
   textName->setFont(FMINI,1,ALIGN_LEFT);

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(193,225,263,244, 
                                              gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(8,225,78,244,
                                               gettext("Cancel"),1);

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
         textDesc->setText(actualAlign->description);
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

