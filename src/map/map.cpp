/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include "map.h"
#include "../engine/culling.h"
#include "../engine/util.h"
#include "../etc/extensions.h"
#include "../etc/dirs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define VIS_DELTA 64  /**< Delta to Visible Culling */

//////////////////////////////////////////////////////////////////////////////
//                                  SQUARE                                  //
//////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *                      Square Constructor                          *
 ********************************************************************/
Square::Square()
{
   flags = 0;
   h1 = h2 = h3 = h4 = 0;
   mapConection.active = false;
   mapConection.mapName = "Nothing";
   divisions = 1;
   int aux;
   for(aux=0; aux < MAX_WALLS; aux++)
   {
     walls[aux] = NULL;
   }
   objList = NULL;
   totalObjects = 0;
}

/********************************************************************
 *                       Square Destructor                          *
 ********************************************************************/
Square::~Square()
{
   /* Remove all objects on the list */
   while(totalObjects > 0)
   {
      removeObject(objList);
   }
   return;
}

/********************************************************************
 *                           addObject                              *
 ********************************************************************/
objSquare* Square::addObject(bool draw, int squareX, int squareZ, 
                             int orientation, float x, float y, float z, 
                             bool colision, object* obj)
{
   objSquare* n = new objSquare;
   n->draw = draw;
   n->status = 0;
   n->squareX = squareX;
   n->squareZ = squareZ;
   n->orientation = orientation;
   n->x = x;
   n->y = y;
   n->z = z;
   n->colision = colision;
   n->obj = obj;

   if(totalObjects == 0)
   {
      n->next = n;
      n->previous = n;
   }
   else
   {
      n->next = objList;
      n->previous = objList->previous;
      objList->previous->next = n;
      objList->previous = n;
   }
   objList = n;

   totalObjects++;

   return(n);
}

/********************************************************************
 *                         removeObject                             *
 ********************************************************************/
void Square::removeObject(objSquare* obj)
{
   if(obj == objList)
   {
      objList = obj->next;
   }
   obj->previous->next = obj->next;
   obj->next->previous = obj->previous;

   if(obj->draw)
   {
      /* If mark as draw, dec the used flag of the object, since it
       * is no more used here. */
      obj->obj->decUsedFlag();
      if(obj->obj->isStaticScenery())
      {
         //TODO Remove Position
      }
   }
   delete(obj);

   totalObjects--;
   if(totalObjects == 0)
   {
      objList = NULL;
   }
}

/********************************************************************
 *                         getFirstObject                           *
 ********************************************************************/
objSquare* Square::getFirstObject()
{
   return(objList);
}

/********************************************************************
 *                        getTotalObjects                           *
 ********************************************************************/
int Square::getTotalObjects()
{
   return(totalObjects);
}

/********************************************************************
 *                            setDivisions                          *
 ********************************************************************/
void Square::setDivisions()
{
   /*divisions = (int) (( fabs(h4-h1) + fabs(h4-h2) + fabs(h4-h1) +
                        fabs(h3-h1) + fabs(h3-h2) + fabs(h2-h1)) /
                       SQUARE_DIVISIONS_INC) + 1;
   */
   divisions = 1;
}

//////////////////////////////////////////////////////////////////////////////
//                                  OTHERS                                  //
//////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *                             Texture ID                           *
 ********************************************************************/
int Map::getTextureID(string textureName, GLuint& R, GLuint& G, GLuint& B)
{
   /* search for texture */
   int aux=0;
   texture* tex = textures;
   while(aux < numTextures)
   {
      if(!(tex->name.compare(textureName)) )
      {
         R = tex->R; G = tex->G; B = tex->B;
         return(tex->index); //texture found 
      }
      tex = tex->next;
      aux++;
   }
   return(-1);
}

/*********************************************************************
 *                   Returns texture's name                          *
 *********************************************************************/
string Map::getTextureName(GLuint ID)
{
   int aux=0;
   texture* tex = textures;
   while(aux < numTextures)
   {
      if(tex->index == ID)
      {
         return(tex->name);
      }
      tex = tex->next;
      aux++;
   }
   return(NULL);
}

/*********************************************************************
 *                      Returns texture                              *
 *********************************************************************/
texture* Map::getTexture(GLuint id)
{
   int aux=0;
   texture* tex = textures;
   while(aux < numTextures)
   {
      if(tex->index == id)
      {
         return(tex);
      }
      tex = tex->next;
      aux++;
   }
   return(NULL);
}

/********************************************************************
 *                         Insert texture                           *
 ********************************************************************/
GLuint Map::insertTexture(string arq, string name, GLuint R, GLuint G, GLuint B)
{
   dirs dir;
   texture* tex;
   int aux;

   /* Verify if the texture is already inserted */
   int id = getTextureID(arq, R, G, B);
   if(id != -1)
   {
      return(id);
   }

   /* Load Texture Images */
   SDL_Surface* img = IMG_Load(dir.getRealFile(arq).c_str());
   if(!img)
   {
      printf("Error Opening Texture: %s\n",
             dir.getRealFile(arq).c_str());
      return(0);
   }

   /* Create the Texture Structs */ 
   tex = new(texture);
   if(numTextures == 0)
   {
      textures = tex;
      tex->next = tex;
      tex->previous = tex;
   }
   else
   {
      tex->next = textures;
      tex->previous = textures->previous;
      tex->next->previous = tex;
      tex->previous->next = tex;
      textures = tex;
   }

   tex->fileName = arq.c_str();
   tex->name = name.c_str();

   tex->R = R;
   tex->G = G;
   tex->B = B;

   tex->definedAlpha = false;

   /* Alloc the alpha values */
   tex->alphaValues = new float*[getSizeX()*ALPHA_TEXTURE_INC];
   for(aux = 0; aux < (getSizeX()*ALPHA_TEXTURE_INC); aux++)
   {
      tex->alphaValues[aux] = new float[getSizeZ() * ALPHA_TEXTURE_INC];
   }

   /* Generate the openGL texture */
   glGenTextures(1, &(tex->index));
   glBindTexture(GL_TEXTURE_2D, tex->index);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img->w,img->h, 
                0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);


   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img->w,
                     img->h, GL_RGB, GL_UNSIGNED_BYTE, 
                     img->pixels );

   numTextures++;

   /* Free the image */
   SDL_FreeSurface(img);

   return(tex->index);
}

/********************************************************************
 *                      removeUnusedTextures                        *
 ********************************************************************/
void Map::removeUnusedTextures()
{
   /* For Each texture, verify if some square is using it. */
   int aux=0;
   int x1,z1;
   texture* tex = textures;
   texture* rmTex = NULL;
   bool used = false;
   int total = numTextures;
   wall* w;
   for(aux = 0; aux < total; aux++)
   {
      /* Look on all squares if one is using it */
      used = false;

      /* Verify the use of texture At Squares */
      for(x1 = 0; ((x1 < x) && (!used)); x1++)
      {
         for(z1 = 0; ((z1 < z) && (!used)) ;z1++)
         {
            used |= (getTexture(MapSquares[x1][z1].texture) == tex);
         }
      }

      /* Verify use of texture at Walls  */
      w = walls;
      for(x1 = 0; ((x1 < totalWalls) && (!used)); x1++)
      {
         used |= (getTexture(w->texture) == tex);
         w = w->next;
      }

      if(!used)
      {
         /* Remove it from the linked list */
         rmTex = tex;
         tex = tex->next;
         rmTex->next->previous = rmTex->previous;
         rmTex->previous->next = rmTex->next;
         numTextures--;
         if(rmTex == textures)
         {
            textures = rmTex->next;
         }
         if(numTextures == 0)
         {
            textures = NULL;
         }
         printf("Removed: %s\n", rmTex->name.c_str());
         delete(rmTex);
      }
      else
      {
         tex = tex->next;
      }
   }
}

//////////////////////////////////////////////////////////////////////////////
//                                   MAP                                    //
//////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *                          removeObject                            *
 ********************************************************************/
void Map::removeObject(GLfloat xObj, GLfloat zObj, object* obj)
{
    removeObject(xObj, zObj, obj->getName());
}

/********************************************************************
 *                          removeObject                            *
 ********************************************************************/
