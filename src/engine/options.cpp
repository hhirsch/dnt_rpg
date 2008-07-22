#include "options.h"

#include "../sound/sound.h"
#include "camera.h"
#include "util.h"
#include "../etc/userinfo.h"
#include "../etc/dirs.h"
#include "../etc/extensions.h"

#ifdef _MSC_VER
   #include "../config_win.h"
#else
   #include "../config.h"
   #include <sys/stat.h>
   #include <sys/types.h>
   #include <errno.h>
#endif

#define NM_PORTUGUESE gettext("Portuguese")
#define NM_ENGLISH    gettext("English")
#define NM_FRENCH     gettext("French")
#define NM_GERMAN     gettext("German")
#define NM_SPANISH    gettext("Spanish")
#define NM_RUSSIAN    gettext("Russian")

#define FL_PORTUGUESE "pt_BR"
#define FL_ENGLISH    "en"
#define FL_FRENCH     "fr"
#define FL_GERMAN     "de"
#define FL_SPANISH    "es"
#define FL_RUSSIAN    "ru"


#define DNT_LANG_RUSSIAN    5 /**< Russian Language */
#define DNT_LANG_GERMAN     4 /**< German Language */
#define DNT_LANG_PORTUGUESE 3 /**< Portuguese Language */
#define DNT_LANG_SPANISH    2 /**< Spanish Language */
#define DNT_LANG_FRENCH     1 /**< French Language */
#define DNT_LANG_ENGLISH    0 /**< English Language */

#define DNT_LANG_LAST  4
#define DNT_LANG_FIRST 0

/****************************************************************
 *                          Constructor                         *
 ****************************************************************/
options::options()
{
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
bool options::load()
{
   string file;
   /* Try to Load From Users Directory */
   userInfo info;
   file  = info.getUserHome()+"options.cfg";
   if(!load(file))
   {
      printf(gettext("Can't Open the options file: %s\n"), file.c_str());
      /* Try to Load from Default Installed Data Dir */
      file = DATADIR;
      file += "/";
      file += PACKAGE;
      file += "/dcc.opc";
      if(!load(file))
      {
         printf(gettext("Can't Open the options file: %s\n"), file.c_str());
         /* Load from default executable path */
         if(!load("./dcc.opc"))
         {
            printf(gettext("No Options File Avaible!\n"));
            return(false);
         }
      }

      #ifndef _MSC_VER
         fileName = info.getUserHome() + "options.cfg";
         printf(gettext("Creating Directory: %s : "), 
                info.getUserHome().c_str());
         /* Create the User directory */
         mkdir(info.getUserHome().c_str(),0755);
         printf("%s\n", strerror(errno));

         /* Save the options file */
         printf(gettext("Creating Options: %s\n"), fileName.c_str());
         save();
      #endif

   }
   return(true);
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
         /* Read FullScreen Options */
         fgets(buffer, sizeof(buffer), arq); 
         sscanf(buffer,"%d",&aux);
         enableFullScreen = (aux == 1);
      }
      else if(s == "AntiAliasing:")
      {
         /* Read Antialising Options */
         fgets(buffer, sizeof(buffer), arq); 
         sscanf(buffer,"%d",&antiAliasing);
      }
      else if(s == "FarViewFactor:")
      {
         /* Read FarViewFactor Options */
         fgets(buffer, sizeof(buffer), arq); 
         sscanf(buffer,"%f",&farViewFactor);
      }
      else 
      {
         printf(gettext("Unknow option: %s\n"), buffer);
      }
   }

   /* Limits */
   if(farViewFactor > 1.0)
   {
      farViewFactor = 1.0;
   }
   else if(farViewFactor < 0.2)
   {
      farViewFactor = 0.2;
   }

   fclose(arq);

   fileName = file;
   
   return(true);

}

/****************************************************************
 *                               Save                           *
 ****************************************************************/
void options::save()
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

   /* AntiAliasing */
   fprintf(arq, "AntiAliasing: %d\n",antiAliasing);

   /* FarViewFactor */
   fprintf(arq, "FarViewFactor: %.2f\n", farViewFactor);

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
      case DNT_LANG_RUSSIAN:
      {
         saux = NM_RUSSIAN;
         break;
      }
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
      case DNT_LANG_GERMAN:
      {
         saux = NM_GERMAN;
         break;
      }
   }
   return(saux);
}

/****************************************************************
 *                      isLanguageUnicode                       *
 ****************************************************************/
bool options::isLanguageUnicode()
{
   return( (langNumber == DNT_LANG_RUSSIAN) );
}

