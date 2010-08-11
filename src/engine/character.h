/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_character_h
#define _dnt_character_h

#include <string>
using namespace std;

#include "../gui/draw.h"
#include "../gui/picture.h"
#include "../gui/healthBar.h"
#include "../classes/thing.h"
#include "../classes/feats.h"
#include "../classes/classes.h"
#include "../classes/race.h"
#include "../classes/inventory.h"
#include "../classes/modifier.h"
#include "../ia/astar.h"
#include "../etc/animodel.h"
#include "../etc/list.h"
#include "../map/map.h"

#include "itemwindow.h"
#include "sun.h"

#define POSRETX 8   /**< X screen portrait position */
#define POSRETY 20  /**< Y screen portrait position */

#define MAX_DISTINCT_CLASSES 3 /**< Max Different Classes for MultiClass */


/*! Character Class */
class character: public aniModel, public dntListElement
{
   public:
      /*! Constructor
       * \param ft -> pointer to all feats list
       * \param usedEngine -> pointer to the current engine */
      character(featsList* ft, void* usedEngine);
      /*! Destructor */
      ~character();

      /*! Get all the attributes modifiers for the character
       * \param mods -> vector that will contain the attribute modifiers */
      void getAttModifiers(int mods[6]);

      /*! Get the total level of the character (AKA sum of all classes levels
       * the character has.
       * \return total level of the character */
      int getLevel();

      /*! Get the character level for a specific class
       * \param classId -> id of the class
       * \return -> number of levels the character has on class */
      int getLevel(string classId);

      /*! Get the character level for a specific class
       * \param cl -> clas to get how many levels the character has
       * \return -> number of lvels the character has on class cl */
      int getLevel(classe* cl);

      /*! Get the current active (selected) feat for the character
       * \return current active feat number */
      int getActiveFeat();

      /*! Get the current active feat pointer
       * \return pointer to the active feat */
      feat* getActiveFeatPtr();

      /*! Set the current character active feat
       * \param f -> pointer to the feat to active */
      void setActiveFeat(feat* f);

      /*! Set the current character active feat
       * \param f -> number of the feat to active */
      void setActiveFeat(int f);

      /*! Get the equiped character weapon (if any)
       * \return poitner to the equiped weapon (or NULL) */
      weapon* getEquipedWeapon();

      /*! Verify if the character alignment is of type represented by al
       * \param al -> string with align type
       * \return -> true if the align string identifier have the string al
       *            in it, false otherwise. 
       * \note -> a common use is, for example: isAlignOf("FREE_SOFTWARE") */
      bool isAlignOf(string al);

      /*! Define the image used as portrait
       * \param portraitFile -> file name of the image to use*/
      void definePortrait(string portraitFile);

      /*! Get the filename of the image used as portrait
       * \return -> file name of the image used */
      string getPortraitFileName();

      /*! Get the filename of the inventory defined to the character
       * \return -> string with inventory file name or "" */
      string getInventoryFile();

      /*! Draw the Portrait */
      void drawMainPortrait();

      /*! Get First Level Skill Points
       * \param cl -> pointer to the class
       * \return -> total points at first level */
      int getFirstLevelSkillPoints(classe* cl);

      /*! Get Other Levels Skill Points
       * \param cl -> pointer to the class
       * \return -> total points at first level */
      int getOtherLevelSkillPoints(classe* cl);

      /*! Define Character initial life points, based on its class */
      void defineInitialLifePoints();

      /*! Update the health bar draw */
      void updateHealthBar();

      /*! Verify if the character can take levels on a specific class
       * \param cl -> class to verify if can take levels 
       * \return -> true if can take a level, false otherwise */
      bool canClass(classe* cl);

      /*! Get a new level at the class
       * \param cl -> pointer to the class to get a new level
       * \note -> will only take a level if have enought XP
       *          and can take a level at the specified class */
      void getNewClassLevel(classe* cl);

      /*! Verify if a character can have a feat or not (based on its
       * pre-requisites)
       * \param f -> pointer to the featDescription to verify
       * \return true if can  */
      bool canHaveFeat(featDescription* f);

      /*! Verify if already have a feat
       * \param featId -> identifier of the feat
       * \return true if already have the feat */
      bool haveFeat(string featId);

      /*! Clear Skills */
      void clearSkills();

      /*! Set character orientation value
       * \param ori -> new orientation value */
      void setOrientation(GLfloat ori);

      /*! Kill the character without calling dead animation
       * \note this is usually used at modState  */
      void instantKill();

      /*! Call the thing dead animation */
      void callDeadAnimation();

      /*! Call the thing attack animation */
      void callAttackAnimation();

      /*! Call the thing attack animation */
      void callIdleAnimation();

      /*! Delete the Existed Inventory and Create a new One. 
       *  Usually called after death*/
      void newInventory();

      /*! Apply all Race and Classes Skills Costs */
      void applySkillCosts();

      /*! Apply Bonus And Saves from Classes to the character */
      void applyBonusAndSaves();

      /*! Get the character filename
       * \return -> the character filename */
      string getCharacterFile(){return(characterFile);};

      /*! Set the character file
       * \param fileName -> new fileName of the character */
      void setCharacterFile(string fileName){characterFile = fileName;}

