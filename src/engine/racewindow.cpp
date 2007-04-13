#include "racewindow.h"

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
raceWindow::raceWindow(races* rc, skills* sk, interface* inter,
                       race** retRace)
{
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   externalSkills = sk;

   choosedRace = retRace;
   
   externalRaces = rc;
   if(*choosedRace != NULL)
   {
      actualRace = *choosedRace;
   }
   else
   {
      actualRace = externalRaces->getRaceByInteger(0);
   }
   
   /* create window */
   window = inter->insertWindow(90,100,710,499,
                                language.RACEW_TITLE.c_str(),1,1);
   /* Race Image */
   raceImage = window->objects->InserirFigura(7,20,0,0,NULL);   
   raceImage->fig = actualRace->image;

   /* Race Description */
   textDescTitle = window->objects->InserirQuadroTexto(71,20,342,35,1,
                                            language.RACEW_DESCRIPTION.c_str());
   textDescTitle->fonte = FHELVETICA;
   textDescTitle->tamFonte = 1;
   
   textDesc = window->objects->insertRolBar(71,36,342,345,
              (actualRace->citation + "||" + actualRace->description).c_str(),
              window->cara);

   /* Race Characteristics */
   textCharacTitle = window->objects->InserirQuadroTexto(343,20,613,35,1,
                                       language.RACEW_CHARACTERISTICS.c_str());
   textCharacTitle->fonte = FHELVETICA;
   textCharacTitle->tamFonte = 1;

   textCharac = window->objects->insertRolBar(343,36,613,345,
                                              getCharacteristics().c_str(),
                                              window->cara);

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
                                                  actualRace->name.c_str());
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
string raceWindow::getCharacteristics()
{
   int i;
   //char mod[5];
   string text = language.RACEW_MODIFIERS + "||";
   for(i=0; i<actualRace->totalModifiers; i++)
   {
      text += actualRace->raceModifiers[i].description + "||";
   }

   if(actualRace->totalModifiers == 0)
   {
      text += language.RACEW_NO_MODIFIERS + "||";
   }

   //TODO get race Name
   text += language.RACEW_FEATS + "||";
   for(i=0; i<actualRace->totalFeats; i++)
   {
      text += actualRace->raceFeats[i] + "|";
   }

   if(actualRace->totalFeats == 0)
   {
      text += language.RACEW_NO_FEATS + "|";
   }

   /* Race Skills */
   skill* skTmp;
   text += "|" + language.RACEW_SKILLS + "||";
   for(i=0; i<actualRace->totalSkills; i++)
   {
      skTmp = externalSkills->getSkillByString(actualRace->raceSkills[i]);
      if(skTmp)
      {
         text += skTmp->nome + "|";
      }
      else
      {
         text += actualRace->raceSkills[i] + "|";
      }
   }

   if(actualRace->totalSkills == 0)
   {
      text += language.RACEW_NO_SKILLS;
   }

   return(text);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int raceWindow::treat(Tobjeto* object, int eventInfo, 
                      interface* inter)
{
   if(eventInfo == BOTAOPRESSIONADO)
   {
      if( (object == (Tobjeto*) buttonNext) || 
          (object == (Tobjeto*) buttonPrevious))
      {
         if(object == (Tobjeto*) buttonNext)
         {
            actualRace = actualRace->next;
         }
         else
         {
            actualRace = actualRace->previous;
         }
         textName->texto = actualRace->name;
         textDesc->setText(actualRace->citation + "||" +
                           actualRace->description);
         textCharac->setText(getCharacteristics());
         raceImage->fig = actualRace->image;
         window->Desenhar(0,0);
      }
      else if(object == (Tobjeto*) buttonConfirm)
      {
         raceImage->fig = NULL;
         *choosedRace = actualRace;
         inter->closeWindow(window);
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(RACEW_CONFIRM);
      }
      else if(object == (Tobjeto*) buttonCancel) 
      {
         raceImage->fig = NULL; //to not delete race images
         inter->closeWindow(window);
         *choosedRace = NULL;
         window = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(RACEW_CANCEL);
      }
   }
   return(RACEW_OTHER);
}

