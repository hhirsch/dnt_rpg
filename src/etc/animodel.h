#ifndef _animodel_h
#define _animodel_h

#include <cal3d/cal3d.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

#define STATE_IDLE  0 /**< Character Animation State Idle */

/*! Animated Model Definition (using cal3d) */
class aniModel
{
   public:
      /*! Constructor */
      aniModel();
      /*! Destructor */
      ~aniModel();

      /*! Update Model to pos time 
       * \param pos -> time of the animation */
      void update(GLfloat pos);

      /*! Load the cal3D model to be this character's body and animations.
       * \param strFilename -> \c string with the cal3d file name.
       * \return true if success on load. */
      bool loadModel(const string& strFilename);

      /*! Set the animation state of the model.
       * \param state -> state ID to be defined. */
      void setState(int state);

      /*! Gets the animation state of the model.
       * \return state ID of the current animation */
      int getState();

      /*! Render the model to the current frame state on screen. */
      void render();

      /*! Calculate the model static bounding box */
      void calculateBoundingBox();

      /*! Render the model bounding box. Only used on debugging */
      void renderBoundingBox();

      GLfloat min[3];           /**< Min points of static bounding box */
      GLfloat max[3];           /**< Max points of static bounding box */

   protected:
      int m_state;                   /**< current animation state */
      CalCoreModel* m_calCoreModel;  /**< Cal3D Core Model of character */
      CalModel* m_calModel;          /**< Cal3D Model of character */
      int m_animationId[16];         /**< Cal3D animation ID */
      int m_animationCount;          /**< Cal3D number of animations */
      int m_meshId[32];              /**< Cal3D meshes ID */
      int m_meshCount;               /**< Cal3D meshes count */
      GLuint m_textureId[32];        /**< Cal3D texture ID */
      int m_textureCount;            /**< Cal3D texture Count */
      float m_motionBlend[3];        /**< Cal3D motion blend */
      float m_renderScale;           /**< Cal3D scale on render */
      float m_lodLevel;              /**< Cal3D Level of Detail to render */
      string m_path;                 /**< Path to cal3D model */

      /*! Load the a texture to the model.
       * \param strFilename -> \c string with the texture file name.
       * \return the \c GLuint with the GL texture ID. */
      GLuint loadTexture(const string& strFilename);
};

#endif

