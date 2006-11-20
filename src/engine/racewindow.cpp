#include "racewindow.h"


raceWindow::raceWindow(races* rc, interface* inter)
{
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);
   
   externalRaces = rc;
   actualRace = externalRaces->getRaceByInteger(0);
   
   /* create window */
   window = inter->ljan->InserirJanela(100,100,699,499,
                                       language.RACEW_TITLE.c_str(),
                                       1,1,NULL,NULL);
   /* Race Image */
   raceImage = window->objects->InserirFigura(8,20,0,0,NULL);   
   raceImage->fig = actualRace->image;

   /* Race Description */
   textDescTitle = window->objects->InserirQuadroTexto(72,20,330,35,1,
                                             language.RACEW_DESCRIPTION.c_str());
   textDescTitle->fonte = FHELVETICA;
   textDescTitle->tamFonte = 1;
   
   textDesc = window->objects->InserirQuadroTexto(72,36,330,345,1,
                (actualRace->citation + "||" + actualRace->description).c_str());
   textDesc->Cores.corTexto.R = 246;
   textDesc->Cores.corTexto.G = 190;
   textDesc->Cores.corTexto.B = 190;
   /*textDesc->Cores.corBot.R = 155;
   textDesc->Cores.corBot.G = 5;
   textDesc->Cores.corBot.B = 5;*/
   textDesc->fonte = FMINI;
   textDesc->tamFonte = 1;

   /* Race Characteristics */
   textCharacTitle = window->objects->InserirQuadroTexto(332,20,592,35,1,
                                         language.RACEW_CHARACTERISTICS.c_str());
   textCharacTitle->fonte = FHELVETICA;
   textCharacTitle->tamFonte = 1;

   textCharac = window->objects->InserirQuadroTexto(332,36,592,345,1,
                                              getCharacteristics().c_str());
   textCharac->Cores.corTexto.R = 246;
   textCharac->Cores.corTexto.G = 190;
   textCharac->Cores.corTexto.B = 190;
   textCharac->fonte = FMINI;
   textCharac->tamFonte = 1;


   /* Name and Selectors */
   buttonPrevious = window->objects->InserirBotao(72,346,87,364,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   buttonNext = window->objects->InserirBotao(577,346,592,364,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,NULL);
   textName = window->objects->InserirQuadroTexto(88,346,576,364,1, 
                                                  actualRace->name.c_str());
   textName->fonte = FMINI;
   textName->tamFonte = 1;

   /* Confirm Button */
   buttonConfirm = window->objects->InserirBotao(522,370,592,389, 
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
   window->Abrir(inter->ljan);
}

string raceWindow::getCharacteristics()
{
   int i;
   //char mod[5];
   string text = language.RACEW_MODIFIERS + "||";
   for(i=0; i<actualRace->totalModifiers; i++)
   {
      /*sprintf(mod, "%d", actualRace->raceModifiers[i].mod);
      text += actualRace->raceModifiers[i].cause.type + " " + 
              actualRace->raceModifiers[i].cause.id + " " +
              actualRace->raceModifiers[i].target.type + " " + 
              actualRace->raceModifiers[i].target.id + 
              " " + mod + "|";*/
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

   //TODO get Skill Name
   text += "|" + language.RACEW_SKILLS + "||";
   for(i=0; i<actualRace->totalSkills; i++)
   {
      text += actualRace->raceSkills[i] + "|";
   }

   if(actualRace->totalSkills == 0)
   {
      text += language.RACEW_NO_SKILLS;
   }


   return(text);
}

int raceWindow::treat(Tobjeto* object, int eventInfo, 
                       interface* inter, race** actual)
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
         textDesc->texto = actualRace->citation + "||" +actualRace->description;
         textCharac->texto = getCharacteristics();
         raceImage->fig = actualRace->image;
         window->Desenhar(0,0);
      }
      else if(object == (Tobjeto*) buttonConfirm)
      {
         raceImage->fig = NULL;
         *actual = actualRace;
         window->Fechar(inter->ljan);
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(RACEW_CONFIRM);
      }
      else if(object == (Tobjeto*) buttonCancel) 
      {
         raceImage->fig = NULL; //to not delete skill images
         window->Fechar(inter->ljan);
         *actual = NULL;
         window = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(RACEW_CANCEL);
      }
   }
   return(RACEW_OTHER);
}

