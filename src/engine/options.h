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
      /*! Save actual options on file.
       * \param fileName -> name of the file to save */
      void save(string fileName);
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

      static int     musicVolume;     /**< Actual Music Volume */
      static int     sndfxVolume;     /**< Actual Sound Effects Volume */
      static int     langNumber;      /**< Actual Language Number */
      static int     cameraNumber;    /**< Actual Camera Mode */
      static bool    enableParticles; /**< Enable/Disable Particles */
      static bool    enableGrass;     /**< Enable/Disable Grass */
      static int     reflexionType;   /**< Reflexion Type */
      static int     screenWidth;     /**< Screen Height */
      static int     screenHeight;    /**< Screen Width */
      static bool    enableFullScreen;/**< If fullscreen is defined */

      window* intWindow;       /**< Options Window Pointer */


   private:
      SDL_Rect**   resolutions;    /**< the avaible resolutions */
      textBox* txtMusicVolume;     /**< Music Volume Text on Window*/
      textBox* txtSndfxVolume;     /**< Sound Effects Volume Text on Window*/
      textBox* txtLanguage;        /**< Language Text on Window */
      textBox* txtCamera;          /**< Camera Mode Text on Window */
      textBox* txtReflexion;       /**< Reflexion Type Text on Window */
      textBox* txtResolution;      /**< Resolution Text on Window */

      int          prevCamera;     /**< Previous used Camera Mode */
      int          prevMusicVolume;/**< Previous used Music Volume */
      int          prevSndfxVolume;/**< Previous Used Sound Effects Volume */
      int          prevLanguage;   /**< Previous Used Language */
      int          prevReflexion;  /**< Previous Used Reflexion */
      int          prevWidth;      /**< Previous Used Resolution Width */
      int          prevHeight;     /**< Previous Used Resolution Height */
      int          resPosition;    /**< Resolution Position on Vector */

      int timeLastOperation;       /**< Ticks when done last operation */

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

      cxSel* cxSelFullScreen;

      cxSel* cxSelGrass;
      cxSel* cxSelParticles;

      button* buttonConfirm;
      button* buttonCancel;

      /*! Get the Actual Language Name
       * \return the name of the language. */
      string languageName();

      /*! Get the Actual Camera Name
       * \return the name of the camera. */
      string cameraName();

      /*! Get the Actual Reflexion Type Name
       * \return the name of the reflexion type */
      string reflexionName();

      /*! Get the current resolution name
       * \return the name of the resolution */
      string resolutionName();

};


#endif

