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

#ifndef _dnt_texture_renderer_h
#define _dnt_texture_renderer_h

#include "btree.h"
#include "btreecell.h"
#include <SDL/SDL_opengl.h>
#include <iostream>
#include <string>
using namespace std;

#include "list.h"

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
class renderTexture: public bTreeCell, public dntListElement
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
 * buffers. It's just a bTree implementation of renderTexture's.
 * \note: the list is used here to simplify the render function,
 *        and is just a replication of the btree pointers */
class texRenderer: public bTree, public dntList
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
      /*! Just to complete the list.
       * \param obj -> pointer to the element
       * \note -> this function does nothing, since the FreeCell from
       *          the bTree already delete the element. */
      void freeElement(dntListElement* obj);

};


#endif


