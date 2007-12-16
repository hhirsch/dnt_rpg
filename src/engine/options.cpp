#include "options.h"

#include "../sound/sound.h"
#include "camera.h"
#include "util.h"

#define NM_PORTUGUESE gettext("Portuguese")
#define NM_ENGLISH    gettext("English")
#define NM_FRENCH     gettext("French")
#define NM_SPANISH    gettext("Spanish")

#define FL_PORTUGUESE "pt_BR"
#define FL_ENGLISH    "en"
#define FL_FRENCH     "fr"
#define FL_SPANISH    "es"

#define DNT_LANG_PORTUGUESE 3 /**< Portuguese Language */
#define DNT_LANG_SPANISH    2 /**< Spanish Language */
#define DNT_LANG_FRENCH     1 /**< French Language */
#define DNT_LANG_ENGLISH    0 /**< English Language */

#define DNT_LANG_LAST  3
#define DNT_LANG_FIRST 0

/****************************************************************
 *                          Constructor                         *
 ****************************************************************/
options::options()
{
   reflexionType = REFLEXIONS_NONE;
   timeLastOperation = SDL_GetTicks();
}

/****************************************************************
 *                             Destructor                       *
 ****************************************************************/
options::~options()
{
}

/****************************************************************
 *                     getAvaibleResolutions                    *
 ****************************************************************/
void options::getAvaibleResolutions()
{
   /* Get available fullscreen/hardware modes */
   resolutions=SDL_ListModes(NULL,SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_OPENGL);
}

/****************************************************************
 *                              Load                            *
 ****************************************************************/
