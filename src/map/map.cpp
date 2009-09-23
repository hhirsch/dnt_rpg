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


#include "map.h"

#include "../engine/culling.h"
#include "../engine/util.h"
#include "../etc/extensions.h"
#include "../engine/options.h"
#include "../etc/dirs.h"
#include "../etc/defparser.h"
#include "../lang/translate.h"

#ifdef _MSC_VER
   #include "../config_win.h"
#else
   #include "../config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#define VIS_DELTA 64  /**< Delta to Visible Culling */

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                                  SQUARE                                  //
//                                                                          //
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
   mapConection.angle = 0.0;
   divisions = 1;
   int aux;
   for(aux=0; aux < MAX_WALLS; aux++)
   {
     walls[aux] = NULL;
   }
}

/********************************************************************
 *                       Square Destructor                          *
 ********************************************************************/
Square::~Square()
{
   clearList();
}

/********************************************************************
 *                           addObject                              *
 ********************************************************************/
objSquare* Square::addObject(bool draw, GLfloat x, GLfloat y, GLfloat z,
                             GLfloat orientation, bool colision, object* obj)
{
   /* Define the new objSquare */
   objSquare* n = new objSquare;
   n->draw = draw;
   n->status = 0;
   n->x = x;
   n->y = y;
   n->z = z;
   n->orientation = orientation;
   n->colision = colision;
   n->obj = obj;

   /* Add it to the list */
   insert(n);

   return(n);
}

/********************************************************************
 *                         removeObject                             *
 ********************************************************************/
void Square::removeObject(objSquare* obj)
{
   remove(obj);
}

/********************************************************************
 *                          freeElement                             *
 ********************************************************************/
void Square::freeElement(dntListElement* obj)
{
   objSquare* o = (objSquare*)obj;
   delete(o);
}

/********************************************************************
 *                         getFirstObject                           *
 ********************************************************************/
objSquare* Square::getFirstObject()
{
   return((objSquare*)first);
}

/********************************************************************
 *                        getTotalObjects                           *
 ********************************************************************/
