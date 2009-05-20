#ifndef _options_h
#define _options_h

#include "../gui/farso.h"
#include <string>
using namespace std;

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

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
      void displayOptionsScreen(guiInterface* interf);
      /*! Save current options on file. */
      void save();

      /*! Load the Options File from default dirs
       * (first try from user home, next from installed one, 
       *  and if can't any of those, from the ./ dir )*/
      bool load();
      /*! Load options from file
       * \param file -> filename with the options file to be loaded.
       * \return true if can load the options, false otherwise */
      bool load(string file);
      /*! Treat options Window Events */
      int treat(guiObject* object, int eventInfo, guiInterface* interf,
                GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Set language to one defined at the option's file */
      void setLanguage();

      /*! Get all avaible resolutions at fullscreen mode */
      void getAvaibleResolutions();

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
       * \return -> current reflextion type */
      int getReflexionType();
      /*! Get the screen width
       * \return -> current screen width */
      int getScreenWidth();
      /*! Get the screen height
       * \return -> current screen height */
      int getScreenHeight();
      /*! Verify if the fullscreen is enabled
       * \return -> true if fullscreen is enabled */
      bool getEnableFullScreen();
      /*! Verify if the multitexture is enabled (and avalaible) 
       * \return true if enabled */
      bool getEnableMultiTexture();
      /*! Verify if the anisotropic filter is enabled (and available)
       * \return true if enabled */
      bool getEnableAnisotropicFilter();
      /*! Verify if the auto end turn is enabled
       * \return -> true if enabled 
       * \note -> if enabled, if the character have no possible actions,
       *          the turn automatically ends.  */
      bool getAutoEndTurn();
      /*! Verify if will show enemies circles at battle
       * \return true if will show, false if not. */
      bool getShowEnemyCircles();
      /*! Verify if playable characters will always run or not
       * \return -> true if will always run */
      bool getAlwaysRun();
      /*! Get the current antialiasing
       * \return -> current antialiasing*/
      int getAntiAliasing();
      /*! Set the current antialiasing
       * \param alias -> new antialiasing*/
      void setAntiAliasing(int alias);
      /*! Get the current farView Factor
       * \return -> current farView Factor */
      float getFarViewFactor();
      /*! Set the current farview factor
       * \param factor -> new farview factor */
      void setFarViewFactor(float factor);

      /*! Get the SDL_Key value for the DNT key
       * \param key -> DNT key constant
       * \return -> SDLK_ constant */
      Uint32 getKey(int key);

   private:
      /*! Insert Key-Set related Objects at guiList
       * \param firstKey -> number of first key
       * \param lastKey -> number of last key
       * \param list -> pointer to the desired guiList */
      void insertKeys(int firstKey, int lastKey, guiList* list);

      /*! Set the default keys */
      void defaultKeys();

      SDL_Rect**   resolutions;    /**< the avaible resolutions */
      textBox* txtLanguage;        /**< Language Text on Window */
      textBox* txtCamera;          /**< Camera Mode Text on Window */
      textBox* txtReflexion;       /**< Reflexion Type Text on Window */
      textBox* txtResolution;      /**< Resolution Text on Window */
      textBox* txtAntiAliasing;    /**< AntiAliasing Text on Window */
      
      healthBar* barMusicVolume;   /**< Music Volume Text on Window*/
      healthBar* barSndfxVolume;   /**< Sound Effects Volume Text on Window*/
      healthBar* barFarView;       /**< FarViewFactor Text on Window */

      static int     musicVolume;     /**< Current Music Volume */
      static int     sndfxVolume;     /**< Current Sound Effects Volume */
      static int     langNumber;      /**< Current Language Number */
      static int     cameraNumber;    /**< Current Camera Mode */
      static bool    enableParticles; /**< Enable/Disable Particles */
      static bool    enableGrass;     /**< Enable/Disable Grass */
      static int     reflexionType;   /**< Reflexion Type */
      static int     screenWidth;     /**< Screen Height */
      static int     screenHeight;    /**< Screen Width */
      static bool    enableFullScreen;/**< If fullscreen is defined */
      static int     antiAliasing;    /**< Current Anti-Aliasing */
      static float   farViewFactor;   /**< Current FarView Factor */
      static bool    enableMultiTexture; /**< If terrain multi texture */
      static bool    enableAnisotropic; /**< If anisotropic filter is used */
      static bool    autoEndTurn;     /**< If will autoend turn or not */
      static bool    showEnemyCircles; /**< if show enemies battle circles */
      static bool    alwaysRun;        /**< if will always run */
      static Uint32  keys[DNT_TOTAL_KEYS]; /**< The key values */

      window* intWindow;       /**< Options Window Pointer */

      int      prevCamera;         /**< Previous used Camera Mode */
      int      prevMusicVolume;    /**< Previous used Music Volume */
      int      prevSndfxVolume;    /**< Previous Used Sound Effects Volume */
      int      prevLanguage;       /**< Previous Used Language */
      int      prevReflexion;      /**< Previous Used Reflexion */
      int      prevWidth;          /**< Previous Used Resolution Width */
      int      prevHeight;         /**< Previous Used Resolution Height */
      int      prevAntiAliasing;   /**< Previous Used AntiAliasing */
      float    prevFarViewFactor;  /**< Previous FarViewFactor */
      int      resPosition;        /**< Resolution Position on Vector */

      int timeLastOperation;       /**< Ticks when done last operation */

      static string fileName;      /**< The current options fileName */            
      button* buttonKeys[DNT_TOTAL_KEYS];

      button* buttonMusSum;
      button* buttonMusDec;
      
      button* buttonSndSum;
      button* buttonSndDec;
      
      button* buttonLangSum;
      button* buttonLangDec;

      button* buttonCamSum;
      button* buttonCamDec;

      button* buttonReflSum;
      button* buttonReflDec;

      button* buttonResSum;
      button* buttonResDec;

      button* buttonAliasSum;
      button* buttonAliasDec;

      button* buttonFarViewSum;
      button* buttonFarViewDec;

      cxSel* cxSelFullScreen;

      cxSel* cxSelGrass;
      cxSel* cxSelParticles;

      cxSel* cxSelMultiTexture;
      cxSel* cxSelAnisotropic;

      cxSel* cxSelShowEnemyCircles;
      cxSel* cxSelAutoEndTurn;
      cxSel* cxSelAlwaysRun;

      button* buttonConfirm;
      button* buttonCancel;

      /*! Get the Current Language Name
       * \return the name of the language. */
      string languageName();

      /*! Get the Current Camera Name
       * \return the name of the camera. */
      string cameraName();

      /*! Get the Current Reflexion Type Name
       * \return the name of the reflexion type */
      string reflexionName();

      /*! Get the current resolution name
       * \return the name of the resolution */
      string resolutionName();

      /*! Get the Current antiAliasing Name
       * \return name of the antiAliasing */
      string antiAliasingName();

};


#endif

