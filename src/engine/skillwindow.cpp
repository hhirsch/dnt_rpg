/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "skillwindow.h"
#include "../classes/defs.h"

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
skillWindow::skillWindow(skills* sk, skills* savSkill, interface* inter)
{
   char tmp[5];
   string saux;
   int aux;

   curSkill = ATT_SKILL_FIRST; //Set to first skill after attributes
   externalSkill = sk;
   saveSkill = savSkill;
   avaiblePoints = saveSkill->getAvaiblePoints();


   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   /* Define previous points, for undo */
   for(aux = curSkill; aux < ATT_SKILL_LAST; aux++)
   { 
     saveSkill->m_skills[aux].prevPoints = saveSkill->m_skills[aux].points;
   }
   
   /* Create Skill Window */
   intWindow = inter->insertWindow(276,186,539,441, gettext("Skills"));

   /* Free Points */
   sprintf(tmp,"%d",avaiblePoints);
   saux = tmp;
   intWindow->getObjectsList()->insertTextBox(8,20,125,33,0,
                                         gettext("Total Free Points:"));
   txtAvaiblePoints = intWindow->getObjectsList()->insertTextBox(127,20,162,
                                                                 33,0,
                                                                 saux.c_str());

   /* Skill Description */
   desc = intWindow->getObjectsList()->insertRolBar(8,38,256,170,
                         externalSkill->m_skills[curSkill].description.c_str(),
                         intWindow->getSurface());

   /* Contorns */
   intWindow->getObjectsList()->insertTextBox(8,171,256,224,2,"");
   intWindow->getObjectsList()->insertTextBox(8,18,256,37,2,"");
   intWindow->getObjectsList()->insertTextBox(8,225,256,250,2,"");
 
   /* Skill Name & Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(52,175,66,193,
                                                              "<",0);
   buttonNext = intWindow->getObjectsList()->insertButton(234,175,248,193,
                                                          ">",0);
   skillName = intWindow->getObjectsList()->insertTextBox(67,175,233,193,1,
                                externalSkill->m_skills[curSkill].name.c_str());
   skillName->setFont(FMINI,1,ALIGN_LEFT);

   /* Skill Image */
   skFig = intWindow->getObjectsList()->insertPicture(13,175,0,0,NULL);
   skFig->set(externalSkill->m_skills[curSkill].image);
 
   /* Skill Points */
   intWindow->getObjectsList()->insertTextBox(52,200,101,214,0,
                                       gettext("Points:"));
   sprintf(tmp,"%d",saveSkill->m_skills[curSkill].points);
   saux = tmp;
   txtPoints = intWindow->getObjectsList()->insertTextBox(111,198,133,216,1,
                                                   saux.c_str());
   txtPoints->setFont(FMINI,1,ALIGN_LEFT);
   buttonSum = intWindow->getObjectsList()->insertButton(134,198,144,216,">",0);
   buttonDec = intWindow->getObjectsList()->insertButton(100,198,110,216,"<",0);

   /* Skill Costs */
   intWindow->getObjectsList()->insertTextBox(160,200,215,214,0,
                                       gettext("Cost"));
   sprintf(tmp,"%d",saveSkill->m_skills[curSkill].mod);
   saux = tmp;
   txtCosts = intWindow->getObjectsList()->insertTextBox(216,200,256,214,0,
                                                  saux.c_str());

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(178,228,248,247,
                                              gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(13,228,83,247,
                                              gettext("Cancel"),1);
   
   /* Open Skill Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   inter->openWindow(intWindow);
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
int skillWindow::treat(guiObject* object, int eventInfo, interface* inter)
{
   if(eventInfo == PRESSED_BUTTON)
   {
      if(object == (guiObject*) buttonSum)
      {
         if( avaiblePoints - saveSkill->m_skills[curSkill].mod >=0 )
         {
             saveSkill->m_skills[curSkill].points++;
             avaiblePoints -= saveSkill->m_skills[curSkill].mod;
             
             char tmp[5];
             sprintf(tmp,"%d",avaiblePoints);
             string saux = tmp;
             txtAvaiblePoints->setText(saux);
        
             sprintf(tmp,"%d",saveSkill->m_skills[curSkill].points);
             saux = tmp;
             txtPoints->setText(saux);

         }
      }
      else if(object == (guiObject*) buttonDec)
      {
         if(saveSkill->m_skills[curSkill].points - 1 >= 
            saveSkill->m_skills[curSkill].prevPoints)
         {
            saveSkill->m_skills[curSkill].points--;
            avaiblePoints += saveSkill->m_skills[curSkill].mod;

            char tmp[5];
            sprintf(tmp,"%d",avaiblePoints);
            string saux = tmp;
            txtAvaiblePoints->setText(saux);

            sprintf(tmp,"%d",saveSkill->m_skills[curSkill].points);
            saux = tmp;
            txtPoints->setText(saux);
         }
      }
      else if(object == (guiObject*) buttonNext)
      {
         if(curSkill < ATT_SKILL_LAST)
         {
            curSkill++;
         }
         else
         {
            curSkill = ATT_SKILL_FIRST;
         } 
         skillName->setText(externalSkill->m_skills[curSkill].name.c_str());
         desc->setText(externalSkill->m_skills[curSkill].description.c_str());
         skFig->set(externalSkill->m_skills[curSkill].image);
          
         char tmp[5];
         sprintf(tmp,"%d",saveSkill->m_skills[curSkill].points);
         string saux = tmp;
         txtPoints->setText(saux);
         sprintf(tmp,"%d",saveSkill->m_skills[curSkill].mod);
         saux = tmp;
         txtCosts->setText(saux);

      }
      else if(object == (guiObject*) buttonPrevious)
      {
         if(curSkill > ATT_SKILL_FIRST)
         {
            curSkill--;
         }
         else
         {
            curSkill = ATT_SKILL_LAST;
         }
         skillName->setText(externalSkill->m_skills[curSkill].name.c_str());
         desc->setText(externalSkill->m_skills[curSkill].description.c_str());
         skFig->set(externalSkill->m_skills[curSkill].image);

         char tmp[5];
         sprintf(tmp,"%d",saveSkill->m_skills[curSkill].points);
         string saux = tmp;
         txtPoints->setText(saux);
         sprintf(tmp,"%d",saveSkill->m_skills[curSkill].mod);
         saux = tmp;
         txtCosts->setText(saux);

      }
      else if(object == (guiObject*) buttonConfirm)
      {
         skFig->set(NULL); //to not delete skill images
         inter->closeWindow(intWindow);
         intWindow = NULL;
         saveSkill->setAvaiblePoints(avaiblePoints);
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(SKILLW_CONFIRM);
      }
      else if(object == (guiObject*) buttonCancel) 
      {
         /* Undo */
         int aux;
         for(aux = curSkill; aux < VAR_AUX1; aux++)
         { 
            saveSkill->m_skills[aux].points = 
                                            saveSkill->m_skills[aux].prevPoints;
          }
          skFig->set(NULL); //to not delete skill images
          inter->closeWindow(intWindow);
          intWindow = NULL;
          glEnable(GL_LIGHTING);
          SDL_ShowCursor(SDL_DISABLE);
         return(SKILLW_CANCEL);
      }
   }

   if( saveSkill->m_skills[curSkill].points > 
       saveSkill->m_skills[curSkill].prevPoints )
   {
      /*txtPoints->Cores.corTexto.R = 13;
      txtPoints->Cores.corTexto.G = 250;
      txtPoints->Cores.corTexto.B = 85; */
   }
   else
   {
      /*
      txtPoints->Cores.corTexto.R = 255;
      txtPoints->Cores.corTexto.G = 255;
      txtPoints->Cores.corTexto.B = 255;
      */
   }
   
   intWindow->draw(0,0);
   return(SKILLW_OTHER);
}