      /*! Save the character to a new file 
       * \param fileName -> name of the file to save
       * \return-> true if can save, false otherwise */
      bool save(string fileName);

      /*! Get the active attack feat range
       * \return range value in meters */ 
      int getActiveFeatRange();

      /*! Insert the default needed feats (for example: base attack feat)
       * \param ft -> pointer to all available feats on game */
      void insertDefaultNeededFeats(featsList* ft);

      /*! Get the general script pointer
       * \return genereal iaScript pointer */
      void* getGeneralScript();

      /*! Add a modEffect to the character
       * \param mod -> modifier value
       * \param time -> time to expire (0 == forever)
       * \param periodicTime -> time to apply the modEffect again 
       *                        (0 to not re-apply)
       * \param factorId -> id of the target factor
       * \param factorType -> type of the target factor */
      void addModEffect(int mod, int time, int periodicTime,
            string factorId, string factorType);

      /*! Remove all modEffects from the character */
      void removeAllModEffects();

      /*! Get the modEffectList pointer
       * \return pointer to the modEffectList */
      modEffectList* getEffects();

      /*! Define the current occuped square by the character
       * \param curMap -> pointer to the current map opened */
      void defineOcSquare(Map* curMap);

      /*! Render the character
       * \param updateAnimations -> true to update animation state
       * \param reflexion -> true to render reflexion
       * \param shadow -> tru to render shadow
       * \param gameSun -> pointer to the sun */
      void render(bool updateAnimations, bool reflexion, bool shadow,
            sun* gameSun);

      /*! Get the damage dice of character 'empty hands'
       * \return bared hands damage dice */
      diceThing getBaredHandsDice();

      /*! Define the character list as friend class */
      friend class characterList;

      classe* actualClass[MAX_DISTINCT_CLASSES]; /**< Pointer to each class */
      int classLevels[MAX_DISTINCT_CLASSES]; /**< Current level of each class */
      race* actualRace;         /**< Pointer to Race */
      align* actualAlign;       /**< Pointer to Align */
      feats* actualFeats;       /**< Feats owned by character */      

      int actualFightGroup;     /**< fightGroup of character, used on battles*/

      healthBar* lifeBar;       /**< Character's Life Bar */

      aStar pathFind;           /**< The A* pathFind to the character */
      Square* ocSquare;         /**< Square occuped by character */

      inventory* inventories;   /**< Inventory */

   protected:
      /*! Get points based on calculation
       * \param pt -> calculation info
       * \return points */
      int getPoints(points pt);

      string talkPortrait;        /**< Portrait talk file name */
      string characterFile;       /**< Name of the character file */
      float portraitPropX;        /**< X proportion for portrait texture */
      float portraitPropY;        /**< Y proportion for portrait texture */
      SDL_Surface* portraitImage; /**< Character's portrait image */
      GLuint portraitTexture;     /**< The Portrait Texture */
      string inventoryFile;       /**< The inventory file name */

      int activeFeat;             /**< The character active feat */
      
      diceThing bareHandsDice;    /**< Damage by bare hands */

      modEffectList* effects;     /**< Current character effects */

      void* generalScript;         /**< Pointer to the general iaScript */
      string generalScriptFileName;/**< The General Script Filename */
};


/*! Character's List */
class characterList: public dntList
{
   public:
      /*! List Constructor */
      characterList();
      /*! List Destructor */
      ~characterList();

      /*!
       * Insert one character on list
       * \param file -> file name of the character to insert
       * \param ft -> featsList of all feats on game 
       * \param pEngine -> pointer to current engine
       * \param curMap -> fileName of the current map
       * \return pointer to opened character*/
      character* insertCharacter(string file, featsList* ft, 
            void* pEngine, string curMap);

      /*!
       * Remove one character from list
       * \param persona -> character pointer to remove */
      void removeCharacter(character* dude);

      /*!
       * Gets hostile characters from the list
       * \param last -> last hostile taken
       * \return pointer to the hostile character
       */
      character* getEnemyCharacter(character* last);

      /*! Get the active character
       * \return pointer to the active character */
      character* getActiveCharacter();

      /*! Get the first character with fileName on the list
       * \param fileName -> fileName of the character to get
       * \return -> characte pointer or NULL */
      character* getCharacter(string fileName);

      /*! Get the next character with the same model from the list
       * \param ch -> character to get next witg same model
       * \return -> next character with same model or NULL if
       *            end of the list was reached. */
      character* getNextSameCharacter(character* ch);

      /*! Set the active character
       * \param dude -> pointer to the new active character */
      void setActiveCharacter(character* dude);

      /*! Treat Character's general scripts
       * \param actualMap -> current opened map
       * \param NPCs -> current NPCs list  */
      void treatGeneralScripts(Map* actualMap, characterList* NPCs);

      /*! Verify if the character is in the list
       * \paarm ch -> pointer to the character
       * \return -> true if the character is in the list */
      bool isCharacterIn(character* ch);

   protected:

      /*! Call the delete for a character
       * \param dude -> pointer to the character */
      void freeElement(dntListElement* dude);


      character* activeCharacter;  /**< Active Character's on list */
      character* curTreat;         /**< Current NPC To treat Scripts */

};

#endif

