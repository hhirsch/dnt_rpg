#ifndef _options_h
#define _options_h

#include "../gui/farso.h"
#include "../lang/lang.h"
#include <string>
using namespace std;


#define OPTIONSW_CANCEL  0
#define OPTIONSW_CONFIRM 1
#define OPTIONSW_OTHER   2

class options
{
   public:
      options(string file);
      ~options();

      void DisplayOptionsScreen(interface* inter);
      void Save();
      int Treat(interface* interf);

      int     musicVolume;
      int     sndfxVolume;
      int     langNumber;
      janela* window;


   private:
      string       fileName;
      quadroTexto* txtMusicVolume;
      quadroTexto* txtSndfxVolume;
      int          prevMusicVolume;
      int          prevSndfxVolume;
      lang         language;
};


#endif

