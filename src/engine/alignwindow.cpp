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
   
   /* create window */
   window = inter->insertWindow(270,186,537,441,language.ALIGNW_TITLE.c_str(),
                                1,1);
   /* Align Image */
   alignImage = window->objects->insertPicture(111,185,0,0,NULL);   
   alignImage->set(actualAlign->image);

   /* Align Description */
   textDesc = window->objects->insertRolBar(5,38,262,180, 
                                            actualAlign->description.c_str(),
                                            window->cara);
      
   /* Name and Selectors */
   buttonPrevious = window->objects->insertButton(5,19,19,37,"<",0);
   buttonNext = window->objects->insertButton(248,19,262,37,">",0);
   textName = window->objects->insertTextBox(20,19,247,37,1, 
                                                  actualAlign->name.c_str());
   textName->setFont(FMINI,1,ESQUERDA);

   /* Confirm Button */
   buttonConfirm = window->objects->insertButton(193,225,263,244, 
                                                 language.SKILL_CONFIRM.c_str(),1);
   
   /* Cancel Button */
   buttonCancel = window->objects->insertButton(8,225,78,244,
                                                language.SKILL_CANCEL.c_str(),1);

   /* Open Skill Window */
   window->ptrExterno = &window;
   window->fechavel = false;
   inter->openWindow(window);
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
         window->Desenhar(0,0);
      }
      else if(object == (guiObject*) buttonConfirm)
      {
         alignImage->set(NULL);
         *choosedAlign = actualAlign;
         inter->closeWindow(window);
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ALIGNW_CONFIRM);
      }
      else if(object == (guiObject*) buttonCancel) 
      {
         alignImage->set(NULL); //to not delete align images
         inter->closeWindow(window);
         *choosedAlign = NULL;
         window = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ALIGNW_CANCEL);
      }
   }
   return(ALIGNW_OTHER);
}