int Square::getTotalObjects()
{
   return(total);
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
//                                                                          //
//                               WallTexture                                //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *                            Constructor                           *
 ********************************************************************/
wallTexture::wallTexture()
{
   textureId = -1;
   textureName = "";
   dX = 16;
   dY = 16;
   dZ = 16;
}

/********************************************************************
 *                                 =                                *
 ********************************************************************/
void wallTexture::operator=(wallTexture* t)
{
   dX = t->dX;
   dY = t->dY;
   dZ = t->dZ;
   textureId = t->textureId;
   textureName = t->textureName;
}

/********************************************************************
 *                           getTextureId                           *
 ********************************************************************/
int wallTexture::getTextureId()
{
   return(textureId);
}

/********************************************************************
 *                           setTextureId                           *
 ********************************************************************/
void wallTexture::setTextureId(int id)
{
   textureId = id;
}

/********************************************************************
 *                          getTextureName                          *
 ********************************************************************/
string wallTexture::getTextureName()
{
   return(textureName);
}

/********************************************************************
 *                          setTextureName                          *
 ********************************************************************/
void wallTexture::setTextureName(string name)
{
   textureName = name;
}

/********************************************************************
 *                              getDelta                            *
 ********************************************************************/
void wallTexture::getDelta(GLuint& x, GLuint& y, GLuint& z)
{
   x = dX;
   y = dY;
   z = dZ;
}

/********************************************************************
 *                              setDelta                            *
 ********************************************************************/
void wallTexture::setDelta(GLuint x, GLuint y, GLuint z)
{
   dX = x;
   dY = y;
   dZ = z;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                                 wallList                                 //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *                            constructor                           *
 ********************************************************************/
wallList::wallList(): dntList()
{
}

/********************************************************************
 *                             Destructor                           *
 ********************************************************************/
wallList::~wallList()
{
   clearList();
}

/********************************************************************
 *                            freeElement                           *
 ********************************************************************/
void wallList::freeElement(dntListElement* obj)
{
   wall* w = (wall*)obj;
   delete(w);
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                               mapTexture                                 //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *                            constructor                           *
 ********************************************************************/
mapTexture::mapTexture()
{
   alphaValues = NULL;
   definedAlpha = false;
   mapX = -1;
}

/********************************************************************
 *                             destructor                           *
 ********************************************************************/
mapTexture::~mapTexture()
{
   int i;

   if(mapX != -1)
   {
      /* Delete the OpenGL Texture */
      glDeleteTextures(1,&index);

      /* Delete the alpha Texture */
      if(definedAlpha)
      {
         glDeleteTextures(1,&(alphaTexture));
      }

      /* Delete the Alpha Matrix */
      for(i = 0; i < (mapX*ALPHA_TEXTURE_INC); i++)
      {
         delete[] (alphaValues[i]);
      }
      delete[] alphaValues;
   }
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                             mapTextureList                               //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *                            constructor                           *
 ********************************************************************/
mapTextureList::mapTextureList()
{
}

/********************************************************************
 *                             destructor                           *
 ********************************************************************/
mapTextureList::~mapTextureList()
{
   clearList();
}

/********************************************************************
 *                            freeElement                           *
 ********************************************************************/
void mapTextureList::freeElement(dntListElement* obj)
{
   mapTexture* tex = (mapTexture*)obj;
   delete(tex);
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                                  MAP                                     //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *                          Map Constructor                         *
 ********************************************************************/
Map::Map()
{
   miniMap = NULL;
   fileName = "oxi!";
   name = "mapName";
   squareInic = NULL;
   MapSquares = NULL;
   doors = NULL;
   totalDoors = 0;
   music = "";
   npcFileName = "";
   particlesFileName = "";
   soundsFileName = "";
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
   x = z = 0;
   xInic = zInic = 0;

   /* Create sound */
   sounds = new mapSound();

   /* Create texture renderer for walls */
   wallRenderer = new texRenderer();
}

/********************************************************************
 *                         Map Destructor                           *
 ********************************************************************/
Map::~Map()
{
   int i;

   /* Deleting all related objects */
   deleteObjects();

   /* Delete All Textures */
   textures.clearList();
   
   /* Delete all Walls */
   walls.clearList();

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

   /* Delete the Wall texture renderer */
   delete(wallRenderer);

   /* Deleting all squares */
   int x1;
   if(MapSquares != NULL)
   {
      for(x1 = 0; x1<x;x1++)
      {
         delete[](MapSquares[x1]);
      }
      delete[] (MapSquares);
   }

   /* Deleting Sounds */
   if(sounds)
   {
      delete(sounds);
   }

   /* Deleting Lakes */
   lake* laux;
   for(i = 0; i < totalLakes; i++)
   {
      laux = lakes;
      lakes = lakes->next;
      delete(laux);
   }

   /* Free MiniMap Surface, if needed */
   if(miniMap)
   {
      SDL_FreeSurface(miniMap);
   }

   /* Deleting Roads */
   /*if(roads)
   {
      delete(roads);
   }*/
}

/********************************************************************
 *                             alloc                                *
 ********************************************************************/
void Map::alloc()
{
   int i;

   /* Alloc MapSquares */
   MapSquares = new Square*[x];
   for(i = 0; i < x; i++)
   {
      MapSquares[i] = new Square[z];
   } 

   /* Alloc Vertex Buffer (4 x,y,z per square, so 12 floats per square) */
   vertexBuffer = new float[x*z*12];

   /* Alloc UV Buffers (4 u,v per square, so 8 floats per square) */
   uvBuffer = new float[x*z*8];
   uvAlphaBuffer = new float[x*z*8];

   /* Alloc Roads Struct */
   //roads = new mapRoad(x, z);

   /* Add Upper Wall Texture */
   insertTexture("texturas/wall/upper_wall.png", 
                 "UpperWall",235,147,89);
}

/********************************************************************
 *                            getTextureID                          *
 ********************************************************************/
int Map::getTextureID(string textureName, GLuint& R, GLuint& G, GLuint& B)
{
   /* search for texture */
   int aux=0;
   mapTexture* tex = (mapTexture*)textures.getFirst();

   while(aux < textures.getTotal())
   {
      if(!(tex->name.compare(textureName)) )
      {
         /* Found! */
         R = tex->R; G = tex->G; B = tex->B;
         return(tex->index); 
      }
      tex = (mapTexture*)tex->getNext();
      aux++;
   }
   return(-1);
}

/*********************************************************************
 *                         getTextureName                            *
 *********************************************************************/
string Map::getTextureName(GLuint ID)
{
   mapTexture* tex = getTexture(ID);
   if(tex)
   {
      return(tex->name);
   }
   return("");
}

/*********************************************************************
 *                           getTexture                              *
 *********************************************************************/
mapTexture* Map::getTexture(GLuint id)
{
   int aux=0;
   mapTexture* tex = (mapTexture*)textures.getFirst();
   while(aux < textures.getTotal())
   {
      if(tex->index == id)
      {
         return(tex);
      }
      tex = (mapTexture*)tex->getNext();
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
   options opt;
   mapTexture* tex;
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

   /* Show Warning if not power of 2 */
   if( (img->h != smallestPowerOfTwo(img->h)) ||
       (img->w != smallestPowerOfTwo(img->w)) )
   {
      cerr << "Warning: image '" << arq 
           << "' is of non-power of two dimension '" 
           << img->w << "x" << img->h << "'" << endl;
   }

   /* Create the Texture Structs */ 
   tex = new(mapTexture);

   /* Insert it at the list */
   textures.insert(tex);
 
   /* Define it */
   tex->fileName = arq.c_str();
   tex->name = name.c_str();

   tex->R = R;
   tex->G = G;
   tex->B = B;
   tex->mapX = getSizeX();

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

   /* Enable Anisotropic filtering, if defined */
   if(opt.getEnableAnisotropicFilter())
   {
      GLfloat maxAni=1.0;
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAni);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAni);
   }

   /* Define other texture parameters */
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   
   gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, img->w,
                     img->h, GL_RGB, GL_UNSIGNED_BYTE, 
                     img->pixels );

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
   mapTexture* tex = (mapTexture*)textures.getFirst();
   mapTexture* rmTex = NULL;
   bool used = false;
   int total = textures.getTotal();
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

      /* Verify Upper Wall Texture */
      GLuint R=0,G=0,B=0;
      used |= (getTexture(getTextureID("UpperWall", R,G,B)) == tex);

      /* Verify use of texture at Walls  */
      w = (wall*)walls.getFirst();
      for(x1 = 0; ((x1 < walls.getTotal()) && (!used)); x1++)
      {
         used |= (getTexture(w->frontTexture.getTextureId()) == tex);
         used |= (getTexture(w->backTexture.getTextureId()) == tex);
         used |= (getTexture(w->rightTexture.getTextureId()) == tex);
         used |= (getTexture(w->leftTexture.getTextureId()) == tex);
         w = (wall*)w->getNext();
      }

      if(!used)
      {
         /* Set pointers */
         rmTex = tex;
         tex = (mapTexture*)tex->getNext();
         
         cout << "Will remove texture: " << rmTex->name << endl;
         /* Remove it from the linked list */
         textures.remove(rmTex);
      }
      else
      {
         tex = (mapTexture*)tex->getNext();
      }
   }
}

/********************************************************************
 *                          deleteObjects                           *
 ********************************************************************/
void Map::deleteObjects()
{
   int Xaux = 0, Zaux = 0, o = 0;
   objSquare* sobj;
   objSquare* next;

   /* First, remove all sceneries */
   objectsList::removeStaticSceneries();

   /* Now, all doors Objects */
   door* dor = doors;
   for(o = 0; o < totalDoors; o++)
   {
      delete(dor->obj);
      dor = dor->next;
   }

   /* and finally, search all squares for non sceneries objects */
   for(Xaux = 0; Xaux < x; Xaux++)
   {
     for(Zaux = 0; Zaux < z; Zaux++)
     {
        /* all objects at the square */
        sobj = MapSquares[Xaux][Zaux].getFirstObject();
        for(o=0; o < MapSquares[Xaux][Zaux].getTotalObjects(); o++)
        {
           next = (objSquare*)sobj->getNext();
           if(sobj != NULL) 
           {
              /* If will draw here and isn't scenery */
              if( (sobj->draw) && (!sobj->obj->isStaticScenery()) ) 
              {
                 /* delete it! */
                 switch(sobj->obj->getType())
                 {
                    deleteObject(sobj->obj);
                 }
              }
           }
           sobj = next;
        }
     }
   }

}

/********************************************************************
 *                          removeObject                            *
 ********************************************************************/
void Map::removeObject(object* obj)
{
   int Xaux = 0, Zaux = 0, o = 0;
   objSquare* sobj;
   objSquare* next;

   /* Search on all squares for the object */
   for(Xaux = 0; Xaux < x; Xaux++)
   {
     for(Zaux = 0; Zaux < z; Zaux++)
     {
        /* Search on all objects at the square */
        sobj = MapSquares[Xaux][Zaux].getFirstObject();
        for(o=0; o < MapSquares[Xaux][Zaux].getTotalObjects(); o++)
        {
           next = (objSquare*)sobj->getNext();
           if(sobj != NULL) 
           {
              if(sobj->obj == obj)
              {
                 /* Remove it from square */
                 MapSquares[Xaux][Zaux].removeObject(sobj);
              }
           }
           sobj = next;
        }
     }
   }

}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void Map::insertObject(GLfloat xReal, GLfloat yReal, GLfloat zReal, 
                       GLfloat orObj, object* obj, bool collision)
{
   int qx = (int)xReal / squareSize();
   int qz = (int)zReal / squareSize();
   insertObject(xReal, yReal, zReal, orObj, obj, qx, qz, collision);
}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void Map::insertObject(GLfloat xReal, GLfloat yReal, GLfloat zReal, 
                       GLfloat orObj, object* obj, int qx, int qz, 
                       bool collision)
{
   /* Define the object Position */
   obj->xPosition = xReal;
   obj->yPosition = yReal;
   obj->zPosition = zReal;
   obj->orientation = orObj;

   /* Get the main square where object is */
   Square* saux = relativeSquare(qx,qz);
   int ob=0;
   if(saux)
   {
      /* Add Object to the square */
      saux->addObject(true, xReal, yReal, zReal, orObj, collision, obj);
      boundingBox  bounds = obj->getBoundingBox();

      /* Now will search all other squares the object can be */
      float X[4], Z[4];
      GLfloat min2[3];
      GLfloat max2[3];

      X[0] = bounds.x1;
      Z[0] = bounds.z1;
      X[1] = bounds.x1;
      Z[1] = bounds.z2;
      X[2] = bounds.x2;
      Z[2] = bounds.z2;
      X[3] = bounds.x2;
      Z[3] = bounds.z1;
      rotTransBoundingBox(orObj, X, Z, xReal, bounds.y1+yReal, 
            bounds.y2+yReal, zReal, min2, max2);


      int minqx, minqz, maxqx, maxqz;
      int ssize = squareSize();
      minqx = (int)(min2[0]) / ssize;
      minqz = (int)(min2[2]) / ssize;
      maxqx = (int)(max2[0]) / ssize;
      maxqz = (int)(max2[2]) / ssize; 
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
               qaux->addObject(false,xReal, yReal, zReal, orObj,collision,obj);
            }
         }
      }

      /* If is a scenery one, the render is controlled by model3d, so
       * add a render position to it! */
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
   /* Create and define the wall */
   wall* maux = new(wall);
   maux->x1 = x1;
   maux->x2 = x2;
   maux->z1 = z1;
   maux->z2 = z2;

   /* Add it to the list */
   walls.insert(maux);
   
   return(maux);
}

