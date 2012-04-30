/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "options.h"
#include "../lang/translate.h"
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

#ifdef __APPLE__
   #include "../config_mac.h"
   #include <sys/stat.h>
   #include <sys/types.h>
   #include <errno.h>
#else
   #include "../config.h"
   #include <sys/stat.h>
   #include <sys/types.h>
   #include <errno.h>
#endif
#endif

#include <iostream>
using namespace std;

/*! The language names */
string dntLanguageName[] =
{
   gettext("English"),
   gettext("French"),  
   gettext("Spanish"),
   gettext("Portuguese"),
   gettext("German"),
   gettext("Italian"),
   gettext("Dutch"),
   gettext("Catalan"),
   gettext("Occitan"),
   gettext("Russian"),
   gettext("Japanese"),
   gettext("Polish")
};

/*! The language code  */
string dntLanguageCode[] =
{
   "en",
   "fr",
   "es",
   "pt_BR",
   "de",
   "it",
   "nl",
   "ca",
   "oc",
   "ru",
   "ja",
   "pl"
};

/*! The language numbers */
enum 
{
   DNT_LANG_ENGLISH,
   DNT_LANG_FRENCH,
   DNT_LANG_SPANISH,
   DNT_LANG_PORTUGUESE,
   DNT_LANG_GERMAN,
   DNT_LANG_ITALIAN,
   DNT_LANG_DUTCH,
   DNT_LANG_CATALAN,
   DNT_LANG_OCCITAN,
   DNT_LANG_RUSSIAN,
   DNT_LANG_JAPANESE,
   DNT_LANG_POLISH,
   DNT_LANG_LAST
};

/*! Token for keys at option's file */
string dntKeyToken[] =
{
   /* Camera Keys */
   "cameraRotateLeft",
   "cameraRotateRight",
   "cameraRotateUp",
   "cameraRotateDown",
   "cameraZoomUp",
   "cameraZoomDown",
   "cameraTop",
   "cameraBottom",
   "cameraMaxZoom",
   "cameraMinZoom",

   /* Character move keys */
   "toggleRun1",
   "toggleRun2",
   "moveRight",
   "moveLeft",
   "moveForward",
   "moveBackward",
   "rotateLeft",
   "rotateRight",
   "changeCharacter",
   "rest",

   /* Battle Keys */
   "battleTurn",

   /* Shortcuts keys */
   "miniMapWindow",
   "shortcutsWindow",
   "briefingWindow",
   "inventoryWindow",
   "characterWindow",
   "journalWindow"
};

/*! the keys descriptions */
string dntKeyDesc[] = 
{
   /* Camera Keys */
   gettext("Camera Rotate Left"),
   gettext("Camera Rotate Right"),
   gettext("Camera Rotate Up"),
   gettext("Camera Rotate Down"),
   gettext("Camera Zoom Up"),
   gettext("Camera Zoom Down"),
   gettext("Camera Top"),
   gettext("Camera Bottom"),
   gettext("Camera Zoom Max"),
   gettext("Camera Zoom Min"),
   /* Character Move */
   gettext("Toggle Run(1)"),
   gettext("Toggle Run(2)"),
   gettext("Move Right"),
   gettext("Move Left"),
   gettext("Move Forward"),
   gettext("Move Backward"),
   gettext("Rotate Left"),
   gettext("Rotate Right"),
   gettext("Change Character"),
   gettext("Rest"),
   /* Battle Keys */
   gettext("End Turn"),
   /* Shortcuts Keys */
   gettext("Minimap"),
   gettext("Shortcuts"),
   gettext("Briefing"),
   gettext("Inventory"),
   gettext("Character Info"),
   gettext("Journal")
};

/****************************************************************
 *                          Constructor                         *
 ****************************************************************/
options::options()
{
   timeLastOperation = SDL_GetTicks();
   intWindow = NULL;
}

/****************************************************************
 *                             Destructor                       *
 ****************************************************************/
options::~options()
{
}

/****************************************************************
 *                   getAvailableResolutions                    *
 ****************************************************************/
void options::getAvailableResolutions()
{
   /* Get available fullscreen/hardware modes */
#if defined (__amigaos4__)
   resolutions = SDL_ListModes(NULL,SDL_FULLSCREEN|SDL_HWSURFACE);
#else
   resolutions=SDL_ListModes(NULL,SDL_FULLSCREEN | SDL_DOUBLEBUF | SDL_OPENGL);
#endif

   if(resolutions == (SDL_Rect**)0)
   {
      cerr << "Couldn't get available screen resolutions!" << endl;
      resolutions = NULL;
   }
   else if(resolutions == (SDL_Rect**)1)
   {
      cerr << "Got all resolutions available." << endl;
      resolutions = NULL;
   }
}

/****************************************************************
 *                              Load                            *
 ****************************************************************/
