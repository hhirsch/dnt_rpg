#include "options.h"

#include "../sound/sound.h"
#include "camera.h"
#include "util.h"
#include "../etc/userinfo.h"
#include "../etc/defparser.h"
#include "../etc/dirs.h"
#include "../etc/extensions.h"

#ifdef _MSC_VER
   #include "../config_win.h"
   #include <windows.h>
#else
   #include "../config.h"
   #include <sys/stat.h>
   #include <sys/types.h>
   #include <errno.h>
#endif

#include <iostream>
using namespace std;

#define NM_PORTUGUESE gettext("Portuguese")
#define NM_ENGLISH    gettext("English")
#define NM_FRENCH     gettext("French")
#define NM_GERMAN     gettext("German")
#define NM_SPANISH    gettext("Spanish")
#define NM_ITALIAN    gettext("Italian")
#define NM_RUSSIAN    gettext("Russian")

#define FL_PORTUGUESE "pt_BR"
#define FL_ENGLISH    "en"
#define FL_FRENCH     "fr"
#define FL_GERMAN     "de"
#define FL_ITALIAN    "it"
#define FL_SPANISH    "es"
#define FL_RUSSIAN    "ru"


#define DNT_LANG_RUSSIAN    6 /**< Russian Language */
#define DNT_LANG_ITALIAN    5 /**< Italian Language */
#define DNT_LANG_GERMAN     4 /**< German Language */
#define DNT_LANG_PORTUGUESE 3 /**< Portuguese Language */
#define DNT_LANG_SPANISH    2 /**< Spanish Language */
#define DNT_LANG_FRENCH     1 /**< French Language */
#define DNT_LANG_ENGLISH    0 /**< English Language */

#define DNT_LANG_LAST  5
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
      cerr << "Can't Open the options file: " << file << endl;
      /* Try to Load from Default Installed Data Dir */
      file = DATADIR;
      file += "/";
      file += PACKAGE;
      file += "/dcc.opc";
      if(!load(file))
      {
         cerr << "Can't Open the options file: " << file << endl;
         /* Load from default executable path */
         if(!load("./dcc.opc"))
         {
            cerr << "No Options File Avaible!" << endl;
            return(false);
         }
      }

      fileName = info.getUserHome() + "options.cfg";
      cerr << "Creating Directory: " << info.getUserHome() << ":";
      /* Create the User directory */

      #ifdef _MSC_VER
         if(CreateDirectory(info.getUserHome().c_str(),NULL))
         {
            cerr << "ok" << endl;
         }
         else
         {
            cerr << "failed!" << endl;
         }
         /* Create the saves directories */
         cerr << "Creating Saves Directory... " << endl;
         CreateDirectory(info.getSavesDirectory().c_str(), NULL);
      #else
         mkdir(info.getUserHome().c_str(),0755);
         if(errno != EEXIST)
         {
            cerr << strerror(errno) << endl;
         }
         /* Create the saves directories */
         cerr << "Creating Saves Directory... " << endl;
         mkdir(info.getSavesDirectory().c_str(), 0755);
      #endif

      /* Save the options file */
      cerr << "Creating Options: " << fileName << endl;
      save();
   }
   return(true);
}

/****************************************************************
 *                              Load                            *
 ****************************************************************/