/********************************************************************
 *                          getFirstWall                            *
 ********************************************************************/
wall* Map::getFirstWall()
{
   return((wall*)walls.getFirst());
}

/********************************************************************
 *                          getTotalWalls                           *
 ********************************************************************/
int Map::getTotalWalls()
{
   return(walls.getTotal());
}

/********************************************************************
 *                           removeWall                             *
 ********************************************************************/
void Map::removeWall(wall* w)
{
   if(w)
   {
      walls.remove(w);
   }
}

/********************************************************************
 *                          getFirstDoor                            *
 ********************************************************************/
door* Map::getFirstDoor()
{
   return(doors);
}

/********************************************************************
 *                          getTotalDoors                           *
 ********************************************************************/
int Map::getTotalDoors()
{
   return(totalDoors);
}

/********************************************************************
 *                            insertDoor                            *
 ********************************************************************/
void Map::insertDoor(door* newDoor)
{
   newDoor->next = doors;
   doors = newDoor;
   newDoor->status = DOOR_STATUS_CLOSED;
   newDoor->delta = 0;
   totalDoors++;
}

/********************************************************************
 *                            getFog                                *
 ********************************************************************/
mapFog Map::getFog()
{
   return(fog);
}

/********************************************************************
 *                           renderQuad                             *
 ********************************************************************/
void renderQuad(GLfloat x1, GLfloat z1,
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
 *                      renderFloorIndoor                           *
 ********************************************************************/
void Map::renderInvisibleSurface()
{
   glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
   glBegin(GL_QUADS);
 
   if(outdoor)
   {
      glVertex3f(-OUTDOOR_FARVIEW_D4, -5.0f, -OUTDOOR_FARVIEW_D4);
      glVertex3f(-OUTDOOR_FARVIEW_D4, -5.0f, 
            z*OUTDOOR_SQUARE_SIZE+OUTDOOR_FARVIEW_D4);
      glVertex3f(x*OUTDOOR_SQUARE_SIZE+OUTDOOR_FARVIEW_D4, -5.0f, 
            z*OUTDOOR_SQUARE_SIZE+OUTDOOR_FARVIEW_D4);
      glVertex3f(x*OUTDOOR_SQUARE_SIZE+OUTDOOR_FARVIEW_D4, -5.0f, 
            -OUTDOOR_FARVIEW_D4);
   }
   else
   {
      glVertex3f(-INDOOR_FARVIEW_D4, -5.0f, -INDOOR_FARVIEW_D4);
      glVertex3f(-INDOOR_FARVIEW_D4, -5.0f, 
            z*INDOOR_SQUARE_SIZE+INDOOR_FARVIEW_D4);
      glVertex3f(x*INDOOR_SQUARE_SIZE+INDOOR_FARVIEW_D4, -5.0f, 
            z*INDOOR_SQUARE_SIZE+INDOOR_FARVIEW_D4);
      glVertex3f(x*INDOOR_SQUARE_SIZE+INDOOR_FARVIEW_D4, -5.0f, 
            -INDOOR_FARVIEW_D4);
   }
   glEnd();
   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

/********************************************************************
 *                      renderFloorIndoor                           *
 ********************************************************************/
void Map::renderFloorIndoor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                            GLfloat** matriz, bool selectionRender,
                            bool outdoorCompatible)
{
   int aux = 0;
   int x1, z1;
   mapTexture* tex;

   if(selectionRender)
   {
      /* At Selection Mode, no need for textures */
      aux = textures.getTotal() - 1;
      glColor4f(1.0,1.0,1.0,1.0);
   }
   else
   {
      renderInvisibleSurface();
      aux = 0;

      if(!outdoorCompatible)
      {
         glColor4f(1.0, 1.0, 1.0, 0.9);
      }
      else
      {
         glColor4f(1.0, 1.0, 1.0, 1.0);
      }

      /* For Reflexions */
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_TEXTURE_2D);
   }

   tex = (mapTexture*)textures.getFirst();
   while(aux < textures.getTotal())
   {
      /* Bind the texture */

      if(!selectionRender)
      {
         glBindTexture(GL_TEXTURE_2D, tex->index);
         glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
               GL_LINEAR_MIPMAP_LINEAR );
         glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      }

      /* Draw all vertex with this texture */
      glBegin(GL_QUADS);

      for(x1=0; x1 < x; x1++)
      {
         for(z1=0; z1 < z; z1++)
         {
            if( ( (MapSquares[x1][z1].texture == (int)tex->index) ||
                  (selectionRender) )                             &&
                ( (matriz == NULL) ||
                  (visibleCube(MapSquares[x1][z1].x1 - VIS_DELTA,0,
                             MapSquares[x1][z1].z1 - VIS_DELTA,
                             MapSquares[x1][z1].x2 + VIS_DELTA,
                             MAX_HEIGHT,
                             MapSquares[x1][z1].z2 + VIS_DELTA, matriz)) ))
            {
               renderQuad(MapSquares[x1][z1].x1, MapSquares[x1][z1].z1,
                          MapSquares[x1][z1].x2, MapSquares[x1][z1].z2,
                          MapSquares[x1][z1].h1, MapSquares[x1][z1].h2, 
                          MapSquares[x1][z1].h3, MapSquares[x1][z1].h4,
                          0.0, 0.0, 1.0, 1.0);
            }
         }
      }

      glEnd();

      tex = (mapTexture*)tex->getNext();
      aux++;
   }

   if(!selectionRender)
   {
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_BLEND);
   }
}

/********************************************************************
 *                     renderFloorOutdoor                           *
 ********************************************************************/
