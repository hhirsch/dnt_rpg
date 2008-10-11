#include "texrenderer.h"

/////////////////////////////////////////////////////////////////////////
//                                                                     //
//                          renderTexture                              //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
renderTexture::renderTexture(GLuint id, string name): bTreeCell()
{
   /* Define texture Info */
   textureId = id;
   textureName = name;
  
   /* Alloc Initial Buffers */
   totalQuads = 0;
   maxQuads = TEXTURE_INITIAL_MAX_QUADS;
   vertexBuffer = new GLfloat[maxQuads*12];
   uvBuffer = new GLfloat[maxQuads*8];
   normalBuffer = new GLfloat[maxQuads*3];
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
void renderTexture::addQuad(GLfloat x1, GLfloat z1,
                            GLfloat x2, GLfloat z2,
                            GLfloat y1, GLfloat y2,
                            GLfloat y3, GLfloat y4,
                            GLfloat u1, GLfloat v1,
                            GLfloat u2, GLfloat v2,
                            GLfloat nX, GLfloat nY, GLfloat nZ)
{
   int posUv = totalQuads*8;
   int posVertex = totalQuads*12;
   int posNormal = totalQuads*3;

   if(totalQuads >= maxQuads)
   {
      /* Must realloc the buffers */
      increaseBuffers();
   }

   /* Now insert the quad at each buffer */
   normalBuffer[posNormal] = nX;
   normalBuffer[posNormal] = nY;
   normalBuffer[posNormal] = nZ;

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

   vertexBuffer[posVertex+3] = x1;
   vertexBuffer[posVertex+4] = y2;
   vertexBuffer[posVertex+5] = z2;

   vertexBuffer[posVertex+6] = x2;
   vertexBuffer[posVertex+7] = y3;
   vertexBuffer[posVertex+8] = z2;

   vertexBuffer[posVertex+9] = x2;
   vertexBuffer[posVertex+10] = y4;
   vertexBuffer[posVertex+11] = z1;

   /* And increase the counter! */
   totalQuads++;
}

/***********************************************************************
 *                         increaseBuffers                             *
 ***********************************************************************/
void renderTexture::increaseBuffers()
{
   cout << "Calling increaseBuffers for " << textureName << endl;

   /* Keep previous buffers */
   GLfloat* prevVertex = vertexBuffer;
   GLfloat* prevUv = uvBuffer;
   GLfloat* prevNormal = normalBuffer;
   int numVertex = totalQuads*12;
   int numNormal = totalQuads*3;
   int numUv = totalQuads*8;

   /* Realloc the buffers with 2*size */
   maxQuads = maxQuads*2;
   vertexBuffer = new GLfloat[maxQuads*12];
   uvBuffer = new GLfloat[maxQuads*8];
   normalBuffer = new GLfloat[maxQuads*3];

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
void renderTexture::render()
{
   /* Only need to render if the quantity is not null */
   if(totalQuads > 0)
   {
      /* Enable Array States */
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
      glEnable(GL_TEXTURE_2D);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);

      /* Bind the texture */
      glBindTexture(GL_TEXTURE_2D, textureId);
      glColor3f(1.0f, 1.0f, 1.0f);

      /* Set the Buffers */
      glVertexPointer(3, GL_FLOAT, 0, vertexBuffer);
      glNormalPointer(GL_FLOAT, 0, normalBuffer);
      glTexCoordPointer(2, GL_FLOAT, 0, uvBuffer);

      /* And Render! */
      glDrawArrays(GL_QUADS, 0, totalQuads);

      /* Disable Array States */
      glDisableClientState(GL_NORMAL_ARRAY);
      glDisableClientState(GL_VERTEX_ARRAY);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   }
}

/***********************************************************************
 *                             compare                                 *
 ***********************************************************************/
int renderTexture::compare(bTreeCell* cell)
{
   renderTexture* rt = (renderTexture*)cell;

   return(textureName.compare(rt->getTextureName()));
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
texRenderer::texRenderer():bTree()
{
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
texRenderer::~texRenderer()
{
}

/***********************************************************************
 *                            dupplicateCell                           *
 ***********************************************************************/
bTreeCell* texRenderer::dupplicateCell(bTreeCell* cell)
{
   cerr << "Something called texRenderer::dupplicateCell! It's an error!" 
        << endl << "Something really BAD will happn soon..." << endl;
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
 *                           insertTexture                             *
 ***********************************************************************/
void texRenderer::insertTexture(GLuint textureId, string textureName)
{
   renderTexture* rt = new renderTexture(textureId, textureName);
   insert(rt);
}