/****************************************************************
 *                         setLanguage                          *
 ****************************************************************/
void options::setLanguage()
{
   string saux = "";

   switch(langNumber)
   {
      case DNT_LANG_RUSSIAN:
      {
         saux = FL_RUSSIAN;
         break;
      }
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
      case DNT_LANG_GERMAN:
      {
         saux = FL_GERMAN;
         break;
      }
   }


   if(!saux.empty())
   {
      /* Change language.  */
      #ifdef _MSC_VER
         putenv((string("LANG=" + saux)).c_str());
      #else
         setenv("LANGUAGE", saux.c_str(), 1);
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
 *                          CameraName                          *
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
 *                       AntiAliasingName                       *
 ****************************************************************/
string options::antiAliasingName()
{
   string saux;
   switch(antiAliasing)
   {
      case 0:
         saux = gettext("Disabled");
      break;
      case 2:
         saux = gettext("2x");
      break;
      case 4:
         saux = gettext("4x");
      break;
   }
   return(saux);
}


/****************************************************************
 *                        resolutionName                        *
 ****************************************************************/
string options::resolutionName()
{
   string saux = "";
   char buf[16];
   sprintf(buf,"%dx%d",screenWidth, screenHeight);
   saux = buf;

   int i;

   resPosition = 0;
   for(i = 0; resolutions[i]; i++)
   {
      if( (resolutions[i]->w == screenWidth) &&
          (resolutions[i]->h == screenHeight) )
      {
         resPosition = i;
      }
   }
   return(saux);
}

/****************************************************************
 *                    Open Options Screen                       *
 ****************************************************************/
void options::displayOptionsScreen(guiInterface* interf)
{
   dirs dir;
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL, 10);
   char tmp[8];
   string saux;
   textBox* qt;

   getAvaibleResolutions();

   prevMusicVolume = musicVolume;
   prevSndfxVolume = sndfxVolume;

   prevAntiAliasing = antiAliasing;
   prevFarViewFactor = farViewFactor;

   int xPos = (int)(SCREEN_X / 2.0);
   int yPos = (int)(SCREEN_Y / 2.0);

   intWindow = interf->insertWindow(xPos-128,yPos-185,xPos+128,yPos+185,
                                    gettext("Options"));

   /* Music Things */
   qt = intWindow->getObjectsList()->insertTextBox(8,27,145,44,0,
                                         gettext("Music Volume:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonMusDec = intWindow->getObjectsList()->insertButton(121,27,131,44,
                                                  fnt.createUnicode(0x25C4),0);
   buttonMusDec->defineFont(DNT_FONT_ARIAL, 9);
   barMusicVolume = intWindow->getObjectsList()->insertHealthBar(133,27,
                                                                 196,44,255);
   barMusicVolume->defineActualHealth(musicVolume);
   buttonMusSum = intWindow->getObjectsList()->insertButton(198,27,208,44,
                                                  fnt.createUnicode(0x25BA),0);
   buttonMusSum->defineFont(DNT_FONT_ARIAL, 9);
   intWindow->getObjectsList()->insertPicture(220,27,40,112,
                         dir.getRealFile("texturas/options/music.png").c_str());
   
  
   /* Sound Effects Things */
   sprintf(tmp,"%d",sndfxVolume);
   saux = tmp;
   qt = intWindow->getObjectsList()->insertTextBox(8,52,145,69,0,
                                         gettext("Effects Volume:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonSndDec = intWindow->getObjectsList()->insertButton(121,52,131,69,
                                                  fnt.createUnicode(0x25C4),0);
   buttonSndDec->defineFont(DNT_FONT_ARIAL, 9);
   barSndfxVolume = intWindow->getObjectsList()->insertHealthBar(133,52,
                                                                 196,69,255);
   barSndfxVolume->defineActualHealth(sndfxVolume);
   buttonSndSum = intWindow->getObjectsList()->insertButton(198,52,208,69,
                                                  fnt.createUnicode(0x25BA),0);
   buttonSndSum->defineFont(DNT_FONT_ARIAL, 9);
   intWindow->getObjectsList()->insertPicture(220,52,40,112,
                         dir.getRealFile("texturas/options/sndfx.png").c_str());


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
   intWindow->getObjectsList()->insertPicture(220,88,40,112,
                      dir.getRealFile("texturas/options/language.png").c_str());

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
   intWindow->getObjectsList()->insertPicture(220,126,40,112,
                        dir.getRealFile("texturas/options/camera.png").c_str());


   /* Grass Enabled or Not */
   qt = intWindow->getObjectsList()->insertTextBox(20,156,200,173,0,
                          gettext("Enable Grass Effects (need particles)"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelGrass = intWindow->getObjectsList()->insertCxSel(8,160, enableGrass);
   intWindow->getObjectsList()->insertPicture(220,156,40,112,
                         dir.getRealFile("texturas/options/grass.png").c_str());


   /* Particle System Enabled or Not */
   qt = intWindow->getObjectsList()->insertTextBox(20,174,200,191,0,
                                         gettext("Enable Particles Effects"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelParticles = intWindow->getObjectsList()->insertCxSel(8,178, 
                                                             enableParticles);
   intWindow->getObjectsList()->insertPicture(220,174,40,112,
                     dir.getRealFile("texturas/options/particles.png").c_str());
   cxSelParticles->setAvaible(getAvaibleParticles());
   cxSelGrass->setAvaible(getAvaibleParticles() && 
                          cxSelParticles->isSelected());


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
   intWindow->getObjectsList()->insertPicture(220,203,40,220,
                    dir.getRealFile("texturas/options/reflexions.png").c_str());

   /* Resolution */
   prevHeight = screenHeight;
   prevWidth = screenWidth;
   saux = resolutionName();
   qt = intWindow->getObjectsList()->insertTextBox(8,240,145,257,0,
                                            gettext("Screen Resolution:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonResDec = intWindow->getObjectsList()->insertButton(121,240,131,257,
                                                  fnt.createUnicode(0x25C4),0);
   buttonResDec->defineFont(DNT_FONT_ARIAL, 9);
   txtResolution = intWindow->getObjectsList()->insertTextBox(132,240,197,257,1,
                                 saux.c_str());
   txtResolution->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonResSum = intWindow->getObjectsList()->insertButton(198,240,208,257,
                                                  fnt.createUnicode(0x25BA),0);
   buttonResSum->defineFont(DNT_FONT_ARIAL, 9);
   intWindow->getObjectsList()->insertPicture(220,240,40,220,
                    dir.getRealFile("texturas/options/resolution.png").c_str());

   /* Fullscreen */
   qt = intWindow->getObjectsList()->insertTextBox(20,265,200,282,0,
                                               gettext("Enable FullScreen"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelFullScreen = intWindow->getObjectsList()->insertCxSel(8,269, 
                                                              enableFullScreen);

   intWindow->getObjectsList()->insertPicture(220,265,40,220,
                    dir.getRealFile("texturas/options/fullscreen.png").c_str());

   /* AntiAliasing */                 
   saux = antiAliasingName();
   qt = intWindow->getObjectsList()->insertTextBox(8,288,145,305,0,
                                            gettext("Anti-Aliasing:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonAliasDec = intWindow->getObjectsList()->insertButton(121,288,131,305,
                                                  fnt.createUnicode(0x25C4),0);
   buttonAliasDec->defineFont(DNT_FONT_ARIAL, 9);
   txtAntiAliasing = intWindow->getObjectsList()->insertTextBox(132,288,
                                                                197,305,1,
                                                                saux.c_str());
   txtAntiAliasing->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonAliasSum = intWindow->getObjectsList()->insertButton(198,288,208,305,
                                                  fnt.createUnicode(0x25BA),0);
   buttonAliasSum->defineFont(DNT_FONT_ARIAL, 9);
   intWindow->getObjectsList()->insertPicture(220,288,40,220,
                 dir.getRealFile("texturas/options/antialiasing.png").c_str());

   /* FarViewFactor */
   qt = intWindow->getObjectsList()->insertTextBox(8,311,145,328,0,
                                                   gettext("FarView:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonFarViewDec = intWindow->getObjectsList()->insertButton(121,311,131,328,
                                                  fnt.createUnicode(0x25C4),0);
   buttonFarViewDec->defineFont(DNT_FONT_ARIAL, 9);
   barFarView = intWindow->getObjectsList()->insertHealthBar(133,311,196,328,9);
   barFarView->defineActualHealth((int)floor(farViewFactor*9));                                                          
   buttonFarViewSum = intWindow->getObjectsList()->insertButton(198,311,208,328,
                                                  fnt.createUnicode(0x25BA),0);
   buttonFarViewSum->defineFont(DNT_FONT_ARIAL, 9);
   intWindow->getObjectsList()->insertPicture(220,311,40,328,
                 dir.getRealFile("texturas/options/antialiasing.png").c_str());

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(177,338,247,357,
                                              gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(8,338,78,357,
                                              gettext("Cancel"),1);

   /* borders */
   intWindow->getObjectsList()->insertTextBox(5,20,250,77,2,"");
   intWindow->getObjectsList()->insertTextBox(5,78,250,115,2,"");
   intWindow->getObjectsList()->insertTextBox(5,116,250,153,2,"");
   intWindow->getObjectsList()->insertTextBox(5,154,250,192,2,"");
   intWindow->getObjectsList()->insertTextBox(5,193,250,230,2,"");
   intWindow->getObjectsList()->insertTextBox(5,231,250,333,2,"");
   intWindow->getObjectsList()->insertTextBox(5,334,250,361,2,"");

   
   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   interf->openWindow(intWindow);
}

/****************************************************************
 *                             Treat                            *
 ****************************************************************/
int options::treat(guiObject* object, int eventInfo, guiInterface* interf,
                   GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   if( (eventInfo == ON_PRESS_BUTTON) && 
         (SDL_GetTicks() - timeLastOperation > 100) )
   {
      timeLastOperation = SDL_GetTicks();

      /* Music */
      if(object == (guiObject*) buttonMusSum)
      {
         if(musicVolume < 255)
         {
             musicVolume += 10;
             if(musicVolume > 255)
             {
                musicVolume = 255;
             }
         }
      }
      else if(object == (guiObject*) buttonMusDec) 
      {
         if(musicVolume > 0)
         {
             musicVolume -= 10;
             if(musicVolume < 0)
             {
                musicVolume = 0;
             }
         }
      }
      /* Sound Effects */
      else if(object == (guiObject*) buttonSndSum) 
      {
         if(sndfxVolume < 255)
         {
             sndfxVolume += 10;
             if(sndfxVolume > 255)
             {
                sndfxVolume = 255;
             }
         }
      }
      else if(object == (guiObject*) buttonSndDec) 
      {
         if(sndfxVolume > 0)
         {
             sndfxVolume -= 10;
             if(sndfxVolume < 0)
             {
                sndfxVolume = 0;
             }
         }
      }
      /* Language */
      else if(object == (guiObject*) buttonLangSum)
      {
         if(langNumber < DNT_LANG_LAST)
         {
            langNumber++;
         }
      }
      else if(object == (guiObject*) buttonLangDec)
      {
         if(langNumber > DNT_LANG_FIRST)
         {
            langNumber--;
         }
      }
      /* Camera */
      else if(object == (guiObject*) buttonCamSum)
      {
         if(cameraNumber < CAMERA_TYPE_DRIVE)
         {
            cameraNumber++;
         }
      }
      else if(object == (guiObject*) buttonCamDec)
      {
         if(cameraNumber > CAMERA_TYPE_NORMAL)
         {
            cameraNumber--;
         }
      }
      /* Reflexion */
      else if(object == (guiObject*) buttonReflSum)
      {
         if(reflexionType < REFLEXIONS_ALL)
         {
            reflexionType++;
         }
      }
      else if(object == (guiObject*) buttonReflDec)
      {
         if(reflexionType > REFLEXIONS_NONE)
         {
            reflexionType--;
         }
      }
      /* Resolution */
      else if(object == (guiObject*) buttonResSum)
      {
         if(resPosition > 0)
         {
            resPosition--;
            
            /* Only accept resolutions >= 800x600 */
            if( (resolutions[resPosition]->w >= 800) && 
                (resolutions[resPosition]->h >= 600) )
            {
               screenWidth = resolutions[resPosition]->w;
               screenHeight = resolutions[resPosition]->h;
            }
         }
      }
      else if(object == (guiObject*) buttonResDec)
      {
         if(resolutions[resPosition+1])
         {
            resPosition++;
            /* Only accept resolutions >= 800x600 */
            if( (resolutions[resPosition]->w >= 800) && 
                (resolutions[resPosition]->h >= 600) )
            {
               screenWidth = resolutions[resPosition]->w;
               screenHeight = resolutions[resPosition]->h;
            }
         }
      }

      /* Anti Aliasing */
      else if(object == (guiObject*) buttonAliasSum) 
      {
         if(antiAliasing < 4)
         {
             antiAliasing += 2;
         }
      }
      else if(object == (guiObject*) buttonAliasDec) 
      {
         if(antiAliasing > 0)
         {
             antiAliasing -= 2;
         }
      }
      /* FarView Factor */
      else if(object == (guiObject*) buttonFarViewSum) 
      {
         if(farViewFactor < 1.0)
         {
            farViewFactor += 0.1;
         }
      }
      else if(object == (guiObject*) buttonFarViewDec) 
      {
         if(farViewFactor > 0.2)
         {
            farViewFactor -= 0.1;
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

         bool prevFullScreen = enableFullScreen;
         enableFullScreen = cxSelFullScreen->isSelected();
         save();
         interf->closeWindow(intWindow);

         /* Verify if need to show advise of reinit */
         if( (screenWidth != prevWidth) || (screenHeight != prevHeight) ||
             (prevFullScreen != enableFullScreen) || 
             (antiAliasing != prevAntiAliasing) )
         {
            showMessage(gettext("Warning"), 
                  gettext("Some changes will only take effect upon restart."),
                  proj, modl, viewPort);
            /* Disable things */
            glDisable(GL_FOG);
            glDisable(GL_LIGHTING);
         }
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
         screenWidth = prevWidth;
         screenHeight = prevHeight;
         antiAliasing = prevAntiAliasing;
         farViewFactor = prevFarViewFactor;
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
         cxSelGrass->setAvaible(getAvaibleParticles() &&
                                cxSelParticles->isSelected());
      }
   }

   txtLanguage->setText(languageName());
   txtCamera->setText(cameraName());
   txtReflexion->setText(reflexionName());
   txtResolution->setText(resolutionName());
   txtAntiAliasing->setText(antiAliasingName());

   barMusicVolume->defineActualHealth(musicVolume);
   barSndfxVolume->defineActualHealth(sndfxVolume);
   barFarView->defineActualHealth((int)floor(farViewFactor*9));                                                          

   intWindow->draw(0,0);
   return(OPTIONSW_OTHER);
} 

/****************************************************************
 *                             Treat                            *
 ****************************************************************/
int options::getMusicVolume()
{
   return(musicVolume);
}
/****************************************************************
 *                             Treat                            *
 ****************************************************************/
int options::getSndFxVolume()
{
   return(sndfxVolume);
}
/****************************************************************
 *                             Treat                            *
 ****************************************************************/
int options::getLanguage()
{
   return(langNumber);
}
 /****************************************************************
 *                             Treat                            *
 ****************************************************************/     
int options::getCameraNumber()
{
   return(cameraNumber);
}

/****************************************************************
 *                       getAvaibleParticles                    *
 ****************************************************************/
bool options::getAvaibleParticles()
{
   extensions ext;

   return( (ext.PointParameterf != NULL) && 
           (ext.PointParameterfv != NULL) );
}

/****************************************************************
 *                        getEnableParticles                    *
 ****************************************************************/
bool options::getEnableParticles()
{
   enableParticles &= getAvaibleParticles(); 

   return(enableParticles);
}

/****************************************************************
 *                        getEnableGrass                        *
 ****************************************************************/
bool options::getEnableGrass()
{
   return(enableParticles && enableGrass);
}

/****************************************************************
 *                         getReflexionType                     *
 ****************************************************************/
int options::getReflexionType()
{
   return(reflexionType);
}

/****************************************************************
 *                        getScreenWidth                        *
 ****************************************************************/
int options::getScreenWidth()
{
   return(screenWidth);
}

/****************************************************************
 *                        getScreenHeight                       *
 ****************************************************************/
int options::getScreenHeight()
{
   return(screenHeight);
}

/****************************************************************
 *                     getEnableFullScreen                      *
 ****************************************************************/
bool options::getEnableFullScreen()
{
   return(enableFullScreen);
}

/****************************************************************
 *                        getAntiAliasing                       *
 ****************************************************************/
int options::getAntiAliasing()
{
   return(antiAliasing);
}

/****************************************************************
 *                        setAntiAliasing                       *
 ****************************************************************/
void options::setAntiAliasing(int alias)
{
   antiAliasing = alias;
}

/****************************************************************
 *                       getFarViewFactor                       *
 ****************************************************************/
float options::getFarViewFactor()
{
   return(farViewFactor);
}

/****************************************************************
 *                       setFarViewFactor                       *
 ****************************************************************/
void options::setFarViewFactor(float factor)
{
   farViewFactor = factor;
}

/****************************************************************
 *                         Static Members                       *
 ****************************************************************/
int    options::musicVolume = 0;
int    options::sndfxVolume = 0;
int    options::langNumber = 0;
int    options::cameraNumber = 0;
bool   options::enableParticles = false;
bool   options::enableGrass = false;
int    options::reflexionType = 0;
int    options::screenWidth = 800;
int    options::screenHeight = 600; 
bool   options::enableFullScreen = false;
int    options::antiAliasing = 0;
float  options::farViewFactor = 1.0;

string options::fileName = "";

