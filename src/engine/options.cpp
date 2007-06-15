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
   textBox* qt;

   prevMusicVolume = musicVolume;
   prevSndfxVolume = sndfxVolume;

   intWindow = interf->insertWindow(276,169,531,434,
                                    language.OPTIONS_TITLE.c_str());

   /* Music Things */
   sprintf(tmp,"%d",musicVolume);
   saux = tmp;
   qt = intWindow->getObjectsList()->insertTextBox(8,27,145,44,0,
                                         language.OPTIONS_MUSIC_VOLUME.c_str());
   qt->setFont(FMINI,1,ALIGN_LEFT);
   buttonMusDec = intWindow->getObjectsList()->insertButton(146,27,156,44,"<",0);
   txtMusicVolume = intWindow->getObjectsList()->insertTextBox(157,27,197,44,1,
                                 saux.c_str());
   txtMusicVolume->setFont(FMINI,1,ALIGN_LEFT);
   buttonMusSum = intWindow->getObjectsList()->insertButton(198,27,208,44,">",0);
   intWindow->getObjectsList()->insertPicture(212,27,40,112,
                                  "../data/texturas/options/music.png");
   
  
   /* Sound Effects Things */
   sprintf(tmp,"%d",sndfxVolume);
   saux = tmp;
   qt = intWindow->getObjectsList()->insertTextBox(8,52,145,69,0,
                                         language.OPTIONS_SNDFX_VOLUME.c_str());
   qt->setFont(FMINI,1,ALIGN_LEFT);
   buttonSndDec = intWindow->getObjectsList()->insertButton(146,52,156,69,"<",0);
   txtSndfxVolume = intWindow->getObjectsList()->insertTextBox(157,52,197,69,1,
                                 saux.c_str());
   txtSndfxVolume->setFont(FMINI,1,ALIGN_LEFT);
   buttonSndSum = intWindow->getObjectsList()->insertButton(198,52,208,69,">",0);
   intWindow->getObjectsList()->insertPicture(212,52,40,112,
                                  "../data/texturas/options/sndfx.png");


   /* Language Things */
   prevLanguage = langNumber;
   saux = languageName();
   qt = intWindow->getObjectsList()->insertTextBox(8,88,145,105,0,
                                         language.OPTIONS_LANGUAGE.c_str());
   qt->setFont(FMINI,1,ALIGN_LEFT);
   buttonLangDec = intWindow->getObjectsList()->insertButton(121,88,131,105,"<",0);
   txtLanguage = intWindow->getObjectsList()->insertTextBox(132,88,197,105,1,
                                 saux.c_str());
   txtLanguage->setFont(FMINI,1,ALIGN_LEFT);
   buttonLangSum = intWindow->getObjectsList()->insertButton(198,88,208,105,">",0);
   intWindow->getObjectsList()->insertPicture(212,88,40,112,
                                  "../data/texturas/options/language.png");

   /* Camera Mode Things */
   prevCamera = cameraNumber;
   saux = cameraName();
   qt = intWindow->getObjectsList()->insertTextBox(8,126,145,143,0,
                                            language.OPTIONS_CAMERA.c_str());
   qt->setFont(FMINI,1,ALIGN_LEFT);
   buttonCamDec = intWindow->getObjectsList()->insertButton(121,126,131,143,"<",0);
   txtCamera = intWindow->getObjectsList()->insertTextBox(132,126,197,143,1,
                                 saux.c_str());
   txtCamera->setFont(FMINI,1,ALIGN_LEFT);
   buttonCamSum = intWindow->getObjectsList()->insertButton(198,126,208,143,">",0);
   intWindow->getObjectsList()->insertPicture(214,126,40,112,
                                  "../data/texturas/options/camera.png");


   /* Grass Enabled or Not */
   qt = intWindow->getObjectsList()->insertTextBox(20,156,200,173,0,
                                         language.OPTIONS_GRASS.c_str());
   qt->setFont(FMINI,1,ALIGN_LEFT);
   cxSelGrass = intWindow->getObjectsList()->insertCxSel(8,160, enableGrass);
   intWindow->getObjectsList()->insertPicture(214,156,40,112,
                                  "../data/texturas/options/grass.png");


   /* Particle System Enabled or Not */
   qt = intWindow->getObjectsList()->insertTextBox(20,174,200,191,0,
                                         language.OPTIONS_PARTICLES.c_str());
   qt->setFont(FMINI,1,ALIGN_LEFT);
   cxSelParticles = intWindow->getObjectsList()->insertCxSel(8,178, enableParticles);
   intWindow->getObjectsList()->insertPicture(214,174,40,112,
                                  "../data/texturas/options/particles.png");

   /* Reflexions */
   prevReflexion = reflexionType;
   saux = reflexionName();
   qt = intWindow->getObjectsList()->insertTextBox(8,203,145,220,0,
                                            language.OPTIONS_REFLECTS.c_str());
   qt->setFont(FMINI,1,ALIGN_LEFT);
   buttonReflDec = intWindow->getObjectsList()->insertButton(121,203,131,220,"<",0);
   txtReflexion = intWindow->getObjectsList()->insertTextBox(132,203,197,220,1,
                                 saux.c_str());
   txtReflexion->setFont(FMINI,1,ALIGN_LEFT);
   buttonReflSum = intWindow->getObjectsList()->insertButton(198,203,208,220,">",0);
   intWindow->getObjectsList()->insertPicture(212,203,40,220,
                                  "../data/texturas/options/reflexions.png");


   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(181,235,251,254,
                                              language.SKILL_CONFIRM.c_str(),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(8,235,78,254,
                                               language.SKILL_CANCEL.c_str(),1);

   /* borders */
   intWindow->getObjectsList()->insertTextBox(5,20,250,77,1,"");
   intWindow->getObjectsList()->insertTextBox(5,78,250,115,1,"");
   intWindow->getObjectsList()->insertTextBox(5,116,250,153,1,"");
   intWindow->getObjectsList()->insertTextBox(5,154,250,192,1,"");
   intWindow->getObjectsList()->insertTextBox(5,193,250,230,1,"");

   
   /* Open Skill Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   interf->openWindow(intWindow);
}

/****************************************************************
 *                             Treat                            *
 ****************************************************************/
int options::Treat(guiObject* object, int eventInfo, interface* interf)
{
   if( (eventInfo == ON_PRESS_BUTTON) && 
         (SDL_GetTicks() - timeLastOperation > 100) )
   {
      timeLastOperation = SDL_GetTicks();

      /* Music */
      if(object == (guiObject*) buttonMusSum)
      {
         if(musicVolume < SDL_MIX_MAXVOLUME)
         {
             musicVolume++;
         }
      }
      if(object == (guiObject*) buttonMusDec) 
      {
         if(musicVolume > 0)
         {
             musicVolume--;
         }
      }
      /* Sound Effects */
      if(object == (guiObject*) buttonSndSum) 
      {
         if(sndfxVolume < SDL_MIX_MAXVOLUME)
         {
             sndfxVolume++;
         }
      }
      if(object == (guiObject*) buttonSndDec) 
      {
         if(sndfxVolume > 0)
         {
             sndfxVolume--;
         }
      }
      /* Language */
      if(object == (guiObject*) buttonLangSum)
      {
         if(langNumber < LANG_LAST)
         {
            langNumber++;
         }
      }
      if(object == (guiObject*) buttonLangDec)
      {
         if(langNumber > LANG_FIRST)
         {
            langNumber--;
         }
      }
      /* Camera */
      if(object == (guiObject*) buttonCamSum)
      {
         if(cameraNumber < CAMERA_TYPE_DRIVE)
         {
            cameraNumber++;
         }
      }
      if(object == (guiObject*) buttonCamDec)
      {
         if(cameraNumber > CAMERA_TYPE_NORMAL)
         {
            cameraNumber--;
         }
      }
      /* Reflexin */
      if(object == (guiObject*) buttonReflSum)
      {
         if(reflexionType < REFLEXIONS_ALL)
         {
            reflexionType++;
         }
      }
      if(object == (guiObject*) buttonReflDec)
      {
         if(reflexionType > REFLEXIONS_NONE)
         {
            reflexionType--;
         }
      }

   }
   else if(eventInfo == PRESSED_BUTTON) 
   {
      /* Confirm */
      if( (object == (guiObject*) buttonConfirm) )
      {
         enableParticles = cxSelParticles->isSelected();
         enableGrass = cxSelGrass->isSelected();
         Save();
         interf->closeWindow(intWindow);
         return(OPTIONSW_CONFIRM);
      }
      /* Cancel */
      if( (object == (guiObject*) buttonCancel) )
      {
         musicVolume = prevMusicVolume;
         sndfxVolume = prevSndfxVolume;
         langNumber  = prevLanguage;
         cameraNumber = prevCamera;
         reflexionType = prevReflexion;
         interf->closeWindow(intWindow);
         return(OPTIONSW_CANCEL);
      }
   }
   else if(eventInfo == MODIFIED_CX_SEL)
   {
      /* cxSelParticles */
      if( (object == (guiObject*) cxSelParticles) || 
          (object == (guiObject*) cxSelGrass))
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
   txtMusicVolume->setText(tmp);

   sprintf(tmp,"%d",sndfxVolume);
   txtSndfxVolume->setText(tmp);

   txtLanguage->setText(languageName());
   txtCamera->setText(cameraName());
   txtReflexion->setText(reflexionName());

   intWindow->draw(0,0);
   return(OPTIONSW_OTHER);
} 