void Map::removeObject(GLfloat xObj, GLfloat zObj, string fileName)
{
   int Xaux = 0, Zaux = 0, o = 0;
   objSquare* obj;
   objSquare* next;
   for(Xaux = 0; Xaux < x; Xaux++)
   {
     for(Zaux = 0; Zaux < z; Zaux++)
     {
        obj = MapSquares[Xaux][Zaux].getFirstObject();
        for(o=0; o < MapSquares[Xaux][Zaux].getTotalObjects(); o++)
        {
           next = obj->next;
           if( (obj != NULL) && (obj->x == xObj) && (obj->z == zObj) )
           {
              if(obj->obj->getName() == fileName)
              {
                 MapSquares[Xaux][Zaux].removeObject(obj);
              }
           }
           obj = next;
        }
     }
   }
}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void Map::insertObject(GLfloat xReal, GLfloat yReal, GLfloat zReal, int orObj,
                        object* obj, bool collision)
{
   int qx = (int)xReal / squareSize();
   int qz = (int)zReal / squareSize();
   insertObject(xReal, yReal, zReal, orObj, obj, qx, qz, collision);
}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void Map::insertObject(GLfloat xReal, GLfloat yReal, GLfloat zReal, int orObj,
                       object* obj, int qx, int qz, bool collision)
{
   Square* saux = relativeSquare(qx,qz);
   int ob=0;
   if(saux)
   {
      saux->addObject(true, qx, qz, orObj, xReal, yReal, zReal, collision, obj);
      boundingBox  bounds = obj->getBoundingBox();

      float X[2], Z[2];
      X[0] = bounds.x1;
      X[1] = bounds.x2;
      Z[0] = bounds.z1;
      Z[1] = bounds.z2;
      if(orObj!=0)
      {
         GLfloat oldX, oldZ;
         GLfloat cosseno = cos(deg2Rad(orObj));
         GLfloat seno = sin(deg2Rad(orObj));
         int aux;
         for(aux = 0;aux<=1;aux++)
         {
            oldX = X[aux];
            oldZ = Z[aux];
            X[aux] = (oldZ*seno) + (oldX*cosseno);
            Z[aux] = (oldZ*cosseno) - (oldX*seno);
         }
         if(X[0]>X[1])
         {
            oldX = X[0];
            X[0] = X[1];
            X[1] = oldX;
         }
         if(Z[0]>Z[1])
         {
            oldZ = Z[0];
            Z[0] = Z[1];
            Z[1] = oldZ;
         }
     }

     int minqx, minqz, maxqx, maxqz;
     int ssize = squareSize();
     minqx = (int)(X[0] + xReal) / ssize;
     minqz = (int)(Z[0] + zReal) / ssize;
     maxqx = (int)(X[1] + xReal) / ssize;
     maxqz = (int)(Z[1] + zReal) / ssize; 
     int X1, Z1;
     Square* qaux;
     for(X1 = minqx; X1<=maxqx; X1++)
     {
        for(Z1 = minqz; Z1 <=maxqz; Z1++) 
        {
           qaux = relativeSquare(X1,Z1);
           if((qaux) && (qaux != saux))
           {
              ob =0;
              qaux->addObject(false,qx,qz,orObj,xReal,yReal,zReal,
                              collision,obj);
           }
        }
     }
     /* Mark the object as used */
     obj->incUsedFlag();

     /* If is a scenery one, the render is controlled by model3d, so..  */
     if(obj->isStaticScenery())
     {
        obj->addRenderPosition(xReal, yReal, zReal, orObj);
     }
   }
   else
   {
     printf("Warn: Try to insert object out of Map's Limits!\n");
   }
}

/********************************************************************
 *                             addWall                              *
 ********************************************************************/
wall* Map::addWall(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2)
{
   wall* maux = new(wall);
   maux->x1 = x1;
   maux->x2 = x2;
   maux->z1 = z1;
   maux->z2 = z2;
   if(walls)
   {
      maux->next = walls;
      maux->previous = walls->previous;
      maux->next->previous = maux;
      maux->previous->next = maux;
   }
   else
   {
      maux->next = maux;
      maux->previous = maux;
   }
   maux->texture = -1;
   totalWalls++;
   walls = maux;
   return(maux);
}

/********************************************************************
 *                          getFirstWall                            *
 ********************************************************************/
wall* Map::getFirstWall()
{
   return(walls);
}

/********************************************************************
 *                          getTotalWalls                           *
 ********************************************************************/
int Map::getTotalWalls()
{
   return(totalWalls);
}

/********************************************************************
 *                           removeWall                             *
 ********************************************************************/
void Map::removeWall(wall* w)
{
   if(w)
   {
      if(walls == w)
      {
         walls = w->next;
      }
      w->next->previous = w->previous;
      w->previous->next = w->next;
      delete(w);
      totalWalls--;
      if(totalWalls == 0)
      {
         walls = NULL;
      }
   }
}

/********************************************************************
 *                            drawQuad                              *
 ********************************************************************/
void drawQuad(GLfloat x1, GLfloat z1,
              GLfloat x2, GLfloat z2,
              GLfloat h1, GLfloat h2, GLfloat h3, GLfloat h4,
              GLfloat texCoordX1, GLfloat texCoordZ1, 
              GLfloat texCoordX2, GLfloat texCoordZ2)
{
   glTexCoord2f(texCoordX1, texCoordZ1);
   glNormal3i(0,1,0);
   glVertex3f( x1 , h1, z1 );
   glTexCoord2f(texCoordX1, texCoordZ2);
   glNormal3i(0,1,0);
   glVertex3f( x1 , h2, z2);
   glTexCoord2f(texCoordX2, texCoordZ2);
   glNormal3i(0,1,0);
   glVertex3f( x2, h3, z2 );
   glTexCoord2f(texCoordX2, texCoordZ1);
   glNormal3i(0,1,0);
   glVertex3f( x2, h4, z1 );
}

/********************************************************************
 *                       drawFloorIndoor                            *
 ********************************************************************/
void Map::drawFloorIndoor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                          GLfloat matriz[6][4])
{
   int aux = 0;
   int x1, z1;
   texture* tex;

   glColor4f(1.0, 1.0, 1.0, 0.9);

   /* For Reflexions */
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);

   tex = textures;
   while(aux < numTextures)
   {
      /* Bind the texture */
      glBindTexture(GL_TEXTURE_2D, tex->index);
      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR );
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      /* Draw all vertex with this texture */
      glBegin(GL_QUADS);

      for(x1=0; x1 < x; x1++)
      {
         for(z1=0; z1 < z; z1++)
         {
            if( (MapSquares[x1][z1].texture == (int)tex->index) &&
                (visibleCube(MapSquares[x1][z1].x1 - VIS_DELTA,0,
                             MapSquares[x1][z1].z1 - VIS_DELTA,
                             MapSquares[x1][z1].x2 + VIS_DELTA,
                             MAX_HEIGHT,
                             MapSquares[x1][z1].z2 + VIS_DELTA, matriz)))
            {
               drawQuad(MapSquares[x1][z1].x1, MapSquares[x1][z1].z1,
                        MapSquares[x1][z1].x2, MapSquares[x1][z1].z2,
                        MapSquares[x1][z1].h1, MapSquares[x1][z1].h2, 
                        MapSquares[x1][z1].h3, MapSquares[x1][z1].h4,
                        0.0, 0.0, 1.0, 1.0);
            }
         }
      }

      glEnd();

      tex = tex->next;
      aux++;
   }

   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
}

/********************************************************************
 *                      drawFloorOutdoor                            *
 ********************************************************************/
void Map::drawFloorOutdoor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                           GLfloat matriz[6][4])
{
   extensions ext;
   int aux = 0;
   texture* tex;

   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, vertexBuffer);

   /* First Draw with the common texture. */
   glTexCoordPointer(2, GL_FLOAT, 0, uvBuffer);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glBindTexture(GL_TEXTURE_2D, commonTexture);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR );
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glEnable(GL_TEXTURE_2D);
   glDrawArrays(GL_QUADS, 0, (int)totalVertex / (int)3);
   glDisable(GL_TEXTURE_2D);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);

   /* Next, define and draw all textures, with multitexture.
    * \FIXME -> when no multitexture is avaible! */
   if(ext.ARBActiveTexture != NULL)
   { 
      ext.ARBClientActiveTexture(GL_TEXTURE0_ARB);
      glTexCoordPointer(2, GL_FLOAT, 0, uvAlphaBuffer);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      ext.ARBClientActiveTexture(GL_TEXTURE1_ARB);
      glTexCoordPointer(2, GL_FLOAT, 0, uvBuffer);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   }

   tex = textures;
   while(aux < numTextures)
   {
      /* Only Draw texture with floor count > 0 */
      if( (ext.ARBActiveTexture != NULL) && (tex->count > 0))
      {
         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_LEQUAL);
         glDepthMask(GL_TRUE);

         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

         /* Bind the Alpha Texture */
         ext.ARBActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, tex->alphaTexture);
         glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
         glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE0);
         glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);
         glEnable(GL_TEXTURE_2D);

         /* Bind the Texture */ 
         ext.ARBActiveTexture(GL_TEXTURE1);
         glBindTexture(GL_TEXTURE_2D, tex->index);
         glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
         glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
         glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE1);
         glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);
         glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_TEXTURE1);
         glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);
         glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS);
         glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);
         glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_PREVIOUS);
         glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, GL_SRC_ALPHA);
         glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                         GL_LINEAR_MIPMAP_LINEAR );
         glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glEnable(GL_TEXTURE_2D);
         
         /* Draw The Array */
         glNormal3i(0,1,0);
         glDrawArrays(GL_QUADS, 0, (int)totalVertex / (int)3);
         
         ext.ARBActiveTexture(GL_TEXTURE1_ARB);
      	glDisable(GL_TEXTURE_2D);

      	ext.ARBActiveTexture(GL_TEXTURE0_ARB);
      	glDisable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, tex->alphaTexture);
         glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

         glDisable(GL_BLEND);

         glEnable(GL_DEPTH_TEST);
         glDepthFunc(GL_LESS);
         glDepthMask(GL_TRUE);

      }
      tex = tex->next;
      aux++;
   }

   /* So disable the multitexture state. */
   if(ext.ARBActiveTexture != NULL)
   { 
      ext.ARBClientActiveTexture(GL_TEXTURE1_ARB);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      ext.ARBClientActiveTexture(GL_TEXTURE0_ARB);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   }

   glDisableClientState(GL_VERTEX_ARRAY);
}