void Map::renderFloorOutdoor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                             GLfloat** matriz, bool selectionRender)
{
   extensions ext;
   int aux = 0;
   mapTexture* tex;

   /* Create the buffers with visible squares */
   createBuffers(matriz);

   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, vertexBuffer);

   if(selectionRender)
   {
      /* At selection mode, only need to draw it one time, 
       * without textures */
      glDrawArrays(GL_QUADS, 0, (int)totalVertex / (int)3);
      glDisableClientState(GL_VERTEX_ARRAY);
      return;
  }

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
   if(ext.hasMultiTexture())
   { 
      ext.arbClientActiveTexture(GL_TEXTURE0_ARB);
      glTexCoordPointer(2, GL_FLOAT, 0, uvAlphaBuffer);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      ext.arbClientActiveTexture(GL_TEXTURE1_ARB);
      glTexCoordPointer(2, GL_FLOAT, 0, uvBuffer);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   }

   tex = (mapTexture*)textures.getFirst();
   if(ext.hasMultiTexture())
   {
      /* Define Blend Status */
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      glDepthMask(GL_TRUE);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      
      while(aux < textures.getTotal())
      {
         /* Only Draw texture with floor count > 0 */
         if(tex->count > 0)
         {
            /* Bind the Alpha Texture */
            ext.arbActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex->alphaTexture);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE0);
            glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);
            glEnable(GL_TEXTURE_2D);

            /* Bind the Texture */ 
            ext.arbActiveTexture(GL_TEXTURE1);
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
         
         }
         tex = (mapTexture*)tex->getNext();
         aux++;
      }

      /* Disable Textures */
      ext.arbActiveTexture(GL_TEXTURE1_ARB);
      glDisable(GL_TEXTURE_2D);
      ext.arbActiveTexture(GL_TEXTURE0_ARB);
      glDisable(GL_TEXTURE_2D);
      //glBindTexture(GL_TEXTURE_2D, tex->alphaTexture);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

      /* Reset blend status */
      glDisable(GL_BLEND);
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
      glDepthMask(GL_TRUE);
   }

   /* So disable the multitexture state. */
   if(ext.hasMultiTexture())
   { 
      ext.arbClientActiveTexture(GL_TEXTURE1_ARB);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      ext.arbClientActiveTexture(GL_TEXTURE0_ARB);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   }

   glDisableClientState(GL_VERTEX_ARRAY);
}

/********************************************************************
 *                           renderFloor                            *
 ********************************************************************/
void Map::renderFloor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                      GLfloat** matriz, bool selectionRender)
{
   /* Draw Terrain */
   options opt;
   if( (outdoor) && (opt.getEnableMultiTexture()) )
   {
      renderFloorOutdoor(cameraX, cameraY, cameraZ, matriz, selectionRender);
   }
   else
   {
      renderFloorIndoor(cameraX, cameraY, cameraZ, matriz, selectionRender, 
                        outdoor);
   }


   if(!selectionRender)
   {
      /* Render Lakes */
      lake* l = lakes;
      int i;
      for(i = 0; i < totalLakes; i++)
      {
         l->draw();
         l = l->next;
      }
   }

   return;
}

/********************************************************************
 *                             render                               *
 ********************************************************************/
int Map::render(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                GLfloat** matriz, GLfloat perX, GLfloat perZ)
{
   /* Update Lights */
   lights.update(perX, perZ);

   glColor3f(1.0,1.0,1.0);
   /* Render Walls */
   renderWalls(cameraX, cameraY, cameraZ, matriz, false);

   glEnable(GL_COLOR_MATERIAL);
   /* Render Floor */
   glColor4f(1.0, 1.0, 1.0, 1.0);
   renderFloor( cameraX, cameraY, cameraZ, matriz );

   /* Render Roads */
   /*if(roads)
   {
      roads->draw();
   }*/

   /* Render objects */
   renderObjects(cameraX, cameraY, cameraZ, matriz, false);

   glDisable(GL_COLOR_MATERIAL);
   glColor3f(1.0,1.0,1.0);

   return(0);
}

/********************************************************************
 *                            renderWalls                           *
 ********************************************************************/
void Map::renderWalls(GLfloat cameraX, GLfloat cameraY, 
                      GLfloat cameraZ, GLfloat** matriz,
                      bool inverted)
{
   glColor3f(1.0,1.0,1.0);
   wall* maux = (wall*)walls.getFirst();
   bool visible = false;
   int wNum;
   GLfloat altura = WALL_HEIGHT;
   GLfloat u,v;
   GLuint R=0,G=0,B=0;
   GLuint dX=0, dY=0, dZ=0;

   /* Clear the wall renderer buffers */
   wallRenderer->clear();

   /* Render All Walls and Curbs */
   for(wNum=0; wNum < walls.getTotal(); wNum++)
   {
      if(inverted)
      {
         if(matriz)
         {
            visible = visibleCube(maux->x1-VIS_DELTA,
                                  -altura-VIS_DELTA,maux->z1-VIS_DELTA,
                                  maux->x2+VIS_DELTA,
                                  VIS_DELTA,maux->z2+VIS_DELTA,matriz);
         }
         else
         {
            visible = true;
         }
      }
      else
      {
         if(matriz)
         {
            visible = visibleCube(maux->x1-VIS_DELTA,-VIS_DELTA,
                                  maux->z1-VIS_DELTA,maux->x2+VIS_DELTA,
                                  altura+VIS_DELTA,maux->z2+VIS_DELTA,matriz);
         }
         else
         {
            visible = true;
         }
      }
      if(visible)
      {
         /* Inserting each quad at the renderer */         

         /* Front Face */
         maux->frontTexture.getDelta(dX, dY, dZ);
         u = (maux->x2-maux->x1) / dX;
         v = (altura+1) / dY;
         wallRenderer->addQuad(maux->frontTexture.getTextureId(),
                               maux->frontTexture.getTextureName(),
                               maux->x1, 0, maux->z1, 
                               maux->x1, altura, maux->z1,
                               maux->x2, altura, maux->z1,
                               maux->x2, 0, maux->z1,
                               0, 0, u, v,
                               0, 0, 1);
         
         /* Back Face */
         maux->backTexture.getDelta(dX, dY, dZ);
         u = (maux->x2-maux->x1) / dX;
         v = (altura+1) / dY;
         wallRenderer->addQuad(maux->backTexture.getTextureId(),
                               maux->backTexture.getTextureName(),
                               maux->x1, 0, maux->z2, 
                               maux->x1, altura, maux->z2,
                               maux->x2, altura, maux->z2,
                               maux->x2, 0, maux->z2,
                               0, 0, u, v,
                               0, 0, -1);
                                
         /* Left Face */
         maux->leftTexture.getDelta(dX, dY, dZ);
         u = (maux->z2-maux->z1) / dX;
         v = (altura+1) / dY;
         wallRenderer->addQuad(maux->leftTexture.getTextureId(),
                               maux->leftTexture.getTextureName(),
                               maux->x1, 0, maux->z1,
                               maux->x1, altura, maux->z1,
                               maux->x1, altura, maux->z2,
                               maux->x1, 0, maux->z2,
                               0, 0, u, v,
                               -1, 0, 0);

         /* Right Face */
         maux->rightTexture.getDelta(dX, dY, dZ);
         u = (maux->z2-maux->z1) / dX;
         v = (altura+1) / dY;
         wallRenderer->addQuad(maux->rightTexture.getTextureId(),
                               maux->rightTexture.getTextureName(),
                               maux->x2, 0, maux->z1,
                               maux->x2, altura, maux->z1,
                               maux->x2, altura, maux->z2,
                               maux->x2, 0, maux->z2,
                               0, 0, u, v,
                               1, 0, 0);

         /* Upper Side */
         u = (maux->x2-maux->x1) / 16.0;
         v = (maux->z2-maux->z1) / 16.0;
         wallRenderer->addQuad(getTextureID("UpperWall", R, G, B),
                               "UpperWall",
                               maux->x1, altura, maux->z1, 
                               maux->x1, altura, maux->z2,
                               maux->x2, altura, maux->z2,
                               maux->x2, altura, maux->z1,
                               0, 0, u, v,
                               0, 1, 0);
                               
      }
      maux = (wall*)maux->getNext();
   }

   /* Now, finally render! */
   options option;
   wallRenderer->render((option.getReflexionType() >= REFLEXIONS_ALL) &&
                        (!isOutdoor()));

}

/********************************************************************
 *                           renderObjects                          *
 ********************************************************************/
