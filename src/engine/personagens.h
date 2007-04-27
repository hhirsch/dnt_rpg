/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#ifndef _personagens_h
#define _personagens_h

#include <string>
using namespace std;

#include "../gui/desenho.h"
#include "../gui/figura.h"
#include "../gui/objeto.h"
#include "../gui/lista.h"
#include "../gui/healthBar.h"
#include "../classes/thing.h"
#include "../classes/feats.h"
#include "../classes/classes.h"
#include "../classes/race.h"
#include "../classes/inventory.h"
#include "../ia/astar.h"
#include "../etc/animodel.h"
#include "../map/map.h"

#define POSRETX 8   /**< X screen portrait position */
#define POSRETY 20  /**< Y screen portrait position */

#define STATE_WALK  1 /**< Character Animation State Walking */

#define MAX_DISTINCT_CLASSES 3 /**< Max Different Classes for MultiClass */



/*! Character Class */
class personagem: public Tobjeto, public thing, public aniModel
{
   public:
      /*! Constructor
       * \param ft -> pointer to all feats list */
      personagem(featsList* ft);
      /*! Destructor */
      ~personagem();

      /*! Define the image used as portrait
       * \param portraitFile -> file name of the image to use*/
      void definePortrait(string portraitFile);

      /*! Gets the filename of the image used as portrait
       * \return -> file name of the image used */
      string getPortraitFileName();

      /*! Draw the Portrait at the world position
       * \param x1 -> x1 position 
       * \param y1 -> y1 position
       * \param z1 -> z1 position
       * \param x2 -> x2 position 
       * \param y2 -> y2 position
       * \param z2 -> z2 position
       * \param x3 -> x3 position 
       * \param y3 -> y3 position
       * \param z3 -> z3 position
       * \param x4 -> x4 position 
       * \param y4 -> y4 position
       * \param z4 -> z4 position*/
      void drawMainPortrait(GLdouble x1, GLdouble y1, GLdouble z1,
                                  GLdouble x2, GLdouble y2, GLdouble z2,
                                  GLdouble x3, GLdouble y3, GLdouble z3,
                                  GLdouble x4, GLdouble y4, GLdouble z4);

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

      /*! Clear Skills */
      void clearSkills();

      Tobjeto *actualWeapon;    /**< Actual weapon */

      classe* actualClass[MAX_DISTINCT_CLASSES]; /**< Pointer to each class */
      int classLevels[MAX_DISTINCT_CLASSES]; /**< Current level of each class */
      race* actualRace;         /**< Pointer to Race */
      align* actualAlign;       /**< Pointer to Align */
      feats actualFeats;        /**< Feats owned by character */

      string nome;              /**< Character's name */

      int actualFightGroup;     /**< fightGroup of character, used on battles*/
      personagem* actualEnemy;  /**< Pointer to actual fight enemy (target) */
      healthBar* lifeBar;       /**< Character's Life Bar */

      aStar pathFind;           /**< The A* pathFind to the character */
      Square* ocupaQuad;        /**< Square occuped by character */

      inventory* inventories[INVENTORY_PER_CHARACTER]; /**< Inventory */

      /*! Open, if exists and not opened, the conversation dialog */
      void openConversationDialog(interface* gui, personagem * PC);

      /*! Create the conversation (load all things, but not open the dialog)
       * \param pEngine -> pointer to current engine */
      void createConversation(void* pEngine);

      /*! Treat Events on Conversation Window. 
       * \param guiObject -> active GUI object
       * \param eventInfo -> last GUI Event 
       * \param gui -> window interface used
       * \return true if event is threated, false otherwise. */
      bool treatConversation(Tobjeto* guiObject, int eventInfo, interface* gui);


      

   protected:
       /*! Define max value of the lifePoints
       * \param maxPoints -> new max points to life points */
      void defineMaxLifePoints(int maxPoints);

      /*! Get points based on calculation
       * \param pt -> calculation info
       * \return points */
      int getPoints(points pt);

      /*! Call the thing dead animation */
      void callDeadAnimation();

      string retratoConversa;     /**< Portrait talk file name */
      SDL_Surface* portraitImage; /**< Character's portrait image */
      GLuint portrait;            /**< Up screen portrait GL texture */
      string conversationFile;    /**< Name of the Conversation File */
      void* conv;                 /**< Pointer to the conversation */
};


/*! Character's List */
class Lpersonagem
{
   public:
      /*! List Constructor */
      Lpersonagem();
      /*! List Destructor */
      ~Lpersonagem();

      /*!
       * Insert one character on list
       * \param file -> file name of the character to insert
       * \param ft -> featsList of all feats on game 
       * \param pEngine -> pointer to current engine
       * \return pointer to opened character*/
      personagem* insertCharacter(string file, featsList* ft, void* pEngine);
                                    
      /*!
       * Remove one character from list
       * \param persona -> character pointer to remove */
      void removeCharacter(personagem* persona);

      /*!
       * Gets hostile characters from the list
       * \param last -> last hostile taken
       * \return pointer to the hostile character
       */
      personagem* getEnemyCharacter(personagem* last);

      /*! Get the active character
       * \return pointer to the active character */
      personagem* getActiveCharacter();

      /*! Set the active character
       * \param character -> pointer to the new active character */
      void setActiveCharacter(personagem* character);

      int getTotal(){return(total);};

      personagem* primeiro;         /**< List Node Pointer */

   protected:
      personagem* activeCharacter;  /**< Active Character's on list */
      int total;                    /**< Total Elements on List */

};

#endif

