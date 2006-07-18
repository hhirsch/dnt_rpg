/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _personagens_h
#define _personagens_h

#include "../gui/desenho.h"
#include "../gui/figura.h"
#include "../gui/objeto.h"
#include "../gui/lista.h"
#include "../gui/healthBar.h"
#include "../classes/thing.h"
#include "../classes/feats.h"
//#include "habilidades.h"
//#include "../etc/glm.h"
#include <cal3d/cal3d.h>
#include <string>
using namespace std;
#include "../map/map.h"

#define POSRETX 8   /**< X screen portrait position */
#define POSRETY 20  /**< Y screen portrait position */

#define STATE_IDLE  0 /**< Character Animation State Idle */
#define STATE_WALK  1 /**< Character Animation State Walking */


/*! Character Class */
class personagem: public Tobjeto, public thing
{
   public:

      int m_state;              /**< current animation state */

      Tlista *portraits;        /**< All character's portraits */
      Tlista *objects;          /**< Actual character objects */
      Tobjeto *actualWeapon;    /**< Actual weapon */

      string nome;              /**< Character's name */
      string retratoConversa;   /**< Portrait talk file name */
      float orientacao;         /**< Character's orientation (angle) */
      float posicaoFrente;      /**< Character's front postion. FIXME Used?? */
      float posicaoLadoX;       /**< Character's X Position */
      float posicaoLadoY;       /**< Character's Y Position (UP) */
      float posicaoLadoZ;       /**< Character's Z Position */

      GLfloat min[3];           /**< Min points of static bounding box */
      GLfloat max[3];           /**< Max points of static bounding box */

      Square* ocupaQuad;        /**< Square occuped by character */
      int ID;                   /**< Character's ID FIXME used?? */

      feats actualFeats;        /**< Feats owned by character */

      int actualFightGroup;     /**< fightGroup of character, used on battles*/
      personagem* actualEnemy;  /**< Pointer to actual fight enemy (target) */

      GLuint portrait;          /**< Up screen portrait GL texture */
      healthBar* lifeBar;       /**< Character's Life Bar */

      // CAL3D related member variables
      CalCoreModel* m_calCoreModel;  /**< Cal3D Core Model of character */
      CalModel* m_calModel;          /**< Cal3D Model of character */


      /*! Constructor
       * \param ft -> pointer to all feats list */
      personagem(featsList* ft);
      /*! Destructor */
      ~personagem();

      /*! Load the cal3D model to be this character's body and animations.
       * \param strFilename -> \c string with the cal3d file name.
       * \return true if success on load. */
      bool LoadModel(const string& strFilename);

      /*! Load the a texture to the model.
       * \param strFilename -> \c string with the texture file name.
       * \return the \c GLuint with the GL texture ID. */
      GLuint loadTexture(const string& strFilename);

      /*! Set the animation state of the model.
       * \param state -> state ID to be defined. */
      void SetState(int state);

      /*! Render the model to the current frame state on screen. */
      void Render();

      /*! Calculate the model static bounding box */
      void CalculateBoundingBox();

      /*! Render the model bounding box. Only used on debugging */
      void RenderBoundingBox();

      void DrawMainPortrait(GLdouble x1, GLdouble y1, GLdouble z1,
                                  GLdouble x2, GLdouble y2, GLdouble z2,
                                  GLdouble x3, GLdouble y3, GLdouble z3,
                                  GLdouble x4, GLdouble y4, GLdouble z4);
      void DefineActualLifePoints(int newLife);
      void DefineMaxLifePoints(int maxPoints);


protected:
  
  int m_animationId[16]; /**< Cal3D animation ID */
  int m_animationCount;  /**< Cal3D number of animations */
  int m_meshId[32];      /**< Cal3D meshes ID */
  int m_meshCount;       /**< Cal3D meshes count */
  GLuint m_textureId[32];/**< Cal3D texture ID */
  int m_textureCount;    /**< Cal3D texture Count */
  float m_motionBlend[3];/**< Cal3D motion blend */
  float m_renderScale;   /**< Cal3D scale on render */
  float m_lodLevel;      /**< Cal3D Level of Detail to render */
  std::string m_path;    /**< Path to cal3D model */
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
       * \param retrato -> file name of the portrait
       * \param nome -> Character's Name
       * \param arqmodelo -> character's Cal3D file name
       * \param ft -> featsList of all feats on game */
      personagem* InserirPersonagem(char* retrato,string nome,string arqmodelo,
                                    featsList* ft);
                                    
      /*!
       * Remove one character from list
       * \param persona -> character pointer to remove
       * \param tiraMemoria -> != 0 if remove character from memory list, 
                             0 in the case the destructor calls the function. */
      void Lpersonagem::RetirarPersonagem(personagem* persona, int tiraMemoria);

};

#endif