/********************************************************************
 *                            DrawFloor                             *
 ********************************************************************/
void Map::drawFloor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                    GLfloat matriz[6][4])
{
   /* Draw Terrain */
   if(outdoor)
   {
      drawFloorOutdoor(cameraX, cameraY, cameraZ, matriz);
   }
   else
   {
      drawFloorIndoor(cameraX, cameraY, cameraZ, matriz);
   }

   /* Draw Lakes */
   lake* l = lakes;
   int i;
   for(i = 0; i < totalLakes; i++)
   {
      l->draw();
      l = l->next;
   }

   return;
}

/********************************************************************
 *                           Draw  Map                              *
 ********************************************************************/
int Map::draw(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
              GLfloat matriz[6][4], GLfloat perX, GLfloat perZ)
{
   /* Actualize Lights */
   lights.actualize(perX, perZ);

   glEnable(GL_COLOR_MATERIAL);

   glColor4f(1.0, 1.0, 1.0, 1.0);
   drawFloor( cameraX, cameraY, cameraZ, matriz );

   glColor3f(1.0,1.0,1.0);

   /* Draw Walls */
   drawWalls(cameraX, cameraY, cameraZ, matriz, false);

   /* Draw Roads */
   /*if(roads)
   {
      roads->draw();
   }*/

   /* Draw objects */
   drawObjects(cameraX, cameraY, cameraZ, matriz, false);

   glDisable(GL_COLOR_MATERIAL);
   glColor3f(1.0,1.0,1.0);

   return(0);
}

/********************************************************************
 *                             drawWalls                            *
 ********************************************************************/
void Map::drawWalls(GLfloat cameraX, GLfloat cameraY, 
                    GLfloat cameraZ, GLfloat matriz[6][4],
                    bool inverted)
{
   glColor3f(1.0,1.0,1.0);
   wall* maux = walls;
   int texture = -1;
   bool visible = false;
   int wNum;
        int fezMeioFio = 0;
        GLfloat altura = WALL_HEIGHT;
        if(!maux)
        {
            maux = curbs;
            fezMeioFio = 1;
            altura = CURB_HEIGHT;
        }
        glBegin(GL_QUADS);
        for(wNum=0;wNum<totalWalls;wNum++ )
        {
           
           if((texture!= -1) && (maux->texture == -1))
           {
               glDisable(GL_TEXTURE_2D); 
               texture = -1;
           }
           else if(texture != maux->texture)
           {
              glEnd();
              texture = maux->texture;
              glEnable(GL_TEXTURE_2D);
              glBindTexture(GL_TEXTURE_2D, texture);
              glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
              glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

              glBegin(GL_QUADS);
           }
           if(inverted)
           {
              visible = visibleCube(maux->x1-VIS_DELTA,
                                    -altura-VIS_DELTA,maux->z1-VIS_DELTA,
                                    maux->x2+VIS_DELTA,
                                    VIS_DELTA,maux->z2+VIS_DELTA,matriz);
           }
           else
           {
              visible = visibleCube(maux->x1-VIS_DELTA,-VIS_DELTA,
                                    maux->z1-VIS_DELTA,maux->x2+VIS_DELTA,
                                    altura+VIS_DELTA,maux->z2+VIS_DELTA,matriz);
           }
           if(visible)
           {
              double X = (maux->x2-maux->x1) / maux->dX;
              double Z = (maux->z2-maux->z1) / maux->dZ;
              double Y = (altura+1) / maux->dY;
           /* Front Face */
              glNormal3i(0,0,1);
              glTexCoord2f(0,Y);
              glVertex3f(maux->x1,altura,maux->z1);
              glTexCoord2f(X,Y);
              glVertex3f(maux->x2,altura,maux->z1);
              glTexCoord2f(X,0);
              glVertex3f(maux->x2,0,maux->z1);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,0,maux->z1);
           /* Back Face */
              glNormal3i(0,0,-1);
              glTexCoord2f(0,Y);
              glVertex3f(maux->x1,altura,maux->z2);
              glTexCoord2f(X,Y);
              glVertex3f(maux->x2,altura,maux->z2);
              glTexCoord2f(X,0);
              glVertex3f(maux->x2,0,maux->z2);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,0,maux->z2);
           /* Left Face */
              glNormal3i(-1,0,0);
              glTexCoord2f(0,Y);
              glVertex3f(maux->x1,altura,maux->z1);
              glTexCoord2f(Z,Y);
              glVertex3f(maux->x1,altura,maux->z2);
              glTexCoord2f(Z,0);
              glVertex3f(maux->x1,0,maux->z2);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,0,maux->z1);
           /* Right Face */
              glNormal3i(1,0,0);
              glTexCoord2f(0,Y);
              glVertex3f(maux->x2,altura,maux->z1);
              glTexCoord2f(Z,Y);
              glVertex3f(maux->x2,altura,maux->z2);
              glTexCoord2f(Z,0);
              glVertex3f(maux->x2,0,maux->z2);
              glTexCoord2f(0,0);
              glVertex3f(maux->x2,0,maux->z1);
           /* Upper Face */
              glNormal3i(0,1,0);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,altura,maux->z1);
              glTexCoord2f(X,0);
              glVertex3f(maux->x2,altura,maux->z1);
              glTexCoord2f(X,Z);
              glVertex3f(maux->x2,altura,maux->z2);
              glTexCoord2f(0,Z);
              glVertex3f(maux->x1,altura,maux->z2);
           }
           maux = maux->next;
           if( (!maux) && (!fezMeioFio) )
           {
               maux = curbs;
               fezMeioFio = 1;
               altura = CURB_HEIGHT;
           }
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
}

/********************************************************************
 *                            drawObjects                           *
 ********************************************************************/
void Map::drawObjects(GLfloat cameraX, GLfloat cameraY, 
                      GLfloat cameraZ, GLfloat matriz[6][4],
                      bool inverted)
{
   int o;
   int Xaux, Zaux;
   GLfloat distancia;
   GLfloat deltaX, deltaZ;
   GLfloat deltaY2 = cameraY*cameraY;
   GLfloat min[3], max[3];
   GLfloat X[4], Z[4];
   boundingBox bound;
   objSquare* obj;

   float ssize = squareSize();
   float hsize = squareSize() / 2.0;

   for(Xaux = 0; Xaux < x; Xaux++)
   for(Zaux = 0; Zaux < z; Zaux++)
   {
      deltaX = (cameraX-MapSquares[Xaux][Zaux].x1+hsize);
      deltaZ = (cameraZ-MapSquares[Xaux][Zaux].z1+hsize);
      distancia = sqrt(deltaX*deltaX+deltaY2+deltaZ*deltaZ) / ssize;
      obj = MapSquares[Xaux][Zaux].getFirstObject();

      /* Draw All Needed Square Objects */
      for(o=0; o < MapSquares[Xaux][Zaux].getTotalObjects(); o++)
      {
          if( (obj != NULL) && (obj->draw) && 
              (!obj->obj->isStaticScenery()))
          {
            /* Do the Rotation of the Bounding Box */
            bound = obj->obj->getBoundingBox();
            X[0] = bound.x1;
            Z[0] = bound.z1;
            X[1] = bound.x1;
            Z[1] = bound.z2; 
            X[2] = bound.x2;
            Z[2] = bound.z2;
            X[3] = bound.x2;
            Z[3] = bound.z1;
            if(inverted)
            {
               rotTransBoundingBox(obj->orientation, X, Z, obj->x, 
                                   obj->y - bound.y2, obj->y - bound.y1, 
                                   obj->z, min, max);
            }
            else
            {
               rotTransBoundingBox(obj->orientation, X, Z, obj->x, 
                                   obj->y + bound.y1, obj->y + bound.y2, 
                                   obj->z, min, max );
            }

            /* Verify ViewFrustum Culling */
            if(visibleCube(min[0],min[1],min[2],max[0],max[1],max[2],
                           matriz))
            {
               glPushMatrix();
                glTranslatef(0.0, obj->y + obj->obj->yPosition, 0.0);
                obj->obj->draw(obj->x, obj->z, distancia, obj->orientation, 
                               inverted);
               glPopMatrix();
            }
         }
         obj = obj->next;
      }
      MapSquares[Xaux][Zaux].visible = 0;
   }

   /* Draw Doors */
   door* door = doors;
   while(door != NULL)
   {
      if(door->obj != NULL)
      {
         door->obj->draw(door->x,door->z,0,door->orientation, inverted);
      }
      door = door->next;
   }
}

