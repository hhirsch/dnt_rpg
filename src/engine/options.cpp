/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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
   gettext("Russian")
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
   "ru"
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
   DNT_LANG_RUSSIAN,
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

   /* First define default keys */
   defaultKeys();
   
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
      else if(key == "MultiTexture")
      {
         /* Read Multitexture options */
         enableMultiTexture = (value == "true");
      }
      else if(key == "Anisotropic")
      {
         /* Read Anisotropi option */
         enableAnisotropic = (value == "true");
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
   /* Multi Texture */
   fprintf(arq, "MultiTexture = %s\n",enableMultiTexture?"true":"false");
   /* Anisotropic Filter */
   fprintf(arq, "Anisotropic = %s\n",enableAnisotropic?"true":"false");

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
   return( (langNumber == DNT_LANG_RUSSIAN) );
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
void options::insertKeys(int firstKey, int lastKey, guiList* list)
{
   int posY = 77;
   int i;
   textBox* tb;

   if( (firstKey < 0) || (firstKey >= DNT_TOTAL_KEYS) ||
       (lastKey < 0) || (lastKey >= DNT_TOTAL_KEYS) )
   {
      return;
   }

   for(i = firstKey; i <= lastKey; i++)
   {
      tb = list->insertTextBox(14,posY,185,posY+17,0, 
                               gettext(dntKeyDesc[i].c_str()));
      buttonKeys[i] = list->insertButton(188,posY,262,posY+17,
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
void options::displayOptionsScreen(guiInterface* interf)
{
   dirs dir;
   extensions ext;
   dntFont fnt;
   fnt.defineFont(DNT_FONT_ARIAL, 10);
   char tmp[8];
   string saux;
   textBox* qt;

   getAvailableResolutions();

   keyWindow = NULL;

   /* Define Previous Values */
   prevMusicVolume = musicVolume;
   prevSndfxVolume = sndfxVolume;

   prevAntiAliasing = antiAliasing;
   prevStencilBufferSize = stencilBufferSize;
   prevFarViewFactor = farViewFactor;

   /* Previous Keys values too */
   int i;
   for(i=0; i < DNT_TOTAL_KEYS; i++)
   {
      prevKeys[i] = keys[i];
   }

   int xPos = (int)(SCREEN_X / 2.0);
   int yPos = (int)(SCREEN_Y / 2.0);

   intWindow = interf->insertWindow(xPos-138,yPos-192,xPos+138,yPos+192,
                                    gettext("Options"));

   /* Insert the Objects TabBox */
   tabBox* tb=(tabBox*)intWindow->getObjectsList()->defineTabBox(8,27,268,352);
   guiList* list;
   int posY;

   /************************************************
    *                Input Options                 *
    ************************************************/
   list = tb->insertOption(gettext("Input"));
   tabBox* inputTab = (tabBox*)list->defineTabBox(10,52,266,350);
   insertKeys(DNT_KEY_FIRST_WINDOW, DNT_KEY_LAST_WINDOW,
              inputTab->insertOption(gettext("Windows")));

   insertKeys(DNT_KEY_FIRST_CHARACTER, DNT_KEY_LAST_CHARACTER,
              inputTab->insertOption(gettext("Character")));

   insertKeys(DNT_KEY_FIRST_CAMERA, DNT_KEY_LAST_CAMERA,
              inputTab->insertOption(gettext("Camera")));

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
   cxSelParticles->setAvailable(getAvaibleParticles());
   cxSelGrass->setAvailable(getAvaibleParticles() && 
                          cxSelParticles->isSelected());
   posY += 25;

   /* MultiTexture Enable or Not */
   qt = list->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Enable MultiTextures"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelMultiTexture = list->insertCxSel(12,posY+4,getEnableMultiTexture());
   list->insertPicture(220,posY,40,112,
                  dir.getRealFile("texturas/options/multitexture.png").c_str());
   cxSelMultiTexture->setAvailable(ext.hasMultiTexture());
   posY += 25;

   /* MultiTexture Enable or Not */
   qt = list->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Enable Anisotropic Filter"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   cxSelAnisotropic = list->insertCxSel(12,posY+4,getEnableAnisotropicFilter());
   list->insertPicture(220,posY,40,112,
                   dir.getRealFile("texturas/options/anisotropic.png").c_str());
   cxSelAnisotropic->setAvailable(ext.hasAnisotropic());
   posY += 35;

   /* Stncil Buffer Size */
   prevStencilBufferSize = stencilBufferSize;
   saux = stencilBufferSizeName();
   qt = list->insertTextBox(12,posY,145,posY+17,0,gettext("Stencil Buffer:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonStencilDec = list->insertButton(121,posY,131,posY+17,
                                      fnt.createUnicode(0x25C4),0);
   buttonStencilDec->defineFont(DNT_FONT_ARIAL, 9);
   txtStencil = list->insertTextBox(132,posY,197,posY+17,1,saux.c_str());
   txtStencil->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonStencilSum = list->insertButton(198,posY,208,posY+17,
                                      fnt.createUnicode(0x25BA),0);
   buttonStencilSum->defineFont(DNT_FONT_ARIAL, 9);
   list->insertPicture(220,posY,40,220,
                  dir.getRealFile("texturas/options/stencil_size.png").c_str());
   posY += 25;

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

   /* Shadows */
   prevShadow = shadowType;
   saux = shadowName();
   qt = list->insertTextBox(12,posY,145,posY+17,0,gettext("Shadows:"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT);
   buttonShadDec = list->insertButton(121,posY,131,posY+17,
                                      fnt.createUnicode(0x25C4),0);
   buttonShadDec->defineFont(DNT_FONT_ARIAL, 9);
   txtShadow = list->insertTextBox(132,posY,197,posY+17,1,saux.c_str());
   txtShadow->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   buttonShadSum = list->insertButton(198,posY,208,posY+17,
                                      fnt.createUnicode(0x25BA),0);
   buttonShadSum->defineFont(DNT_FONT_ARIAL, 9);
   list->insertPicture(220,posY,40,220,
                    dir.getRealFile("texturas/options/shadow.png").c_str());
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

   /* AlwaysRun Enable or Not */
   qt = list->insertTextBox(24,posY,219,posY+17,0,
                            gettext("Always Run"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   cxSelAlwaysRun = list->insertCxSel(12,posY+4,alwaysRun);
   list->insertPicture(220,posY,40,112,
                  dir.getRealFile("texturas/options/alwaysrun.png").c_str());
   posY += 25;

   /* ShowEnemyCircles or or Not */
   qt = list->insertTextBox(24,posY,219,posY+34,0,
                            gettext("Show Enemy Battle Circle"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   cxSelShowEnemyCircles = list->insertCxSel(12,posY+4,showEnemyCircles);
   list->insertPicture(220,posY,40,112,
              dir.getRealFile("texturas/options/showenemycircles.png").c_str());
   posY += 25;

   /* ShowEnemyCircles or or Not */
   qt = list->insertTextBox(24,posY,219,posY+34,0,
                            gettext("Highlight current enemy"));
   qt->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   cxSelHighlightEnemy = list->insertCxSel(12,posY+4,highlightEnemy);
   list->insertPicture(220,posY,40,112,
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
int options::treat(guiObject* object, int eventInfo, guiInterface* interf,
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
   if( (eventInfo == FARSO_EVENT_ON_PRESS_BUTTON) && 
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
             barMusicVolume->defineActualHealth(musicVolume);
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
             barMusicVolume->defineActualHealth(musicVolume);
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
             barSndfxVolume->defineActualHealth(sndfxVolume);
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
             barSndfxVolume->defineActualHealth(sndfxVolume);
         }
      }
      /* Language */
      else if(object == (guiObject*) buttonLangSum)
      {
         if(langNumber < DNT_LANG_LAST-1)
         {
            langNumber++;
         }
         txtLanguage->setText(languageName());
      }
      else if(object == (guiObject*) buttonLangDec)
      {
         if(langNumber > DNT_LANG_ENGLISH)
         {
            langNumber--;
         }
         txtLanguage->setText(languageName());
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
         if((reflexionType < REFLEXIONS_ALL) && (stencilBufferSize > 0))
         {
            reflexionType++;
         }
         txtReflexion->setText(reflexionName());
      }
      else if(object == (guiObject*) buttonReflDec)
      {
         if(reflexionType > REFLEXIONS_NONE)
         {
            reflexionType--;
         }
         txtReflexion->setText(reflexionName());
      }
      /* Shadow */
      else if(object == (guiObject*) buttonShadSum)
      {
         if( (shadowType < SHADOWS_PROJECTIVE) && (stencilBufferSize > 0))
         {
            shadowType++;
         }
         txtShadow->setText(shadowName());
      }
      else if(object == (guiObject*) buttonShadDec)
      {
         if(shadowType > SHADOWS_NONE)
         {
            shadowType--;
         }
         txtShadow->setText(shadowName());
      }
      /* Resolution */
      else if(object == (guiObject*) buttonResSum)
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
      else if(object == (guiObject*) buttonResDec)
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

      /* Stencil Buffer Size */
      else if(object == (guiObject*) buttonStencilSum) 
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
      else if(object == (guiObject*) buttonStencilDec) 
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
      else if(object == (guiObject*) buttonAliasSum) 
      {
         if(antiAliasing < 4)
         {
             antiAliasing += 2;
         }
         txtAntiAliasing->setText(antiAliasingName());
      }
      else if(object == (guiObject*) buttonAliasDec) 
      {
         if(antiAliasing > 0)
         {
             antiAliasing -= 2;
         }
         txtAntiAliasing->setText(antiAliasingName());
      }
      /* FarView Factor */
      else if(object == (guiObject*) buttonFarViewSum) 
      {
         if(farViewFactor < 1.0)
         {
            farViewFactor += 0.1;
            barFarView->defineActualHealth((int)floor(farViewFactor*9));
         }
      }
      else if(object == (guiObject*) buttonFarViewDec) 
      {
         if(farViewFactor > 0.4)
         {
            farViewFactor -= 0.1;
            barFarView->defineActualHealth((int)floor(farViewFactor*9));
         }
      }

   }
   else if(eventInfo == FARSO_EVENT_PRESSED_BUTTON) 
   {
      /* Confirm */
      if( (object == (guiObject*) buttonConfirm) )
      {
         enableParticles = cxSelParticles->isSelected();
         enableGrass = cxSelGrass->isSelected();
         enableMultiTexture = cxSelMultiTexture->isSelected();
         enableAnisotropic = cxSelAnisotropic->isSelected();
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
      else if( (object == (guiObject*) buttonCancel) )
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
            if(object == (guiObject*)buttonKeys[i])
            {
               curKey = i;
               /* Show a modal window until got the key */
               int xPos = (int)(SCREEN_X / 2.0);
               int yPos = (int)(SCREEN_Y / 2.0);
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
   else if(eventInfo == FARSO_EVENT_MODIFIED_CX_SEL)
   {
      /* cxSelParticles */
      if( (object == (guiObject*) cxSelParticles) || 
          (object == (guiObject*) cxSelGrass))
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
 *                     setEnableMultiTexture                    *
 ****************************************************************/
void options::setEnableMultiTexture(bool val)
{
   enableMultiTexture = val;
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
 *                   getEnableAnisotropicFilter                 *
 ****************************************************************/
bool options::getEnableAnisotropicFilter()
{
   extensions ext;
   return(enableAnisotropic && ext.hasAnisotropic());
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
bool   options::enableMultiTexture = true;
bool   options::autoEndTurn = true;
bool   options::showEnemyCircles = false;
bool   options::highlightEnemy = true;
bool   options::enableAnisotropic = true;
bool   options::alwaysRun = true;
Uint32 options::keys[DNT_TOTAL_KEYS];

Uint32 options::prevKeys[DNT_TOTAL_KEYS];
string options::fileName = "";

