#include "options.h"

#include "../lang/lang.h"
#include "../sound/sound.h"
#include "camera.h"
#include "util.h"

#define NM_PORTUGUES "Português"
#define NM_INGLES    "English"
#define NM_FRANCES   "Français"
#define NM_ESPANHOL  "Español"

/****************************************************************
 *                          Constructor                         *
 ****************************************************************/
options::options(string file)
{
   FILE* arq;
   char buffer[256];
   int aux;

   reflexionType = REFLEXIONS_NONE;
  
   timeLastOperation = SDL_GetTicks();

   if(!(arq = fopen(file.c_str(),"r")))
   {
      printf("Error while opening Options: %s\n",file.c_str());
      return;
   }
   fileName = file;

   while(fscanf(arq, "%s", buffer) != EOF)
   {
      switch(buffer[0])
      {
         case 'S':
           /* Read Sound and Music Options */
           fgets(buffer, sizeof(buffer), arq);
           sscanf(buffer,"%d %d",&musicVolume,&sndfxVolume);
         break;
         case 'L':
           /* Read Language Options */
           fgets(buffer, sizeof(buffer), arq); 
           sscanf(buffer,"%d",&langNumber);
         break;
         case 'C':
           /* Read Camera Options */
           fgets(buffer, sizeof(buffer), arq); 
           sscanf(buffer,"%d",&cameraNumber);
         break;
         case 'P':
           /* Read Particles Options */
           fgets(buffer, sizeof(buffer), arq); 
           sscanf(buffer,"%d",&aux);
           enableParticles = (aux == 1);
         break;
         case 'G':
           /* Read Grass Options */
           fgets(buffer, sizeof(buffer), arq); 
           sscanf(buffer,"%d",&aux);
           enableGrass = (aux == 1);
         break;
         case 'R':
            /* Read Reflexions Options */
            fgets(buffer, sizeof(buffer), arq);
            sscanf(buffer,"%d",&aux);
            reflexionType = aux;
         break;
      }
   }
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

/****************************************************************
 *                             Destructor                       *
 ****************************************************************/
options::~options()
{
}

/****************************************************************
 *                               Save                           *
 ****************************************************************/
void options::Save()
{
   FILE* arq;
   if(!(arq = fopen(fileName.c_str(),"w")))
   {
      printf("Error while opening Options: %s\n",fileName.c_str());
      return;
   }

   /* Sound */
   fprintf(arq,"Sound: %d %d\n",musicVolume,sndfxVolume);

   /* Language */
   fprintf(arq,"Language: %d\n",langNumber);

   /* Camera */
   fprintf(arq,"Camera: %d\n", cameraNumber);

   /* Particles */
   if(enableParticles)
   {
      fprintf(arq,"Particles: 1\n");
   }
   else
   {
      fprintf(arq,"Particles: 0\n");
   }
   
   /* Grass */
   if(enableGrass)
   {
      fprintf(arq,"Grass: 1\n");
   }
   else
   {
      fprintf(arq,"Grass: 0\n");
   }

   /* Reflexion */
   fprintf(arq, "Reflexions: %d\n", reflexionType);

   language.ReloadFile(langNumber);
   fclose(arq);
}

/****************************************************************
 *                         languageName                         *
 ****************************************************************/
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
 *                         languageName                         *
 ****************************************************************/
string options::reflexionName()
{
   string saux;
   switch(reflexionType)
   {
      case REFLEXIONS_NONE:
      {
         saux = language.OPTIONS_REFLECTS_NONE;
         break;
      }
      case REFLEXIONS_CHARACTERS:
      {
         saux = language.OPTIONS_REFLECTS_CHARACTER;
         break;
      }
      case REFLEXIONS_ALL: 
      {
         saux = language.OPTIONS_REFLECTS_ALL;
         break;
      }
   }
   return(saux);
}

/****************************************************************
 *                         languageName                         *
 ****************************************************************/
string options::cameraName()
{
   string saux;
   switch(cameraNumber)
   {
      case CAMERA_TYPE_NORMAL:
         saux = language.OPTIONS_CAMERA_NORMAL;
      break;
      case CAMERA_TYPE_DRIVE:
         saux = language.OPTIONS_CAMERA_DRIVE;
      break;
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

   window = interf->insertWindow(276,169,531,434,language.OPTIONS_TITLE.c_str(),
                                 1,1);

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
   window->objects->InserirFigura(212,27,40,112,
                                  "../data/texturas/options/music.png");
   
  
   /* Sound Effects Things */
   sprintf(tmp,"%d",sndfxVolume);
   saux = tmp;
   qt = window->objects->InserirQuadroTexto(8,52,145,69,0,
                                         language.OPTIONS_SNDFX_VOLUME.c_str());
   qt->fonte = FMINI;
   buttonSndDec = window->objects->InserirBotao(146,52,156,69,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   txtSndfxVolume = window->objects->InserirQuadroTexto(157,52,197,69,1,
                                 saux.c_str());
   txtSndfxVolume->fonte = FMINI;
   buttonSndSum = window->objects->InserirBotao(198,52,208,69,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,NULL);
   window->objects->InserirFigura(212,52,40,112,
                                  "../data/texturas/options/sndfx.png");


   /* Language Things */
   prevLanguage = langNumber;
   saux = languageName();
   qt = window->objects->InserirQuadroTexto(8,88,145,105,0,
                                         language.OPTIONS_LANGUAGE.c_str());
   qt->fonte = FMINI;
   buttonLangDec = window->objects->InserirBotao(121,88,131,105,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   txtLanguage = window->objects->InserirQuadroTexto(132,88,197,105,1,
                                 saux.c_str());
   txtLanguage->fonte = FMINI;
   buttonLangSum = window->objects->InserirBotao(198,88,208,105,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,NULL);
   window->objects->InserirFigura(212,88,40,112,
                                  "../data/texturas/options/language.png");

   /* Camera Mode Things */
   prevCamera = cameraNumber;
   saux = cameraName();
   qt = window->objects->InserirQuadroTexto(8,126,145,143,0,
                                            language.OPTIONS_CAMERA.c_str());
   qt->fonte = FMINI;
   buttonCamDec = window->objects->InserirBotao(121,126,131,143,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   txtCamera = window->objects->InserirQuadroTexto(132,126,197,143,1,
                                 saux.c_str());
   txtCamera->fonte = FMINI;
   buttonCamSum = window->objects->InserirBotao(198,126,208,143,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,NULL);
   window->objects->InserirFigura(214,126,40,112,
                                  "../data/texturas/options/camera.png");


   /* Grass Enabled or Not */
   qt = window->objects->InserirQuadroTexto(20,156,200,173,0,
                                         language.OPTIONS_GRASS.c_str());
   qt->fonte = FMINI;
   cxSelGrass = window->objects->insertCxSel(8,160, enableGrass);
   window->objects->InserirFigura(214,156,40,112,
                                  "../data/texturas/options/grass.png");


   /* Particle System Enabled or Not */
   qt = window->objects->InserirQuadroTexto(20,174,200,191,0,
                                         language.OPTIONS_PARTICLES.c_str());
   qt->fonte = FMINI;
   cxSelParticles = window->objects->insertCxSel(8,178, enableParticles);
   window->objects->InserirFigura(214,174,40,112,
                                  "../data/texturas/options/particles.png");

   /* Reflexions */
   prevReflexion = reflexionType;
   saux = reflexionName();
   qt = window->objects->InserirQuadroTexto(8,203,145,220,0,
                                            language.OPTIONS_REFLECTS.c_str());
   qt->fonte = FMINI;
   buttonReflDec = window->objects->InserirBotao(121,203,131,220,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   txtReflexion = window->objects->InserirQuadroTexto(132,203,197,220,1,
                                 saux.c_str());
   txtReflexion->fonte = FMINI;
   buttonReflSum = window->objects->InserirBotao(198,203,208,220,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,NULL);
   window->objects->InserirFigura(212,203,40,220,
                                  "../data/texturas/options/reflexions.png");


   /* Confirm Button */
   buttonConfirm = window->objects->InserirBotao(181,235,251,254, 
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CONFIRM.c_str(),1,NULL);
   
   /* Cancel Button */
   buttonCancel = window->objects->InserirBotao(8,235,78,254, 
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CANCEL.c_str(),1,NULL);

   /* borders */
   window->objects->InserirQuadroTexto(5,20,250,77,1,"");
   window->objects->InserirQuadroTexto(5,78,250,115,1,"");
   window->objects->InserirQuadroTexto(5,116,250,153,1,"");
   window->objects->InserirQuadroTexto(5,154,250,192,1,"");
   window->objects->InserirQuadroTexto(5,193,250,230,1,"");

   
   /* Open Skill Window */
   window->ptrExterno = &window;
   window->fechavel = false;
   interf->openWindow(window);
}

/****************************************************************
 *                             Treat                            *
 ****************************************************************/
int options::Treat(Tobjeto* object, int eventInfo, interface* interf)
{
   if( (eventInfo == BOTAOEMPRESSAO) && 
         (SDL_GetTicks() - timeLastOperation > 100) )
   {
      timeLastOperation = SDL_GetTicks();

      /* Music */
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
      /* Sound Effects */
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
      /* Language */
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
      /* Camera */
      if(object == (Tobjeto*) buttonCamSum)
      {
         if(cameraNumber < CAMERA_TYPE_DRIVE)
         {
            cameraNumber++;
         }
      }
      if(object == (Tobjeto*) buttonCamDec)
      {
         if(cameraNumber > CAMERA_TYPE_NORMAL)
         {
            cameraNumber--;
         }
      }
      /* Reflexin */
      if(object == (Tobjeto*) buttonReflSum)
      {
         if(reflexionType < REFLEXIONS_ALL)
         {
            reflexionType++;
         }
      }
      if(object == (Tobjeto*) buttonReflDec)
      {
         if(reflexionType > REFLEXIONS_NONE)
         {
            reflexionType--;
         }
      }

   }
   else if(eventInfo == BOTAOPRESSIONADO) 
   {
      /* Confirm */
      if( (object == (Tobjeto*) buttonConfirm) )
      {
         enableParticles = cxSelParticles->isSelected();
         enableGrass = cxSelGrass->isSelected();
         Save();
         interf->closeWindow(window);
         return(OPTIONSW_CONFIRM);
      }
      /* Cancel */
      if( (object == (Tobjeto*) buttonCancel) )
      {
         musicVolume = prevMusicVolume;
         sndfxVolume = prevSndfxVolume;
         langNumber  = prevLanguage;
         cameraNumber = prevCamera;
         reflexionType = prevReflexion;
         interf->closeWindow(window);
         return(OPTIONSW_CANCEL);
      }
   }
   else if(eventInfo == CXSELMODIFICADA)
   {
      /* cxSelParticles */
      if( (object == (Tobjeto*) cxSelParticles) || 
          (object == (Tobjeto*) cxSelGrass))
      {
         /* When disable particles, disable the grass */
         if(!cxSelParticles->isSelected())
         {
            cxSelGrass->setSelection(false);
         }
      }
   }

   char tmp[5];
   sprintf(tmp,"%d",musicVolume);
   txtMusicVolume->texto = tmp;

   sprintf(tmp,"%d",sndfxVolume);
   txtSndfxVolume->texto = tmp;

   txtLanguage->texto = languageName();
   txtCamera->texto = cameraName();
   txtReflexion->texto = reflexionName();

   window->Desenhar(0,0);
   return(OPTIONSW_OTHER);
} 