bool options::load()
{
   string file;
   userInfo info;

   /* First define default keys */
   defaultKeys();
   
   /* Define default save file */
   fileName = info.getUserHome() + "options.cfg";
   
   /* Try to Load From Users Directory */
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
         }
      }

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
   int i;
   bool got;
   defParser def;
   string key="", value="";

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
      else if(key == "HighlightEnemy")
      {
         highlightEnemy = (value == "true");
      }
      else if(key == "AlwaysRun")
      {
         /* If will always run or not */
         alwaysRun = (value == "true");
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
      else if(key == "StencilBufferSize")
      {
         /* Read Stencil buffer size Option */
         sscanf(value.c_str(), "%d", &stencilBufferSize);
      }
      else if(key == "Reflexions")
      {
         /* Read Reflexions Options */
         sscanf(value.c_str(),"%d", &reflexionType);
      }
      else if(key == "Shadows")
      {
         /* Read Reflexions Options */
         sscanf(value.c_str(),"%d", &shadowType);
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
      else if(key == "SplattingType")
      {
         /* Read Splatting option */
         sscanf(value.c_str(), "%d", &splattingType);
      }
      else if(key == "Anisotropic")
      {
         /* Read Anisotropic option */
         enableAnisotropic = (value == "true");
      }
      else if(key == "BumpMapping")
      {
         /* Read BumpMapping option */
         enableBumpMapping = (value == "true");
      }

      else
      {
         got = false;
         for(i=0; ((i < DNT_TOTAL_KEYS) && (!got)); i++)
         {
            if(key == dntKeyToken[i])
            {
               sscanf(value.c_str(), "%d", &keys[i]);
               got = true;
            }
         }

         /**********************************************
          *                 Unknow Option              *
          **********************************************/
         if(!got)
         {
            cerr << "Warning: Unknow option: " << key << endl;
         }
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
   /* HighLight Enemy */
   fprintf(arq, "HighlightEnemy = %s\n", highlightEnemy?"true":"false");
   /* Always Run */
   fprintf(arq, "AlwaysRun = %s\n", alwaysRun?"true":"false");

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
   /* Stencil Buffer */
   fprintf(arq, "StencilBufferSize = %d\n", stencilBufferSize);
   /* Reflexion */
   fprintf(arq, "Reflexions = %d\n", reflexionType);
   /* Shadow */
   fprintf(arq, "Shadows = %d\n", shadowType);
   /* AntiAliasing */
   fprintf(arq, "AntiAliasing = %d\n",antiAliasing);
   /* FarViewFactor */
   fprintf(arq, "FarViewFactor = %.2f\n", farViewFactor);  
   /* Splatting Type */
   fprintf(arq, "SplattingType = %d\n", splattingType);
   /* Anisotropic Filter */
   fprintf(arq, "Anisotropic = %s\n",enableAnisotropic?"true":"false");
   /* Bump Mapping */
   fprintf(arq, "BumpMapping = %s\n",enableBumpMapping?"true":"false");

   /**********************************************
    *                 Keys Options               *
    **********************************************/
   fprintf(arq, "# Keyboard Preferences\n");
   int i;
   for(i=0; i < DNT_TOTAL_KEYS; i++)
   {
      fprintf(arq, "%s = %d\n",dntKeyToken[i].c_str(), keys[i]);
   }

   fclose(arq);
}

/****************************************************************
 *                         languageName                         *
 ****************************************************************/
string options::languageName()
{
   if( (langNumber >= 0) && (langNumber < DNT_LANG_LAST) )
   {
      return(gettext(dntLanguageName[langNumber].c_str()));
   }
   return(gettext(dntLanguageName[DNT_LANG_ENGLISH].c_str()));
}

/****************************************************************
 *                      isLanguageUnicode                       *
 ****************************************************************/
bool options::isLanguageUnicode()
{
   return( (langNumber == DNT_LANG_RUSSIAN) || 
           (langNumber == DNT_LANG_JAPANESE) );
}

/****************************************************************
 *                         setLanguage                          *
 ****************************************************************/
void options::setLanguage()
{
   string saux = dntLanguageCode[DNT_LANG_ENGLISH];
   
   if( (langNumber >= 0) && (langNumber < DNT_LANG_LAST) )
   {
      saux = dntLanguageCode[langNumber];
   }

#if ENABLE_NLS == 1
   if(isLanguageUnicode())
   {
      bind_textdomain_codeset(PACKAGE, "UTF-8");
   }
   else
   {
      bind_textdomain_codeset(PACKAGE, "ISO-8859-1");
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
#endif

   Farso::Options farOpt;
   farOpt.setUnicodeLanguage(isLanguageUnicode());
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
 *                          shadowName                          *
 ****************************************************************/
string options::shadowName()
{
   string saux;
   switch(shadowType)
   {
      case SHADOWS_NONE:
      {
         saux = gettext("None");
         break;
      }
      case SHADOWS_PROJECTIVE:
      {
         saux = gettext("Projective");
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
 *                    stencilBufferSizeName                     *
 ****************************************************************/
string options::stencilBufferSizeName()
{
   string saux="";
   char buf[8];
   switch(stencilBufferSize)
   {
      case 0:
         saux = gettext("Disabled");
      break;
      default:
         sprintf(buf, "%d", stencilBufferSize);
         saux = buf;
      break;
   }
   return(saux);
}

/****************************************************************
 *                       splattingTypeName                      *
 ****************************************************************/
string options::splattingTypeName()
{
   string saux = "!!!";

   switch(splattingType)
   {
      case DNT_SPLATTING_NONE:
         saux = gettext("Disabled");
      break;
      case DNT_SPLATTING_EXTENSION:
         saux = "glMultiTextureARB";
      break;
      case DNT_SPLATTING_SHADER:
         saux = "GLSL Shader";
      break;
   }

   return(saux);
}

/****************************************************************
 *                       AntiAliasingName                       *
 ****************************************************************/
string options::antiAliasingName()
{
   string saux="";
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

   if(resolutions != NULL)
   {
      for(i = 0; resolutions[i]; i++)
      {
         if( (resolutions[i]->w == screenWidth) &&
             (resolutions[i]->h == screenHeight) )
         {
            resPosition = i;
         }
      }
   }
   return(saux);
}

/****************************************************************
 *                          insertKeys                          *
 ****************************************************************/
void options::insertKeys(int firstKey, int lastKey, Farso::GuiList* gList)
{
   int posY = 77;
   int i;
   Farso::TextBox* tb;

   if( (firstKey < 0) || (firstKey >= DNT_TOTAL_KEYS) ||
       (lastKey < 0) || (lastKey >= DNT_TOTAL_KEYS) )
   {
      return;
   }

   for(i = firstKey; i <= lastKey; i++)
   {
      tb = gList->insertTextBox(14,posY,185,posY+17,0, 
                               gettext(dntKeyDesc[i].c_str()));
      buttonKeys[i] = gList->insertButton(188,posY,262,posY+17,
                                         SDL_GetKeyName((SDLKey)keys[i]),true);
      if(i % 2 != 0)
      {
         tb->setColor(10, 200, 10);
         buttonKeys[i]->setTextColor(10, 200, 10);
      }

      posY += 18;
   }
}

/****************************************************************
 *                    Open Options Screen                       *
 ****************************************************************/
void options::displayOptionsScreen(Farso::GuiInterface* interf)
{
   dirs dir;
   std::string fontArial = dir.getRealFile(DNT_FONT_ARIAL);
   extensions ext;
   Farso::Font fnt;
   fnt.defineFont(fontArial, 10);
   char tmp[8];
   string saux;
   Farso::TextBox* qt;

   getAvailableResolutions();

   keyWindow = NULL;

   /* Define Previous Values */
   prevMusicVolume = musicVolume;
   prevSndfxVolume = sndfxVolume;

   prevAntiAliasing = antiAliasing;
   prevStencilBufferSize = stencilBufferSize;
   prevSplattingType = splattingType;
   prevFarViewFactor = farViewFactor;

   /* Previous Keys values too */
   int i;
   for(i=0; i < DNT_TOTAL_KEYS; i++)
   {
      prevKeys[i] = keys[i];
   }

   int xPos = (int)(Farso::SCREEN_X / 2.0);
   int yPos = (int)(Farso::SCREEN_Y / 2.0);

   intWindow = interf->insertWindow(xPos-138,yPos-192,xPos+138,yPos+192,
                                    gettext("Options"));

   /* Insert the Objects TabBox */
   Farso::TabBox* tb=(Farso::TabBox*)intWindow->getObjectsList()->defineTabBox(
         8,27,268,352);
   Farso::GuiList* gList;
   int posY;

   /************************************************
    *                Input Options                 *
    ************************************************/
   gList = tb->insertOption(gettext("Input"));
   Farso::TabBox* inputTab = (Farso::TabBox*)gList->defineTabBox(10,52,266,350);
   insertKeys(DNT_KEY_FIRST_WINDOW, DNT_KEY_LAST_WINDOW,
              inputTab->insertOption(gettext("Windows")));

   insertKeys(DNT_KEY_FIRST_CHARACTER, DNT_KEY_LAST_CHARACTER,
              inputTab->insertOption(gettext("Character")));

   insertKeys(DNT_KEY_FIRST_CAMERA, DNT_KEY_LAST_CAMERA,
              inputTab->insertOption(gettext("Camera")));

   /************************************************
    *                Audio Options                 *
    ************************************************/
   gList = tb->insertOption(gettext("Audio"));

   posY = 52;
   /* Music Things */
   qt = gList->insertTextBox(12,posY,145,posY+17,0, gettext("Music Volume:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonMusDec = gList->insertButton(121,posY,131,posY+17,
                                     fnt.createUnicode(0x25C4),0);
   buttonMusDec->defineFont(fontArial, 9);
   barMusicVolume = gList->insertHealthBar(133,posY,196,posY+17,255);
   barMusicVolume->defineActualHealth(musicVolume);
   buttonMusSum = gList->insertButton(198,posY,208,posY+17,
                                     fnt.createUnicode(0x25BA),0);
   buttonMusSum->defineFont(fontArial, 9);
   gList->insertPicture(220,posY,40,112,
                       dir.getRealFile("texturas/options/music.png").c_str());
   posY += 25;

   /* Sound Effects Things */
   sprintf(tmp,"%d",sndfxVolume);
   saux = tmp;
   qt = gList->insertTextBox(12,posY,145,posY+17,0, gettext("Effects Volume:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonSndDec = gList->insertButton(121,posY,131,posY+17,
                                     fnt.createUnicode(0x25C4),0);
   buttonSndDec->defineFont(fontArial, 9);
   barSndfxVolume = gList->insertHealthBar(133,posY,196,posY+17,255);
   barSndfxVolume->defineActualHealth(sndfxVolume);
   buttonSndSum = gList->insertButton(198,posY,208,posY+17,
                                     fnt.createUnicode(0x25BA),0);
   buttonSndSum->defineFont(fontArial, 9);
   gList->insertPicture(220,posY,40,112,
                       dir.getRealFile("texturas/options/sndfx.png").c_str());


   /************************************************
    *                Video Options                 *
    ************************************************/
   gList = tb->insertOption(gettext("Video"));
   
   posY = 52;
   /* Resolution */
   prevHeight = screenHeight;
   prevWidth = screenWidth;
   saux = resolutionName();
   qt = gList->insertTextBox(12,posY,145,posY+17,0,
                            gettext("Screen Resolution:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonResDec = gList->insertButton(121,posY,131,posY+17,
                                     fnt.createUnicode(0x25C4),0);
   buttonResDec->defineFont(fontArial, 9);
   txtResolution = gList->insertTextBox(132,posY,197,posY+17,1,saux.c_str());
   txtResolution->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   buttonResSum = gList->insertButton(198,posY,208,posY+17,
                                     fnt.createUnicode(0x25BA),0);
   buttonResSum->defineFont(fontArial, 9);
   gList->insertPicture(220,posY,40,220,
                    dir.getRealFile("texturas/options/resolution.png").c_str());
   posY += 25;
  
   /* Fullscreen */
   qt =gList->insertTextBox(24,posY,219,posY+17,0,gettext("Enable FullScreen"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   cxSelFullScreen = gList->insertCxSel(12,posY+4,enableFullScreen);
   gList->insertPicture(220,posY,40,220,
                    dir.getRealFile("texturas/options/fullscreen.png").c_str());
   posY += 25;

   /* Grass Enabled or Not */
   qt = gList->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Enable Grass Effects (need particles)"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   cxSelGrass = gList->insertCxSel(12,posY+4, enableGrass);
   gList->insertPicture(220,posY,40,112,
                       dir.getRealFile("texturas/options/grass.png").c_str());
   posY += 25;

   /* Particle System Enabled or Not */
   qt = gList->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Enable Particles Effects"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   cxSelParticles = gList->insertCxSel(12, posY+4, enableParticles);
   gList->insertPicture(220,posY,40,112,
                     dir.getRealFile("texturas/options/particles.png").c_str());
   cxSelParticles->setAvailable(getAvaibleParticles());
   cxSelGrass->setAvailable(getAvaibleParticles() && 
                          cxSelParticles->isSelected());
   posY += 25;

   /* Anisotropic Enabled or Not */
   qt = gList->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Enable Anisotropic Filter"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   cxSelAnisotropic = gList->insertCxSel(12,posY+4,
         getEnableAnisotropicFilter());
   gList->insertPicture(220,posY,40,112,
                   dir.getRealFile("texturas/options/anisotropic.png").c_str());
   cxSelAnisotropic->setAvailable(ext.hasAnisotropic());
   posY += 25;

   /* BumpMapping Enabled or Not */
   qt = gList->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Enable BumpMapping"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   cxSelBumpMapping = gList->insertCxSel(12,posY+4,
         getEnableBumpMapping());
   gList->insertPicture(220,posY,40,112,
         dir.getRealFile("texturas/options/bumpmapping.png").c_str());
   cxSelBumpMapping->setAvailable(ext.hasShader());
   posY += 25;



   /* Splatting Type */
   prevSplattingType = splattingType;
   saux = splattingTypeName();
   qt = gList->insertTextBox(12,posY,145,posY+17,0,gettext("Splatting:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonSplattingDec = gList->insertButton(121,posY,131,posY+17,
                                      fnt.createUnicode(0x25C4),0);
   buttonSplattingDec->defineFont(fontArial, 9);
   txtSplatting = gList->insertTextBox(132,posY,227,posY+17,1,saux.c_str());
   txtSplatting->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   buttonSplattingSum = gList->insertButton(228,posY,238,posY+17,
                                      fnt.createUnicode(0x25BA),0);
   buttonSplattingSum->defineFont(fontArial, 9);
   gList->insertPicture(245,posY,40,220,
                  dir.getRealFile("texturas/options/multitexture.png").c_str());
   posY += 25;

   /* Stncil Buffer Size */
   prevStencilBufferSize = stencilBufferSize;
   saux = stencilBufferSizeName();
   qt = gList->insertTextBox(12,posY,145,posY+17,0,gettext("Stencil Buffer:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonStencilDec = gList->insertButton(121,posY,131,posY+17,
                                      fnt.createUnicode(0x25C4),0);
   buttonStencilDec->defineFont(fontArial, 9);
   txtStencil = gList->insertTextBox(132,posY,227,posY+17,1,saux.c_str());
   txtStencil->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   buttonStencilSum = gList->insertButton(228,posY,238,posY+17,
                                      fnt.createUnicode(0x25BA),0);
   buttonStencilSum->defineFont(fontArial, 9);
   gList->insertPicture(245,posY,40,220,
                  dir.getRealFile("texturas/options/stencil_size.png").c_str());
   posY += 25;

   /* Reflexions */
   prevReflexion = reflexionType;
   saux = reflexionName();
   qt = gList->insertTextBox(12,posY,145,posY+17,0,gettext("Reflections:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonReflDec = gList->insertButton(121,posY,131,posY+17,
                                      fnt.createUnicode(0x25C4),0);
   buttonReflDec->defineFont(fontArial, 9);
   txtReflexion = gList->insertTextBox(132,posY,227,posY+17,1,saux.c_str());
   txtReflexion->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   buttonReflSum = gList->insertButton(228,posY,238,posY+17,
                                      fnt.createUnicode(0x25BA),0);
   buttonReflSum->defineFont(fontArial, 9);
   gList->insertPicture(245,posY,40,220,
                    dir.getRealFile("texturas/options/reflexions.png").c_str());
   posY += 25;

   /* Shadows */
   prevShadow = shadowType;
   saux = shadowName();
   qt = gList->insertTextBox(12,posY,145,posY+17,0,gettext("Shadows:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonShadDec = gList->insertButton(121,posY,131,posY+17,
                                      fnt.createUnicode(0x25C4),0);
   buttonShadDec->defineFont(fontArial, 9);
   txtShadow = gList->insertTextBox(132,posY,227,posY+17,1,saux.c_str());
   txtShadow->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   buttonShadSum = gList->insertButton(228,posY,238,posY+17,
                                      fnt.createUnicode(0x25BA),0);
   buttonShadSum->defineFont(fontArial, 9);
   gList->insertPicture(245,posY,40,220,
                    dir.getRealFile("texturas/options/shadow.png").c_str());
   posY += 25;

   /* AntiAliasing */                 
   saux = antiAliasingName();
   qt = gList->insertTextBox(12,posY,145,posY+17,0,gettext("Anti-Aliasing:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonAliasDec = gList->insertButton(121,posY,131,posY+17,
                                       fnt.createUnicode(0x25C4),0);
   buttonAliasDec->defineFont(fontArial, 9);
   txtAntiAliasing = gList->insertTextBox(132,posY,227,posY+17,1,saux.c_str());
   txtAntiAliasing->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   buttonAliasSum = gList->insertButton(228,posY,238,posY+17,
                                       fnt.createUnicode(0x25BA),0);
   buttonAliasSum->defineFont(fontArial, 9);
   gList->insertPicture(245,posY,40,220,
                  dir.getRealFile("texturas/options/antialiasing.png").c_str());
   posY += 25;
 
   /* FarViewFactor */
   qt = gList->insertTextBox(12,posY,145,posY+17,0, gettext("FarView:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonFarViewDec = gList->insertButton(121,posY,131,posY+17,
                                         fnt.createUnicode(0x25C4),0);
   buttonFarViewDec->defineFont(fontArial, 9);
   barFarView = gList->insertHealthBar(133,posY,226,posY+17,9);
   barFarView->defineActualHealth((int)floor(farViewFactor*9));                                                          
   buttonFarViewSum = gList->insertButton(228,posY,238,posY+17,
                                         fnt.createUnicode(0x25BA),0);
   buttonFarViewSum->defineFont(fontArial, 9);
   gList->insertPicture(245,posY,40,223,
                     dir.getRealFile("texturas/options/farview.png").c_str());

   /************************************************
    *                 Game Options                 *
    ************************************************/
   gList = tb->insertOption(gettext("Game"));

   posY = 52;
   /* Language Things */
   prevLanguage = langNumber;
   saux = languageName();
   qt = gList->insertTextBox(12,posY,145,posY+17,0,gettext("Language:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonLangDec = gList->insertButton(121,posY,131,posY+17,
                                      fnt.createUnicode(0x25C4),0);
   buttonLangDec->defineFont(fontArial, 9);
   txtLanguage = gList->insertTextBox(132,posY,197,posY+17,1,saux.c_str());
   txtLanguage->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   buttonLangSum = gList->insertButton(198,posY,208,posY+17,
                                      fnt.createUnicode(0x25BA),0);
   buttonLangSum->defineFont(fontArial, 9);
   gList->insertPicture(220,posY,40,112,
                      dir.getRealFile("texturas/options/language.png").c_str());
   posY += 25;                      

   /* Camera Mode Things */
   prevCamera = cameraNumber;
   saux = cameraName();
   qt = gList->insertTextBox(12,posY,145,posY+17,0,gettext("Camera Mode:"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_LEFT);
   buttonCamDec = gList->insertButton(121,posY,131,posY+17,
                                     fnt.createUnicode(0x25C4),0);
   buttonCamDec->defineFont(fontArial, 9);
   txtCamera = gList->insertTextBox(132,posY,197,posY+17,1,saux.c_str());
   txtCamera->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   buttonCamSum = gList->insertButton(198,posY,208,posY+17,
                                     fnt.createUnicode(0x25BA),0);
   buttonCamSum->defineFont(fontArial, 9);
   gList->insertPicture(220,posY,40,posY+17,
                       dir.getRealFile("texturas/options/camera.png").c_str());
   posY += 35;

   /* AutoEndTurn Enable or Not */
   qt = gList->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Auto End Turn"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   cxSelAutoEndTurn = gList->insertCxSel(12,posY+4,autoEndTurn);
   gList->insertPicture(220,posY,40,112,
                  dir.getRealFile("texturas/options/autoendturn.png").c_str());
   posY += 25;

   /* AlwaysRun Enable or Not */
   qt = gList->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Always Run"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   cxSelAlwaysRun = gList->insertCxSel(12,posY+4,alwaysRun);
   gList->insertPicture(220,posY,40,112,
                  dir.getRealFile("texturas/options/alwaysrun.png").c_str());
   posY += 25;

   /* ShowEnemyCircles or or Not */
   qt = gList->insertTextBox(24,posY,219,posY+34,0,
                            gettext("Show Enemy Battle Circle"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   cxSelShowEnemyCircles = gList->insertCxSel(12,posY+4,showEnemyCircles);
   gList->insertPicture(220,posY,40,112,
              dir.getRealFile("texturas/options/showenemycircles.png").c_str());
   posY += 25;

   /* ShowEnemyCircles or or Not */
   qt = gList->insertTextBox(24,posY,219,posY+34,0,
                            gettext("Highlight current enemy"));
   qt->setFont(fontArial, 10, Farso::Font::ALIGN_CENTER);
   cxSelHighlightEnemy = gList->insertCxSel(12,posY+4,highlightEnemy);
   gList->insertPicture(220,posY,40,112,
              dir.getRealFile("texturas/options/highlightenemy.png").c_str());
   posY += 25;


   /************************************************
    *                 Action Buttons               *
    ************************************************/
   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(197,356,267,375,
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
int options::treat(Farso::GuiObject* object, int eventInfo, 
      Farso::GuiInterface* interf,
      GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   int i;

   /* The key window verify */
   if(keyWindow)
   {
      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
         if(event.type == SDL_KEYUP)
         {
            /* Redefine the key */
            keys[curKey] = event.key.keysym.sym;
            buttonKeys[curKey]->setText(SDL_GetKeyName((SDLKey)keys[curKey]));
            /* Disable the key event */
            SDL_EventState(SDL_KEYUP, SDL_IGNORE);
            /* And close the window */
            interf->closeWindow(keyWindow);
         }
      }
      return(OPTIONSW_OTHER);
   }

   /* The other events verify */
   if( (eventInfo == Farso::EVENT_ON_PRESS_BUTTON) && 
         (SDL_GetTicks() - timeLastOperation > 100) )
   {
      timeLastOperation = SDL_GetTicks();

      /* Music */
      if(object == (Farso::GuiObject*) buttonMusSum)
      {
         if(musicVolume < 255)
         {
             musicVolume += 10;
             if(musicVolume > 255)
             {
                musicVolume = 255;
             }
             barMusicVolume->defineActualHealth(musicVolume);
         }
      }
      else if(object == (Farso::GuiObject*) buttonMusDec) 
      {
         if(musicVolume > 0)
         {
             musicVolume -= 10;
             if(musicVolume < 0)
             {
                musicVolume = 0;
             }
             barMusicVolume->defineActualHealth(musicVolume);
         }
      }
      /* Sound Effects */
      else if(object == (Farso::GuiObject*) buttonSndSum) 
      {
         if(sndfxVolume < 255)
         {
             sndfxVolume += 10;
             if(sndfxVolume > 255)
             {
                sndfxVolume = 255;
             }
             barSndfxVolume->defineActualHealth(sndfxVolume);
         }
      }
      else if(object == (Farso::GuiObject*) buttonSndDec) 
      {
         if(sndfxVolume > 0)
         {
             sndfxVolume -= 10;
             if(sndfxVolume < 0)
             {
                sndfxVolume = 0;
             }
             barSndfxVolume->defineActualHealth(sndfxVolume);
         }
      }
      /* Language */
      else if(object == (Farso::GuiObject*) buttonLangSum)
      {
         if(langNumber < DNT_LANG_LAST-1)
         {
            langNumber++;
         }
         txtLanguage->setText(languageName());
      }
      else if(object == (Farso::GuiObject*) buttonLangDec)
      {
         if(langNumber > DNT_LANG_ENGLISH)
         {
            langNumber--;
         }
         txtLanguage->setText(languageName());
      }
      /* Camera */
      else if(object == (Farso::GuiObject*) buttonCamSum)
      {
         if(cameraNumber < CAMERA_TYPE_DRIVE)
         {
            cameraNumber++;
         }
      }
      else if(object == (Farso::GuiObject*) buttonCamDec)
      {
         if(cameraNumber > CAMERA_TYPE_NORMAL)
         {
            cameraNumber--;
         }
      }
      /* Reflexion */
      else if(object == (Farso::GuiObject*) buttonReflSum)
      {
         if((reflexionType < REFLEXIONS_ALL) && (stencilBufferSize > 0))
         {
            reflexionType++;
         }
         txtReflexion->setText(reflexionName());
      }
      else if(object == (Farso::GuiObject*) buttonReflDec)
      {
         if(reflexionType > REFLEXIONS_NONE)
         {
            reflexionType--;
         }
         txtReflexion->setText(reflexionName());
      }
      /* Shadow */
      else if(object == (Farso::GuiObject*) buttonShadSum)
      {
         if( (shadowType < SHADOWS_PROJECTIVE) && (stencilBufferSize > 0))
         {
            shadowType++;
         }
         txtShadow->setText(shadowName());
      }
      else if(object == (Farso::GuiObject*) buttonShadDec)
      {
         if(shadowType > SHADOWS_NONE)
         {
            shadowType--;
         }
         txtShadow->setText(shadowName());
      }
      /* Resolution */
      else if(object == (Farso::GuiObject*) buttonResSum)
      {
         if(resPosition > 0)
         {
            resPosition--;
            
            /* Only accept resolutions >= 640x480 */
            if( (resolutions != NULL) && 
                (resolutions[resPosition]->w >= 640) && 
                (resolutions[resPosition]->h >= 480) )
            {
               screenWidth = resolutions[resPosition]->w;
               screenHeight = resolutions[resPosition]->h;
            }
            txtResolution->setText(resolutionName());
         }
      }
      else if(object == (Farso::GuiObject*) buttonResDec)
      {
         if((resolutions != NULL) && (resolutions[resPosition+1]))
         {
            resPosition++;
            /* Only accept resolutions >= 640x480 */
            if( (resolutions[resPosition]->w >= 640) && 
                (resolutions[resPosition]->h >= 480) )
            {
               screenWidth = resolutions[resPosition]->w;
               screenHeight = resolutions[resPosition]->h;
            }
            txtResolution->setText(resolutionName());
         }
      }

      /* Splatting Type */
      else if(object == (Farso::GuiObject*) buttonSplattingSum) 
      {
         splattingType++;
         if(splattingType > DNT_SPLATTING_SHADER)
         {
            splattingType = DNT_SPLATTING_SHADER;
         }
         txtSplatting->setText(splattingTypeName());
      }
      else if(object == (Farso::GuiObject*) buttonSplattingDec) 
      {
         splattingType--;
         if(splattingType < DNT_SPLATTING_NONE)
         {
            splattingType = DNT_SPLATTING_NONE;
         }
         txtSplatting->setText(splattingTypeName());
      }

      /* Stencil Buffer Size */
      else if(object == (Farso::GuiObject*) buttonStencilSum) 
      {
         if(stencilBufferSize == 0)
         {
            stencilBufferSize = 2;
         }
         else if(stencilBufferSize < 8)
         {
             stencilBufferSize *= 2;
         }
         txtStencil->setText(stencilBufferSizeName());
      }
      else if(object == (Farso::GuiObject*) buttonStencilDec) 
      {
         if(stencilBufferSize == 2)
         {
            stencilBufferSize = 0;
         }
         else if(stencilBufferSize > 0)
         {
             stencilBufferSize /= 2;
         }

         if(stencilBufferSize == 0)
         {
            shadowType = 0;
            txtShadow->setText(shadowName());
            reflexionType = 0;
            txtReflexion->setText(reflexionName());
         }
         txtStencil->setText(stencilBufferSizeName());
      }

      /* Anti Aliasing */
      else if(object == (Farso::GuiObject*) buttonAliasSum) 
      {
         if(antiAliasing < 4)
         {
             antiAliasing += 2;
         }
         txtAntiAliasing->setText(antiAliasingName());
      }
      else if(object == (Farso::GuiObject*) buttonAliasDec) 
      {
         if(antiAliasing > 0)
         {
             antiAliasing -= 2;
         }
         txtAntiAliasing->setText(antiAliasingName());
      }
      /* FarView Factor */
      else if(object == (Farso::GuiObject*) buttonFarViewSum) 
      {
         if(farViewFactor < 1.0)
         {
            farViewFactor += 0.1;
            barFarView->defineActualHealth((int)floor(farViewFactor*9));
         }
      }
      else if(object == (Farso::GuiObject*) buttonFarViewDec) 
      {
         if(farViewFactor > 0.4)
         {
            farViewFactor -= 0.1;
            barFarView->defineActualHealth((int)floor(farViewFactor*9));
         }
      }

   }
   else if(eventInfo == Farso::EVENT_PRESSED_BUTTON) 
   {
      /* Confirm */
      if(object == (Farso::GuiObject*) buttonConfirm)
      {
         enableParticles = cxSelParticles->isSelected();
         enableGrass = cxSelGrass->isSelected();
         enableAnisotropic = cxSelAnisotropic->isSelected();
         enableBumpMapping = cxSelBumpMapping->isSelected();
         autoEndTurn = cxSelAutoEndTurn->isSelected();
         alwaysRun = cxSelAlwaysRun->isSelected();
         showEnemyCircles = cxSelShowEnemyCircles->isSelected();
         highlightEnemy = cxSelHighlightEnemy->isSelected();

         bool prevFullScreen = enableFullScreen;
         enableFullScreen = cxSelFullScreen->isSelected();
         save();
         interf->closeWindow(intWindow);

         /* Verify if need to show advise of reinit */
         if( (screenWidth != prevWidth) || (screenHeight != prevHeight) ||
             (prevFullScreen != enableFullScreen) || 
             (antiAliasing != prevAntiAliasing) ||
             (stencilBufferSize != prevStencilBufferSize) ||
             (langNumber != prevLanguage) )
         {
            Farso::Warning warn;
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
      else if(object == (Farso::GuiObject*) buttonCancel)
      {
         /* Redo to previous values */
         musicVolume = prevMusicVolume;
         sndfxVolume = prevSndfxVolume;
         langNumber  = prevLanguage;
         cameraNumber = prevCamera;
         reflexionType = prevReflexion;
         shadowType = prevShadow;
         screenWidth = prevWidth;
         screenHeight = prevHeight;
         antiAliasing = prevAntiAliasing;
         stencilBufferSize = prevStencilBufferSize;
         splattingType = prevSplattingType;
         farViewFactor = prevFarViewFactor;
         
         /* Keys to previous values too */
         for(i=0; i < DNT_TOTAL_KEYS; i++)
         {
            keys[i] = prevKeys[i];
         }
         
         interf->closeWindow(intWindow);
         return(OPTIONSW_CANCEL);
      }
      else
      {
         /* Verify key buttons, to change state */
         bool got = false;
         for(i=0; ((i < DNT_TOTAL_KEYS) && (!got)); i++)
         {
            if(object == (Farso::GuiObject*)buttonKeys[i])
            {
               curKey = i;
               /* Show a modal window until got the key */
               int xPos = (int)(Farso::SCREEN_X / 2.0);
               int yPos = (int)(Farso::SCREEN_Y / 2.0);
               string s = gettext("Press a key to: ");
               keyWindow = interf->insertWindow(xPos-138,yPos-243,
                                                xPos+138,yPos-203,
                                                gettext("Waiting..."));
               s += gettext(dntKeyDesc[i].c_str());
               keyWindow->getObjectsList()->insertTextBox(10,17,270,27,0,s);
               keyWindow->setExternPointer(&keyWindow);
               keyWindow->setModal();
               interf->openWindow(keyWindow);

               /* Enable, temporally, key events to get the key */
               SDL_EventState(SDL_KEYUP, SDL_ENABLE);

               got = true;
            }
         }
      }
   }
   else if(eventInfo == Farso::EVENT_MODIFIED_CX_SEL)
   {
      /* cxSelParticles */
      if( (object == (Farso::GuiObject*) cxSelParticles) || 
          (object == (Farso::GuiObject*) cxSelGrass))
      {
         /* When disable particles, disable the grass */
         cxSelGrass->setAvailable(getAvaibleParticles() &&
                                  cxSelParticles->isSelected());
      }
   }

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
 *                   verifyOptionsAvailability                  *
 ****************************************************************/
void options::verifyOptionsAvailability()
{
   verifySplattingAvailability();
}

/****************************************************************
 *                  verifySplattingAvailability                 *
 ****************************************************************/
void options::verifySplattingAvailability()
{
   extensions ext;

   switch(splattingType)
   {
      case DNT_SPLATTING_SHADER:
      {
         if(!ext.hasShader())
         {
            cerr << "Warning: no shader; reverting splatting to "
                 << "glMultitextureARB" << endl;
            /* Hasn't shader, let's try as multitexture extension */
            if(ext.hasMultiTexture())
            {
               splattingType = DNT_SPLATTING_EXTENSION;
            }
            else
            {
               /* Must disable */
               cerr << "Warning: no glMultitextureARB; no splatting available!"
                    << endl;
               splattingType = DNT_SPLATTING_NONE;
            }
         }
      }
      break;
      case DNT_SPLATTING_EXTENSION:
      {
         if(!ext.hasMultiTexture())
         {
            cerr << "Warning: no glMultitextureARB; no splatting available!"
                 << endl;
            /* No extension, must disable */
            splattingType = DNT_SPLATTING_NONE;
         }
      }
      break;
   }
}

/****************************************************************
 *                        getSplattingType                      *
 ****************************************************************/
int options::getSplattingType()
{
   return(splattingType);
}

/****************************************************************
 *                        setSplattingType                      *
 ****************************************************************/
void options::setSplattingType(int t)
{
   splattingType = t;
   verifySplattingAvailability();
}

/****************************************************************
 *                   getEnableAnisotropicFilter                 *
 ****************************************************************/
bool options::getEnableAnisotropicFilter()
{
   extensions ext;
   return(enableAnisotropic && ext.hasAnisotropic());
}

/****************************************************************
 *                     getEnableBumpMapping                     *
 ****************************************************************/
bool options::getEnableBumpMapping()
{
   extensions ext;
   return(enableBumpMapping && ext.hasShader());
}

/****************************************************************
 *                         getReflexionType                     *
 ****************************************************************/
int options::getReflexionType()
{
   return(reflexionType);
}

/****************************************************************
 *                           getShadowType                      *
 ****************************************************************/
int options::getShadowType()
{
   return(shadowType);
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
 *                     getHighLightEnemy                        *
 ****************************************************************/
bool options::getHighlightEnemy()
{
   return(highlightEnemy);
}

/****************************************************************
 *                        getAlwaysRun                          *
 ****************************************************************/
bool options::getAlwaysRun()
{
   return(alwaysRun);
}

/****************************************************************
 *                     getStencilBufferSize                     *
 ****************************************************************/
int options::getStencilBufferSize()
{
   return(stencilBufferSize);
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
 *                          defaultKeys                         *
 ****************************************************************/
Uint32 options::getKey(int key)
{
   if((key >= 0) && (key < DNT_TOTAL_KEYS))
   {
      return(keys[key]);
   }

   return(0);
}

/****************************************************************
 *                          defaultKeys                         *
 ****************************************************************/
void options::defaultKeys()
{
   /* Camera Keys */
   keys[DNT_KEY_CAMERA_ROTATE_LEFT] = SDLK_LEFT;
   keys[DNT_KEY_CAMERA_ROTATE_RIGHT] = SDLK_RIGHT;
   keys[DNT_KEY_CAMERA_ROTATE_UP] = SDLK_PAGEUP;
   keys[DNT_KEY_CAMERA_ROTATE_DOWN] = SDLK_PAGEDOWN;
   keys[DNT_KEY_CAMERA_ZOOM_UP] = SDLK_UP;
   keys[DNT_KEY_CAMERA_ZOOM_DOWN] = SDLK_DOWN;
   keys[DNT_KEY_CAMERA_TOP] = SDLK_INSERT;
   keys[DNT_KEY_CAMERA_BOTTOM] = SDLK_DELETE;
   keys[DNT_KEY_CAMERA_MAX_ZOOM] = SDLK_HOME;
   keys[DNT_KEY_CAMERA_MIN_ZOOM] = SDLK_END;

   /* Character move keys */
   keys[DNT_KEY_TOGGLE_RUN_1] = SDLK_LSHIFT; 
   keys[DNT_KEY_TOGGLE_RUN_2] = SDLK_RSHIFT;
   keys[DNT_KEY_MOVE_RIGHT] = SDLK_e;
   keys[DNT_KEY_MOVE_LEFT] = SDLK_q;
   keys[DNT_KEY_MOVE_FORWARD] = SDLK_w;
   keys[DNT_KEY_MOVE_BACKWARD] = SDLK_s;
   keys[DNT_KEY_ROTATE_LEFT] = SDLK_a;
   keys[DNT_KEY_ROTATE_RIGHT] = SDLK_d;
   keys[DNT_KEY_CHANGE_CHARACTER] = SDLK_TAB;
   keys[DNT_KEY_REST] = SDLK_r;

   /* Battle Keys */
   keys[DNT_KEY_BATTLE_TURN] = SDLK_SPACE;

   /* Shortcuts keys */
   keys[DNT_KEY_WINDOW_MINI_MAP] = SDLK_m;
   keys[DNT_KEY_WINDOW_SHORTCUTS] = SDLK_n;
   keys[DNT_KEY_WINDOW_BRIEFING] = SDLK_l;
   keys[DNT_KEY_WINDOW_INVENTORY] = SDLK_i;
   keys[DNT_KEY_WINDOW_CHARACTER] = SDLK_c;
   keys[DNT_KEY_WINDOW_JOURNAL] = SDLK_j;
}

/****************************************************************
 *                         Static Members                       *
 ****************************************************************/
int    options::musicVolume = 50;
int    options::sndfxVolume = 128;
int    options::langNumber = 0;
int    options::cameraNumber = 1;
bool   options::enableParticles = true;
bool   options::enableGrass = true;
int    options::reflexionType = 2;
int    options::shadowType = SHADOWS_PROJECTIVE;
int    options::screenWidth = 1024;
int    options::screenHeight = 768; 
SDL_Rect** options::resolutions = NULL;
bool   options::enableFullScreen = false;
int    options::antiAliasing = 4;
int    options::stencilBufferSize = 8;
float  options::farViewFactor = 1.0;
int    options::splattingType = DNT_SPLATTING_SHADER;
bool   options::autoEndTurn = true;
bool   options::showEnemyCircles = false;
bool   options::highlightEnemy = true;
bool   options::enableAnisotropic = true;
bool   options::enableBumpMapping = true;
bool   options::alwaysRun = true;
Uint32 options::keys[DNT_TOTAL_KEYS];

Uint32 options::prevKeys[DNT_TOTAL_KEYS];
string options::fileName = "";