void Map::renderObjects(GLfloat cameraX, GLfloat cameraY, 
                      GLfloat cameraZ, GLfloat** matriz,
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
              (!obj->obj->isStaticScenery()) &&
              ( (!inverted) || ((inverted) && (obj->colision)) ) )
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
               rotTransBoundingBox(obj->obj->orientation, X, Z, 
                                   obj->obj->xPosition, 
                                   obj->obj->yPosition - bound.y2, 
                                   -obj->obj->yPosition - bound.y1, 
                                   obj->obj->zPosition, min, max);
            }
            else
            {
               rotTransBoundingBox(obj->obj->orientation, X, Z, 
                                   obj->obj->xPosition, 
                                   obj->obj->yPosition + bound.y1, 
                                   obj->obj->yPosition + bound.y2, 
                                   obj->obj->zPosition, min, max );
            }

            /* Verify ViewFrustum Culling */
            if( (matriz == NULL) ||
                (visibleCube(min[0],min[1],min[2],max[0],max[1],max[2],
                             matriz)) )
            {
               obj->obj->draw(inverted);
            }
         }
         obj = (objSquare*)obj->getNext();
      }
      MapSquares[Xaux][Zaux].visible = 0;
   }

   /* Draw Doors */
   door* dor = doors;
   while(dor != NULL)
   {
      if(dor->obj != NULL)
      {
         /* Remove previous delta */
         dor->obj->orientation -= dor->delta;

         /* Do the "animation" */
         if( (dor->status == DOOR_STATUS_OPENED) && (dor->delta < 90))
         {
            dor->delta += 5;
         }
         else if( (dor->status == DOOR_STATUS_CLOSED) && (dor->delta > 0) )
         {
            dor->delta -= 5;
         }

         /* Apply the new delta */
         dor->obj->orientation += dor->delta; 

         /* Draw it */
         dor->obj->draw(inverted);
      }
      dor = dor->next;
   }
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
 *                           relativeSquare                         *
 ********************************************************************/
Square* Map::relativeSquare(int xa, int za)
{
   if( (z <= za) || (x <= xa) || ( xa < 0) || (za < 0)) 
      return(NULL);

   return(&MapSquares[xa][za]);
}

/********************************************************************
 *                           getInitialSquare                       *
 ********************************************************************/
Square* Map::getInitialSquare()
{
   return(squareInic);
}

/********************************************************************
 *                             squareSize                           *
 ********************************************************************/
int Map::squareSize()
{
   if(outdoor)
   {
      return(OUTDOOR_SQUARE_SIZE);
   }
   return(INDOOR_SQUARE_SIZE);
}

/********************************************************************
 *                         getSquareMiniSize                        *
 ********************************************************************/