/********************************************************************
 *                          Map Constructor                         *
 ********************************************************************/
Map::Map(lObject* lObjects)
{
   numTextures = 0;
   textures = NULL;
   name = "oxi!";
   squareInic = NULL;
   walls = NULL;
   totalWalls = 0;
   curbs = NULL; 
   totalCurbs = 0;
   MapSquares = NULL;
   doors = NULL;
   music = "";
   npcFileName = "";
   particlesFileName = "";
   outdoor = false;
   MapSquares = NULL;

   totalVertex = 0;
   vertexBuffer = NULL;
   uvBuffer = NULL;
   uvAlphaBuffer = NULL;
   commonTexture = 0;

   lakes = NULL;
   totalLakes = 0;
   
   /* Initialize Structs */
   objects = lObjects;
   x = z = 0;
   xInic = zInic = 0;
}

/********************************************************************
 *                                addLake                           *
 ********************************************************************/
lake* Map::addLake(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2)
{
   lake* l = new lake();
   l->definePosition(x1,z1,x2,z2);

   if(lakes == NULL)
   {
      l->next = l;
      l->previous = l;
   }
   else
   {
      l->next = lakes;
      l->previous = lakes->previous;
      l->next->previous = l;
      l->previous->next = l;
   }
   lakes = l;
   totalLakes++;
   return(l);
}

/********************************************************************
 *               Returns Square Relative to coordinate              *
 ********************************************************************/
Square* Map::relativeSquare(int xa, int za)
{
   if( (z <= za) || (x <= xa) || ( xa < 0) || (za < 0)) 
      return(NULL);

   return(&MapSquares[xa][za]);
}

/********************************************************************
 *                            getHeight                             *
 ********************************************************************/
GLfloat Map::getHeight(GLfloat nx, GLfloat nz)
{
   int posX =(int)floor( nx / (squareSize()));
   int posZ =(int)floor( nz / (squareSize())); 

   Square* saux = relativeSquare(posX, posZ);

   return(getHeight(nx, nz, saux));
}

/********************************************************************
 *                            getHeight                             *
 ********************************************************************/
GLfloat Map::getHeight(GLfloat nx, GLfloat nz, Square* saux)
{
   float d2, d4, h = 0;
   double a = 0, 
          b = 0,
          c = 0,
          d = 0,
          l = 0;

   if(!saux)
   {
      /* No positon on map, so no height! */
      return(0.0);
   }

   /* Calculate the squared distances */
   d2 = sqrt( (nx-saux->x2)*(nx-saux->x2) + (nz-saux->z1)*(nz-saux->z1) );
   d4 = sqrt( (nx-saux->x1)*(nx-saux->x1) + (nz-saux->z2)*(nz-saux->z2) );

   /* Verify to each triangle the nx,ny is at, by just looking
    * if it is near to v2 or v4 vertex. */

   /* FIXME -> precalcualte for each square the 2 equations, to
    * make less calculates here. */

   if( d2 <= d4 )
   {
      /* is at the upper triangle */
      a = ( (saux->h4 - saux->h1) * (saux->z2 - saux->z1) ); 
      b = -( (saux->z2 - saux->z1) * (saux->x2 - saux->x1) );
      c = ( (saux->x2 - saux->x1) * (saux->h3 - saux->h1) ) - 
          ( (saux->h4 - saux->h1) * (saux->x2 - saux->x1) );
      l = sqrt( a*a + b*b + c*c);
      a = a / l;
      b = b / l;
      c = c / l;
      d = -( (saux->x1*a) + (saux->h1*b) + (saux->z1*c) );
   }
   else
   {
      /* is at botton triangle */
      a = ( (saux->h3 - saux->h1) * (saux->z2 - saux->z1) ) -
          ( (saux->h2 - saux->h1) * (saux->z2 - saux->z1) );
      b = -( (saux->x2 - saux->x1) * (saux->z2 - saux->z1) );
      c = ( (saux->x2 - saux->x1) * (saux->h2 - saux->h1) );
      l = sqrt( a*a + b*b + c*c);
      a = a / l;
      b = b / l;
      c = c / l;
      d = -( (saux->x1*a) + (saux->h1*b) + (saux->z1*c) );
   }

   if(b != 0)
   {
      h = (-(a*nx) - (c*nz) - (d)) / b;
   }
   return(h);
}

/********************************************************************
 *                        drawSurfaceOnMap                          *
 ********************************************************************/
void Map::drawSurfaceOnMap(GLuint image, GLfloat xa, GLfloat za, 
                           GLfloat xb, GLfloat zb, GLfloat sumY,
                           int divisions)
{
   GLfloat incTex, incPosX, incPosZ;
   GLfloat texX, texZ;
   GLfloat pX1, pX2, pZ1, pZ2;
   int k, l;

   incTex = (1.0 / divisions);
   incPosX = (GLfloat) (xb - xa) / (GLfloat) divisions;
   incPosZ = (GLfloat) (zb - za) / (GLfloat) divisions;

   glColor4f(1,1,1,1);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glPushMatrix();
     glDisable(GL_LIGHTING);
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, image );
     glBegin(GL_QUADS);
     for(k=0; k < divisions; k++)
     {
        texX = k*incTex;
        pX1 = (k*incPosX) + xa;
        pX2 = (incPosX) + pX1;
        for(l=0; l < divisions; l++)
        {
           texZ = l*incTex;
           pZ1 = (l*incPosZ) + za;
           pZ2 = (incPosZ) + pZ1;
           glTexCoord2f(texX, texZ);
           glNormal3i(0,1,0);
           glVertex3f( pX1 , getHeight(pX1, pZ1)+sumY+0.5, pZ1 );
           glTexCoord2f(texX, texZ + incTex);
           glNormal3i(0,1,0);
           glVertex3f( pX1 , getHeight(pX1, pZ2)+sumY+0.5, pZ2);
           glTexCoord2f(texX + incTex, texZ + incTex);
           glNormal3i(0,1,0);
           glVertex3f( pX2, getHeight(pX2, pZ2)+sumY+0.5, pZ2 );
           glTexCoord2f(texX + incTex, texZ);
           glNormal3i(0,1,0);
           glVertex3f( pX2, getHeight(pX2, pZ1)+sumY+0.5, pZ1 );
        }
     }
     glEnd();
     glDisable(GL_TEXTURE_2D);
     glDisable(GL_BLEND);
     glEnable(GL_LIGHTING);
   glPopMatrix();
}

/********************************************************************
 *                          getFileName                             *
 ********************************************************************/
string Map::getFileName()
{
   return(name);
}

/********************************************************************
 *                          setFileName                             *
 ********************************************************************/
void Map::setFileName(string fileName)
{
   name = fileName;
}

/********************************************************************
 *                        getNpcFileName                            *
 ********************************************************************/
string Map::getNpcFileName()
{
   return(npcFileName);
}

/********************************************************************
 *                        setNpcFileName                            *
 ********************************************************************/
void Map::setNpcFileName(string fileName)
{
   npcFileName = fileName;
}

/********************************************************************
 *                        getMusicFileName                          *
 ********************************************************************/
string Map::getMusicFileName()
{
   return(music);
}

/********************************************************************
 *                        setMusicFileName                          *
 ********************************************************************/
void Map::setMusicFileName(string fileName)
{
   music = fileName;
}

/********************************************************************
 *                      getParticlesFileName                        *
 ********************************************************************/
string Map::getParticlesFileName()
{
   return(particlesFileName);
}

/********************************************************************
 *                      setParticlesFileName                        *
 ********************************************************************/
void Map::setParticlesFileName(string fileName)
{
   particlesFileName = fileName;
}

/********************************************************************
 *                             getSizeX                             *
 ********************************************************************/
int Map::getSizeX()
{
   return(x);
}

/********************************************************************
 *                             setSizeX                             *
 ********************************************************************/
void Map::setSizeX(int sizeX)
{
   x = sizeX;
}

/********************************************************************
 *                             getSizeZ                             *
 ********************************************************************/
int Map::getSizeZ()
{
   return(z);
}

/********************************************************************
 *                             setSizeZ                             *
 ********************************************************************/
void Map::setSizeZ(int sizeZ)
{
   z = sizeZ;
}

/********************************************************************
 *                        getInitialPosition                        *
 ********************************************************************/
void Map::getInitialPosition(float& iX, float& iZ)
{
   iX = xInic;
   iZ = zInic;
}

/********************************************************************
 *                        setInitialPosition                        *
 ********************************************************************/
void Map::setInitialPosition(GLfloat iX, GLfloat iZ)
{
   xInic = iX;
   zInic = iZ;
}

/********************************************************************
 *                             isOutDoor                            *
 ********************************************************************/
bool Map::isOutdoor()
{
   return(outdoor);
}

/********************************************************************
 *                            setOutDoor                            *
 ********************************************************************/
void Map::setOutdoor(bool val)
{
   outdoor = val;
}

/********************************************************************
 *                           getObject                              *
 ********************************************************************/
object* Map::getObject(string fileName)
{
   return(objects->getObject(fileName));
}


