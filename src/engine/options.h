#ifndef _options_h
#define _options_h

#include "../gui/farso.h"
#include "../lang/lang.h"
#include <string>
using namespace std;


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
      int Treat(Tobjeto* object, int eventInfo, interface* interf);

      int     musicVolume; /**< Actual Music Volume */
      int     sndfxVolume; /**< Actual Sound Effects Volume */
      int     langNumber;  /**< Actual Language Number */
      janela* window;      /**< Options Window Pointer */


   private:
      string       fileName;       /**< options fileName to be used */
      quadroTexto* txtMusicVolume; /**< Music Volume Text on Window*/
      quadroTexto* txtSndfxVolume; /**< Sound Effects Volume Text on Window*/
      quadroTexto* txtLanguage;    /**< Language Text on Window */
      int          prevMusicVolume;/**< Previous used Music Volume */
      int          prevSndfxVolume;/**< Previous Used Sound Effects Volume */
      int          prevLanguage;   /**< Previous Used Language */
      lang         language;       /**< Language Used */

      int timeLastOperation;       /**< Ticks when done last operation */

      botao* buttonMusSum;
      botao* buttonMusDec;
      botao* buttonSndSum;
      botao* buttonSndDec;
      botao* buttonLangSum;
      botao* buttonLangDec;
      botao* buttonConfirm;
      botao* buttonCancel;

      /*! Get the Actual Language Name
       * \return the name of the language. */
      string languageName();
};


#endif

