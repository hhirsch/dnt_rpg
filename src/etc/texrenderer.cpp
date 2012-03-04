/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#include "texrenderer.h"
#include <string.h>
using namespace std;

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                          renderTexture                              //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
renderTexture::renderTexture(GLuint id, string name)
{
   /* Define texture Info */
   textureId = id;
   textureName = name;
  
   /* Alloc Initial Buffers */
   totalQuads = 0;
   maxQuads = TEXTURE_INITIAL_MAX_QUADS;
   vertexBuffer = new GLfloat[maxQuads*12];
   uvBuffer = new GLfloat[maxQuads*8];
   normalBuffer = new GLfloat[maxQuads*12];
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
renderTexture::~renderTexture()
{
   /* Delete All Buffers Structures */
   if(vertexBuffer)
   {
      delete[] vertexBuffer;
   }
   if(uvBuffer)
   {
      delete[] uvBuffer;
   }
   if(normalBuffer)
   {
      delete[] normalBuffer;
   }
}

/***********************************************************************
 *                           getTextureName                            *
 ***********************************************************************/
string renderTexture::getTextureName()
{
   return(textureName);
}

/***********************************************************************
 *                           clearBuffers                              *
 ***********************************************************************/
void renderTexture::clearBuffers()
{
   totalQuads = 0;
}

/***********************************************************************
 *                             addQuad                                 *
 ***********************************************************************/
void renderTexture::addQuad(GLfloat x1, GLfloat y1, GLfloat z1,
                            GLfloat x2, GLfloat y2, GLfloat z2,
                            GLfloat x3, GLfloat y3, GLfloat z3,
                            GLfloat x4, GLfloat y4, GLfloat z4,
                            GLfloat u1, GLfloat v1,
                            GLfloat u2, GLfloat v2,
                            GLfloat nX, GLfloat nY, GLfloat nZ)
{
   int posUv = totalQuads*8;
   int posVertex = totalQuads*12;
   int posNormal = totalQuads*12;

   if(totalQuads >= maxQuads)
   {
      /* Must realloc the buffers */
      increaseBuffers();
   }

   /* Now insert the quad at each buffer */
   int i;
   for(i=0; i<4; i++)
   {
      normalBuffer[posNormal+i*3] = nX;
      normalBuffer[posNormal+1+i*3] = nY;
      normalBuffer[posNormal+2+i*3] = nZ;
   }

   uvBuffer[posUv] = u1;
   uvBuffer[posUv+1] = v1;
   uvBuffer[posUv+2] = u1;
   uvBuffer[posUv+3] = v2;
   uvBuffer[posUv+4] = u2;
   uvBuffer[posUv+5] = v2;
   uvBuffer[posUv+6] = u2;
   uvBuffer[posUv+7] = v1;

   vertexBuffer[posVertex] = x1;
   vertexBuffer[posVertex+1] = y1;
   vertexBuffer[posVertex+2] = z1;

   vertexBuffer[posVertex+3] = x2;
   vertexBuffer[posVertex+4] = y2;
   vertexBuffer[posVertex+5] = z2;

   vertexBuffer[posVertex+6] = x3;
   vertexBuffer[posVertex+7] = y3;
   vertexBuffer[posVertex+8] = z3;

   vertexBuffer[posVertex+9] = x4;
   vertexBuffer[posVertex+10] = y4;
   vertexBuffer[posVertex+11] = z4;

   /* And increase the counter! */
   totalQuads++;
}

/***********************************************************************
 *                         increaseBuffers                             *
 ***********************************************************************/
void renderTexture::increaseBuffers()
{
#ifdef DEBUG
   cout << "Calling increaseBuffers for " << textureName << endl;
#endif   

   /* Keep previous buffers */
   GLfloat* prevVertex = vertexBuffer;
   GLfloat* prevUv = uvBuffer;
   GLfloat* prevNormal = normalBuffer;
   int numVertex = totalQuads*12;
   int numNormal = totalQuads*12;
   int numUv = totalQuads*8;

   /* Realloc the buffers with 2*size */
   maxQuads = maxQuads*2;
   vertexBuffer = new GLfloat[maxQuads*12];
   uvBuffer = new GLfloat[maxQuads*8];
   normalBuffer = new GLfloat[maxQuads*12];

   /* And, finally, copy all values to the new buffers... ouch,
    * it really is expensive (3 new calls, 3 delete calls and
    * 3 BIG memcpy calls!) */
   memcpy(vertexBuffer, prevVertex, sizeof(GLfloat)*numVertex);
   memcpy(normalBuffer, prevNormal, sizeof(GLfloat)*numNormal);
   memcpy(uvBuffer, prevUv, sizeof(GLfloat)*numUv);

   /* Delete previous buffers */
   delete[] prevVertex;
   delete[] prevNormal;
   delete[] prevUv;
}

/***********************************************************************
 *                              render                                 *
 ***********************************************************************/
void renderTexture::render(bool floorReflexion)
{
   glPushAttrib(GL_ENABLE_BIT);
   /* Only need to render if the quantity is not null */
   if(totalQuads > 0)
   {
      /* Enable Array States */
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
      glEnable(GL_TEXTURE_2D);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glEnable(GL_COLOR_MATERIAL);

      /* Bind the texture */
      glBindTexture(GL_TEXTURE_2D, textureId);
      glColor3f(1.0f, 1.0f, 1.0f);

      /* Set the Buffers */
      glVertexPointer(3, GL_FLOAT, 0, vertexBuffer);
      glNormalPointer(GL_FLOAT, 0, normalBuffer);
      glTexCoordPointer(2, GL_FLOAT, 0, uvBuffer);

      /* And Render! */
      glDrawArrays(GL_QUADS, 0, totalQuads*4);

      if(floorReflexion)
      {
         /* Render The Floor Reflexion */
         glEnable(GL_STENCIL_TEST);
         glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
         glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
         glEnable(GL_NORMALIZE);
         glPushMatrix();
            glScalef(1.0,-1.0,1.0);
            glDrawArrays(GL_QUADS, 0, totalQuads*4);
         glPopMatrix();
         glDisable(GL_NORMALIZE);
         glDisable(GL_STENCIL_TEST);
      }

      /* Disable Array States */
      glDisableClientState(GL_NORMAL_ARRAY);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   }
   glPopAttrib();
}

/***********************************************************************
 *                             compare                                 *
 ***********************************************************************/
int renderTexture::compare(bTreeCell* cell)
{
   renderTexture* rt = (renderTexture*)cell;

   return(compare(rt->getTextureName()));
}

/***********************************************************************
 *                             compare                                 *
 ***********************************************************************/
int renderTexture::compare(string name)
{
   return(textureName.compare(name));
}

/***********************************************************************
 *                               merge                                 *
 ***********************************************************************/
void renderTexture::merge(bTreeCell* cell)
{
   /* No effect, one time that merging here just means use the current one */
}

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                         textureRenderer                             //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
texRenderer::texRenderer()
{
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
texRenderer::~texRenderer()
{
   clearSubTree(getRoot());
}

/***********************************************************************
 *                            dupplicateCell                           *
 ***********************************************************************/
bTreeCell* texRenderer::dupplicateCell(bTreeCell* cell)
{
   cerr << "Something called texRenderer::dupplicateCell! It's an error!" 
        << endl << "Something really BAD will happen soon..." << endl;
   return(NULL);
}

/***********************************************************************
 *                               freeCell                              *
 ***********************************************************************/
void texRenderer::freeCell(bTreeCell* cell)
{
   renderTexture* rt = (renderTexture*)cell;
   delete(rt);
}

/***********************************************************************
 *                              freeElement                            *
 ***********************************************************************/
void texRenderer::freeElement(dntListElement* obj)
{
   /* Does nothing. the freeCell should clean the pointer,
    * so the list do not need to delete it. */
}

/***********************************************************************
 *                           insertTexture                             *
 ***********************************************************************/
renderTexture* texRenderer::insertTexture(GLuint textureId, string textureName)
{
   /* Create the renderTexture */
   renderTexture* rt = new renderTexture(textureId, textureName);

   /* Insert it at the binary tree */
   rt = (renderTexture*)bTree::insert(rt);

   /* Insert it at the chain list */
   dntList::insert(rt);

   return(rt);
}

/***********************************************************************
 *                                search                               *
 ***********************************************************************/
renderTexture* texRenderer::search(string textureName)
{
   renderTexture* pos = (renderTexture*)this->getRoot();
   int res;

   while(pos != NULL)
   {
      /* Inverse compare (textureName with renderTexture),
       * so must inverse the results ( >0 to left, <0 to right) */
      res = pos->compare(textureName);

      if(res == 0)
      {
         /* Here it is! */
         return(pos);
      }
      else if(res > 0)
      {
         /* Must be at left */
         pos = (renderTexture*)pos->getLeft();
      }
      else
      {
         /* Must be at right  */
         pos = (renderTexture*)pos->getRight();
      }
   }

   return(NULL);
}

/***********************************************************************
 *                                addQuad                              *
 ***********************************************************************/
void texRenderer::addQuad(GLuint textureId, string textureName,
                          GLfloat x1, GLfloat y1, GLfloat z1,
                          GLfloat x2, GLfloat y2, GLfloat z2,
                          GLfloat x3, GLfloat y3, GLfloat z3,
                          GLfloat x4, GLfloat y4, GLfloat z4,
                          GLfloat u1, GLfloat v1,
                          GLfloat u2, GLfloat v2,
                          GLfloat nX, GLfloat nY, GLfloat nZ)
{
   /* Get the desired texture */
   renderTexture* rt = search(textureName);
   if(!rt)
   {
      /* No texture with this name yet, so must create one and insert */
      rt = insertTexture(textureId, textureName);
   }

   /* Insert the GL_QUAD */
   rt->addQuad(x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4, u1,v1,u2,v2, nX,nY,nZ);
}

/***********************************************************************
 *                                render                               *
 ***********************************************************************/
void texRenderer::render(bool floorReflexion)
{
   int i;

   /* Render the list (to avoid recursive calls rendering the btree) */
   renderTexture* rt = (renderTexture*)first;
   for(i = 0; i < total; i++)
   {
      rt->render(floorReflexion);
      rt = (renderTexture*)rt->getNext();
   }
}

/***********************************************************************
 *                                 clear                               *
 ***********************************************************************/
void texRenderer::clear()
{
   int i;

   /* Clear the buffers of each renderTexture */
   renderTexture* rt = (renderTexture*)first;
   for(i = 0; i < total; i++)
   {
      rt->clearBuffers();
      rt = (renderTexture*)rt->getNext();
   }
}