/********************************************************************
 *                       insertMapObject                            *
 ********************************************************************/
object* Map::insertObject(string arquivo, modelList& mdlList, 
                          weaponTypes& wTypes)
{
   return(objects->insertObject(arquivo, mdlList, wTypes));
}

/********************************************************************
 *                       Open Map File                              *
 ********************************************************************/
int Map::open(string arquivo, modelList& mdlList, weaponTypes& wTypes)
{
   FILE* arq;        // file used for the map
   char buffer[128]; // buffer used to read
   char nomeArq[128], nome[128];
   string arqVelho;
   int i;
   int posX,posZ;    //actual position of last active square
   int IDtextureAtual = -1;
   int IDwallTexturaAtual = -1;
   string nameMuroTexturaAtual = "nada";
   int pisavel=0;
   GLuint R,G,B;
   GLuint Ratual,Gatual,Batual;
   dirs dir; 
   
   if(!(arq = fopen(dir.getRealFile(arquivo).c_str(),"r")))
   {
      printf("Error while opening map file: %s\n",
             dir.getRealFile(arquivo).c_str());
	return(0);
   }

   /* Define map file Name */
   arqVelho = name;
   name = arquivo;

   xInic = -1;

   /* Read size of the map */   
   fscanf(arq, "%s", buffer);
   if(buffer[0] == 'T')
   {
      fgets(buffer, sizeof(buffer), arq); 
      sscanf(buffer,"%dX%d",&x,&z);
   }
   else
   {
      printf("Map Size not defined: %s\n", arquivo.c_str()); 
      printf(" Found %s, where espected T iXi\n",buffer);
      fclose(arq);
      return(0);
   }


   /* Alloc MapSquares */
   MapSquares = new Square*[x];
   for(i = 0; i < x; i++)
   {
      MapSquares[i] = new Square[z];
   } 

   /* Alloc Draw Buffers */
   vertexBuffer = new float[x*z*12];
   uvBuffer = new float[x*z*8];
   uvAlphaBuffer = new float[x*z*8];

   /* Alloc Roads Struct */
   //roads = new mapRoad(x, z);

   
   wall* maux = NULL;
   door* doorAux = NULL;

   posX = -1;
   posZ = 0;

   /* Read All File */
   while(fscanf(arq, "%s", buffer) != EOF)
   {
      switch(buffer[0])
      {
         case 'l':
         
         switch(buffer[1])
         {
            
            /* Define Light File */
            case 'i':
            {
               fgets(buffer, sizeof(buffer),arq);
               sscanf(buffer,"%s",nome);
               lights.Load(nome);
               break;
            }
            /* Add Lake to Map */
            case 'a':
            {
               fgets(buffer, sizeof(buffer),arq);
               float xa=0, za=0, xb=0, zb=0;
               float r=0,g=0,b=0,a=0;
               sscanf(buffer,"%f,%f,%f,%f:%f,%f,%f,%f", 
                              &xa, &za, &xb, &zb,
                              &r, &g, &b, &a);
               lake* l = addLake(xa,za,xb,zb);
               l->defineColor(r,g,b,a);
               break;
            }
         }
         break;

         case 'f':/* Define Fog File */
         {
            fgets(buffer, sizeof(buffer),arq);
            sscanf(buffer,"%s",nome);
            fog.Load(nome);
            break;
         }
         case 'P':/* Define Particle System File */
         {
            fgets(buffer, sizeof(buffer),arq);
            sscanf(buffer,"%s",nome);
            particlesFileName = nome;
            break;
         }
         case 'n':
         {
            fgets(buffer, sizeof(buffer),arq);
            sscanf(buffer,"%s",nome);
            npcFileName = nome;
            break;
         }
         case 'd': /* Define Doors */
         {
            doorAux = new(door);
            fgets(buffer, sizeof(buffer),arq);
            sscanf(buffer,"%s %f,%f:%d",nome,&doorAux->x,&doorAux->z,
                                        &doorAux->orientation);
            doorAux->obj = objects->getObject(nome);
            if(doorAux->obj == NULL)
            {
               printf("Can't Locate Door File: %s\n",nome);
            }
            else
            {
               doorAux->status = 0;
               doorAux->next = doors;
               doors = doorAux;
            }
            break;
         }
         case 'M': /* Define Music */
         {
            fgets(buffer,sizeof(buffer),arq);
            sscanf(buffer,"%s",nome);
            music = nome;
            break;
         }
         case 'w': /* Define Walls and Half Walls */
         {
            switch(buffer[1])
            {
               case 'a': /* Define Wall */
               {
                  maux = addWall(0,0,0,0);
                  fgets(buffer, sizeof(buffer),arq);
                  sscanf(buffer,"%f,%f,%f,%f:%d,%d,%d",&maux->x1,&maux->z1,
                                              &maux->x2,&maux->z2,
                                              &maux->dX,&maux->dY,&maux->dZ);
                  double tmp;
                  if(maux->x2 < maux->x1)
                  {
                     tmp = maux->x2;
                     maux->x2 = maux->x1;
                     maux->x1 = tmp;
                  }
                  if(maux->z2 < maux->z1)
                  {
                     tmp = maux->z2;
                     maux->z2 = maux->z1;
                     maux->z1 = tmp;
                  }
                  maux->texture = -1;
                  break;
               }
               case 't': /* Define Wall texture */
               {
                  fgets(buffer, sizeof(buffer), arq);
                  sscanf(buffer,"%s",nome);
                     nameMuroTexturaAtual = nome;
                     IDwallTexturaAtual = getTextureID(nome,R,G,B);
                  maux->texture = IDwallTexturaAtual;
                  break;
               }
               case 'e': /* Define Half Wall */
               {
                  maux = new(wall);
                  fgets(buffer, sizeof(buffer),arq);
                  sscanf(buffer,"%f,%f,%f,%f",&maux->x1,&maux->z1,
                                              &maux->x2,&maux->z2);

                  double tmp;
                  if(maux->x2 < maux->x1)
                  {
                     tmp = maux->x2;
                     maux->x2 = maux->x1;
                     maux->x1 = tmp;
                  }
                  if(maux->z2 < maux->z1)
                  {
                     tmp = maux->z2;
                     maux->z2 = maux->z1;
                     maux->z1 = tmp;
                  }  
                  maux->next = curbs;
                  maux->texture = -1;
                  curbs = maux;
                  totalCurbs++;
                  break;
               }
            }
            break;
         }
         case 'i':/* Define initial Character Position */
         {
            fgets(buffer, sizeof(buffer), arq); 
            if(xInic == -1)
            {
               sscanf(buffer, "%f,%f",&xInic,&zInic);
               int posX =(int)floor( xInic / (squareSize()));
               int posZ =(int)floor( zInic / (squareSize()));
               squareInic = relativeSquare(posX,posZ);
            }
            break;
         }
         case 'o': /* Insert Object */
         {
             fgets(buffer, sizeof(buffer), arq); 
             sscanf(buffer, "%s",nomeArq);
             objects->insertObject(nomeArq, mdlList, wTypes);
             break;
         }
         case 'O': /* Define OutDoor */
         {
            int a;
            fgets(buffer, sizeof(buffer), arq);
            sscanf(buffer, "%d", &a);
            outdoor = a;
            break;
         }
         case 't': /* Insert Textures */
         {
            fgets(buffer, sizeof(buffer), arq);
            sscanf(buffer, "%s %s %d %d %d",nome,nomeArq,&R,&G,&B);  
            insertTexture(nomeArq,nome,R,G,B);
            break;
         }
         case 'p': /* Insert new square */
         {
            if(posX == (x-1)) //end of the line of squares
            { 
                posX = 0;
                posZ++;
            }
            else  //remains in the same line of squares
            {
               posX++;
            }
            fgets(buffer, sizeof(buffer), arq); 
            sscanf(buffer, "%d,%f,%f,%f,%f",&pisavel,
                                 &MapSquares[posX][posZ].h1,
                                 &MapSquares[posX][posZ].h2,
                                 &MapSquares[posX][posZ].h3,
                                 &MapSquares[posX][posZ].h4);

            MapSquares[posX][posZ].setDivisions(); 
            MapSquares[posX][posZ].x1 = (posX) * squareSize();
            MapSquares[posX][posZ].x2 = MapSquares[posX][posZ].x1+squareSize();
            MapSquares[posX][posZ].z1 = (posZ) * squareSize();
            MapSquares[posX][posZ].z2 = MapSquares[posX][posZ].z1+squareSize(); 
            MapSquares[posX][posZ].posX = posX;
            MapSquares[posX][posZ].posZ = posZ;
            if(pisavel) 
            {
               MapSquares[posX][posZ].flags = SQUARE_CAN_WALK;
            }
            break;
         }
         case 'u':/* Use something already declared */
         {
            switch(buffer[1])
            {
               case 'c': /* define conection on square */
               {
                 fgets(buffer, sizeof(buffer), arq);
                 sscanf(buffer,"%f,%f,%f,%f:%s",
                        &MapSquares[posX][posZ].mapConection.x1,
                        &MapSquares[posX][posZ].mapConection.z1,
                        &MapSquares[posX][posZ].mapConection.x2,
                        &MapSquares[posX][posZ].mapConection.z2,
                        nome );
                 MapSquares[posX][posZ].mapConection.mapName = nome;
                 MapSquares[posX][posZ].mapConection.active = true;

                 if(arqVelho == (MapSquares[posX][posZ].mapConection.mapName))
                 {
                     squareInic = &MapSquares[posX][posZ];
                     xInic = (MapSquares[posX][posZ].mapConection.x1 + 
                              MapSquares[posX][posZ].mapConection.x2) / 2.0;
                     zInic = (MapSquares[posX][posZ].mapConection.z1 + 
                              MapSquares[posX][posZ].mapConection.z2) / 2.0;
                 }

                 break;
               }
               case 't': /* Define Square's Textura */
               {
                  fgets(buffer, sizeof(buffer), arq);
                  sscanf(buffer,"%s",nome);
                  IDtextureAtual = getTextureID(nome,Ratual,Gatual,Batual);
                  MapSquares[posX][posZ].texture = IDtextureAtual;
                  MapSquares[posX][posZ].R = Ratual;
                  MapSquares[posX][posZ].G = Gatual;
                  MapSquares[posX][posZ].B = Batual;
                  break;
               }
               case 'o': /* Insert Object on Square */
               {
                  int des, quadX, quadZ, oOri, oPis;
                  float oX, oY, oZ;
                  objSquare* oObj;
                  fgets(buffer, sizeof(buffer), arq);
                  sscanf(buffer,"%s %d:%d,%d:%f,%f,%f:%d:%d",nome,
                         &des, &quadX, &quadZ, &oX, &oY, &oZ, &oOri, &oPis);

                  oObj = MapSquares[posX][posZ].addObject(des==1, quadX,quadZ,
                                                          oOri,oX,oY,oZ,oPis!=1,
                                                    objects->getObject(nome));
                  if(oObj->draw)
                  {
                     oObj->obj->incUsedFlag();
                     if(oObj->obj->isStaticScenery())
                     {
                        oObj->obj->addRenderPosition(oX, oY, oZ, oOri);
                     }
                  }
                  break;
               }
               default:
               {
                  printf("What the Hell: %s on %s\n",buffer,arquivo.c_str());
                  break;
               }
            }
            break; 
         }
         case '#': //ignore comments
         {
             fgets(buffer, sizeof(buffer), arq);
             break;
         }
         default: //something not defined!
         {
                 printf("What is: %s on %s ?\n",buffer,arquivo.c_str());
                 break;
         }
      }
   }
   fclose(arq);

   int ax,az;
   
   /* Now, actualize pointers to the walls */
   maux = walls;
   int wNum;
   int inix,iniz,maxx,maxz;
   int indexMuro;
   Square* aux;
   float ssize = squareSize();
   
   for(wNum = 0; wNum < totalWalls; wNum++)
   {
      inix = (int)floor(maux->x1 / ssize);
      iniz = (int)floor(maux->z1 / ssize);
      maxx = (int)floor(maux->x2 / ssize);
      maxz = (int)floor(maux->z2 / ssize);
      for(ax = inix;ax<=maxx;ax++)
      {
          for(az = iniz;az<=maxz;az++)
          {
              indexMuro = 0;
              aux = relativeSquare(ax,az);
              while((aux!=NULL) && (indexMuro < MAX_WALLS) && 
                    (aux->walls[indexMuro] != NULL))
              {
                 indexMuro++;
              }
              if((aux!=NULL) && (indexMuro < MAX_WALLS))
              {
                 aux->walls[indexMuro] = maux;
              }
              else if(indexMuro >= MAX_WALLS)
                 printf("Quad: %d %d has more walls than permitted: %d\n",ax,az,
                          indexMuro);
          }
      }
      maux = maux->next;
   }

   /* Define minimap sizes */
   SQUAREMINISIZE = 2;
   SQUAREMINIDIV = (squareSize() / SQUAREMINISIZE);

   /* And create the splats */
   createSplats();

   return(1);
}


