#ifndef _options_h
#define _options_h

#include "../gui/farso.h"
#include "../lang/lang.h"
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
      /*! options Constructor that will load options from file.
       * \param file -> filename with the options file to be loaded. */
      options(string file);
      /*! options destructor */
      ~options();

      /*! Display the Options Screen 
       * \param interf -> GUI interface to be used.*/
      void DisplayOptionsScreen(interface* interf);
      /*! Save actual options on file. */
      void Save();
      /*! Treat options Window Events */
      int Treat(guiObject* object, int eventInfo, interface* interf);

      int     musicVolume;     /**< Actual Music Volume */
      int     sndfxVolume;     /**< Actual Sound Effects Volume */
      int     langNumber;      /**< Actual Language Number */
      int     cameraNumber;    /**< Actual Camera Mode */
      bool    enableParticles; /**< Enable/Disable Particles */
      bool    enableGrass;     /**< Enable/Disable Grass */
      int     reflexionType;   /**< Reflexion Type */
      window* intWindow;       /**< Options Window Pointer */


   private:
      string       fileName;       /**< options fileName to be used */
      textBox* txtMusicVolume;     /**< Music Volume Text on Window*/
      textBox* txtSndfxVolume;     /**< Sound Effects Volume Text on Window*/
      textBox* txtLanguage;        /**< Language Text on Window */
      textBox* txtCamera;          /**< Camera Mode Text on Window */
      textBox* txtReflexion;       /**< Reflexion Type Text on Window */
      int          prevCamera;     /**< Previous used Camera Mode */
      int          prevMusicVolume;/**< Previous used Music Volume */
      int          prevSndfxVolume;/**< Previous Used Sound Effects Volume */
      int          prevLanguage;   /**< Previous Used Language */
      int          prevReflexion;  /**< Previous Used Reflexion */
      lang         language;       /**< Language Used */

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

};


#endif

