#include "lang.h"
#include <iostream>
#include <fstream>

#define FILE_PORTUGUES "../data/lang/portugues.lng"
#define FILE_ESPANHOL "../data/lang/espanhol.lng"
#define FILE_FRANCES "../data/lang/frances.lng"
#define FILE_INGLES "../data/lang/ingles.lng"




lang::lang()
{
}

lang::~lang()
{
}

   
void lang::ReloadFile(int lng)
{
   std::ifstream file;
   string arqName;

   switch(lng)
   {
      case LANG_PORTUGUES:
      {
          arqName = FILE_PORTUGUES;
          break;
      }
      case LANG_ESPANHOL:
      {
          arqName = FILE_ESPANHOL;
          break;
      }
      case LANG_FRANCES:
      {
          arqName = FILE_FRANCES;
          break;
      }
      case LANG_INGLES:
      {
          arqName = FILE_INGLES;
          break;
      }
   }

   file.open(arqName.c_str(), ios::in | ios::binary);

   if(!file)
   {
       printf("Error while opening language file: %s\n",arqName.c_str());
       return;
   }

   /* Load strings */  
   getline(file, VERSION);

   getline(file, TEXTURE_INFORMATION);

   /* dccnit.cpp */
   getline(file, LOAD_NPC);
   getline(file, LOAD_PC);
   getline(file, LOAD_MAP);
   getline(file, LOAD_WINDOWS);
   getline(file, LOAD_PARTICLE);
   getline(file, LOAD_DONE);

   getline(file, OBJ_DOOR);
   getline(file, OBJ_NOTHING);

   getline(file, WINDOW_MAP);
   getline(file, WINDOW_SHORTCUTS);
   getline(file, WINDOW_SHORTCUTS_HELP);
   getline(file, WINDOW_SHORTCUTS_FPS);

   getline(file, INITIAL_NEW);
   getline(file, INITIAL_CONTINUE);
   getline(file, INITIAL_SAVE);
   getline(file, INITIAL_LOAD);
   getline(file, INITIAL_EXIT);
   getline(file, INITIAL_OPTIONS);

   /* options.cpp */
   getline(file, OPTIONS_TITLE);
   getline(file, OPTIONS_MUSIC_VOLUME);
   getline(file, OPTIONS_SNDFX_VOLUME);
   getline(file, OPTIONS_LANGUAGE);

   /* skillwindow.cpp */
   getline(file, SKILL_TITLE);
   getline(file, SKILL_FREE_POINTS);
   getline(file, SKILL_POINTS);
   getline(file, SKILL_COST);
   getline(file, SKILL_CONFIRM);
   getline(file, SKILL_CANCEL);

   /* fight */
   getline(file,FIGHT_ROLLS_INITIATIVE);
   getline(file,FIGHT_END_NOENEMIES);
   getline(file,FIGHT_ROUND_NEW);
   getline(file,FIGHT_ERROR_NO_CHARACTERS);
   getline(file,FIGHT_ATTACKS);
   getline(file,FIGHT_DEAD);
   getline(file,FIGHT_TURN);
   getline(file,FIGHT_EXIT);
   getline(file,FIGHT_SURPRISE_TURN);
   getline(file,FIGHT_NO_NPCS);

   /* feats */
   getline(file,FEAT_INVALID);
   getline(file,FEAT_HIT_FOR);
   getline(file,FEAT_CRITICAL_HIT);
   getline(file,FEAT_MISS);
   getline(file,FEAT_CRITICAL_MISS);
   getline(file,FEAT_POINTS);
   getline(file,FEAT_NO_MORE_POINTS);

   /* DIRECTORYS */
   getline(file, SKILLS_DIR);
   getline(file, FEATS_DIR);

   /* Errors */
   getline(file, ERROR_MAP_OUT);
   getline(file, ERROR_INIT_SDL);

   //fclose(file);

   file.close();

}


string lang::VERSION;
   string lang::TEXTURE_INFORMATION;

   
   string lang::LOAD_NPC;
   string lang::LOAD_PC;
   string lang::LOAD_MAP;
   string lang::LOAD_WINDOWS;
   string lang::LOAD_PARTICLE;
   string lang::LOAD_DONE;

   string lang::OBJ_DOOR;
   string lang::OBJ_NOTHING;

   string lang::WINDOW_MAP;
   string lang::WINDOW_SHORTCUTS;
   string lang::WINDOW_SHORTCUTS_HELP;
   string lang::WINDOW_SHORTCUTS_FPS;


   string lang::INITIAL_NEW;
   string lang::INITIAL_CONTINUE;
   string lang::INITIAL_SAVE;
   string lang::INITIAL_LOAD;
   string lang::INITIAL_EXIT;
   string lang::INITIAL_OPTIONS;

   string lang::OPTIONS_TITLE;
   string lang::OPTIONS_MUSIC_VOLUME;
   string lang::OPTIONS_SNDFX_VOLUME;
   string lang::OPTIONS_LANGUAGE;

   
   string lang::SKILL_TITLE;
   string lang::SKILL_FREE_POINTS;
   string lang::SKILL_POINTS;
   string lang::SKILL_COST;
   string lang::SKILL_CONFIRM;
   string lang::SKILL_CANCEL;

   /*fight */
   string lang::FIGHT_ROLLS_INITIATIVE;
   string lang::FIGHT_END_NOENEMIES;
   string lang::FIGHT_ROUND_NEW;
   string lang::FIGHT_ERROR_NO_CHARACTERS;
   string lang::FIGHT_ATTACKS;
   string lang::FIGHT_DEAD;
   string lang::FIGHT_TURN;
   string lang::FIGHT_EXIT;
   string lang::FIGHT_SURPRISE_TURN;
   string lang::FIGHT_NO_NPCS;

   /* feats */
   string lang::FEAT_INVALID;
   string lang::FEAT_HIT_FOR;
   string lang::FEAT_CRITICAL_HIT;
   string lang::FEAT_MISS;
   string lang::FEAT_CRITICAL_MISS;
   string lang::FEAT_POINTS;
   string lang::FEAT_NO_MORE_POINTS;

   /* DIRECTORYS */
   string lang::SKILLS_DIR;
   string lang::FEATS_DIR;

   /* Errors */
   string lang::ERROR_MAP_OUT;
   string lang::ERROR_INIT_SDL;

