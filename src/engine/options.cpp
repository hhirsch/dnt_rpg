#include "options.h"

#include "../lang/lang.h"
#include "../sound/sound.h"

#define ACT_NONE     0
#define ACT_MUS_SUM  1
#define ACT_MUS_SUB  2
#define ACT_SND_SUM  3
#define ACT_SND_SUB  4
#define ACT_CONFIRM  5
#define ACT_LNG_SUM  6
#define ACT_LNG_SUB  7
#define ACT_CANCEL   8

#define NM_PORTUGUES "Português"
#define NM_INGLES    "English"
#define NM_FRANCES   "Français"
#define NM_ESPANHOL  "Español"

int cAction = ACT_NONE;

int botaoMusSum(void *jan,void *ljan,SDL_Surface *screen)
{
   cAction = ACT_MUS_SUM;
   return(1);
}

int botaoMusSub(void *jan,void *ljan,SDL_Surface *screen)
{
   cAction = ACT_MUS_SUB;
   return(1);
}

int botaoSndSum(void *jan,void *ljan,SDL_Surface *screen)
{
   cAction = ACT_SND_SUM;
   return(1);
}

int botaoSndSub(void *jan,void *ljan,SDL_Surface *screen)
{
   cAction = ACT_SND_SUB;
   return(1);
}

int botaoLngSum(void *jan,void *ljan,SDL_Surface *screen)
{
   cAction = ACT_LNG_SUM;
   return(1);
}

int botaoLngSub(void *jan,void *ljan,SDL_Surface *screen)
{
   cAction = ACT_LNG_SUB;
   return(1);
}


int botaoConf(void *jan,void *ljan,SDL_Surface *screen)
{
   cAction = ACT_CONFIRM;
   return(1);
}

int botaoCanc(void *jan,void *ljan,SDL_Surface *screen)
{
   cAction = ACT_CANCEL;
   return(1);
}



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
   qt = window->objetos->InserirQuadroTexto(8,27,145,42,0,
                                         language.OPTIONS_MUSIC_VOLUME.c_str());
   qt->fonte = FMINI;
   window->objetos->InserirBotao(146,27,156,42,window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,&botaoMusSub);
   txtMusicVolume = window->objetos->InserirQuadroTexto(157,27,197,42,1,
                                 saux.c_str());
   txtMusicVolume->fonte = FMINI;
   window->objetos->InserirBotao(198,27,208,42,window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                   ">",0,&botaoMusSum);
   window->objetos->InserirFigura(210,20,40,112,"../data/texturas/options/music.png");
   
  
   /* Sound Effects Things */
   sprintf(tmp,"%d",sndfxVolume);
   saux = tmp;
   qt = window->objetos->InserirQuadroTexto(8,61,145,79,0,
                                         language.OPTIONS_SNDFX_VOLUME.c_str());
   qt->fonte = FMINI;
   window->objetos->InserirBotao(146,61,156,79,window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,&botaoSndSub);
   txtSndfxVolume = window->objetos->InserirQuadroTexto(157,61,197,79,1,
                                 saux.c_str());
   txtSndfxVolume->fonte = FMINI;
   window->objetos->InserirBotao(198,61,208,79,window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,&botaoSndSum);
   window->objetos->InserirFigura(210,54,40,112,"../data/texturas/options/sndfx.png");


   /* Language Things */
   prevLanguage = langNumber;
   saux = languageName();
   qt = window->objetos->InserirQuadroTexto(8,95,145,112,0,
                                         language.OPTIONS_LANGUAGE.c_str());
   qt->fonte = FMINI;
   window->objetos->InserirBotao(131,95,141,112,window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,&botaoLngSub);
   txtLanguage = window->objetos->InserirQuadroTexto(142,95,197,112,1,
                                 saux.c_str());
   txtLanguage->fonte = FMINI;
   window->objetos->InserirBotao(198,95,208,112,window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,&botaoLngSum);
   window->objetos->InserirFigura(210,88,40,112,"../data/texturas/options/language.png");


   /* Confirm Button */
   window->objetos->InserirBotao(181,225,251,244, window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CONFIRM.c_str(),1,&botaoConf);
   
   /* Cancel Button */
   window->objetos->InserirBotao(8,225,78,244, window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CANCEL.c_str(),1,&botaoCanc);

   /* Open Skill Window */
   window->ptrExterno = &window;
   window->fechavel = false;
   window->Abrir(interf->ljan);
}

int options::Treat(interface* interf)
{
   switch(cAction)
   {
      case ACT_MUS_SUM: 
      {
         if(musicVolume < SDL_MIX_MAXVOLUME)
         {
             musicVolume++;
         }
         break;
      }
      case ACT_MUS_SUB: 
      {
         if(musicVolume > 0)
         {
             musicVolume--;
         }
         break;
      }
      case ACT_SND_SUM: 
      {
         if(sndfxVolume < SDL_MIX_MAXVOLUME)
         {
             sndfxVolume++;
         }
         break;
      }
      case ACT_SND_SUB: 
      {
         if(sndfxVolume > 0)
         {
             sndfxVolume--;
         }
         break;
      }
      case ACT_LNG_SUM:
      {
         if(langNumber < LANG_LAST)
         {
            langNumber++;
         }
         break;
      }
      case ACT_LNG_SUB:
      {
         if(langNumber > LANG_FIRST)
         {
            langNumber--;
         }
         break;
      }
      case ACT_CONFIRM: 
      {
         Save();
         window->Fechar(interf->ljan);
         cAction = ACT_NONE;
         return(OPTIONSW_CONFIRM);
         break;
      }
      case ACT_CANCEL:
      {
         musicVolume = prevMusicVolume;
         sndfxVolume = prevSndfxVolume;
         langNumber  = prevLanguage;
         window->Fechar(interf->ljan);
         cAction = ACT_NONE;
         return(OPTIONSW_CANCEL);
         break;
      }
   }
   
   char tmp[5];
   sprintf(tmp,"%d",musicVolume);
   txtMusicVolume->texto = tmp;

   sprintf(tmp,"%d",sndfxVolume);
   txtSndfxVolume->texto = tmp;

   txtLanguage->texto = languageName();

   window->Desenhar(0,0);
   cAction = ACT_NONE;
   return(OPTIONSW_OTHER);
} 

