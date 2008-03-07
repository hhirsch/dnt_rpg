/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#ifndef _character_h
#define _character_h

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
#include "../ia/astar.h"
#include "../etc/animodel.h"
#include "../map/map.h"
#include "barterwindow.h"

#define POSRETX 8   /**< X screen portrait position */
#define POSRETY 20  /**< Y screen portrait position */

#define STATE_WALK  1 /**< Character Animation State Walking */

#define MAX_DISTINCT_CLASSES 3 /**< Max Different Classes for MultiClass */

/*! The character util model class. Usually used to retain weapons 
 * and armors meshes when they are with the character. */
class characterUtilModel
{
   public:
      int meshID;          /**< The cal3D mesh id loaded */
      string modelName;    /**< The name of the model loaded (for example, 
                                it is the weapon name, or the armor name) */
      string soundFile;    /**< The sound file (ogg). It's the sound that
                                will be played when actions on the UtilModel. */
};


/*! Character Class */
class character: public aniModel
{
   public:
      /*! Constructor
       * \param ft -> pointer to all feats list */
      character(featsList* ft);
      /*! Destructor */
      ~character();

      /*! Get all the attributes modifiers for the character
       * \param mods -> vector that will contain the attribute modifiers */
      void getAttModifiers(int mods[6]);

      /*! Get the total level of the character (AKA sum of all classes levels
       * the character has.
       * \return total level of the character */
      int getLevel();

      /*! Define the image used as portrait
       * \param portraitFile -> file name of the image to use*/
      void definePortrait(string portraitFile);

      /*! Gets the filename of the image used as portrait
       * \return -> file name of the image used */
      string getPortraitFileName();

      /*! Draw the Portrait */
      void drawMainPortrait();

      /*! Define actual life points value
       * \param newLife -> new value to the lifepoints*/
      void defineActualLifePoints(int newLife);

      /*! Get First Level Skill Points
       * \param multiClassNumber -> number of the class in the multiclass
       * \return -> total points at first level */
      int getFirstLevelSkillPoints(int multiClassNumber);

      /*! Get Other Levels Skill Points
       * \param multiClassNumber -> number of the class in the multiclass
       * \return -> total points at first level */
      int getOtherLevelSkillPoints(int multiClassNumber);

      /*! Set the conversation file
       * \param file -> name of the conversation file*/
      void setConversationFile(string file);
      
      /*! Get the conversation file
       * \return -> conversation fileName */
      string getConversationFile();

      /*! Define Character initial life points, based on its class */
      void defineInitialLifePoints();

      /*! Define the character weapon, based on the current one at
       * the inventory. If none is there, the current weapon is bare
       * hands. */
      void defineWeapon();

      /*! Clear Skills */
      void clearSkills();

      classe* actualClass[MAX_DISTINCT_CLASSES]; /**< Pointer to each class */
      int classLevels[MAX_DISTINCT_CLASSES]; /**< Current level of each class */
      race* actualRace;         /**< Pointer to Race */
      align* actualAlign;       /**< Pointer to Align */
      feats actualFeats;        /**< Feats owned by character */

      string name;              /**< Character's name */

      int actualFightGroup;     /**< fightGroup of character, used on battles*/
      character* actualEnemy;   /**< Pointer to actual fight enemy (target) */
      healthBar* lifeBar;       /**< Character's Life Bar */

      aStar pathFind;           /**< The A* pathFind to the character */
      Square* ocSquare;         /**< Square occuped by character */

      inventory* inventories;   /**< Inventory */

      /*! Set character orientation value
       * \param ori -> new orientation value */
      void setOrientation(GLfloat ori);

      /*! Open, if exists and not opened, the conversation dialog */
      void openConversationDialog(interface* gui, character *PC);

      /*! Create the conversation (load all things, but not open the dialog)
       * \param pEngine -> pointer to current engine */
      void createConversation(void* pEngine);
   
      /*! Verify if the conversation dialog is opened
       * \return true if the conversation dialog is opened */
      bool isConversationOpened();

      /*! Treat Events on Conversation Window. 
       * \param guiObj -> active GUI object
       * \param eventInfo -> last GUI Event 
       * \param gui -> window interface used
       * \param tradeWindow -> pointer to the pointer of the barterWindow
       * \param infoW -> pointer to the used itemWindow 
       * \return true if event is threated, false otherwise. */
      bool treatConversation(guiObject* guiObj, int eventInfo, interface* gui,
                             barterWindow** tradeWindow, itemWindow* infoW);

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

       /*! Get the character filename
        * \return -> the character filename */
       string getCharacterFile(){return(characterFile);};
   
       /*! Set the character file
        * \param fileName -> new fileName of the character */
       void setCharacterFile(string fileName){characterFile = fileName;}

       /*! Get the range type of the active attack feat
        * \return range type of the actual attack feat */
       int getActiveFeatRangeType();
      
       /*! Get the active attack feat range
        * \return range value in meters */ 
       int getActiveFeatRange();

       character* next;             /**< Next Character on List */
       character* previous;         /**< Previous Character on List */

       /*! Get the general script pointer
        * \return genereal iaScript pointer */
       void* getGeneralScript();    

       /*! Define the character list as friend class */
      friend class characterList;
       
   protected:
       /*! Define max value of the lifePoints
       * \param maxPoints -> new max points to life points */
      void defineMaxLifePoints(int maxPoints);

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
      string conversationFile;    /**< Name of the Conversation File */
      void* conv;                 /**< Pointer to the conversation */
      bool convPressed;           /**< To avoid do some conversation action
                                       before releases the left mouse button */

      void* generalScript;         /**< Pointer to the general iaScript */
      string generalScriptFileName;/**< The General Script Filename */


      characterUtilModel headModel;       /**< Head Model */
      characterUtilModel leftHandModel;   /**< Left Hand Model */
      characterUtilModel rightHandModel;  /**< Right Hand Model */
      characterUtilModel leftFingerModel; /**< Left Finger Model */
      characterUtilModel rightFingerModel;/**< Right Finger Model */
      characterUtilModel neckModel;       /**< Neck Model */
      characterUtilModel footModel;       /**< Foot Model */
      characterUtilModel bodyModel;       /**< Body (Torso) Model */

};


/*! Character's List */
class characterList
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
       * \return pointer to opened character*/
      character* insertCharacter(string file, featsList* ft, void* pEngine);
                                    
      /*!
       * Remove one character from list
       * \param persona -> character pointer to remove */
      void removeCharacter(character* persona);

      /*!
       * Gets hostile characters from the list
       * \param last -> last hostile taken
       * \return pointer to the hostile character
       */
      character* getEnemyCharacter(character* last);

      /*! Get the active character
       * \return pointer to the active character */
      character* getActiveCharacter();

      /*! Set the active character
       * \param dude -> pointer to the new active character */
      void setActiveCharacter(character* dude);

      /*! Get the total number of characters on list
       * \return current number of characters on list */
      int getTotal(){return(total);};

      /*! Treat Character's general scripts
       * \apram actualMap -> current opened map */
      void treatGeneralScripts(Map* actualMap);

      character* first;         /**< List Node Pointer */

   protected:
      character* activeCharacter;  /**< Active Character's on list */
      int total;                   /**< Total Elements on List */

      character* curTreat;         /**< Current NPC To treat Scripts */

};

#endif