float Map::getSquareMiniSize()
{
   return(squareMiniSize);
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
 *                       renderSurfaceOnMap                         *
 ********************************************************************/
void Map::renderSurfaceOnMap(GLuint image, GLfloat xa, GLfloat za, 
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
   return(fileName);
}

/********************************************************************
 *                          setFileName                             *
 ********************************************************************/
void Map::setFileName(string fName)
{
   fileName = fName;
}

/********************************************************************
 *                            getName                               *
 ********************************************************************/
string Map::getName()
{
   return(name);
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
void Map::setNpcFileName(string fName)
{
   npcFileName = fName;
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
void Map::setMusicFileName(string fName)
{
   music = fName;
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
void Map::setParticlesFileName(string fName)
{
   particlesFileName = fName;
}

/********************************************************************
 *                      getSoundsFileName                           *
 ********************************************************************/
string Map::getSoundsFileName()
{
   return(soundsFileName);
}

/********************************************************************
 *                      setSoundsFileName                           *
 ********************************************************************/
void Map::setSoundsFileName(string fName)
{
   soundsFileName = fName;
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
void Map::getInitialPosition(float& iX, float& iZ, float& iAngle)
{
   iX = xInic;
   iZ = zInic;
   iAngle = angleInic;
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
 *                       Open Map File                              *
 ********************************************************************/
int Map::open(string arquivo)
{
   defParser def;
   string key="", value="";

   char nomeArq[128], nome[128];
   string arqVelho;
   int posX,posZ;    //actual position of last active square
   int IDtextureAtual = -1;
   int IDwallTexturaAtual = -1;
   string nameMuroTexturaAtual = "nada";
   int pisavel=0;
   GLuint R,G,B;
   GLuint Ratual,Gatual,Batual;
  
   /* Load the map file defintions */ 
   if(!def.load(arquivo))
   {
      cerr << "Error while opening map file: " << arquivo << endl;
      return(0);
   }

   /* Define map file Name */
   arqVelho = fileName;
   fileName = arquivo;

   xInic = -1;

   /* Read size of the map */
   if( (!def.getNextTuple(key, value)) || (key != "size") )
   {
      cerr << "Map Size not defined: " << arquivo << endl;
      cerr << " Found " << key << " when especting size" << endl;
      return(0);
   }
   sscanf(value.c_str(),"%dX%d",&x,&z);

   /* Alloc all Map Structures */
   alloc();

   wall* maux = NULL;
   door* doorAux = NULL;

   posX = -1;
   posZ = 0;

   /* Read All File */
   while(def.getNextTuple(key, value))
   {
      /* Define Light File */
      if(key == "lightsFile")
      {
         lights.load(value);
      }
      /* Add Lake to Map */
      else if(key == "lake")
      {
         float xa=0, za=0, xb=0, zb=0;
         float r=0,g=0,b=0,a=0;
         sscanf(value.c_str(),"%f,%f,%f,%f:%f,%f,%f,%f", 
               &xa, &za, &xb, &zb,
               &r, &g, &b, &a);
         lake* l = addLake(xa,za,xb,zb);
         l->defineColor(r,g,b,a);
      }
      /* Define Fog File */
      else if(key == "fogFile")
      {
         fog.load(value);
      }
      /* Define Particle System File */
      else if(key == "particlesFile")
      {
         particlesFileName = value;
      }
      /* Define npcs file */
      else if(key == "npcFile")
      {
         npcFileName = value;
      }
      /* Map's Name */
      else if(key == "name")
      {
         name = translateDataString(value);
      }
      /* Door */
      else if(key == "door")
      {
         /* Create and insert the door */
         doorAux = new(door);
         insertDoor(doorAux);

         /* Read info */
         sscanf(value.c_str(),"%s %f,%f:%d",nome,&doorAux->x,&doorAux->z,
               &doorAux->orientation);
         doorAux->delta = 0;
         doorAux->obj = objectsList::search(nome, doorAux->x, 0, doorAux->z);
         if(doorAux->obj == NULL)
         {
            /* Not found on list, so insert it! */
            doorAux->obj = createObject(nome, fileName);
            doorAux->obj->xPosition = doorAux->x;
            doorAux->obj->yPosition = 0;
            doorAux->obj->zPosition = doorAux->z;
            doorAux->obj->orientation = doorAux->orientation;
         }
      }
      /* Music File */
      else if(key == "musicFile")
      {
         music = value;
      }
      /* Sounds File */
      else if(key == "soundsFile")
      {
         soundsFileName = value;
         if(!sounds->load(soundsFileName))
         {
            cerr << "Error loading sounds fileName: " 
               << soundsFileName << endl;
         }
      }
      /* Define Walls */
      else if(key == "wall")
      {
         maux = addWall(0,0,0,0);
         sscanf(value.c_str(),"%f,%f,%f,%f",&maux->x1,&maux->z1,
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
      }
      /* Define Current Wall Texture */
      else if( (key == "wtf") || (key == "wtb") || 
               (key == "wtl") || (key == "wtr") )
      {
         char type = key[2];
         GLuint dX=0, dY=0, dZ=0;

         /* Get the texture Name */
         sscanf(value.c_str(),"%d,%d,%d %s", &dX, &dY, &dZ, nome);
         nameMuroTexturaAtual = nome;
         IDwallTexturaAtual = getTextureID(nome,R,G,B);

         /* Get Wich wall side the texture is */
         switch(type)
         {
            case 'f':
               maux->frontTexture.setTextureId(IDwallTexturaAtual);
               maux->frontTexture.setTextureName(nameMuroTexturaAtual);
               maux->frontTexture.setDelta(dX,dY,dZ);
               break;
            case 'b':
               maux->backTexture.setTextureId(IDwallTexturaAtual);
               maux->backTexture.setTextureName(nameMuroTexturaAtual);
               maux->backTexture.setDelta(dX,dY,dZ);
               break;
            case 'l':
               maux->leftTexture.setTextureId(IDwallTexturaAtual);
               maux->leftTexture.setTextureName(nameMuroTexturaAtual);
               maux->leftTexture.setDelta(dX,dY,dZ);
               break;
            case 'r':
               maux->rightTexture.setTextureId(IDwallTexturaAtual);
               maux->rightTexture.setTextureName(nameMuroTexturaAtual);
               maux->rightTexture.setDelta(dX,dY,dZ);
               break;
         }
      }
      /* Define initial Character Position */
      else if(key == "initial")
      {
         if(xInic == -1)
         {
            sscanf(value.c_str(), "%f,%f",&xInic,&zInic);
            angleInic = 0;
            int posX =(int)floor( xInic / (squareSize()));
            int posZ =(int)floor( zInic / (squareSize()));
            squareInic = relativeSquare(posX,posZ);
         }
      }
      /* Outdoor type */
      else if(key == "outdoor")
      {
         int a;
         sscanf(value.c_str(), "%d", &a);
         outdoor = a;
      }
      /* Texture Definition */
      else if(key == "texture")
      {
         sscanf(value.c_str(), "%s %s %d %d %d",nome,nomeArq,&R,&G,&B);  
         insertTexture(nomeArq,nome,R,G,B);
      }
      /* Square declaration */
      else if(key == "square")
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
         sscanf(value.c_str(), "%d,%f,%f,%f,%f",&pisavel,
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
      }
      /* Declare connection */
      else if(key == "connection")
      {
         sscanf(value.c_str(),"%f,%f,%f,%f:%f:%s",
               &MapSquares[posX][posZ].mapConection.x1,
               &MapSquares[posX][posZ].mapConection.z1,
               &MapSquares[posX][posZ].mapConection.x2,
               &MapSquares[posX][posZ].mapConection.z2,
               &MapSquares[posX][posZ].mapConection.angle,
               nome );
         MapSquares[posX][posZ].mapConection.mapName = nome;
         MapSquares[posX][posZ].mapConection.active = true;

         /* If going from a previous file, the initial position
          * may be the connection itself */
         if(arqVelho == (MapSquares[posX][posZ].mapConection.mapName))
         {
            squareInic = &MapSquares[posX][posZ];
            xInic = (MapSquares[posX][posZ].mapConection.x1 + 
                  MapSquares[posX][posZ].mapConection.x2) / 2.0;
            zInic = (MapSquares[posX][posZ].mapConection.z1 + 
                  MapSquares[posX][posZ].mapConection.z2) / 2.0;
            angleInic = MapSquares[posX][posZ].mapConection.angle;
         }

      }
      /* Square's Texture */
      else if(key == "useTexture")
      {
         IDtextureAtual = getTextureID(value,Ratual,Gatual,Batual);
         MapSquares[posX][posZ].texture = IDtextureAtual;
         MapSquares[posX][posZ].R = Ratual;
         MapSquares[posX][posZ].G = Gatual;
         MapSquares[posX][posZ].B = Batual;
      }
      /* Define Object at the square */
      else if(key == "useObject")
      {
         int des, quadX, quadZ, oPis;
         float oX, oY, oZ, oOri;
         objSquare* oObj;
         sscanf(value.c_str(),"%s %d:%d,%d:%f,%f,%f:%f:%d",nome,
               &des, &quadX, &quadZ, &oX, &oY, &oZ, &oOri, &oPis);

         object* obj = objectsList::search(nome, oX, oY, oZ);
         if(obj == NULL)
         {
            /* Insert it */
            obj = createObject(nome, fileName);
            /* set the object position */
            obj->xPosition = oX;
            obj->yPosition = oY;
            obj->zPosition = oZ;
            obj->orientation = oOri;
         }

         oObj = MapSquares[posX][posZ].addObject(des==1, oX, oY, oZ,
               oOri, oPis!=1,
               obj);
         if(oObj->draw)
         {
            if(oObj->obj->isStaticScenery())
            {
               oObj->obj->addRenderPosition(oX, oY, oZ, oOri);
            }
         }

      }

      /* ERROR: Unknow Key! */
      else
      {
         cerr << "What is: " << key << " on " << arquivo << endl;
      }
   }

   int ax,az;
   
   /* Now, update pointers to the walls */
   maux = (wall*)walls.getFirst();
   int wNum;
   int inix,iniz,maxx,maxz;
   int indexMuro;
   Square* aux;
   float ssize = squareSize();
   
   for(wNum = 0; wNum < walls.getTotal(); wNum++)
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
      maux = (wall*)maux->getNext();
   }

   /* Define minimap sizes */
   squareMiniSize = 20;
   squareMiniDiv = (squareSize() / squareMiniSize);

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
   Square* saux;
   x = X;
   z = Z;

   /* Alloc all buffers */
   alloc();

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
   squareMiniSize = 20.0f;
   squareMiniDiv = (squareSize() / squareMiniSize);

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
    wall* maux = (wall*)walls.getFirst();
    int wNum, wNum2;
    for(wNum = 0; wNum < walls.getTotal(); wNum++)
    {
        wall* maux2 = (wall*)walls.getFirst();
        for(wNum2 = 0; wNum2 < walls.getTotal(); wNum2++)
        {
            if(maux != maux2)
            {
                if( (maux->x1 == maux2->x1) && 
                    ( ((maux->z1 >= maux2->z1) && (maux->z2 <= maux2->z2)) ||
                      ((maux->z1 <= maux2->z1) && (maux->z2 >= maux2->z2)) ))
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
                    ( ((maux->z1 >= maux2->z1) && (maux->z2 <= maux2->z2)) ||
                      ((maux->z1 <= maux2->z1) && (maux->z2 >= maux2->z2)) ))
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
                    ( ((maux->x1 >= maux2->x1) && (maux->x2 <= maux2->x2)) ||
                      ((maux->x1 <= maux2->x1) && (maux->x2 >= maux2->x2)) ))
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
                    ( ((maux->x1 >= maux2->x1) && (maux->x2 <= maux2->x2)) ||
                      ((maux->x1 <= maux2->x1) && (maux->x2 >= maux2->x2)) ))
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
            maux2 = (wall*)maux2->getNext();
        }
        maux = (wall*)maux->getNext();
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
   fprintf(arq,"# Made by DccNiTghtmare's MapEditor, %s\n", VERSION);
   fprintf(arq,"size = %dX%d\n",x,z);

   /* Write Map's name */
   fprintf(arq,"name = gettext(\"%s\")\n", name.c_str());

   /* Write fog file name, if exists */
   if( !fog.fileName.empty() )
   {
      fprintf(arq,"fogFile = %s\n",dir.getRelativeFile(fog.fileName).c_str());
   }
  
   /* Write NPC file name, if exists */
   if( !npcFileName.empty())
   {
      fprintf(arq,"npcFile = %s\n",dir.getRelativeFile(npcFileName).c_str());
   }

   /* Write Particles file */
   if(!particlesFileName.empty())
   {
      fprintf(arq,"particlesFile = %s\n",
                   dir.getRelativeFile(particlesFileName).c_str());
   }

 
   /* Write music file name */
   if(!music.empty())
   {
      fprintf(arq,"musicFile = %s\n",dir.getRelativeFile(music).c_str());
   }

   /* Write sounds file name */
   if(!soundsFileName.empty())
   {
      fprintf(arq,"soundsFile = %s\n",
              dir.getRelativeFile(soundsFileName).c_str());
   }

   /* Write Lights file name */
   if(!lights.getFileName().empty())
   {
      fprintf(arq,"lightsFile = %s\n",
              dir.getRelativeFile(lights.getFileName()).c_str());
   }

   /* Write if is outdoor or not */
   fprintf(arq,"outdoor = %d\n", outdoor);

   /* Write lakes */
   int i;
   float xa=0,za=0,xb=0,zb=0;
   float r=0,g=0,b=0,a=0;

   lake* l = lakes;
   for(i = 0; i < totalLakes; i++)
   {
      l->getPosition(xa,za,xb,zb);
      l->getColor(r,g,b,a);
      fprintf(arq,"lake = %.3f,%.3f,%.3f,%.3f:%.3f,%.3f,%.3f,%.3f\n",
                        xa,  za,  xb,  zb,  r,   g,   b,   a);
      l = l->next;
   }

   /* Write used Textures */
   mapTexture* tex = (mapTexture*)textures.getFirst();
   int t;
   for(t = 0; t < textures.getTotal(); t++)
   {
      /* Don't save UpperWall texture, because it will
       * always loaded at alloc() function */
      if(tex->name != "UpperWall")
      {
         fprintf(arq,"texture = %s %s %d %d %d\n",tex->name.c_str(),
                 dir.getRelativeFile(tex->fileName).c_str(),
                 tex->R,tex->G,tex->B);
      }
      tex = (mapTexture*)tex->getNext();
   }

   /* Write Doors */
   door* doorAux = (door*)doors;
   while(doorAux != NULL)
   {
      fprintf(arq,"door = %s %.3f,%.3f:%d\n",
              dir.getRelativeFile(doorAux->obj->getFileName()).c_str(),
              doorAux->x,doorAux->z, doorAux->orientation);
      doorAux = doorAux->next;
   }
   
   /* Write Walls */
   wall* maux = (wall*)walls.getFirst();
   int x1,z1,x2,z2,wNum;
   for(wNum=0; wNum < walls.getTotal(); wNum++)
   {
      GLuint dX=0, dY=0, dZ=0;
      fprintf(arq,"wall = %.3f,%.3f,%.3f,%.3f\n",
              maux->x1,maux->z1,maux->x2,maux->z2);
      maux->frontTexture.getDelta(dX,dY,dZ);
      fprintf(arq,"wtf = %d,%d,%d %s\n", dX, dY, dZ, 
                  getTextureName(maux->frontTexture.getTextureId()).c_str());
      maux->backTexture.getDelta(dX,dY,dZ);
      fprintf(arq,"wtb = %d,%d,%d %s\n", dX,dY,dZ,
                  getTextureName(maux->backTexture.getTextureId()).c_str());
      maux->rightTexture.getDelta(dX,dY,dZ);
      fprintf(arq,"wtr = %d,%d,%d %s\n", dX,dY,dZ,
                  getTextureName(maux->rightTexture.getTextureId()).c_str());
      maux->leftTexture.getDelta(dX,dY,dZ);
      fprintf(arq,"wtl = %d,%d,%d %s\n", dX,dY,dZ,
                  getTextureName(maux->leftTexture.getTextureId()).c_str());
      maux = (wall*)maux->getNext();
   }

   /* Write Squares, line per line */
   for(z1=0;z1<z;z1++)
   {
      fprintf(arq,"#Z: %d\n",z1);
      for(x1=0;x1<x;x1++)
      {
          /* Square Definition */
          fprintf(arq,"square = %d,%f,%f,%f,%f\n",
                  MapSquares[x1][z1].flags,
                  MapSquares[x1][z1].h1,
                  MapSquares[x1][z1].h2,
                  MapSquares[x1][z1].h3,
                  MapSquares[x1][z1].h4);
          
          /* Square Texture */
          fprintf(arq,"useTexture = %s\n",
                  getTextureName(MapSquares[x1][z1].texture).c_str());
          
          /* Connection */
          if( MapSquares[x1][z1].mapConection.active )
          {
              fprintf(arq,"connection = %f,%f,%f,%f:%f:%s\n",
                      MapSquares[x1][z1].mapConection.x1,
                      MapSquares[x1][z1].mapConection.z1,
                      MapSquares[x1][z1].mapConection.x2,
                      MapSquares[x1][z1].mapConection.z2,
                      MapSquares[x1][z1].mapConection.angle,
                      dir.getRelativeFile(
                              MapSquares[x1][z1].mapConection.mapName).c_str());
          }

          /* Objects at Square */
          int aux;
          objSquare* obj = MapSquares[x1][z1].getFirstObject();
          for(aux=0; aux < MapSquares[x1][z1].getTotalObjects(); aux++)
          {
            if(obj->obj)
            {
               x2 = (int)obj->obj->xPosition / squareSize();
               z2 = (int)obj->obj->zPosition / squareSize();
               fprintf(arq,"useObject = %s %d:%d,%d:%.3f,%.3f,%.3f:%.3f:%d\n",
                       dir.getRelativeFile(obj->obj->getFileName()).c_str(),
                       obj->draw, x2 + 1, z2 + 1,
                       obj->x, obj->y, 
                       obj->z, obj->orientation, 
                       !obj->colision);
            }
            obj = (objSquare*)obj->getNext();
          }
      }
   }

   /* Write Initial Character Position */
   fprintf(arq,"initial = %.3f,%.3f\n",xInic,zInic);

   fclose(arq);
   return(1);
}

/********************************************************************
 *                               getMiniMap                         *
 ********************************************************************/
SDL_Surface* Map::getMiniMap()
{
   return(miniMap);
}

/********************************************************************
 *                       Draw MiniMap on Surface                    *
 ********************************************************************/
void Map::drawMiniMap()
{
   modelList models;

   int mapSizeX;
   int mapSizeZ;
   GLfloat ratio;
   GLfloat height;

   /* Define Size to render */
   mapSizeX = (int)squareMiniSize*(x);
   mapSizeZ = (int)squareMiniSize*(z);
   
   /* Define the scale ratio */
   if(outdoor)
   {
      
      ratio = 0.009f;
      if(x > z)
      {
         height = (x-7)*3;
      }
      else
      {
         height = (z-7)*3;
      }
   }
   else
   {
      ratio = 0.036f;
      if(x > z)
      {
         height = x*3;
      }
      else
      {
         height = z*3;
      }
   }

   /* Setting the View  */
   glViewport(0,0, mapSizeX, mapSizeZ);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0, mapSizeX / (float)mapSizeZ, 1.0, OUTDOOR_FARVIEW);
   glMatrixMode(GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glLoadIdentity();

   /* Set the look up, at the map's center looking down at Y axys */
   GLfloat posX = ratio*x*squareSize()/2.0; 
   GLfloat posZ = ratio*z*squareSize()/2.0;
   gluLookAt(posX, height, posZ, posX, 0.0, posZ, 0, 0, -1);

   /* Put some light */
   GLfloat color[4] = {0.8,0.8,0.8,1.0};
   GLfloat where[4] = {(OUTDOOR_FARVIEW / 2.0)-1, 
                       (OUTDOOR_FARVIEW / 2.0)-1 + posX,
                       posZ, 1.0};
   glLightfv(GL_LIGHT0, GL_AMBIENT, color);
   glLightfv(GL_LIGHT0, GL_POSITION, where);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHTING);

   glPushMatrix();
      glScalef(ratio,ratio,ratio);
      models.renderSceneryObjects(NULL, false);
      render(posX, 529, posZ, NULL, 0, 0);
   glPopMatrix();

   printOpenGLErrors();

   /* Define Machine Bit Order */
   Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
   rmask = 0xff000000;
   gmask = 0x00ff0000;
   bmask = 0x0000ff00;
   amask = 0x000000ff;
#else
   rmask = 0x000000ff;
   gmask = 0x0000ff00;
   bmask = 0x00ff0000;
   amask = 0xff000000;
#endif

   /* Create the minimap Surface, if needed) */
   if(miniMap == NULL)
   {
      miniMap = SDL_CreateRGBSurface(SDL_SWSURFACE, 
                                     mapSizeX, mapSizeZ, 32,
                                     rmask, gmask, bmask, amask);
   }

   /* Read the pixels to the surface */
   glReadBuffer(GL_BACK);
   glReadPixels(0, 0, mapSizeX, mapSizeZ, 
                GL_RGBA, GL_UNSIGNED_BYTE, miniMap->pixels);

   /* Define Surface Params to Blit Later */
   SDL_SetAlpha(miniMap, 0,0);
   SDL_SetColorKey(miniMap, SDL_SRCCOLORKEY, 
                   SDL_MapRGB(miniMap->format, 0, 0, 0));
     
   /* Reset the View */
   glDisable(GL_LIGHT0);
   glDisable(GL_LIGHTING);
   glViewport(0,0,SCREEN_X, SCREEN_Y);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, SCREEN_X / (float)SCREEN_Y, 1.0, OUTDOOR_FARVIEW);
   glMatrixMode (GL_MODELVIEW);

}

/********************************************************************
 *                           createBuffers                          *
 ********************************************************************/
void Map::createBuffers(GLfloat** matriz)
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
         /* Only add to the buffer if the square is potentially visible */
         if( (matriz == NULL) ||
             (visibleCube(MapSquares[x1][z1].x1 - VIS_DELTA,0,
                        MapSquares[x1][z1].z1 - VIS_DELTA,
                        MapSquares[x1][z1].x2 + VIS_DELTA,
                        MAX_HEIGHT,
                        MapSquares[x1][z1].z2 + VIS_DELTA, matriz)))
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
            uvAlphaBuffer[actualTexture+3] = (alphaCoordZ + ALPHA_TEXTURE_INC)
                                             / modZ;
            vertexBuffer[totalVertex+3] = MapSquares[x1][z1].x1;
            vertexBuffer[totalVertex+4] = MapSquares[x1][z1].h2;
            vertexBuffer[totalVertex+5] = MapSquares[x1][z1].z2;

            uvBuffer[actualTexture+4] = TEXTURE_REPEATS;
            uvBuffer[actualTexture+5] = TEXTURE_REPEATS;
            uvAlphaBuffer[actualTexture+4] = (alphaCoordX + ALPHA_TEXTURE_INC)
                                             / modX; 
            uvAlphaBuffer[actualTexture+5] = (alphaCoordZ + ALPHA_TEXTURE_INC)
                                             / modZ;
            vertexBuffer[totalVertex+6] = MapSquares[x1][z1].x2;
            vertexBuffer[totalVertex+7] = MapSquares[x1][z1].h3;
            vertexBuffer[totalVertex+8] = MapSquares[x1][z1].z2;

            uvBuffer[actualTexture+6] = TEXTURE_REPEATS;
            uvBuffer[actualTexture+7] = 0.0;
            uvAlphaBuffer[actualTexture+6] = (alphaCoordX + ALPHA_TEXTURE_INC)
                                             / modX; 
            uvAlphaBuffer[actualTexture+7] = alphaCoordZ / modZ;
            vertexBuffer[totalVertex+9] = MapSquares[x1][z1].x2;
            vertexBuffer[totalVertex+10] = MapSquares[x1][z1].h4;
            vertexBuffer[totalVertex+11] = MapSquares[x1][z1].z1;

            totalVertex += 12;
            actualTexture += 8;
         }
         alphaCoordX += ALPHA_TEXTURE_INC;
     }
     alphaCoordZ += ALPHA_TEXTURE_INC;
  }
}