/************************************************************************
 *                     Create a new Map                                 *
 ************************************************************************/
void Map::newMap(int X, int Z)
{
   int auxX, auxZ;
   int i;

   /* Alloc MapSquares */
   MapSquares = new Square*[X];//(Square***) malloc(X*sizeof(Square**));
   for(i = 0; i < X; i++)
   {
      MapSquares[i] = new Square[Z];//(Square**) malloc(Z*sizeof(Square*));
   } 

   //roads = new mapRoad(X, Z);

   
   Square* saux;
   x = X;
   z = Z;

   /* Alloc Draw Buffers */
   vertexBuffer = new float[x*z*12];
   uvBuffer = new float[x*z*8];
   uvAlphaBuffer = new float[x*z*8];


   /* add a first default texture */
   int IDtexture = insertTexture("texturas/floor_outdoor/grass.png", 
                                 "texturas/floor_outdoor/grass.png",
                                 54,102,49);

   for(auxZ = 0; auxZ < z; auxZ++)
   {
      for(auxX = 0; auxX < x; auxX++)
      {
          saux = relativeSquare(auxX,auxZ);
          saux->x1 = (auxX)*squareSize();
          saux->x2 = saux->x1+squareSize();
          saux->z1 = (auxZ)*squareSize();
          saux->z2 = saux->z1+squareSize(); 
          saux->posX = auxX;
          saux->posZ = auxZ;
          saux->flags = SQUARE_CAN_WALK;
          saux->texture = IDtexture;
          saux->R = 130;
          saux->G = 148;
          saux->B = 96;
      }
   }

   /* Define new minimap sizes */
   SQUAREMINISIZE = 2;
   SQUAREMINIDIV = (squareSize() / SQUAREMINISIZE);

   xInic = 1*squareSize();
   zInic = 1*squareSize();
   squareInic = relativeSquare(0,0);

   /* And create the splats */
   createSplats();

}


/*******************************************************************
 *   Optimize map (wall sobreposition, object squares, etc)        *
 *******************************************************************/
void Map::optimize()
{
    /* Verify Wall Superposition */
    wall* maux = walls;
    int wNum, wNum2;
    for(wNum = 0; wNum < totalWalls; wNum++)
    {
        wall* maux2 = walls;
        for(wNum2 = 0; wNum2 < totalWalls; wNum2++)
        {
            if(maux != maux2)
            {
                if( (maux->x1 == maux2->x1) && 
                    ( (maux->z1 >= maux2->z1) && (maux->z2 <= maux2->z2) ||
                      (maux->z1 <= maux2->z1) && (maux->z2 >= maux2->z2) ))
                {
                    if(maux->x2 != maux->x1+10)
                    {
                       maux->x1 += 10;
                    }
                    else if(maux2->x2 != maux2->x1+10)
                    {
                       maux2->x1+= 10;
                    }
                    else
                    {
                       //deleta maux(2)
                    }
                }
                else if( (maux->x2 == maux2->x2) && 
                    ( (maux->z1 >= maux2->z1) && (maux->z2 <= maux2->z2) ||
                      (maux->z1 <= maux2->z1) && (maux->z2 >= maux2->z2) ))
                {
                    if(maux->x2 != maux->x1+10)
                    {
                       maux->x2 -= 10;
                    }
                    else if(maux2->x2 != maux2->x1+10)
                    {
                       maux2->x2 -= 10;
                    }
                    else
                    {
                       //deleta maux(2)
                    }
                }
                else if( (maux->z1 == maux2->z1) && 
                    ( (maux->x1 >= maux2->x1) && (maux->x2 <= maux2->x2) ||
                      (maux->x1 <= maux2->x1) && (maux->x2 >= maux2->x2) ))
                {
                    if(maux->z2 != maux->z1+10)
                    {
                       maux->z1 += 10;
                    }
                    else if(maux2->z2 != maux2->z1+10)
                    {
                       maux2->z1+= 10;
                    }
                    else
                    {
                       //deleta maux(2)
                    }
                }
                else if( (maux->z2 == maux2->z2) && 
                    ( (maux->x1 >= maux2->x1) && (maux->x2 <= maux2->x2) ||
                      (maux->x1 <= maux2->x1) && (maux->x2 >= maux2->x2) ))
                {
                    if(maux->z2 != maux->z1+10)
                    {
                       maux->z2 -= 10;
                    }
                    else if(maux2->z2 != maux2->z1+10)
                    {
                       maux2->z2 -= 10;
                    }
                    else
                    {
                       //deleta maux(2)
                    }
                }
            }
            maux2 = maux2->next;
        }
        maux = maux->next;
    }

   /* Verify Object Occupied Squares */
//TODO

}


/********************************************************************
 *                          Save Map File                           *
 ********************************************************************/