bool options::load(string file)
{
   FILE* arq;
   char buffer[256];
   int aux;
   string s = "";

   reflexionType = REFLEXIONS_NONE;
  
   timeLastOperation = SDL_GetTicks();

   if(!(arq = fopen(file.c_str(),"r")))
   {
      return(false);
   }

   while(fscanf(arq, "%s", buffer) != EOF)
   {
      s = buffer;
      if(s == "Sound:")
      {
         /* Read Sound and Music Options */
         fgets(buffer, sizeof(buffer), arq);
         sscanf(buffer,"%d %d",&musicVolume,&sndfxVolume);
      }
      else if(s == "Language:")
      {
         /* Read Language Options */
         fgets(buffer, sizeof(buffer), arq); 
         sscanf(buffer,"%d",&langNumber);
      }
      else if(s == "Camera:")
      {
         /* Read Camera Options */
         fgets(buffer, sizeof(buffer), arq); 
         sscanf(buffer,"%d",&cameraNumber);
      }
      else if(s == "Particles:")
      {
         /* Read Particles Options */
         fgets(buffer, sizeof(buffer), arq); 
         sscanf(buffer,"%d",&aux);
         enableParticles = (aux == 1);
      }
      else if(s == "Grass:")
      {
         /* Read Grass Options */
         fgets(buffer, sizeof(buffer), arq); 
         sscanf(buffer,"%d",&aux);
         enableGrass = (aux == 1);
      }
      else if(s == "Reflexions:")
      {
         /* Read Reflexions Options */
         fgets(buffer, sizeof(buffer), arq);
         sscanf(buffer,"%d",&aux);
         reflexionType = aux;
      }
      else if(s == "Resolution:")
      {
         /* Read the resolution informations */
         fgets(buffer, sizeof(buffer), arq);
         sscanf(buffer,"%d %d", &screenWidth, &screenHeight);
      }
      else if(s == "FullScreen:")
      {
         /* Read Grass Options */
         fgets(buffer, sizeof(buffer), arq); 
         sscanf(buffer,"%d",&aux);
         enableFullScreen = (aux == 1);
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
   
   return(true);

}

/****************************************************************
 *                               Save                           *
 ****************************************************************/
void options::save(string fileName)
{
   FILE* arq;
   if(!(arq = fopen(fileName.c_str(),"w")))
   {
      printf(gettext("Error while writing Options: %s\n"),fileName.c_str());
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

   /* Resolution */
   fprintf(arq, "Resolution: %d %d\n",screenWidth, screenHeight);

   /* Fullscreen */
   if(enableFullScreen)
   {
      fprintf(arq, "FullScreen: 1\n");
   }
   else
   {
      fprintf(arq, "FullScreen: 0\n");
   }

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
      case DNT_LANG_PORTUGUESE:
      {
         saux = NM_PORTUGUESE;
         break;
      }
      case DNT_LANG_SPANISH:
      {
         saux = NM_SPANISH;
         break;
      }
      case DNT_LANG_FRENCH: 
      {
         saux = NM_FRENCH;
         break;
      }
      case DNT_LANG_ENGLISH:
      {
         saux = NM_ENGLISH;
         break;
      }
   }
   return(saux);
}

/****************************************************************
 *                         setLanguage                          *
 ****************************************************************/
void options::setLanguage()
{
   string saux = "";
   switch(langNumber)
   {
      case DNT_LANG_PORTUGUESE:
      {
         saux = FL_PORTUGUESE;
         break;
      }
      case DNT_LANG_SPANISH:
      {
         saux = FL_SPANISH;
         break;
      }
      case DNT_LANG_FRENCH: 
      {
         saux = FL_FRENCH;
         break;
      }
      case DNT_LANG_ENGLISH:
      {
         saux = FL_ENGLISH;
         break;
      }
   }

   if(!saux.empty())
   {
      /* Change language.  */
	  #ifdef _MSC_VER
	     SetEnvironmentVariable( "LANGUAGE", saux.c_str() );
      #else
         setenv ("LANGUAGE", saux.c_str(), 1);
		 /* Make change known.  */
		 {
            extern int  _nl_msg_cat_cntr;
            ++_nl_msg_cat_cntr;
		 }
      #endif
   }
}

/****************************************************************
 *                        reflexionName                         *
 ****************************************************************/
string options::reflexionName()
{
   string saux;
   switch(reflexionType)
   {
      case REFLEXIONS_NONE:
      {
         saux = gettext("None");
         break;
      }
      case REFLEXIONS_CHARACTERS:
      {
         saux = gettext("Characters");
         break;
      }
      case REFLEXIONS_ALL: 
      {
         saux = gettext("All");
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
         saux = gettext("Normal");
      break;
      case CAMERA_TYPE_DRIVE:
         saux = gettext("Pursue");
      break;
   }
   return(saux);
}
/****************************************************************
 *                    Open Options Screen                       *
 ****************************************************************/
void options::displayOptionsScreen(interface* interf)
{
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL, 10);
   char tmp[5];
   string saux;
   textBox* qt;

   prevMusicVolume = musicVolume;
   prevSndfxVolume = sndfxVolume;

   int xPos = (int)(SCREEN_X / 2.0);
   int yPos = (int)(SCREEN_Y / 2.0);

   intWindow = interf->insertWindow(xPos-128,yPos-133,xPos+128,yPos+133,
                                    gettext("Options"));

   /* Music Things */
   sprintf(tmp,"%d",musicVolume);
   saux = tmp;
   qt = intWindow->getObjectsList()->insertTextBox(8,27,145,44,0,
                                         gettext("Music Volume:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonMusDec = intWindow->getObjectsList()->insertButton(146,27,156,44,
                                                  fnt.createUnicode(0x25C4),0);
   buttonMusDec->defineFont(DNT_FONT_ARIAL, 9);
   txtMusicVolume = intWindow->getObjectsList()->insertTextBox(157,27,197,44,
                                                               1,saux.c_str());
   txtMusicVolume->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonMusSum = intWindow->getObjectsList()->insertButton(198,27,208,44,
                                                  fnt.createUnicode(0x25BA),0);
   buttonMusSum->defineFont(DNT_FONT_ARIAL, 9);
   intWindow->getObjectsList()->insertPicture(212,27,40,112,
                                  "../data/texturas/options/music.png");
   
  
   /* Sound Effects Things */
   sprintf(tmp,"%d",sndfxVolume);
   saux = tmp;
   qt = intWindow->getObjectsList()->insertTextBox(8,52,145,69,0,
                                         gettext("Effects Volume:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonSndDec = intWindow->getObjectsList()->insertButton(146,52,156,69,
                                                  fnt.createUnicode(0x25C4),0);
   buttonSndDec->defineFont(DNT_FONT_ARIAL, 9);
   txtSndfxVolume = intWindow->getObjectsList()->insertTextBox(157,52,197,69,1,
                                 saux.c_str());
   txtSndfxVolume->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonSndSum = intWindow->getObjectsList()->insertButton(198,52,208,69,
                                                  fnt.createUnicode(0x25BA),0);
   buttonSndSum->defineFont(DNT_FONT_ARIAL, 9);
   intWindow->getObjectsList()->insertPicture(212,52,40,112,
                                  "../data/texturas/options/sndfx.png");


   /* Language Things */
   prevLanguage = langNumber;
   saux = languageName();
   qt = intWindow->getObjectsList()->insertTextBox(8,88,145,105,0,
                                                   gettext("Language:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonLangDec = intWindow->getObjectsList()->insertButton(121,88,131,105,
                                                   fnt.createUnicode(0x25C4),0);
   buttonLangDec->defineFont(DNT_FONT_ARIAL, 9);
   txtLanguage = intWindow->getObjectsList()->insertTextBox(132,88,197,105,1,
                                 saux.c_str());
   txtLanguage->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonLangSum = intWindow->getObjectsList()->insertButton(198,88,208,105,
                                                   fnt.createUnicode(0x25BA),0);
   buttonLangSum->defineFont(DNT_FONT_ARIAL, 9);
   intWindow->getObjectsList()->insertPicture(212,88,40,112,
                                  "../data/texturas/options/language.png");

   /* Camera Mode Things */
   prevCamera = cameraNumber;
   saux = cameraName();
   qt = intWindow->getObjectsList()->insertTextBox(8,126,145,143,0,
                                            gettext("Camera Mode:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonCamDec = intWindow->getObjectsList()->insertButton(121,126,131,143,
                                                  fnt.createUnicode(0x25C4),0);
   buttonCamDec->defineFont(DNT_FONT_ARIAL, 9);
   txtCamera = intWindow->getObjectsList()->insertTextBox(132,126,197,143,1,
                                 saux.c_str());
   txtCamera->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonCamSum = intWindow->getObjectsList()->insertButton(198,126,208,143,
                                                  fnt.createUnicode(0x25BA),0);
   buttonCamSum->defineFont(DNT_FONT_ARIAL, 9);
   intWindow->getObjectsList()->insertPicture(214,126,40,112,
                                  "../data/texturas/options/camera.png");


   /* Grass Enabled or Not */
   qt = intWindow->getObjectsList()->insertTextBox(20,156,200,173,0,
                          gettext("Enable Grass Effects (need particles)"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelGrass = intWindow->getObjectsList()->insertCxSel(8,160, enableGrass);
   intWindow->getObjectsList()->insertPicture(214,156,40,112,
                                  "../data/texturas/options/grass.png");


   /* Particle System Enabled or Not */
   qt = intWindow->getObjectsList()->insertTextBox(20,174,200,191,0,
                                         gettext("Enable Particles Effects"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelParticles = intWindow->getObjectsList()->insertCxSel(8,178, 
                                                             enableParticles);
   intWindow->getObjectsList()->insertPicture(214,174,40,112,
                                  "../data/texturas/options/particles.png");

   /* Reflexions */
   prevReflexion = reflexionType;
   saux = reflexionName();
   qt = intWindow->getObjectsList()->insertTextBox(8,203,145,220,0,
                                            gettext("Reflections:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonReflDec = intWindow->getObjectsList()->insertButton(121,203,131,220,
                                                  fnt.createUnicode(0x25C4),0);
   buttonReflDec->defineFont(DNT_FONT_ARIAL, 9);
   txtReflexion = intWindow->getObjectsList()->insertTextBox(132,203,197,220,1,
                                 saux.c_str());
   txtReflexion->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonReflSum = intWindow->getObjectsList()->insertButton(198,203,208,220,
                                                  fnt.createUnicode(0x25BA),0);
   buttonReflSum->defineFont(DNT_FONT_ARIAL, 9);
   intWindow->getObjectsList()->insertPicture(212,203,40,220,
                                  "../data/texturas/options/reflexions.png");


   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(177,235,247,254,
                                              gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(8,235,78,254,
                                              gettext("Cancel"),1);

   /* borders */
   intWindow->getObjectsList()->insertTextBox(5,20,250,77,2,"");
   intWindow->getObjectsList()->insertTextBox(5,78,250,115,2,"");
   intWindow->getObjectsList()->insertTextBox(5,116,250,153,2,"");
   intWindow->getObjectsList()->insertTextBox(5,154,250,192,2,"");
   intWindow->getObjectsList()->insertTextBox(5,193,250,230,2,"");
   intWindow->getObjectsList()->insertTextBox(5,231,250,258,2,"");

   
   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   interf->openWindow(intWindow);
}

/****************************************************************
 *                             Treat                            *
 ****************************************************************/
int options::treat(guiObject* object, int eventInfo, interface* interf)
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
         if(langNumber < DNT_LANG_LAST)
         {
            langNumber++;
         }
      }
      if(object == (guiObject*) buttonLangDec)
      {
         if(langNumber > DNT_LANG_FIRST)
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
         //FIXME -> the save fileName must be at the users home!
         save("dcc.opc");
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


int  options::musicVolume;      /**< Actual Music Volume */
int  options::sndfxVolume;      /**< Actual Sound Effects Volume */
int  options::langNumber;       /**< Actual Language Number */
int  options::cameraNumber;     /**< Actual Camera Mode */
bool options::enableParticles;  /**< Enable/Disable Particles */
bool options::enableGrass;      /**< Enable/Disable Grass */
int  options::reflexionType;    /**< Reflexion Type */
int  options::screenWidth;      /**< Screen Height */
int  options::screenHeight;     /**< Screen Width */
bool options::enableFullScreen; /**< If fullscreen is defined */

