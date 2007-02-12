/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#ifndef _personagens_h
#define _personagens_h

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
#include <string>
using namespace std;
#include "../map/map.h"

#define POSRETX 8   /**< X screen portrait position */
#define POSRETY 20  /**< Y screen portrait position */

#define STATE_WALK  1 /**< Character Animation State Walking */

#define MAX_DISTINCT_CLASSES 3 /**< Max Different Classes for MultiClass */



/*! Character Class */
class personagem: public Tobjeto, public thing, public aniModel
{
   public:
      Tlista *portraits;        /**< All character's portraits */
      Tlista *objects;          /**< Actual character objects */
      Tobjeto *actualWeapon;    /**< Actual weapon */

      
      classe* actualClass[MAX_DISTINCT_CLASSES]; /**< Pointer to each class */
      int classLevels[MAX_DISTINCT_CLASSES]; /**< Current level of each class */
      race* actualRace;         /**< Pointer to Race */
      align* actualAlign;       /**< Pointer to Align */

      string nome;              /**< Character's name */
      string retratoConversa;   /**< Portrait talk file name */
      GLfloat orientacao;       /**< Character's orientation (angle) */
      GLfloat posicaoFrente;    /**< Character's front postion. FIXME Used?? */
      GLfloat posicaoLadoX;     /**< Character's X Position */
      GLfloat posicaoLadoY;     /**< Character's Y Position (UP) */
      GLfloat posicaoLadoZ;     /**< Character's Z Position */

      Square* ocupaQuad;        /**< Square occuped by character */
      int ID;                   /**< Character's ID FIXME used?? */

      feats actualFeats;        /**< Feats owned by character */

      int actualFightGroup;     /**< fightGroup of character, used on battles*/
      personagem* actualEnemy;  /**< Pointer to actual fight enemy (target) */

      GLuint portrait;          /**< Up screen portrait GL texture */
      healthBar* lifeBar;       /**< Character's Life Bar */

      aStar pathFind;           /**< The A* pathFind to the character */

      inventory* inventories[INVENTORY_PER_CHARACTER]; /**< Inventory */

      /*! Constructor
       * \param ft -> pointer to all feats list */
      personagem(featsList* ft);
      /*! Destructor */
      ~personagem();

      void DrawMainPortrait(GLdouble x1, GLdouble y1, GLdouble z1,
                                  GLdouble x2, GLdouble y2, GLdouble z2,
                                  GLdouble x3, GLdouble y3, GLdouble z3,
                                  GLdouble x4, GLdouble y4, GLdouble z4);
      void DefineActualLifePoints(int newLife);
      void DefineMaxLifePoints(int maxPoints);


   protected:
  
     
};


/*! Character's List */
class Lpersonagem: public Tlista
{
   public:
      personagem* personagemAtivo;  /**< Active Character's on list */

      /*! List Constructor */
      Lpersonagem():Tlista(){personagemAtivo=NULL;};
      /*! List Destructor */
      ~Lpersonagem();

      /*!
       * Insert one character on list
       * \param file -> file name of the character to insert
       * \param ft -> featsList of all feats on game 
       * \return pointer to opened character*/
      personagem* InserirPersonagem(string file, featsList* ft);
                                    
      /*!
       * Remove one character from list
       * \param persona -> character pointer to remove
       * \param tiraMemoria -> != 0 if remove character from memory list, 
                             0 in the case the destructor calls the function. */
      void RetirarPersonagem(personagem* persona, int tiraMemoria);

      /*!
       * Gets hostile characters from the list
       * \param last -> last hostile taken
       * \return pointer to the hostile character
       */
      personagem* getEnemyCharacter(personagem* last);

};

#endif