int Map::save(string arquivo)
{
   optimize();
   FILE* arq;
   dirs dir;

   if(!(arq = fopen(dir.getRealFile(arquivo).c_str(),"w")))
   {
      printf("Error while creating: %s to save\n",
             dir.getRealFile(arquivo).c_str());
	return(0);
   }

   /* Remove Unused Textures */
   removeUnusedTextures();
   
   /* Write Dimensions */
   fprintf(arq,"T %dX%d\n",x,z);
   fprintf(arq,"# Made by DccNiTghtmare's MapEditor, v0.2\n");

   /* Write fog file name, if exists */
   if( !fog.fileName.empty() )
   {
     fprintf(arq,"f %s\n",fog.fileName.c_str());
   }
  
   /* Write NPC file name, if exists */
   if( !npcFileName.empty())
   {
     fprintf(arq,"npc %s\n",npcFileName.c_str());
   }

   /* Write Particles file */
   if( !particlesFileName.empty())
   {
     fprintf(arq,"PS %s\n",particlesFileName.c_str());
   }

 
   /* Write music file name */
   if( !music.empty())
   {
     fprintf(arq,"MUSICA %s\n",music.c_str());
   }

   /* Write Lights file name */
   if(!lights.getFileName().empty())
   {
      fprintf(arq,"light %s\n",lights.getFileName().c_str());
   }

   /* Write if is outdoor or not */
   fprintf(arq,"Outdoor: %d\n", outdoor);

   /* Write lakes */
   int i;
   float xa=0,za=0,xb=0,zb=0;
   float r=0,g=0,b=0,a=0;

   lake* l = lakes;
   for(i = 0; i < totalLakes; i++)
   {
      l->getPosition(xa,za,xb,zb);
      l->getColor(r,g,b,a);
      fprintf(arq,"lake %.3f,%.3f,%.3f,%.3f:%.3f,%.3f,%.3f,%.3f\n",
                        xa,  za,  xb,  zb,  r,   g,   b,   a);
      l = l->next;
   }

   /* Write used objects */
   if(objects->total>0)
   {
      object* objAux = objects->first;
      for(i = 0; i < objects->total; i++)
      {
         if(!objAux->getFileName().empty())
         {
            fprintf(arq,"o %s\n",objAux->getFileName().c_str());
         }
         else
         {
            printf("Object FileName is Empty!!!\n");
         }
         objAux = objAux->next;
      }
   }

   /* Write used Textures */
   texture* tex = (texture*)textures;
   int t;
   for(t = 0; t < numTextures; t++)
   {
      fprintf(arq,"t %s %s %d %d %d\n",tex->name.c_str(),tex->fileName.c_str(),
                                       tex->R,tex->G,tex->B);
      tex = (texture*)tex->next;
   }

   /* Write Doors */
   door* doorAux = (door*)doors;
   while(doorAux != NULL)
   {
      fprintf(arq,"d %s %f,%f:%d\n",doorAux->obj->getFileName().c_str(),
                                    doorAux->x,doorAux->z,
                                    doorAux->orientation);
      doorAux = doorAux->next;
   }
   
   /* Write Walls */
   wall* maux = (wall*)walls;
   int x1,z1,x2,z2,wNum;
   for(wNum=0; wNum < totalWalls; wNum++)
   {
      fprintf(arq,"wall %f,%f,%f,%f:%d,%d,%d\n",maux->x1,maux->z1,maux->x2,
                                                maux->z2,maux->dX,maux->dY,
                                                maux->dZ);
      fprintf(arq,"wt %s\n",getTextureName(maux->texture).c_str());
      maux = (wall*)maux->next;
   }

   /* Write Half Walls */
   maux = (wall*)curbs;
   while(maux)
   {
      fprintf(arq,"we %f,%f,%f,%f\n",maux->x1,maux->z1,maux->x2,maux->z2);
      fprintf(arq,"wt %s\n",getTextureName(maux->texture).c_str());
      maux = (wall*)maux->next;
   }

 
   /* Write Squares, line per line */
   for(z1=0;z1<z;z1++)
   {
      fprintf(arq,"#Z: %d\n",z1);
      for(x1=0;x1<x;x1++)
      {
          fprintf(arq,"p %d,%f,%f,%f,%f\n",
                  MapSquares[x1][z1].flags,
                  MapSquares[x1][z1].h1,
                  MapSquares[x1][z1].h2,
                  MapSquares[x1][z1].h3,
                  MapSquares[x1][z1].h4);
          fprintf(arq,"ut %s\n",
                  getTextureName(MapSquares[x1][z1].texture).c_str());
          if( MapSquares[x1][z1].mapConection.active )
          {
              fprintf(arq,"uc %f,%f,%f,%f:%s\n",
                      MapSquares[x1][z1].mapConection.x1,
                      MapSquares[x1][z1].mapConection.z1,
                      MapSquares[x1][z1].mapConection.x2,
                      MapSquares[x1][z1].mapConection.z2,
                      MapSquares[x1][z1].mapConection.mapName.c_str());
          }
          int aux;
          objSquare* obj = MapSquares[x1][z1].getFirstObject();
          for(aux=0; aux < MapSquares[x1][z1].getTotalObjects(); aux++)
          {
            if(obj->obj)
            {
               x2 = (int)obj->x / squareSize();
               z2 = (int)obj->z / squareSize();
               fprintf(arq,"uo %s %d:%d,%d:%f,%f,%f:%d:%d\n",
                       obj->obj->getFileName().c_str(),
                       obj->draw, x2 + 1, z2 + 1,
                       obj->x, obj->y, obj->z, obj->orientation, 
                       !obj->colision);
            }
            obj = obj->next;
          }
      }
   }

   /* Write Initial Character Position */
   fprintf(arq,"i %f,%f\n",xInic,zInic);

   fclose(arq);
   return(1);
}

/********************************************************************
 *                         Map Destructor                           *
 ********************************************************************/
Map::~Map()
{
   /* Delete All Textures */
   texture* tex = textures;
   texture* au;
   int i;
   int aux;
   for(i=0;i<numTextures;i++)
   {
      au = tex;
      tex = tex->next;

      /* Delete the OpenGL Texture */
      glDeleteTextures(1,&(au->index));

      /* Delete the alpha Texture */
      if(au->definedAlpha)
      {
         glDeleteTextures(1,&(au->alphaTexture));
      }

      /* Delete the Alpha Matrix */
      for(aux = 0; aux < (x*ALPHA_TEXTURE_INC); aux++)
      {
         delete[] (au->alphaValues[aux]);
      }
      delete[] au->alphaValues;

      /* Delete the texture struct */
      delete(au);
   }
   
   /* Delete all Walls */
   wall* m = walls;
   wall* am =NULL;
   int wNum;
   for(wNum = 0; wNum < totalWalls; wNum++)
   {
      am = m;
      m = m->next;
      delete(am);
   }

   /* Deleting wall Doors */
   door* door1 = doors;
   door* door2 =NULL;
   while(door1)
   {
      door2 = door1;
      door1 = door1->next;
      delete(door2);
   }

   /* Delete all Buffers */
   if(vertexBuffer != NULL)
   {
      delete[] vertexBuffer;
   }
   if(uvBuffer != NULL)
   {
      delete[] uvBuffer;
   }
   if(uvAlphaBuffer != NULL)
   {
      delete[] uvAlphaBuffer;
   }

   /* Unselect the objects list */
   objects = NULL;

   /* Deleting all squares */
   int x1;
   for(x1 = 0; x1<x;x1++)
   {
      delete[](MapSquares[x1]);
   }
   delete[] (MapSquares);

   /* Deleting Lakes */
   //TODO

   /* Deleting Roads */
   /*if(roads)
   {
      delete(roads);
   }*/
}


/********************************************************************
 *                       Draw MiniMap on Surface                    *
 ********************************************************************/
void Map::drawMinimap(SDL_Surface* img)
{
   int x1,y1,x2,y2, X, Z;

   int limX=0, limZ=0, iX=0, iZ=0, sX = 0, sZ = 0;

   if(isOutdoor())
   {
      limX = x-7;
      limZ = z-7;
      iX = 7;
      iZ = 7;
      sX = x-14;
      sZ = z - 14;
   }
   else
   {
      limX = x;
      limZ = z;
      iX = 0;
      iZ = 0;
      sX = x;
      sZ = z;
   }

   x1 = 0; y1 = 0;
   for(Z = iZ; Z < limZ; Z++)
   {
      for(X = iX; X < limX; X++)
      {
          color_Set(MapSquares[X][Z].R,
                    MapSquares[X][Z].G,
                    MapSquares[X][Z].B, 255);
          rectangle_Fill(img, x1,y1,x1+SQUAREMINISIZE-1,y1+SQUAREMINISIZE-1);
          x1+=SQUAREMINISIZE;
      }
      x1 = 0;
      y1+=SQUAREMINISIZE;
   }

   color_Set(1, 1, 1, 255);
   rectangle_2Colors(img,0,0,sX*SQUAREMINISIZE-1,sZ*SQUAREMINISIZE-1,0,0,0,255);
   
   wall* maux = walls;
   int wNum;
   for(wNum = 0; wNum < totalWalls; wNum++)
   {
      //FIXME walls values when outdoor!
       x1 = (int) ( ((float)maux->x1 / (float)SQUAREMINIDIV ));
       x2 = (int) ( (((float)maux->x2 / (float)SQUAREMINIDIV))-1 );
       y1 = (int) ( ((float)maux->z1 / (float)SQUAREMINIDIV ));
       y2 = (int) ( (((float)maux->z2 / (float)SQUAREMINIDIV))-1 );
       if( (x2-x1) < (y2-y1))
          x2 = x1;
       else 
          y2 = y1;
       color_Set(255,40,30,255);
       line_Draw(img, x1,y1,x2,y2);
       maux = maux->next;
   }

}

