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
      janela* window;          /**< Options Window Pointer */


   private:
      string       fileName;       /**< options fileName to be used */
      quadroTexto* txtMusicVolume; /**< Music Volume Text on Window*/
      quadroTexto* txtSndfxVolume; /**< Sound Effects Volume Text on Window*/
      quadroTexto* txtLanguage;    /**< Language Text on Window */
      quadroTexto* txtCamera;      /**< Camera Mode Text on Window */
      quadroTexto* txtReflexion;   /**< Reflexion Type Text on Window */
      int          prevCamera;     /**< Previous used Camera Mode */
      int          prevMusicVolume;/**< Previous used Music Volume */
      int          prevSndfxVolume;/**< Previous Used Sound Effects Volume */
      int          prevLanguage;   /**< Previous Used Language */
      int          prevReflexion;  /**< Previous Used Reflexion */
      lang         language;       /**< Language Used */

      int timeLastOperation;       /**< Ticks when done last operation */

      botao* buttonMusSum;
      botao* buttonMusDec;
      
      botao* buttonSndSum;
      botao* buttonSndDec;
      
      botao* buttonLangSum;
      botao* buttonLangDec;

      botao* buttonCamSum;
      botao* buttonCamDec;

      botao* buttonReflSum;
      botao* buttonReflDec;

      cxSel* cxSelGrass;
      cxSel* cxSelParticles;
      
      botao* buttonConfirm;
      botao* buttonCancel;

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

