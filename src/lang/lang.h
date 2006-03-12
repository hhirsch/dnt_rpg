#ifndef _lang_h
#define _lang_h

#include <string>
using namespace std;


#define LANG_PORTUGUES 1
#define LANG_ESPANHOL  2
#define LANG_FRANCES   3
#define LANG_INGLES    4

class lang
{
  public:
     lang();
     ~lang();

     void ReloadFile(int lng);

   static string VERSION;
   static string TEXTURE_INFORMATION;

   /* dccnit.cpp */
   static string LOAD_NPC;
   static string LOAD_PC;
   static string LOAD_MAP;
   static string LOAD_WINDOWS;
   static string LOAD_DONE;

   static string OBJ_DOOR;
   static string OBJ_NOTHING;

   static string WINDOW_MAP;
   static string WINDOW_SHORTCUTS;
   static string WINDOW_SHORTCUTS_HELP;
   static string WINDOW_SHORTCUTS_FPS;


   static string INITIAL_NEW;
   static string INITIAL_CONTINUE;
   static string INITIAL_SAVE;
   static string INITIAL_LOAD;
   static string INITIAL_EXIT;
   static string INITIAL_OPTIONS;

   /* options.cpp */
   static string OPTIONS_TITLE;
   static string OPTIONS_MUSIC_VOLUME;
   static string OPTIONS_SNDFX_VOLUME;

   /* skillwindow.cpp */
   static string SKILL_TITLE;
   static string SKILL_FREE_POINTS;
   static string SKILL_POINTS;
   static string SKILL_COST;
   static string SKILL_CONFIRM;
   static string SKILL_CANCEL;

   /* DIRECTORYS */
   static string SKILLS_DIR;

   /* Errors */
   static string ERROR_MAP_OUT;
   static string ERROR_INIT_SDL;
   
};




//#include "../lang/portugues.h"
//#include "../lang/espanhol.h"
//#include "../lang/ingles.h"
//#include "../lang/frances.h"


#endif

