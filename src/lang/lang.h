#ifndef _lang_h
#define _lang_h

#include <string>
using namespace std;

#define LANG_FIRST     1 /**< First Language*/
#define LANG_LAST      4 /**< Las Language */

#define LANG_PORTUGUES 4 /**< Portuguese Language */
#define LANG_ESPANHOL  2 /**< Spanish Language */
#define LANG_FRANCES   3 /**< French Language */
#define LANG_INGLES    1 /**< English Language */

/*! Language Internationalization Class */
class lang
{
  public:
     /*! Constructor */
     lang();
     /*! Destructor */
     ~lang();
 
     /*! Reload the language file
      * \param lng -> language ID of language to load. */
     void ReloadFile(int lng);

   static string VERSION;
   static string TEXTURE_INFORMATION;

   /* dccnit.cpp */
   static string LOAD_NPC;
   static string LOAD_PC;
   static string LOAD_MAP;
   static string LOAD_WINDOWS;
   static string LOAD_PARTICLE;
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
   static string OPTIONS_LANGUAGE;
   static string OPTIONS_CAMERA;
   static string OPTIONS_CAMERA_NORMAL;
   static string OPTIONS_CAMERA_DRIVE;
   static string OPTIONS_GRASS;
   static string OPTIONS_PARTICLES;

   /* skillwindow.cpp */
   static string SKILL_TITLE;
   static string SKILL_FREE_POINTS;
   static string SKILL_POINTS;
   static string SKILL_COST;
   static string SKILL_CONFIRM;
   static string SKILL_CANCEL;

   /* attwindow.cpp */
   static string ATTW_TITLE;
   static string ATTW_ROLL;
   static string ATTW_CLEAR;
   static string ATTW_STRENGTH;
   static string ATTW_DEXTERITY;
   static string ATTW_CONSTITUTION;
   static string ATTW_INTELLIGENCE;
   static string ATTW_WISDOW;
   static string ATTW_CHARISM;
   static string ATTW_DESCRIPTION;
   static string ATTW_WARN_TITLE;
   static string ATTW_WARN_MESSAGE;

   /* alignwindow.cpp */
   static string ALIGNW_TITLE;
   
   /* racewindow.cpp */
   static string RACEW_TITLE;
   static string RACEW_DESCRIPTION;
   static string RACEW_CHARACTERISTICS;
   static string RACEW_MODIFIERS;
   static string RACEW_NO_MODIFIERS;
   static string RACEW_SKILLS;
   static string RACEW_NO_SKILLS;
   static string RACEW_FEATS;
   static string RACEW_NO_FEATS;

   /* classwindow.cpp */
   static string CLASSW_TITLE;
   static string CLASSW_DESCRIPTION;
   static string CLASSW_CHARACTERISTICS;
   static string CLASSW_LIFE_DICE;
   static string CLASSW_SKILL_POINTS;
   static string CLASSW_FIRST_LEVEL;
   static string CLASSW_OTHER_LEVELS;
   static string CLASSW_MODIFIERS;
   static string CLASSW_FEATS;
   static string CLASSW_SKILLS;

   /* inventwindow.cpp */
   static string INVENTW_TITLE;
   static string INVENTW_DROP;
   static string INVENTW_SELL;
   static string INVENTW_USE;
   static string INVENTW_GET;
      
   /* figths */
   static string FIGHT_ROLLS_INITIATIVE;
   static string FIGHT_END_NOENEMIES;
   static string FIGHT_ROUND_NEW;
   static string FIGHT_ERROR_NO_CHARACTERS;
   static string FIGHT_ATTACKS;
   static string FIGHT_DEAD;
   static string FIGHT_TURN;
   static string FIGHT_EXIT;
   static string FIGHT_SURPRISE_TURN;
   static string FIGHT_NO_NPCS;

   /* feats */
   static string FEAT_INVALID;
   static string FEAT_HIT_FOR;
   static string FEAT_CRITICAL_HIT;
   static string FEAT_MISS;
   static string FEAT_CRITICAL_MISS;
   static string FEAT_POINTS;
   static string FEAT_NO_MORE_POINTS;

   /* DIRECTORYS */
   static string SKILLS_DIR;
   static string FEATS_DIR;
   static string ALIGN_DIR;
   static string RACE_DIR;
   static string CLASS_DIR;

   /* Errors */
   static string ERROR_MAP_OUT;
   static string ERROR_INIT_SDL;
   
};

#endif