bool options::load(string file)
{
   defParser def;
   string key="", value="";

   reflexionType = REFLEXIONS_NONE;
  
   timeLastOperation = SDL_GetTicks();

   /* Try to load the definitions */
   if(!def.load(file, true))
   {
      return(false);
   }

   /* The first key is mandatory options version */
   def.getNextTuple(key, value);
   if(key != "Version")
   {
      /* FIXME: convert old files! */
      cerr << "DNT Options too old (<0.3), so ignoring it!" << endl;
      return(false);
   }

   /* Now, parse all tuples got */
   while(def.getNextTuple(key, value))
   {
      /**********************************************
       *                 Sound Options              *
       **********************************************/
      if(key == "SoundVolume")
      {
         /* Read Sound Options */
         sscanf(value.c_str(), "%d", &sndfxVolume);
      }
      else if(key == "MusicVolume")
      {
         /* Read Music Options */
         sscanf(value.c_str(), "%d", &musicVolume);
      }

      /**********************************************
       *                 Game Options               *
       **********************************************/
      else if(key == "Language")
      {
         /* Read Language Options */
         sscanf(value.c_str(), "%d", &langNumber);
      }
      else if(key == "Camera")
      {
         /* Read Camera Options */
         sscanf(value.c_str(),"%d",&cameraNumber);
      }
      else if(key == "AutoEndTurn")
      {
         /* Read Auto End Turn options */
         autoEndTurn = (value == "true");
      }
      else if(key == "ShowEnemyCircles")
      {
         /* Read Show Enemy Circles options */
         showEnemyCircles = (value == "true");
      }

      /**********************************************
       *                 Video Options              *
       **********************************************/
      else if(key == "Resolution")
      {
         /* Read the resolution informations */
         sscanf(value.c_str(),"%dx%d", &screenWidth, &screenHeight);
      }
      else if(key == "FullScreen")
      {
         /* Read FullScreen Options */
         enableFullScreen = (value == "true");
      }
      else if(key == "Particles")
      {
         /* Read Particles Options */
         enableParticles = (value == "true");
      }
      else if(key == "Grass")
      {
         /* Read Grass Options */
         enableGrass = (value == "true");
      }
      else if(key == "Reflexions")
      {
         /* Read Reflexions Options */
         sscanf(value.c_str(),"%d", &reflexionType);
      }
      else if(key == "AntiAliasing")
      {
         /* Read Antialiasing Options */
         sscanf(value.c_str(),"%d",&antiAliasing);
      }
      else if(key == "FarViewFactor")
      {
         /* Read FarViewFactor Options */
         sscanf(value.c_str(),"%f",&farViewFactor);
      }
      else if(key == "MultiTexture")
      {
         /* Read Multitexture options */
         enableMultiTexture = (value == "true");
      }
      
      /**********************************************
       *                 Unknow Option              *
       **********************************************/
      else 
      {
         cerr << "Warning: Unknow option: " << key << endl;
      }
   }

   /* Now, set some limits */
   if(farViewFactor > 1.0)
   {
      farViewFactor = 1.0;
   }
   else if(farViewFactor < 0.4)
   {
      farViewFactor = 0.4;
   }

   /* Define the as the used file */
   fileName = file;
  
   /* Done! */
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
      cerr << "Error while writing Options: " << fileName << endl;
      return;
   }

   /* File Version */
   fprintf(arq, "# Mandatory Version Definition\n");
   fprintf(arq, "Version = %s\n", VERSION);

   /**********************************************
    *                 Sound Options              *
    **********************************************/
   fprintf(arq, "# Sound Options\n");
   /* SoundVolume */
   fprintf(arq,"SoundVolume = %d\n", sndfxVolume);
   /* MusicVolume */
   fprintf(arq,"MusicVolume = %d\n", musicVolume);

   /**********************************************
    *                 Game Options               *
    **********************************************/
   fprintf(arq, "# Game Preferences\n");
   /* Language */
   fprintf(arq,"Language = %d\n",langNumber);
   /* Camera */
   fprintf(arq,"Camera = %d\n", cameraNumber);
   /* AutoEndTurn */
   fprintf(arq, "AutoEndTurn = %s\n", autoEndTurn?"true":"false");
   /* Show Enemy Circles */
   fprintf(arq, "ShowEnemyCircles = %s\n", showEnemyCircles?"true":"false");

   /**********************************************
    *                Video Options               *
    **********************************************/
   fprintf(arq, "# Video Preferences\n");
   /* Resolution */
   fprintf(arq, "Resolution = %dx%d\n",screenWidth, screenHeight);
   /* Fullscreen */
   fprintf(arq, "FullScreen = %s\n",enableFullScreen?"true":"false");
   /* Particles */
   fprintf(arq, "Particles = %s\n",enableParticles?"true":"false");
   /* Grass */
   fprintf(arq, "Grass = %s\n",enableGrass?"true":"false");
   /* Reflexion */
   fprintf(arq, "Reflexions = %d\n", reflexionType);
   /* AntiAliasing */
   fprintf(arq, "AntiAliasing = %d\n",antiAliasing);
   /* FarViewFactor */
   fprintf(arq, "FarViewFactor = %.2f\n", farViewFactor);  
   /* Multi Texture */
   fprintf(arq, "MultiTexture = %s\n",enableMultiTexture?"true":"false");

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
      case DNT_LANG_ITALIAN:
      {
         saux = NM_ITALIAN;
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
      case DNT_LANG_GERMAN:
      {
         saux = NM_GERMAN;
         break;
      }
      case DNT_LANG_ENGLISH:
      default:
      {
         saux = NM_ENGLISH;
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
      case DNT_LANG_ITALIAN:
      {
         saux = FL_ITALIAN;
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
      case DNT_LANG_GERMAN:
      {
         saux = FL_GERMAN;
         break;
      }
      case DNT_LANG_ENGLISH:
      default:
      {
         saux = FL_ENGLISH;
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
   extensions ext;
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

   intWindow = interf->insertWindow(xPos-128,yPos-192,xPos+128,yPos+192,
                                    gettext("Options"));

   /* Insert the Objects TabBox */
   tabBox* tb = intWindow->defineTabBox(8,27,248,352);
   guiList* list;
   int posY;

   /************************************************
    *                Audio Options                 *
    ************************************************/
   list = tb->insertOption(gettext("Audio"));

   posY = 52;
   /* Music Things */
   qt = list->insertTextBox(12,posY,145,posY+17,0, gettext("Music Volume:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonMusDec = list->insertButton(121,posY,131,posY+17,
                                     fnt.createUnicode(0x25C4),0);
   buttonMusDec->defineFont(DNT_FONT_ARIAL, 9);
   barMusicVolume = list->insertHealthBar(133,posY,196,posY+17,255);
   barMusicVolume->defineActualHealth(musicVolume);
   buttonMusSum = list->insertButton(198,posY,208,posY+17,
                                     fnt.createUnicode(0x25BA),0);
   buttonMusSum->defineFont(DNT_FONT_ARIAL, 9);
   list->insertPicture(220,posY,40,112,
                       dir.getRealFile("texturas/options/music.png").c_str());
   posY += 25;

   /* Sound Effects Things */
   sprintf(tmp,"%d",sndfxVolume);
   saux = tmp;
   qt = list->insertTextBox(12,posY,145,posY+17,0, gettext("Effects Volume:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonSndDec = list->insertButton(121,posY,131,posY+17,
                                     fnt.createUnicode(0x25C4),0);
   buttonSndDec->defineFont(DNT_FONT_ARIAL, 9);
   barSndfxVolume = list->insertHealthBar(133,posY,196,posY+17,255);
   barSndfxVolume->defineActualHealth(sndfxVolume);
   buttonSndSum = list->insertButton(198,posY,208,posY+17,
                                     fnt.createUnicode(0x25BA),0);
   buttonSndSum->defineFont(DNT_FONT_ARIAL, 9);
   list->insertPicture(220,posY,40,112,
                       dir.getRealFile("texturas/options/sndfx.png").c_str());


   /************************************************
    *                Video Options                 *
    ************************************************/
   list = tb->insertOption(gettext("Video"));
   
   posY = 52;
   /* Resolution */
   prevHeight = screenHeight;
   prevWidth = screenWidth;
   saux = resolutionName();
   qt = list->insertTextBox(12,posY,145,posY+17,0,
                            gettext("Screen Resolution:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonResDec = list->insertButton(121,posY,131,posY+17,
                                     fnt.createUnicode(0x25C4),0);
   buttonResDec->defineFont(DNT_FONT_ARIAL, 9);
   txtResolution = list->insertTextBox(132,posY,197,posY+17,1,saux.c_str());
   txtResolution->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonResSum = list->insertButton(198,posY,208,posY+17,
                                     fnt.createUnicode(0x25BA),0);
   buttonResSum->defineFont(DNT_FONT_ARIAL, 9);
   list->insertPicture(220,posY,40,220,
                    dir.getRealFile("texturas/options/resolution.png").c_str());
   posY += 35;
  
   /* Fullscreen */
   qt = list->insertTextBox(24,posY,219,posY+17,0,gettext("Enable FullScreen"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelFullScreen = list->insertCxSel(12,posY+4,enableFullScreen);
   list->insertPicture(220,posY,40,220,
                    dir.getRealFile("texturas/options/fullscreen.png").c_str());
   posY += 25;

   /* Grass Enabled or Not */
   qt = list->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Enable Grass Effects (need particles)"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelGrass = list->insertCxSel(12,posY+4, enableGrass);
   list->insertPicture(220,posY,40,112,
                       dir.getRealFile("texturas/options/grass.png").c_str());
   posY += 25;

   /* Particle System Enabled or Not */
   qt = list->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Enable Particles Effects"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelParticles = list->insertCxSel(12, posY+4, enableParticles);
   list->insertPicture(220,posY,40,112,
                     dir.getRealFile("texturas/options/particles.png").c_str());
   cxSelParticles->setAvaible(getAvaibleParticles());
   cxSelGrass->setAvaible(getAvaibleParticles() && 
                          cxSelParticles->isSelected());
   posY += 25;

   /* MultiTexture Enable or Not */
   qt = list->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Enable MultiTextures"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelMultiTexture = list->insertCxSel(12,posY+4,enableMultiTexture);
   list->insertPicture(220,posY,40,112,
                  dir.getRealFile("texturas/options/multitexture.png").c_str());
   cxSelMultiTexture->setAvaible(ext.hasMultiTexture());
   posY += 35;

   /* Reflexions */
   prevReflexion = reflexionType;
   saux = reflexionName();
   qt = list->insertTextBox(12,posY,145,posY+17,0,gettext("Reflections:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonReflDec = list->insertButton(121,posY,131,posY+17,
                                      fnt.createUnicode(0x25C4),0);
   buttonReflDec->defineFont(DNT_FONT_ARIAL, 9);
   txtReflexion = list->insertTextBox(132,posY,197,posY+17,1,saux.c_str());
   txtReflexion->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonReflSum = list->insertButton(198,posY,208,posY+17,
                                      fnt.createUnicode(0x25BA),0);
   buttonReflSum->defineFont(DNT_FONT_ARIAL, 9);
   list->insertPicture(220,posY,40,220,
                    dir.getRealFile("texturas/options/reflexions.png").c_str());
   posY += 25;

   /* AntiAliasing */                 
   saux = antiAliasingName();
   qt = list->insertTextBox(12,posY,145,posY+17,0,gettext("Anti-Aliasing:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonAliasDec = list->insertButton(121,posY,131,posY+17,
                                       fnt.createUnicode(0x25C4),0);
   buttonAliasDec->defineFont(DNT_FONT_ARIAL, 9);
   txtAntiAliasing = list->insertTextBox(132,posY,197,posY+17,1,saux.c_str());
   txtAntiAliasing->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonAliasSum = list->insertButton(198,posY,208,posY+17,
                                       fnt.createUnicode(0x25BA),0);
   buttonAliasSum->defineFont(DNT_FONT_ARIAL, 9);
   list->insertPicture(220,posY,40,220,
                  dir.getRealFile("texturas/options/antialiasing.png").c_str());
   posY += 25;
 
   /* FarViewFactor */
   qt = list->insertTextBox(12,posY,145,posY+17,0, gettext("FarView:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonFarViewDec = list->insertButton(121,posY,131,posY+17,
                                         fnt.createUnicode(0x25C4),0);
   buttonFarViewDec->defineFont(DNT_FONT_ARIAL, 9);
   barFarView = list->insertHealthBar(133,posY,196,posY+17,9);
   barFarView->defineActualHealth((int)floor(farViewFactor*9));                                                          
   buttonFarViewSum = list->insertButton(198,posY,208,posY+17,
                                         fnt.createUnicode(0x25BA),0);
   buttonFarViewSum->defineFont(DNT_FONT_ARIAL, 9);
   list->insertPicture(220,posY,40,223,
                     dir.getRealFile("texturas/options/farview.png").c_str());

   /************************************************
    *                 Game Options                 *
    ************************************************/
   list = tb->insertOption(gettext("Game"));

   posY = 52;
   /* Language Things */
   prevLanguage = langNumber;
   saux = languageName();
   qt = list->insertTextBox(12,posY,145,posY+17,0,gettext("Language:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonLangDec = list->insertButton(121,posY,131,posY+17,
                                      fnt.createUnicode(0x25C4),0);
   buttonLangDec->defineFont(DNT_FONT_ARIAL, 9);
   txtLanguage = list->insertTextBox(132,posY,197,posY+17,1,saux.c_str());
   txtLanguage->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonLangSum = list->insertButton(198,posY,208,posY+17,
                                      fnt.createUnicode(0x25BA),0);
   buttonLangSum->defineFont(DNT_FONT_ARIAL, 9);
   list->insertPicture(220,posY,40,112,
                      dir.getRealFile("texturas/options/language.png").c_str());
   posY += 25;                      

   /* Camera Mode Things */
   prevCamera = cameraNumber;
   saux = cameraName();
   qt = list->insertTextBox(12,posY,145,posY+17,0,gettext("Camera Mode:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonCamDec = list->insertButton(121,posY,131,posY+17,
                                     fnt.createUnicode(0x25C4),0);
   buttonCamDec->defineFont(DNT_FONT_ARIAL, 9);
   txtCamera = list->insertTextBox(132,posY,197,posY+17,1,saux.c_str());
   txtCamera->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonCamSum = list->insertButton(198,posY,208,posY+17,
                                     fnt.createUnicode(0x25BA),0);
   buttonCamSum->defineFont(DNT_FONT_ARIAL, 9);
   list->insertPicture(220,posY,40,posY+17,
                       dir.getRealFile("texturas/options/camera.png").c_str());
   posY += 35;

   /* AutoEndTurn Enable or Not */
   qt = list->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Auto End Turn"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   cxSelAutoEndTurn = list->insertCxSel(12,posY+4,autoEndTurn);
   list->insertPicture(220,posY,40,112,
                  dir.getRealFile("texturas/options/autoendturn.png").c_str());
   posY += 25;

   /* ShowEnemyCircles or or Not */
   qt = list->insertTextBox(24,posY,219,posY+34,0,
                            gettext("Show Enemy Battle Circle"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   cxSelShowEnemyCircles = list->insertCxSel(12,posY+4,showEnemyCircles);
   list->insertPicture(220,posY,40,112,
              dir.getRealFile("texturas/options/showenemycircles.png").c_str());
   posY += 25;


   /************************************************
    *                 Action Buttons               *
    ************************************************/
   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(177,356,247,375,
                                              gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(8,356,78,375,
                                              gettext("Cancel"),1);
   
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
         if(farViewFactor > 0.4)
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
         enableMultiTexture = cxSelMultiTexture->isSelected();
         autoEndTurn = cxSelAutoEndTurn->isSelected();
         showEnemyCircles = cxSelShowEnemyCircles->isSelected();

         bool prevFullScreen = enableFullScreen;
         enableFullScreen = cxSelFullScreen->isSelected();
         save();
         interf->closeWindow(intWindow);

         /* Verify if need to show advise of reinit */
         if( (screenWidth != prevWidth) || (screenHeight != prevHeight) ||
             (prevFullScreen != enableFullScreen) || 
             (antiAliasing != prevAntiAliasing) ||
             (langNumber != prevLanguage) )
         {
            warning warn;
            warn.show(gettext("Warning"), 
                    gettext("Some changes will only take effect upon restart."),
                      interf);
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

   return( ext.hasPointTexture() );
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
 *                     getEnableMultiTexture                    *
 ****************************************************************/
bool options::getEnableMultiTexture()
{
   extensions ext;
   return(enableMultiTexture && ext.hasMultiTexture());
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
 *                       getAutoEndTurn                         *
 ****************************************************************/
bool options::getAutoEndTurn()
{
   return(autoEndTurn);
}

/****************************************************************
 *                     getShowEnemyCircles                      *
 ****************************************************************/
bool options::getShowEnemyCircles()
{
   return(showEnemyCircles);
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
int    options::musicVolume = 90;
int    options::sndfxVolume = 128;
int    options::langNumber = 0;
int    options::cameraNumber = 1;
bool   options::enableParticles = true;
bool   options::enableGrass = true;
int    options::reflexionType = 2;
int    options::screenWidth = 1024;
int    options::screenHeight = 768; 
bool   options::enableFullScreen = false;
int    options::antiAliasing = 4;
float  options::farViewFactor = 1.0;
bool   options::enableMultiTexture = true;
bool   options::autoEndTurn = true;
bool   options::showEnemyCircles = false;

string options::fileName = "";