/********************************************************************
 *                              createSplats                        *
 ********************************************************************/
void Map::createSplats()
{
   int x1, z1;
   int actualTexture = 0;
   float alphaCoordX = 0;
   float alphaCoordZ = 0;
   float modX = smallestPowerOfTwo(x*ALPHA_TEXTURE_INC);
   float modZ = smallestPowerOfTwo(z*ALPHA_TEXTURE_INC);

   /* Reinit the vertex buffer */
   totalVertex = 0;

   /* Create the Buffers */
   for(z1=0;z1<z;z1++)
   {
      alphaCoordX = 0;
      for(x1=0;x1<x;x1++)
      {
         uvBuffer[actualTexture] = 0.0;
         uvBuffer[actualTexture+1] = 0.0;
         uvAlphaBuffer[actualTexture] = alphaCoordX / modX;
         uvAlphaBuffer[actualTexture+1] = alphaCoordZ / modZ;
         vertexBuffer[totalVertex] = MapSquares[x1][z1].x1;
         vertexBuffer[totalVertex+1] = MapSquares[x1][z1].h1;
         vertexBuffer[totalVertex+2] = MapSquares[x1][z1].z1;

         uvBuffer[actualTexture+2] = 0.0;
         uvBuffer[actualTexture+3] = TEXTURE_REPEATS;
         uvAlphaBuffer[actualTexture+2] = alphaCoordX / modX;
         uvAlphaBuffer[actualTexture+3] = (alphaCoordZ+ALPHA_TEXTURE_INC)/modZ;
         vertexBuffer[totalVertex+3] = MapSquares[x1][z1].x1;
         vertexBuffer[totalVertex+4] = MapSquares[x1][z1].h2;
         vertexBuffer[totalVertex+5] = MapSquares[x1][z1].z2;

         uvBuffer[actualTexture+4] = TEXTURE_REPEATS;
         uvBuffer[actualTexture+5] = TEXTURE_REPEATS;
         uvAlphaBuffer[actualTexture+4] = (alphaCoordX+ALPHA_TEXTURE_INC)/modX; 
         uvAlphaBuffer[actualTexture+5] = (alphaCoordZ+ALPHA_TEXTURE_INC)/modZ;
         vertexBuffer[totalVertex+6] = MapSquares[x1][z1].x2;
         vertexBuffer[totalVertex+7] = MapSquares[x1][z1].h3;
         vertexBuffer[totalVertex+8] = MapSquares[x1][z1].z2;

         uvBuffer[actualTexture+6] = TEXTURE_REPEATS;
         uvBuffer[actualTexture+7] = 0.0;
         uvAlphaBuffer[actualTexture+6] = (alphaCoordX+ALPHA_TEXTURE_INC)/modX; 
         uvAlphaBuffer[actualTexture+7] = alphaCoordZ / modZ;
         vertexBuffer[totalVertex+9] = MapSquares[x1][z1].x2;
         vertexBuffer[totalVertex+10] = MapSquares[x1][z1].h4;
         vertexBuffer[totalVertex+11] = MapSquares[x1][z1].z1;

         /* Create The Alphas */
         createAlpha(x1,z1);
         
         totalVertex += 12;
         actualTexture += 8;
         alphaCoordX += ALPHA_TEXTURE_INC;
     }
     alphaCoordZ += ALPHA_TEXTURE_INC;
  }

   updateAlphaTextures();
   defineCommonTexture();
}

/********************************************************************
 *                            createAlpha                           *
 ********************************************************************/
void Map::createAlpha(int x1, int z1)
{
   int z2, x2, aux;
   float total = 0, dist = 0, value = 0;
   float incCoord = 1.0 / (float)ALPHA_TEXTURE_INC;
   float actualCoordX = x1, actualCoordZ = z1;
   int neigX, neigZ;
   texture* tex;

   for(z2 = z1*ALPHA_TEXTURE_INC; z2 < (z1+1)*ALPHA_TEXTURE_INC; z2++)
   {
      actualCoordX = x1;
      for(x2 = x1*ALPHA_TEXTURE_INC; x2 <(x1+1)*ALPHA_TEXTURE_INC; x2++)
      {
         total = 0;

         /* Clear Alpha Values */
         aux = 0;
         tex = textures;
         while(aux < numTextures)
         {
            tex->alphaValues[x2][z2] = 0;
            tex = tex->next;
            aux++;
         }

         /* Make Alpha for Outdoor maps */
         if(isOutdoor())
         {
            /* Visit all 8 potential Neighbors */
            for(neigZ = z1-1; neigZ <= z1+1; neigZ++)
            {           
               /* Verify if the Z coordinate is valid */
               if( (neigZ >= 0) && (neigZ < z) )
               {
                  for(neigX = x1-1; neigX <= x1+1; neigX++)
                  {
                     /* Verify if the X coordinate is valid */
                     if( (neigX >= 0) && (neigX < x) )
                     {
                        tex = getTexture(MapSquares[neigX][neigZ].texture);
                        if(tex)
                        {
                           dist = (actualCoordX-neigX)*(actualCoordX-neigX)+
                                  (actualCoordZ-neigZ)*(actualCoordZ-neigZ);
                           value = 1 - (dist / 3.0625);
                           if(value < 0)
                           {
                              value = 0.0;
                           }
                           tex->alphaValues[x2][z2] += value;
                           total += value;
                        }
                     }
                  }
               }
            }
         }
         /* Do the Alpha for the indoor maps. This is only the texture. */
         else
         {
            tex = getTexture(MapSquares[x1][z1].texture);
            if(tex)
            {
               tex->alphaValues[x2][z2] = 1.0;
               total = 1.0;
            }
         }
         /* Normalize the result */
         if(outdoor)
         {
            aux = 0;
            tex = textures;
            while(aux < numTextures)
            {
               tex->alphaValues[x2][z2] = tex->alphaValues[x2][z2] / total;
               tex = tex->next;
               aux++;
            }
         }

         actualCoordX += incCoord;
      }
      actualCoordZ += incCoord;
   }
}

/********************************************************************
 *                        updateAlphaTextures                       *
 ********************************************************************/
void Map::updateAlphaTextures()
{
   int aux = 0;
   texture* tex = textures;
   int x1, z1;
   SDL_Surface* img = SDL_CreateRGBSurface(SDL_HWSURFACE,
                                     smallestPowerOfTwo(x*ALPHA_TEXTURE_INC),
                                     smallestPowerOfTwo(z*ALPHA_TEXTURE_INC),
                                     32,0x000000FF,0x0000FF00,
                                     0x00FF0000,0xFF000000);
   while(aux < numTextures)
   {
      if(tex->definedAlpha)
      {
         glDeleteTextures(1,&(tex->alphaTexture));
      }
      tex->definedAlpha = true;

      /* Define The SDL_Surface */
      for(x1=0; x1 < x*ALPHA_TEXTURE_INC; x1++)
      {
         for(z1=0; z1 < z*ALPHA_TEXTURE_INC; z1++)
         {
            pixel_Set(img, x1, z1, 255, 255, 255, 
                      (int)floor(((tex->alphaValues[x1][z1])*255)));
         }
      }

      glGenTextures(1, &(tex->alphaTexture));
      glBindTexture(GL_TEXTURE_2D, tex->alphaTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 
                   0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,  GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      tex = tex->next;
      aux++;
   }
   SDL_FreeSurface(img);
}

/********************************************************************
 *                        defineCommonTexture                       *
 ********************************************************************/
void Map::defineCommonTexture()
{
   texture* actualCommon = textures;
   /* Zero the Count of each texture */
   int x1,z1,aux = 0;
   texture* tex = textures;
   while(aux < numTextures)
   {
      tex->count = 0;
      tex = tex->next;
      aux++;
   }

   /* Count the use of each texture */
   for(x1=0;x1<x;x1++)
   {
      for(z1=0;z1<z;z1++)
      {
         tex = getTexture(MapSquares[x1][z1].texture);
         tex->count++;
         if(tex->count > actualCommon->count)
         {
            actualCommon = tex;
         }
      }
   }

   /* Get the most common texture */
   commonTexture = tex->index;
}

/********************************************************************
 *                         defineThingHeight                        *
 ********************************************************************/
bool Map::defineThingHeight(thing* c, GLfloat nx, GLfloat nz)
{
   GLfloat altura_atual = c->yPosition;

   GLfloat res = getHeight(nx, nz);

   if( res - altura_atual > c->walk_interval)
   {
       c->yPosition = altura_atual;
       return(false);
   }

   c->yPosition = res;
   return(true);
}

