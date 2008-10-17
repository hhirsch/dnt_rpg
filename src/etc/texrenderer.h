#ifndef _dnt_texture_renderer_h
#define _dnt_texture_renderer_h

#include "btree.h"
#include "btreecell.h"
#include <SDL/SDL_opengl.h>
#include <iostream>
#include <string>
using namespace std;

#define TEXTURE_INITIAL_MAX_QUADS   16 /**< Initial Max Quads per texture */

/*! The render texture keeps GL_QUADS related to a texture.
 * The buffers are initially alloced for TEXTURE_INITIAL_MAX_QUADS
 * maximun number of quads. If the current totalQuads is greater than
 * the current maxQuads, them the buffers are realloced with a size 
 * 2 times greater then the current maxQuads. The increase buffer size is
 * so an expensive function, but is only called when needed, and only one
 * time per need.
 * \note -> if the increase function become frequently, is better increase
 *          the TEXTURE_INITIAL_MAX_QUADS value. 
 * \note -> we keep the renderTexture in a Btree and in a chain list at the 
 * same time, with the same structure */
class renderTexture: public bTreeCell
{
   public:
      /*! Constructor 
       * \param id -> the texture Id 
       * \param name -> the unique name of the texture
       * \note -> the name is the key (usually Ids are inserted with
       *          ascending values, so it's not a good key) */
      renderTexture(GLuint id, string name);
      /*! Destructor */
      ~renderTexture();

      /*! Get the current texture Name
       * \return -> texture Name */
      string getTextureName();

      /*! Clear the buffer's values, keeping the 
       * buffers's structures (it just reset the 
       * totalQuads counter to 0) */
      void clearBuffers();

      /*! Add a GL_QUAD to the renderTexture buffers */
      void addQuad(GLfloat x1, GLfloat y1, GLfloat z1,
                   GLfloat x2, GLfloat y2, GLfloat z2,
                   GLfloat x3, GLfloat y3, GLfloat z3,
                   GLfloat x4, GLfloat y4, GLfloat z4,
                   GLfloat u1, GLfloat v1,
                   GLfloat u2, GLfloat v2,
                   GLfloat nX, GLfloat nY, GLfloat nZ);
      
      /*! Render the current quads related to this texture! 
       * \param floorReflexion -> with reflextion with floor is enabled */
      void render(bool floorReflexion);

      /*! Compare with another renderTexture key
       * \param name -> name of the texture to compare with this one
       * \return  == 0 if equal, < 0  if lesser or > 0 if greater */
      int compare(string name);

      /*! Compare with another renderTexture
       * \param cell -> pointer to cell to compare this one to
       * \return == 0 if equal, < 0  if lesser or > 0 if greater */
      int compare(bTreeCell* cell);

      /*! Not used. Just to keep implementation from bTreeCell
       * \param cell -> cell to merge the current one with */
      void merge(bTreeCell* cell);

      /*! Set the next texture on the chain list
       * \param rt -> pointer to the render texture*/
      void setNext(renderTexture* rt);
      /*! Set the previous texture on the chain list
       * \param rt -> pointer to the render texture */
      void setPrevious(renderTexture* rt);

      /*! Get the next renderTexture on the list
       * \return -> pointer to the next */
      renderTexture* getNext();
      /*! Get the previous renderTexture on the list
       * \return -> pointer to the previous */
      renderTexture* getPrevious();

   protected:

      /*!Increase, when needed, the size of the buffers to keep
       * 2*maxQuads. It's an expensive function. */
      void increaseBuffers();

      int totalQuads;  /**< Total Quads related to the Texture */

      int maxQuads;    /**< Current Max Quads for this texture.
                            If totalQuads > maxQuads, them the
                            buffers are realloc with 2*maxQuads */

      GLfloat* vertexBuffer;  /**< The vertex buffer */
      GLfloat* uvBuffer;      /**< The uv Buffer */
      GLfloat* normalBuffer;  /**< The normal Buffer */

      GLuint textureId;       /**< The texture Id */
      string textureName;     /**< The texture Name */

      renderTexture* next;     /**< Next render texture on list */
      renderTexture* previous; /**< Previous render texture on list */
};

/*! The texRenderer is used to render GL_QUADS per texture using
 * buffers. It's just a bTree implementation of renderTexture's */
class texRenderer: public bTree
{
   public:
      /*! Constructor */
      texRenderer();
      /*! Destructor */
      ~texRenderer();

      /*! Not used.
       * \note -> an warning is printed if this function is called.
       * \return -> always return NULL */
      bTreeCell* dupplicateCell(bTreeCell* cell);

      /*! Free the memory used by a renderTexture
       * \param cell -> renderTexture to free */
      void freeCell(bTreeCell* cell);

      /*! Insert a renderTexture information on the tree
       * \param textureId -> the texture ID
       * \param textureName  -> the name of the texture (the key)
       * \return -> pointer to the texture inserted */
      renderTexture* insertTexture(GLuint textureId, string textureName);

      /*! Render all renderTextures quads
       * \param floorReflexion -> with reflextion with floor is enabled */
      void render(bool floorReflexion);

      /*! Clear all renderTextures (setting its totalQuads to 0) */
      void clear();

      /*! Search for a renderTexture
       * \param textureName -> name of the texture to search
       * \return -> pointer to the renderTexture found or NULL */
      renderTexture* search(string textureName);

      /*! Add a GL_QUAD to the renderTexture buffers
       * \param textureId -> the Id of the  texture used
       * \param textureName -> the name of the texture used */
      void addQuad(GLuint textureId, string textureName,
                   GLfloat x1, GLfloat y1, GLfloat z1,
                   GLfloat x2, GLfloat y2, GLfloat z2,
                   GLfloat x3, GLfloat y3, GLfloat z3,
                   GLfloat x4, GLfloat y4, GLfloat z4,
                   GLfloat u1, GLfloat v1,
                   GLfloat u2, GLfloat v2,
                   GLfloat nX, GLfloat nY, GLfloat nZ);

   protected:
      renderTexture* texList;   /**< The texture list (to do a non-recursive
                                     rendering) */
      int totalTextures;        /**< Total Textures on the list */
};


#endif


