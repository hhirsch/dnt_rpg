#include "options.h"

#include "../lang/lang.h"
#include "../sound/sound.h"

#define NM_PORTUGUES "Português"
#define NM_INGLES    "English"
#define NM_FRANCES   "Français"
#define NM_ESPANHOL  "Español"

options::options(string file)
{
   FILE* arq;
   char buffer[128];
   if(!(arq = fopen(file.c_str(),"r")))
   {
      printf("Error while opening Options: %s\n",file.c_str());
      return;
   }
   fileName = file;
   fscanf(arq,"%s %d %d %d",&buffer[0],&musicVolume,&sndfxVolume,&langNumber);
   if(musicVolume > SDL_MIX_MAXVOLUME)
   {
      musicVolume = SDL_MIX_MAXVOLUME;
   }
   if(sndfxVolume > SDL_MIX_MAXVOLUME)
   {
      sndfxVolume = SDL_MIX_MAXVOLUME;
   }

   fclose(arq);

}

options::~options()
{
}

void options::Save()
{
   FILE* arq;
   if(!(arq = fopen(fileName.c_str(),"w")))
   {
      printf("Error while opening Options: %s\n",fileName.c_str());
      return;
   }
   fprintf(arq,"Sons: %d %d %d",musicVolume,sndfxVolume,langNumber);
   language.ReloadFile(langNumber);
   fclose(arq);
}

string options::languageName()
{
   string saux;
   switch(langNumber)
   {
      case LANG_PORTUGUES:
      {
         saux = NM_PORTUGUES;
         break;
      }
      case LANG_ESPANHOL:
      {
         saux = NM_ESPANHOL;
         break;
      }
      case LANG_FRANCES: 
      {
         saux = NM_FRANCES;
         break;
      }
      case LANG_INGLES:
      {
         saux = NM_INGLES;
         break;
      }
   }
   return(saux);
}

/****************************************************************
 *                    Open Options Screen                       *
 ****************************************************************/
void options::DisplayOptionsScreen(interface* interf)
{
   char tmp[5];
   string saux;
   quadroTexto* qt;

   prevMusicVolume = musicVolume;
   prevSndfxVolume = sndfxVolume;

   window = interf->ljan->InserirJanela(316,186,571,441,
                                       language.OPTIONS_TITLE.c_str(),
                                       1,1,NULL,NULL);

   /* Music Things */
   sprintf(tmp,"%d",musicVolume);
   saux = tmp;
   qt = window->objects->InserirQuadroTexto(8,27,145,44,0,
                                         language.OPTIONS_MUSIC_VOLUME.c_str());
   qt->fonte = FMINI;
   buttonMusDec = window->objects->InserirBotao(146,27,156,44,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   txtMusicVolume = window->objects->InserirQuadroTexto(157,27,197,44,1,
                                 saux.c_str());
   txtMusicVolume->fonte = FMINI;
   buttonMusSum = window->objects->InserirBotao(198,27,208,44,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                   ">",0,NULL);
   window->objects->InserirFigura(210,20,40,112,"../data/texturas/options/music.png");
   
  
   /* Sound Effects Things */
   sprintf(tmp,"%d",sndfxVolume);
   saux = tmp;
   qt = window->objects->InserirQuadroTexto(8,61,145,78,0,
                                         language.OPTIONS_SNDFX_VOLUME.c_str());
   qt->fonte = FMINI;
   buttonSndDec = window->objects->InserirBotao(146,61,156,78,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   txtSndfxVolume = window->objects->InserirQuadroTexto(157,61,197,78,1,
                                 saux.c_str());
   txtSndfxVolume->fonte = FMINI;
   buttonSndSum = window->objects->InserirBotao(198,61,208,78,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,NULL);
   window->objects->InserirFigura(210,54,40,112,"../data/texturas/options/sndfx.png");


   /* Language Things */
   prevLanguage = langNumber;
   saux = languageName();
   qt = window->objects->InserirQuadroTexto(8,95,145,112,0,
                                         language.OPTIONS_LANGUAGE.c_str());
   qt->fonte = FMINI;
   buttonLangDec = window->objects->InserirBotao(121,95,131,112,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   txtLanguage = window->objects->InserirQuadroTexto(132,95,197,112,1,
                                 saux.c_str());
   txtLanguage->fonte = FMINI;
   buttonLangSum = window->objects->InserirBotao(198,95,208,112,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,NULL);
   window->objects->InserirFigura(210,88,40,112,"../data/texturas/options/language.png");


   /* Confirm Button */
   buttonConfirm = window->objects->InserirBotao(181,225,251,244, 
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CONFIRM.c_str(),1,NULL);
   
   /* Cancel Button */
   buttonCancel = window->objects->InserirBotao(8,225,78,244, 
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CANCEL.c_str(),1,NULL);

   /* Open Skill Window */
   window->ptrExterno = &window;
   window->fechavel = false;
   window->Abrir(interf->ljan);
}

int options::Treat(Tobjeto* object, int eventInfo, interface* interf)
{
   if(eventInfo == BOTAOPRESSIONADO)
   {
      if(object == (Tobjeto*) buttonMusSum)
      {
         if(musicVolume < SDL_MIX_MAXVOLUME)
         {
             musicVolume++;
         }
      }
      if(object == (Tobjeto*) buttonMusDec) 
      {
         if(musicVolume > 0)
         {
             musicVolume--;
         }
      }
      if(object == (Tobjeto*) buttonSndSum) 
      {
         if(sndfxVolume < SDL_MIX_MAXVOLUME)
         {
             sndfxVolume++;
         }
      }
      if(object == (Tobjeto*) buttonSndDec) 
      {
         if(sndfxVolume > 0)
         {
             sndfxVolume--;
         }
      }
      if(object == (Tobjeto*) buttonLangSum)
      {
         if(langNumber < LANG_LAST)
         {
            langNumber++;
         }
      }
      if(object == (Tobjeto*) buttonLangDec)
      {
         if(langNumber > LANG_FIRST)
         {
            langNumber--;
         }
      }
      if(object == (Tobjeto*) buttonConfirm) 
      {
         Save();
         window->Fechar(interf->ljan);
         return(OPTIONSW_CONFIRM);
      }
      if(object == (Tobjeto*) buttonCancel)
      {
         musicVolume = prevMusicVolume;
         sndfxVolume = prevSndfxVolume;
         langNumber  = prevLanguage;
         window->Fechar(interf->ljan);
         return(OPTIONSW_CANCEL);
      }
   }
   
   char tmp[5];
   sprintf(tmp,"%d",musicVolume);
   txtMusicVolume->texto = tmp;

   sprintf(tmp,"%d",sndfxVolume);
   txtSndfxVolume->texto = tmp;

   txtLanguage->texto = languageName();

   window->Desenhar(0,0);
   return(OPTIONSW_OTHER);
} 

