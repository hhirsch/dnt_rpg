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
      void displayOptionsScreen(interface* interf);
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
      int treat(guiObject* object, int eventInfo, interface* interf,
                GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Set language to one defined at the option's file */
      void setLanguage();

      /*! Get all avaible resolutions at fullscreen mode */
      void getAvaibleResolutions();

      /*! Verify if the current language is an Unicode One (like russian,
       * japanese, chinese, etc.) */
      bool isLanguageUnicode();

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

      window* intWindow;       /**< Options Window Pointer */


   private:
      SDL_Rect**   resolutions;    /**< the avaible resolutions */
      textBox* txtMusicVolume;     /**< Music Volume Text on Window*/
      textBox* txtSndfxVolume;     /**< Sound Effects Volume Text on Window*/
      textBox* txtLanguage;        /**< Language Text on Window */
      textBox* txtCamera;          /**< Camera Mode Text on Window */
      textBox* txtReflexion;       /**< Reflexion Type Text on Window */
      textBox* txtResolution;      /**< Resolution Text on Window */
      textBox* txtAntiAliasing;    /**< AntiAliasing Text on Window */

      int      prevCamera;         /**< Previous used Camera Mode */
      int      prevMusicVolume;    /**< Previous used Music Volume */
      int      prevSndfxVolume;    /**< Previous Used Sound Effects Volume */
      int      prevLanguage;       /**< Previous Used Language */
      int      prevReflexion;      /**< Previous Used Reflexion */
      int      prevWidth;          /**< Previous Used Resolution Width */
      int      prevHeight;         /**< Previous Used Resolution Height */
      int      prevAntiAliasing;   /**< Previous Used AntiAliasing */
      int      resPosition;        /**< Resolution Position on Vector */

      int timeLastOperation;       /**< Ticks when done last operation */

      static string fileName;      /**< The current options fileName */            

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

      cxSel* cxSelFullScreen;

      cxSel* cxSelGrass;
      cxSel* cxSelParticles;

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

