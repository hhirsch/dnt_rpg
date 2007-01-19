#include "classwindow.h"

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
classWindow::classWindow(classes* cls, skills* sk, interface* inter)
{
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);
   
   externalSkills = sk;
   
   externalClasses = cls;
   actualClass = externalClasses->getClassByInteger(0);
   
   /* create window */
   window = inter->insertWindow(90,100,710,499, "Class"
                                /*language.CLASSW_TITLE.c_str()*/,1,1);
   /* Class Image */
   classImage = window->objects->InserirFigura(7,20,0,0,NULL);   
   classImage->fig = actualClass->image;

   /* Class Description */
   textDescTitle = window->objects->InserirQuadroTexto(71,20,342,35,1,
                                          "Class Description");
   textDescTitle->fonte = FHELVETICA;
   textDescTitle->tamFonte = 1;
   
   textDesc = window->objects->InsertRolBar(71,36,342,345,
             (actualClass->citation + "||" + actualClass->description).c_str());

   /* Race Characteristics */
   textCharacTitle = window->objects->InserirQuadroTexto(343,20,613,35,1,
                                       "Class Characteristics");
   textCharacTitle->fonte = FHELVETICA;
   textCharacTitle->tamFonte = 1;

   textCharac = window->objects->InsertRolBar(343,36,613,345,
                                              getCharacteristics().c_str());

   /* Name and Selectors */
   buttonPrevious = window->objects->InserirBotao(71,346,86,364,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   buttonNext = window->objects->InserirBotao(598,346,613,364,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,NULL);
   textName = window->objects->InserirQuadroTexto(87,346,597,364,1, 
                                                  actualClass->name.c_str());
   textName->fonte = FMINI;
   textName->tamFonte = 1;

   /* Confirm Button */
   buttonConfirm = window->objects->InserirBotao(543,370,613,389, 
                                window->Cores.corBot.R,
                                window->Cores.corBot.G,window->Cores.corBot.B,
                                language.SKILL_CONFIRM.c_str(),1,NULL);
   
   /* Cancel Button */
   buttonCancel = window->objects->InserirBotao(8,370,78,389, 
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CANCEL.c_str(),1,NULL);

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
   sprintf(tmp,"Life Dice: d%d||",actualClass->lifeDiceID);
   string text = tmp;

   text += "Skill Points|";

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
      sprintf(tmp,"1st Level: ( %d %c %s)x%d|",actualClass->firstLevelSP.sum, 
              c, skTmp->nome.c_str(), 
              actualClass->firstLevelSP.mult);
   }
   else
   {
      sprintf(tmp,"1st Level: ( %d %c %s)x%d|",actualClass->firstLevelSP.sum, 
              c, actualClass->firstLevelSP.attID.c_str(), 
              actualClass->firstLevelSP.mult);
   }
   text += tmp;
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
      sprintf(tmp,"Other Levels: ( %d %c %s)x%d||",
              actualClass->otherLevelsSP.sum,
              c, skTmp->nome.c_str(), 
              actualClass->otherLevelsSP.mult);
   }
   else
   {
      sprintf(tmp,"Other Levels: ( %d %c %s)x%d||",
              actualClass->otherLevelsSP.sum,
              c, actualClass->otherLevelsSP.attID.c_str(), 
              actualClass->otherLevelsSP.mult);
   }
   text += tmp;
   text += "Class Modifiers||";
   for(i=0; i<actualClass->totalModifiers; i++)
   {
      text += actualClass->classModifiers[i].description + "||";
   }

   if(actualClass->totalModifiers == 0)
   {
      text += language.RACEW_NO_MODIFIERS + "||";
   }

   //TODO get Feat Name
   text += "Class Feats||";
   for(i=0; i<actualClass->totalFeats; i++)
   {
      text += actualClass->classFeats[i] + "|";
   }

   if(actualClass->totalFeats == 0)
   {
      text += language.RACEW_NO_FEATS + "|";
   }

   //Print Skills
   text += "|Class Skills||";
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
int classWindow::treat(Tobjeto* object, int eventInfo, 
                       interface* inter, classe** actual)
{
   if(eventInfo == BOTAOPRESSIONADO)
   {
      if( (object == (Tobjeto*) buttonNext) || 
          (object == (Tobjeto*) buttonPrevious))
      {
         if(object == (Tobjeto*) buttonNext)
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
         classImage->fig = actualClass->image;
         window->Desenhar(0,0);
      }
      else if(object == (Tobjeto*) buttonConfirm)
      {
         classImage->fig = NULL;
         *actual = actualClass;
         inter->closeWindow(window);
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(CLASSW_CONFIRM);
      }
      else if(object == (Tobjeto*) buttonCancel) 
      {
         classImage->fig = NULL; //to not delete classes images
         inter->closeWindow(window);
         *actual = NULL;
         window = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(CLASSW_CANCEL);
      }
   }
   return(CLASSW_OTHER);
}