/********************************************************************
 *                              createSplats                        *
 ********************************************************************/
void Map::createSplats()
{
   int x1, z1;
   
   /* Create the alpha channel, for each square */
   for(z1=0;z1<z;z1++)
   {
      for(x1=0;x1<x;x1++)
      {
         /* Create The Alphas */
         createAlpha(x1,z1);
     }
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
   mapTexture* tex;

   for(z2 = z1*ALPHA_TEXTURE_INC; z2 < (z1+1)*ALPHA_TEXTURE_INC; z2++)
   {
      actualCoordX = x1;
      for(x2 = x1*ALPHA_TEXTURE_INC; x2 <(x1+1)*ALPHA_TEXTURE_INC; x2++)
      {
         total = 0;

         /* Clear Alpha Values */
         aux = 0;
         tex = (mapTexture*)textures.getFirst();
         while(aux < textures.getTotal())
         {
            tex->alphaValues[x2][z2] = 0;
            tex = (mapTexture*)tex->getNext();
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
            tex = (mapTexture*)textures.getFirst();
            while(aux < textures.getTotal())
            {
               tex->alphaValues[x2][z2] = tex->alphaValues[x2][z2] / total;
               tex = (mapTexture*)tex->getNext();
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
   mapTexture* tex = (mapTexture*)textures.getFirst();
   int x1, z1;
   SDL_Surface* img = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                     smallestPowerOfTwo(x*ALPHA_TEXTURE_INC),
                                     smallestPowerOfTwo(z*ALPHA_TEXTURE_INC),
                                     32,0x000000FF,0x0000FF00,
                                     0x00FF0000,0xFF000000);
   while(aux < textures.getTotal())
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

      tex = (mapTexture*)tex->getNext();
      aux++;
   }
   SDL_FreeSurface(img);
}

/********************************************************************
 *                        defineCommonTexture                       *
 ********************************************************************/
void Map::defineCommonTexture()
{
   mapTexture* actualCommon = (mapTexture*)textures.getFirst();
   /* Zero the Count of each texture */
   int x1,z1,aux = 0;
   mapTexture* tex = (mapTexture*)textures.getFirst();
   while(aux < textures.getTotal())
   {
      tex->count = 0;
      tex = (mapTexture*)tex->getNext();
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

   if( (res - altura_atual) > 0.35)
   {
       c->yPosition = altura_atual;
       return(false);
   }

   c->yPosition = res;
   return(true);
}

/********************************************************************
 *                             flushSounds                          *
 ********************************************************************/
void Map::flushSounds()
{
   if(sounds)
   {
      sounds->flush();
   }
}

