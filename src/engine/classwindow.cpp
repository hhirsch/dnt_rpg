#include "classwindow.h"

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
classWindow::classWindow(classes* cls, skills* sk, interface* inter, 
                         classe** retClass)
{
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);
   
   externalSkills = sk;
   
   externalClasses = cls;
   choosedClass = retClass;

   if(*choosedClass != NULL)
   {
      actualClass = *choosedClass;
   }
   else
   {
      actualClass = externalClasses->getClassByInteger(0);
   }
   
   /* create window */
   window = inter->insertWindow(90,100,710,499,
                                language.CLASSW_TITLE.c_str(),1,1);
   /* Class Image */
   classImage = window->objects->insertPicture(7,20,0,0,NULL);   
   classImage->set(actualClass->image);

   /* Class Description */
   textDescTitle = window->objects->InserirQuadroTexto(71,20,342,35,1,
                                            language.CLASSW_DESCRIPTION.c_str());
   textDescTitle->fonte = FHELVETICA;
   textDescTitle->tamFonte = 1;
   
   textDesc = window->objects->insertRolBar(71,36,342,345,
             (actualClass->citation + "||" + actualClass->description).c_str(),
             window->cara);

   /* Race Characteristics */
   textCharacTitle = window->objects->InserirQuadroTexto(343,20,613,35,1,
                                        language.CLASSW_CHARACTERISTICS.c_str());
   textCharacTitle->fonte = FHELVETICA;
   textCharacTitle->tamFonte = 1;

   textCharac = window->objects->insertRolBar(343,36,613,345,
                                              getCharacteristics().c_str(),
                                              window->cara);

   /* Name and Selectors */
   buttonPrevious = window->objects->insertButton(71,346,86,364,"<",0);
   buttonNext = window->objects->insertButton(598,346,613,364,">",0);
   textName = window->objects->InserirQuadroTexto(87,346,597,364,1, 
                                                  actualClass->name.c_str());
   textName->fonte = FMINI;
   textName->tamFonte = 1;

   /* Confirm Button */
   buttonConfirm = window->objects->insertButton(543,370,613,389,
                                              language.SKILL_CONFIRM.c_str(),1);
   
   /* Cancel Button */
   buttonCancel = window->objects->insertButton(8,370,78,389,
                                               language.SKILL_CANCEL.c_str(),1);

   /* Open Skill Window */
   window->ptrExterno = &window;
   window->fechavel = false;
   inter->openWindow(window);
}

/********************************************************************
 *                       getCharacteristics                         *
 ********************************************************************/
string classWindow::getCharacteristics()
{
   int i;
   char tmp[50];
   char c;
   skill* skTmp;
   sprintf(tmp,": d%d||",actualClass->lifeDiceID);
   string text = language.CLASSW_LIFE_DICE + tmp;

   text += language.CLASSW_SKILL_POINTS + "|";

   if(actualClass->firstLevelSP.signal == SIGNAL_DEC)
   {
      c = '-';
   }
   else
   {
      c = '+';
   }
   skTmp = externalSkills->getSkillByString(actualClass->firstLevelSP.attID);
   if(skTmp)
   {
      sprintf(tmp,": ( %d %c %s)x%d|",actualClass->firstLevelSP.sum, 
              c, skTmp->nome.c_str(), 
              actualClass->firstLevelSP.mult);
   }
   else
   {
      sprintf(tmp,": ( %d %c %s)x%d|",actualClass->firstLevelSP.sum, 
              c, actualClass->firstLevelSP.attID.c_str(), 
              actualClass->firstLevelSP.mult);
   }
   text += language.CLASSW_FIRST_LEVEL + tmp;
   if(actualClass->otherLevelsSP.signal == SIGNAL_DEC)
   {
      c = '-';
   }
   else
   {
      c = '+';
   }
   skTmp = externalSkills->getSkillByString(actualClass->otherLevelsSP.attID);
   if(skTmp)
   {
      sprintf(tmp,": ( %d %c %s)x%d||",
              actualClass->otherLevelsSP.sum,
              c, skTmp->nome.c_str(), 
              actualClass->otherLevelsSP.mult);
   }
   else
   {
      sprintf(tmp,": ( %d %c %s)x%d||",
              actualClass->otherLevelsSP.sum,
              c, actualClass->otherLevelsSP.attID.c_str(), 
              actualClass->otherLevelsSP.mult);
   }
   text += language.CLASSW_OTHER_LEVELS + tmp;
   text += language.CLASSW_MODIFIERS + "||";
   for(i=0; i<actualClass->totalModifiers; i++)
   {
      text += actualClass->classModifiers[i].description + "||";
   }

   if(actualClass->totalModifiers == 0)
   {
      text += language.RACEW_NO_MODIFIERS + "||";
   }

   //TODO get Feat Name
   text += language.CLASSW_FEATS + "||";
   for(i=0; i<actualClass->totalFeats; i++)
   {
      text += actualClass->classFeats[i] + "|";
   }

   if(actualClass->totalFeats == 0)
   {
      text += language.RACEW_NO_FEATS + "|";
   }

   //Print Skills
   text += "|";
   text += language.CLASSW_SKILLS + "||";
   for(i=0; i<actualClass->totalSkills; i++)
   {
      skTmp = externalSkills->getSkillByString(actualClass->classSkills[i]);
      if(skTmp != NULL)
      {
         text += skTmp->nome + "|";
      }
      else
      {
         text += actualClass->classSkills[i] + "|";
      }
   }

   if(actualClass->totalSkills == 0)
   {
      text += language.RACEW_NO_SKILLS;
   }

   return(text);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int classWindow::treat(guiObject* object, int eventInfo, interface* inter)
{
   if(eventInfo == BOTAOPRESSIONADO)
   {
      if( (object == (guiObject*) buttonNext) || 
          (object == (guiObject*) buttonPrevious))
      {
         if(object == (guiObject*) buttonNext)
         {
            actualClass = actualClass->next;
         }
         else
         {
            actualClass = actualClass->previous;
         }
         textName->texto = actualClass->name;
         textDesc->setText(actualClass->citation + "||" +
                           actualClass->description);
         textCharac->setText(getCharacteristics());
         classImage->set(actualClass->image);
         window->Desenhar(0,0);
      }
      else if(object == (guiObject*) buttonConfirm)
      {
         classImage->set(NULL);
         *choosedClass = actualClass;
         inter->closeWindow(window);
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(CLASSW_CONFIRM);
      }
      else if(object == (guiObject*) buttonCancel) 
      {
         classImage->set(NULL); //to not delete classes images
         inter->closeWindow(window);
         *choosedClass = NULL;
         window = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(CLASSW_CANCEL);
      }
   }
   return(CLASSW_OTHER);
}

