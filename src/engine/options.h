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

#ifndef _dnt_options_h
#define _dnt_options_h

#include "../gui/farso.h"
#include <string>

#define OPTIONSW_CANCEL  0  /**< Cancel Return Value */
#define OPTIONSW_CONFIRM 1  /**< Confirm Return Value */
#define OPTIONSW_OTHER   2  /**< Other Return Value */


/*! Keys constants */
enum 
{
   /* Camera Keys */
   DNT_KEY_CAMERA_ROTATE_LEFT,
   DNT_KEY_CAMERA_ROTATE_RIGHT,
   DNT_KEY_CAMERA_ROTATE_UP,
   DNT_KEY_CAMERA_ROTATE_DOWN,
   DNT_KEY_CAMERA_ZOOM_UP,
   DNT_KEY_CAMERA_ZOOM_DOWN,
   DNT_KEY_CAMERA_TOP,
   DNT_KEY_CAMERA_BOTTOM,
   DNT_KEY_CAMERA_MAX_ZOOM,
   DNT_KEY_CAMERA_MIN_ZOOM,

   /* Character move keys */
   DNT_KEY_TOGGLE_RUN_1,
   DNT_KEY_TOGGLE_RUN_2,
   DNT_KEY_MOVE_RIGHT,
   DNT_KEY_MOVE_LEFT,
   DNT_KEY_MOVE_FORWARD,
   DNT_KEY_MOVE_BACKWARD,
   DNT_KEY_ROTATE_LEFT,
   DNT_KEY_ROTATE_RIGHT,
   DNT_KEY_CHANGE_CHARACTER,
   DNT_KEY_REST,

   /* Battle Keys */
   DNT_KEY_BATTLE_TURN,

   /* Shortcuts keys */
   DNT_KEY_WINDOW_MINI_MAP,
   DNT_KEY_WINDOW_SHORTCUTS,
   DNT_KEY_WINDOW_BRIEFING,
   DNT_KEY_WINDOW_INVENTORY,
   DNT_KEY_WINDOW_CHARACTER,
   DNT_KEY_WINDOW_JOURNAL,

   /* Keys count */
   DNT_TOTAL_KEYS
};

#define DNT_SPLATTING_NONE        0    /**< No texture spatting */
#define DNT_SPLATTING_EXTENSION   1    /**< Splatting by OpenGL extensions */
#define DNT_SPLATTING_SHADER      2    /**< Splatting by Shader (fastest) */

/* Firsts and Lasts for the tabs */
#define DNT_KEY_FIRST_CAMERA     DNT_KEY_CAMERA_ROTATE_LEFT
#define DNT_KEY_LAST_CAMERA      DNT_KEY_CAMERA_MIN_ZOOM

#define DNT_KEY_FIRST_CHARACTER  DNT_KEY_TOGGLE_RUN_1
#define DNT_KEY_LAST_CHARACTER   DNT_KEY_BATTLE_TURN

#define DNT_KEY_FIRST_WINDOW  DNT_KEY_WINDOW_MINI_MAP
#define DNT_KEY_LAST_WINDOW   DNT_KEY_WINDOW_JOURNAL

/*! Options Class */
class options
{
   public:
      /*! Constructor */
      options();
      /*! options destructor */
      ~options();

      /*! Display the Options Screen 
       * \param interf -> GUI interface to be used.*/
      void displayOptionsScreen(Farso::GuiInterface* interf);
      /*! Save current options on file. */
      void save();

