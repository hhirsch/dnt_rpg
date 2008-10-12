#ifndef _dnt_texture_renderer_h
#define _dnt_texture_renderer_h

#include "btree.h"
#include "btreecell.h"
#include <SDL/SDL_opengl.h>
#include <iostream>
#include <string>
using namespace std;

#define TEXTURE_INITIAL_MAX_QUADS   100 /**< Initial Max Quads per texture */

/*! The render texture keeps GL_QUADS related to a texture.
 * The buffers are initially alloced for TEXTURE_INITIAL_MAX_QUADS
 * maximun number of quads. If the current totalQuads is greater than
 * the current maxQuads, them the buffers are realloced with a size 
 * 2 times greater then the current maxQuads. The increase buffer size is
 * so an expensive function, but is only called when needed, and only one
 * time per need.
 * \note -> if the increase function become frequently, is better increase
 *          the TEXTURE_INITIAL_MAX_QUADS value. */
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

      /*! Add a GL_QUAD to the renderTexture buffers
       * \param x1 -> quad X1 value
       * \param z1 -> quad Z1 value
       * \param x2 -> quad X2 value
       * \param z2 -> quad Z2 value
       * \param y1 -> height at x1,z1
       * \param y2 -> height at x1,z2
       * \param y3 -> height at x2,z2
       * \param y4 -> height at x2,z1
       * \param u1 -> quad texture U1 value
       * \param v1 -> quad texture V1 value
       * \param u2 -> quad texture U2 value
       * \param v2 -> quad texture V2 value
       * \param nX -> quad normal X value
       * \param nY -> quad normal Y value
       * \param nZ -> quad normal Z value */
      void addQuad(GLfloat x1, GLfloat z1,
                   GLfloat x2, GLfloat z2,
                   GLfloat y1, GLfloat y2,
                   GLfloat y3, GLfloat y4,
                   GLfloat u1, GLfloat v1,
                   GLfloat u2, GLfloat v2,
                   GLfloat nX, GLfloat nY, GLfloat nZ);
                   
      /*! Render the current quads related to this texture! */
      void render();

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
       * \param rt -> pointer to the root (if none, will get the root) */
      void render(renderTexture* rt=NULL);

      /*! Clear all renderTextures (setting its totalQuads to 0) 
       * \param rt -> pointer to the root (if none, will get the root) */
      void clear(renderTexture* rt=NULL);

      /*! Search for a renderTexture
       * \param textureName -> name of the texture to search
       * \return -> pointer to the renderTexture found or NULL */
      renderTexture* search(string textureName);

      /*! Add a GL_QUAD to the renderTexture buffers
       * \param textureId -> the Id of the  texture used
       * \param textureName -> the name of the texture used
       * \param x1 -> quad X1 value
       * \param z1 -> quad Z1 value
       * \param x2 -> quad X2 value
       * \param z2 -> quad Z2 value
       * \param y1 -> height at x1,z1
       * \param y2 -> height at x1,z2
       * \param y3 -> height at x2,z2
       * \param y4 -> height at x2,z1
       * \param u1 -> quad texture U1 value
       * \param v1 -> quad texture V1 value
       * \param u2 -> quad texture U2 value
       * \param v2 -> quad texture V2 value
       * \param nX -> quad normal X value
       * \param nY -> quad normal Y value
       * \param nZ -> quad normal Z value */
      void addQuad(GLuint textureId, string textureName,
                   GLfloat x1, GLfloat z1,
                   GLfloat x2, GLfloat z2,
                   GLfloat y1, GLfloat y2,
                   GLfloat y3, GLfloat y4,
                   GLfloat u1, GLfloat v1,
                   GLfloat u2, GLfloat v2,
                   GLfloat nX, GLfloat nY, GLfloat nZ);
};


#endif