      /*! Load the Options File from default dirs
       * (first try from user home, next from installed one, 
       *  and if can't any of those, from the ./ dir )*/
      bool load();
      /*! Load options from file
       * \param file -> filename with the options file to be loaded.
       * \return true if can load the options, false otherwise */
      bool load(std::string file);
      /*! Treat options Window Events */
      int treat(Farso::GuiObject* object, int eventInfo, 
            Farso::GuiInterface* interf,
            GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Set language to one defined at the option's file */
      void setLanguage();

      /*! Get all available resolutions at fullscreen mode */
      void getAvailableResolutions();

      /*! Verify if the current language is an Unicode One (like russian,
       * japanese, chinese, etc.) */
      bool isLanguageUnicode();

      /*! Get the Music Volume
       * \return -> current music volume */
      int getMusicVolume();
      /*! Get the Sound Effects Volume
       * \return -> current sound effects volume */
      int getSndFxVolume();
      /*! Get the Language Number
       * \return -> current language */
      int getLanguage();
      /*! Get the current camera mode
       * \return -> current camera mode number */
      int getCameraNumber();
      /*! Verify if the particles are avaible 
       * \return -> true if avaible */
      bool getAvaibleParticles();
      /*! Verify if the particles are enabled
       * \return -> true if enabled */
      bool getEnableParticles();
      /*! Verify if grass is enabled
       * \return -> true if enabled */
      bool getEnableGrass();
      /*! Get the reflexion type
       * \return -> current reflexion type */
      int getReflexionType();
      /*! Get the shadow type
       * \return -> current shadow type */
      int getShadowType();
      /*! Get the screen width
       * \return -> current screen width */
      int getScreenWidth();
      /*! Get the screen height
       * \return -> current screen height */
      int getScreenHeight();
      /*! Verify if the fullscreen is enabled
       * \return -> true if fullscreen is enabled */
      bool getEnableFullScreen();
      /*! Get Splatting type 
       * \return splatting type constant */
      int getSplattingType();
      /*! Set splatting type
       * \param t -> splatting type constant */
      void setSplattingType(int t);
      /*! Verify if the anisotropic filter is enabled (and available)
       * \return true if enabled */
      bool getEnableAnisotropicFilter();
      /*! Verify if bumpmapping is enabled (and available)
       * \return true if enabled */
      bool getEnableBumpMapping();
      /*! Verify if the auto end turn is enabled
       * \return -> true if enabled 
       * \note -> if enabled, if the character have no possible actions,
       *          the turn automatically ends.  */
      bool getAutoEndTurn();
      /*! Verify if will show enemies circles at battle
       * \return true if will show, false if not. */
      bool getShowEnemyCircles();
      /*! Verify if is to hightlight in-action enemy at battles
       * \return true to highlight or falst */
      bool getHighlightEnemy();
      /*! Verify if playable characters will always run or not
       * \return -> true if will always run */
      bool getAlwaysRun();
      /*! Get the current antialiasing
       * \return -> current antialiasing*/
      int getAntiAliasing();
      /*! Get the size of stencil buffer to use
       * \return size of the stencil buffer */
      int getStencilBufferSize();
      /*! Set the current antialiasing
       * \param alias -> new antialiasing*/
      void setAntiAliasing(int alias);
      /*! Get the current farView Factor
       * \return -> current farView Factor */
      float getFarViewFactor();
      /*! Set the current farview factor
       * \param factor -> new farview factor */
      void setFarViewFactor(float factor);

      /*! Verify options availability */
      void verifyOptionsAvailability();

      /*! Get the SDL_Key value for the DNT key
       * \param key -> DNT key constant
       * \return -> SDLK_ constant */
      Uint32 getKey(int key);

   private:
      /*! Insert Key-Set related Objects at guiList
       * \param firstKey -> number of first key
       * \param lastKey -> number of last key
       * \param list -> pointer to the desired guiList */
      void insertKeys(int firstKey, int lastKey, Farso::GuiList* list);

      /*! Set the default keys */
      void defaultKeys();

      /*! Verify if current splatting is avaiable or not */
      void verifySplattingAvailability();

      Farso::TextBox* txtLanguage;    /**< Language Text on Window */
      Farso::TextBox* txtCamera;      /**< Camera Mode Text on Window */
      Farso::TextBox* txtReflexion;   /**< Reflexion Type Text on Window */
      Farso::TextBox* txtShadow;      /**< Shadow Type Text on Window */
      Farso::TextBox* txtStencil;     /**< Stencil buffer size Text on Window */
      Farso::TextBox* txtResolution;  /**< Resolution Text on Window */
      Farso::TextBox* txtAntiAliasing;/**< AntiAliasing Text on Window */
      Farso::TextBox* txtSplatting;   /**< Splatting text on window */
      
      Farso::HealthBar* barMusicVolume;  /**< Music Volume Text on Window*/
      Farso::HealthBar* barSndfxVolume;  /**< Sound Effects Volume Text */
      Farso::HealthBar* barFarView;      /**< FarViewFactor Text on Window */

      static SDL_Rect** resolutions;  /**< the available resolutions */
      static int     musicVolume;     /**< Current Music Volume */
      static int     sndfxVolume;     /**< Current Sound Effects Volume */
      static int     langNumber;      /**< Current Language Number */
      static int     cameraNumber;    /**< Current Camera Mode */
      static bool    enableParticles; /**< Enable/Disable Particles */
      static bool    enableGrass;     /**< Enable/Disable Grass */
      static int     reflexionType;   /**< Reflexion Type */
      static int     shadowType;      /**< The Shadow Type to use */
      static int     screenWidth;     /**< Screen Height */
      static int     screenHeight;    /**< Screen Width */
      static bool    enableFullScreen;/**< If fullscreen is defined */
      static int     antiAliasing;    /**< Current Anti-Aliasing */
      static int     stencilBufferSize; /**< Current Stencil buffer size */
      static float   farViewFactor;   /**< Current FarView Factor */
      static int     splattingType;    /**< Texture Splatting type */
      static bool    enableAnisotropic; /**< If anisotropic filter is used */
      static bool    enableBumpMapping; /**< If bumpMapping is used */
      static bool    autoEndTurn;     /**< If will autoend turn or not */
      static bool    showEnemyCircles; /**< if show enemies battle circles */
      static bool    highlightEnemy;  /**< if highlight enemy at battle */
      static bool    alwaysRun;        /**< if will always run */
      static Uint32  keys[DNT_TOTAL_KEYS]; /**< The key values */

      Farso::Window* intWindow;       /**< Options Window Pointer */
      Farso::Window* keyWindow;       /**< Window to get key */
 
      int      prevCamera;         /**< Previous used Camera Mode */
      int      prevMusicVolume;    /**< Previous used Music Volume */
      int      prevSndfxVolume;    /**< Previous Used Sound Effects Volume */
      int      prevLanguage;       /**< Previous Used Language */
      int      prevReflexion;      /**< Previous Used Reflexion */
      int      prevShadow;         /**< Previous Used Shadow */
      int      prevWidth;          /**< Previous Used Resolution Width */
      int      prevHeight;         /**< Previous Used Resolution Height */
      int      prevStencilBufferSize; /**< Previous stencil buffer size */
      int      prevAntiAliasing;   /**< Previous Used AntiAliasing */
      float    prevFarViewFactor;  /**< Previous FarViewFactor */
      int      prevSplattingType;  /**< Previous Splatting type */
      int      resPosition;        /**< Resolution Position on Vector */
      int      curKey;             /**< Current Key index to scan */
      static Uint32 prevKeys[DNT_TOTAL_KEYS]; /**< The previous key values */
      
      int timeLastOperation;       /**< Ticks when done last operation */

      static std::string fileName;      /**< The current options fileName */            
      Farso::Button* buttonKeys[DNT_TOTAL_KEYS];

      Farso::Button* buttonMusSum;
      Farso::Button* buttonMusDec;
      
      Farso::Button* buttonSndSum;
      Farso::Button* buttonSndDec;
      
      Farso::Button* buttonLangSum;
      Farso::Button* buttonLangDec;

      Farso::Button* buttonCamSum;
      Farso::Button* buttonCamDec;

      Farso::Button* buttonReflSum;
      Farso::Button* buttonReflDec;
      
      Farso::Button* buttonShadSum;
      Farso::Button* buttonShadDec;

      Farso::Button* buttonResSum;
      Farso::Button* buttonResDec;

      Farso::Button* buttonAliasSum;
      Farso::Button* buttonAliasDec;

      Farso::Button* buttonStencilSum;
      Farso::Button* buttonStencilDec;

      Farso::Button* buttonFarViewSum;
      Farso::Button* buttonFarViewDec;

      Farso::Button* buttonSplattingSum;
      Farso::Button* buttonSplattingDec;

      Farso::CxSel* cxSelFullScreen;

      Farso::CxSel* cxSelGrass;
      Farso::CxSel* cxSelParticles;

      Farso::CxSel* cxSelAnisotropic;
      Farso::CxSel* cxSelBumpMapping;

      Farso::CxSel* cxSelShowEnemyCircles;
      Farso::CxSel* cxSelHighlightEnemy;
      Farso::CxSel* cxSelAutoEndTurn;
      Farso::CxSel* cxSelAlwaysRun;

      Farso::Button* buttonConfirm;
      Farso::Button* buttonCancel;

      /*! Get the Current Language Name
      * \return the name of the language. */
      std::string languageName();

      /*! Get the Current Camera Name
       * \return the name of the camera. */
      std::string cameraName();

      /*! Get the Current Reflexion Type Name
       * \return the name of the reflexion type */
      std::string reflexionName();
      
      /*! Get the Current Shadow Type Name
       * \return the name of the shadow type */
      std::string shadowName();

      /*! Get the current resolution name
       * \return the name of the resolution */
      std::string resolutionName();

      /*! Get the Current antiAliasing Name
       * \return name of the antiAliasing */
      std::string antiAliasingName();

      /*! Get the stencil buffer size name
       * \return name of the stencil buffer size */
      std::string stencilBufferSizeName();

      /*! Get a string with splattingType name
       * \return name of the spalttingType */
      std::string splattingTypeName();

};


#endif

