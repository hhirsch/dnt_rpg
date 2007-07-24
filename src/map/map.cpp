/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include "map.h"
#include "../engine/culling.h"
#include "../etc/glm.h"
#include "../engine/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

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
   for(aux=0;aux<MAXMUROS;aux++)
   {
     muros[aux] = NULL;
   }
   objList = NULL;
   totalObjects = 0;
   return;
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
                             int orientation, float x, float z, 
                             bool colision, object* obj)
{
   objSquare* n = new objSquare;
   n->draw = draw;
   n->status = 0;
   n->squareX = squareX;
   n->squareZ = squareZ;
   n->orientation = orientation;
   n->x = x;
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
   divisions = (int) (( fabs(h4-h1) + fabs(h4-h2) + fabs(h4-h1) +
                        fabs(h3-h1) + fabs(h3-h2) + fabs(h2-h1)) /
                       SQUARE_DIVISIONS_INC) + 1;
}

//////////////////////////////////////////////////////////////////////////////
//                                  OTHERS                                  //
//////////////////////////////////////////////////////////////////////////////

/********************************************************************
 *                             Texture ID                           *
 ********************************************************************/
int IDTextura(Map* mapa, string textura, GLuint* R, GLuint* G, GLuint* B)
{
   /* procura pela textura */
   int aux=0;
   texture* tex = mapa->Texturas;
   while(aux < mapa->numtexturas)
   {
      if(!(tex->nome.compare(textura)) )
      {
         *R = tex->R; *G = tex->G; *B = tex->B;
         return(tex->indice); //a textura ja esta presente 
      }
      tex = tex->proximo;
      aux++;
   }
   return(-1);
}

/*********************************************************************
 *                   Returns texture's name                          *
 *********************************************************************/
string NomeTextura(Map* mapa, GLuint ID)
{
   int aux=0;
   texture* tex = mapa->Texturas;
   while(aux < mapa->numtexturas)
   {
      if(tex->indice == ID)
      {
         return(tex->nome);
      }
      tex = tex->proximo;
      aux++;
   }
   return(NULL);
}

/********************************************************************
 *                         Insert texture                           *
 ********************************************************************/
GLuint InserirTextura(Map* mapa, string arq, string nome, 
                    GLuint R, GLuint G, GLuint B)
{
   texture* tex;

   SDL_Surface* img = IMG_Load(arq.c_str());
   if(!img)
   {
      printf("Error Opening Texture: %s\n",arq.c_str());
      return(0);
   }

   /* Insere realmente a textura */ 
   tex = new(texture);
   if(mapa->numtexturas == 0)
   {
      mapa->Texturas = tex;
      tex->proximo = NULL;
   }
   else
   {
      tex->proximo = mapa->Texturas;
      mapa->Texturas = tex;
   }

   tex->arqNome = arq.c_str();
   tex->nome = nome.c_str();

   tex->R = R;
   tex->G = G;
   tex->B = B;

   glGenTextures(1, &(tex->indice));
   glBindTexture(GL_TEXTURE_2D, tex->indice);
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

   mapa->numtexturas++;

   /* Libera a memoria utilizada */
   SDL_FreeSurface(img);

   return(tex->indice);
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
   objSquare* obj = MapSquares[Xaux][Zaux].getFirstObject();
   objSquare* next;
   for(Xaux = 0; Xaux < x; Xaux++)
   {
     for(Zaux = 0; Zaux < z; Zaux++)
     {
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
void Map::insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                        object* obj, bool collision)
{
   int qx = (int)xReal / SQUARE_SIZE;
   int qz = (int)zReal / SQUARE_SIZE;
   insertObject(xReal, zReal, orObj, obj, qx, qz, collision);
}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void Map::insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                       object* obj, int qx, int qz, bool collision)
{
   Square* saux = relativeSquare(qx,qz);
   int ob=0;
   if(saux)
   {
      saux->addObject(true, qx, qz, orObj, xReal, zReal, collision, obj);
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
     minqx = (int)(X[0] + xReal) / SQUARE_SIZE;
     minqz = (int)(Z[0] + zReal) / SQUARE_SIZE;
     maxqx = (int)(X[1] + xReal) / SQUARE_SIZE;
     maxqz = (int)(Z[1] + zReal) / SQUARE_SIZE; 
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
              qaux->addObject(false,qx,qz,orObj,xReal,zReal,collision,obj);
           }
        }
     }
     /* Mark the object as used */
     obj->incUsedFlag();
   }
   else
   {
     printf("Warn: Try to insert object out of Map's Limits!\n");
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
 *                            DrawFloor                             *
 ********************************************************************/
int Map::drawFloor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
              GLfloat matriz[6][4])
{
   int textura = -1;
   int Xaux = 0, Zaux = 0;

   int k,l;
   GLfloat texX, texZ;
   GLfloat pX1, pX2, pZ1, pZ2, incPos, incTex;

   GLint wrap = GL_REPEAT;

   if(isOutdoor())
   {
      wrap = GL_REPEAT;
      incTex = (TEXTURE_REPEATS / MapSquares[Xaux][Zaux].divisions);
   }
   else
   {
      wrap = GL_REPEAT;
      incTex = (TEXTURE_REPEATS / MapSquares[Xaux][Zaux].divisions);
   }

   /* For each square (a square is squarizeble!) */
   textura = MapSquares[Xaux][Zaux].textura;
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, textura);
   
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR );
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

   glBegin(GL_QUADS);
     /* Draw at horizon */
     if(outdoor)
     {
        glNormal3f(0,1,0);
        glTexCoord2d(0,0);
        glVertex3f(-FARVIEW,-1,-FARVIEW);
        glTexCoord2d(0,1);
        glVertex3f(-FARVIEW,-1,(z*SQUARE_SIZE)+FARVIEW);
        glTexCoord2d(1,1);
        glVertex3f((x*SQUARE_SIZE)+FARVIEW,-1,(z*SQUARE_SIZE)+FARVIEW);
        glTexCoord2d(1,0);
        glVertex3f((x*SQUARE_SIZE)+FARVIEW,-1,-FARVIEW);
        glColor3f(1.0,1.0,1.0);
     }

   for(Xaux = 0; Xaux < x; Xaux++)
   {
      for(Zaux = 0; Zaux < z; Zaux++)
      {
         incPos = (GLfloat) SQUARE_SIZE / 
                  (GLfloat)MapSquares[Xaux][Zaux].divisions;
         if((textura!= -1) && (MapSquares[Xaux][Zaux].textura == -1))
         {
             glDisable(GL_TEXTURE_2D); 
             textura = -1;
         }
         else if(textura != MapSquares[Xaux][Zaux].textura)
         {
            glEnd();
            textura = MapSquares[Xaux][Zaux].textura;
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, textura);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );
            glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR );
            /*glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);*/
            glBegin(GL_QUADS);
         }
         if( (MapSquares[Xaux][Zaux].visivel) || 
             (quadradoVisivel(MapSquares[Xaux][Zaux].x1,0,
                              MapSquares[Xaux][Zaux].z1,
                              MapSquares[Xaux][Zaux].x2,
                              ALTURAMAXIMA,
                              MapSquares[Xaux][Zaux].z2, matriz)))
         {
            MapSquares[Xaux][Zaux].visivel = 1;

            /* Draw the Square as some squares */ 
            for(k = 0; k < MapSquares[Xaux][Zaux].divisions; k++)
            {
               texX = k*incTex;
               pX1 = (k*incPos) + MapSquares[Xaux][Zaux].x1;
               pX2 = (incPos) + pX1;
               for(l = 0; l < MapSquares[Xaux][Zaux].divisions; l++)
               {
                  texZ = l*incTex;
                  pZ1 = (l*incPos) + MapSquares[Xaux][Zaux].z1;
                  pZ2 = (incPos) + pZ1;
                  drawQuad(pX1, pZ1, pX2, pZ2,
                           getHeight(pX1, pZ1, &MapSquares[Xaux][Zaux]),
                           getHeight(pX1, pZ2, &MapSquares[Xaux][Zaux]),
                           getHeight(pX2, pZ2, &MapSquares[Xaux][Zaux]),
                           getHeight(pX2, pZ1, &MapSquares[Xaux][Zaux]),
                           texX, texZ, texX + incTex, texZ + incTex);
               }
            }

            /* //Old Draw, as only one square
             * drawQuad(MapSquares[Xaux][Zaux].x1, MapSquares[Xaux][Zaux].z1,
                     MapSquares[Xaux][Zaux].x2, MapSquares[Xaux][Zaux].z2,
                     MapSquares[Xaux][Zaux].h1, MapSquares[Xaux][Zaux].h2,
                     MapSquares[Xaux][Zaux].h3, MapSquares[Xaux][Zaux].h4,
                     0.0, 0.0, 1.0, 1.0);*/

         }
         else
         {
            MapSquares[Xaux][Zaux].visivel = 0;
         }
      }
   }
   glEnd();
   return(1);
}

/********************************************************************
 *                           Draw  Map                              *
 ********************************************************************/
int Map::draw(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
              GLfloat matriz[6][4], GLfloat perX, GLfloat perZ)
{
        int textura = -1;
        int Xaux = 0, Zaux = 0;

        /* Actualize Lights */
        lights.actualize(perX, perZ);

        glEnable(GL_COLOR_MATERIAL);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0, 1.0, 1.0, 0.9);
        drawFloor( cameraX, cameraY, cameraZ, matriz );
        glDisable(GL_BLEND);

        textura = MapSquares[Xaux][Zaux].textura;
           glEnable(GL_TEXTURE_2D);
           glBindTexture(GL_TEXTURE_2D, textura);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
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
   muro* maux = muros;
   int textura = -1;
   bool visible = false;
        int fezMeioFio = 0;
        GLfloat altura = MUROALTURA;
        if(!maux)
        {
            maux = meiosFio;
            fezMeioFio = 1;
            altura = MEIOFIOALTURA;
        }
        glBegin(GL_QUADS);
        while( (maux != NULL) )
        {
           
           if((textura!= -1) && (maux->textura == -1))
           {
               glDisable(GL_TEXTURE_2D); 
               textura = -1;
           }
           else if(textura != maux->textura)
           {
              glEnd();
              textura = maux->textura;
              glEnable(GL_TEXTURE_2D);
              glBindTexture(GL_TEXTURE_2D, textura);
              glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
              glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

              glBegin(GL_QUADS);
           }
           if(inverted)
           {
              visible = quadradoVisivel(maux->x1,-altura,maux->z1,maux->x2,
                                        0,maux->z2,matriz);
           }
           else
           {
              visible = quadradoVisivel(maux->x1,0,maux->z1,maux->x2,
                                        altura,maux->z2,matriz);
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
              glTexCoord2f(Y,0);
              glVertex3f(maux->x1,altura,maux->z1);
              glTexCoord2f(Y,Z);
              glVertex3f(maux->x1,altura,maux->z2);
              glTexCoord2f(0,Z);
              glVertex3f(maux->x1,0,maux->z2);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,0,maux->z1);
           /* Right Face */
              glNormal3i(1,0,0);
              glTexCoord2f(Y,0);
              glVertex3f(maux->x2,altura,maux->z1);
              glTexCoord2f(Y,Z);
              glVertex3f(maux->x2,altura,maux->z2);
              glTexCoord2f(0,Z);
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
           maux = maux->proximo;
           if( (!maux) && (!fezMeioFio) )
           {
               maux = meiosFio;
               fezMeioFio = 1;
               altura = MEIOFIOALTURA;
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

   for(Xaux = 0; Xaux < x; Xaux++)
   for(Zaux = 0; Zaux < z; Zaux++)
   {
      deltaX = (cameraX-MapSquares[Xaux][Zaux].x1+HALF_SQUARE_SIZE);
      deltaZ = (cameraZ-MapSquares[Xaux][Zaux].z1+HALF_SQUARE_SIZE);
      distancia = sqrt(deltaX*deltaX+deltaY2+deltaZ*deltaZ) / SQUARE_SIZE;
      obj = MapSquares[Xaux][Zaux].getFirstObject();
      for(o=0; o < MapSquares[Xaux][Zaux].getTotalObjects(); o++)
      {
          if( (obj != NULL) && (obj->draw))
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
               rotTransBoundingBox(obj->orientation, X, Z, obj->x, -bound.y2, 
                                   -bound.y1, obj->z, min, max);
            }
            else
            {
               rotTransBoundingBox(obj->orientation, X, Z, obj->x, bound.y1, 
                                   bound.y2, obj->z, min, max );
            }

            /* Verify ViewFrustum Culling */
            if(quadradoVisivel(min[0],min[1],min[2],max[0],max[1],max[2],
                               matriz))
            {
               glPushMatrix();
                glTranslatef(0.0, getHeight(obj->x,obj->z) +
                             obj->obj->posicaoLadoY, 0.0);
                obj->obj->draw(obj->x, obj->z, distancia, obj->orientation, 
                               inverted);
               glPopMatrix();
            }
         }
         obj = obj->next;
      }
      MapSquares[Xaux][Zaux].visivel = 0;
   }

   /* Draw Doors */
   door* porta = portas;
   while(porta != NULL)
   {
      if(porta->obj != NULL)
      {
         porta->obj->draw(porta->x,porta->z,0,porta->orientacao, inverted);
      }
      porta = porta->proximo;
   }
}

/********************************************************************
 *                          Map Constructor                         *
 ********************************************************************/
Map::Map(lObject* lObjects)
{
   numtexturas = 0;
   Texturas = NULL;
   name = "oxi!";
   squareInic = NULL;
   muros = NULL;
   meiosFio = NULL; 
   MapSquares = NULL;
   portas = NULL;
   music = "";
   npcFileName = "";
   particlesFileName = "";
   outdoor = false;
   MapSquares = NULL;
   
   /* Initialize Structs */
   objects = lObjects;
   x = z = 0;
   xInic = zInic = 0;
   SQUAREMINISIZE = 4;
   SQUAREMINIDIV = (SQUARE_SIZE / SQUAREMINISIZE);
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
   int posX =(int)floor( nx / (SQUARE_SIZE));
   int posZ =(int)floor( nz / (SQUARE_SIZE)); 

   Square* saux = relativeSquare(posX, posZ);

   return(getHeight(nx, nz, saux));
}

/********************************************************************
 *                            getHeight                             *
 ********************************************************************/
GLfloat Map::getHeight(GLfloat nx, GLfloat nz, Square* saux)
{
   if(!saux)
   {
      /* No positon on map, so no height! */
      return(0.0);
   }

   /* Do Interpolation to define the Height on position */
   GLfloat dx1 = fabs(nx - saux->x1) / SQUARE_SIZE;
   GLfloat dz1 = fabs(nz - saux->z1) / SQUARE_SIZE;
   GLfloat dx2 = fabs(saux->x2 - nx) / SQUARE_SIZE;
   GLfloat dz2 = fabs(saux->z2 - nz) / SQUARE_SIZE;

   GLfloat ha = (dx2 * saux->h1) + (dx1 * saux->h4);
   GLfloat hb = (dx2 * saux->h2) + (dx1 * saux->h3);

   return((ha * dz2) + (hb * dz1));
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
           glVertex3f( pX1 , getHeight(pX1, pZ1)+sumY, pZ1 );
           glTexCoord2f(texX, texZ + incTex);
           glNormal3i(0,1,0);
           glVertex3f( pX1 , getHeight(pX1, pZ2)+sumY, pZ2);
           glTexCoord2f(texX + incTex, texZ + incTex);
           glNormal3i(0,1,0);
           glVertex3f( pX2, getHeight(pX2, pZ2)+sumY, pZ2 );
           glTexCoord2f(texX + incTex, texZ);
           glNormal3i(0,1,0);
           glVertex3f( pX2, getHeight(pX2, pZ1)+sumY, pZ1 );
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
   int IDtexturaAtual = -1;
   int IDmuroTexturaAtual = -1;
   string nomeMuroTexturaAtual = "nada";
   int pisavel=0;
   GLuint R,G,B;
   GLuint Ratual,Gatual,Batual;
  
   
   if(!(arq = fopen(arquivo.c_str(),"r")))
   {
      printf("Error while opening map file: %s\n",arquivo.c_str());
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

   /* Alloc Roads Struct */
   //roads = new mapRoad(x, z);

   
   muro* maux = NULL;
   door* porta = NULL;

   posX = -1;
   posZ = 0;

   /* Read All File */
   while(fscanf(arq, "%s", buffer) != EOF)
   {
      switch(buffer[0])
      {
         case 'l':/* Define Light File */
         {
            fgets(buffer, sizeof(buffer),arq);
            sscanf(buffer,"%s",nome);
            lights.Load(nome);
            break;
         }
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
            porta = new(door);
            fgets(buffer, sizeof(buffer),arq);
            sscanf(buffer,"%s %f,%f:%d",nome,&porta->x,&porta->z,
                                        &porta->orientacao);
            porta->obj = objects->getObject(nome);
            if(porta->obj == NULL)
            {
               printf("Can't Locate Door File: %s\n",nome);
            }
            else
            {
               porta->status = 0;
               porta->proximo = portas;
               portas = porta;
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
         case 'm': /* Define Walls and Half Walls */
         {
            switch(buffer[1])
            {
               case 'u': /* Define Wall */
               {
                  maux = new(muro);
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
                  maux->proximo = muros;
                  maux->textura = -1;
                  muros = maux;
                  break;
               }
               case 't': /* Define Wall texture */
               {
                  fgets(buffer, sizeof(buffer), arq);
                  sscanf(buffer,"%s",nome);
                     nomeMuroTexturaAtual = nome;
                     IDmuroTexturaAtual = IDTextura(this,nome,&R,&G,&B);
                  maux->textura = IDmuroTexturaAtual;
                  break;
               }
               case 'e': /* Define Half Wall */
               {
                  maux = new(muro);
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
                  maux->proximo = meiosFio;
                  maux->textura = -1;
                  meiosFio = maux;
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
               int posX =(int)floor( xInic / (SQUARE_SIZE));
               int posZ =(int)floor( zInic / (SQUARE_SIZE));
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
            InserirTextura(this,nomeArq, nome,R,G,B);
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
            MapSquares[posX][posZ].x1 = (posX) * SQUARE_SIZE;
            MapSquares[posX][posZ].x2 = MapSquares[posX][posZ].x1+SQUARE_SIZE;
            MapSquares[posX][posZ].z1 = (posZ) * SQUARE_SIZE;
            MapSquares[posX][posZ].z2 = MapSquares[posX][posZ].z1+SQUARE_SIZE; 
            MapSquares[posX][posZ].posX = posX;
            MapSquares[posX][posZ].posZ = posZ;
            if(pisavel) 
            {
               MapSquares[posX][posZ].flags = PISAVEL;
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
                  IDtexturaAtual = IDTextura(this,nome,&Ratual,&Gatual,&Batual);
                  MapSquares[posX][posZ].textura = IDtexturaAtual;
                  MapSquares[posX][posZ].R = Ratual;
                  MapSquares[posX][posZ].G = Gatual;
                  MapSquares[posX][posZ].B = Batual;
                  break;
               }
               case 'o': /* Insert Object on Square */
               {
                  int des, quadX, quadZ, oOri, oPis;
                  float oX, oZ;
                  objSquare* oObj;
                  fgets(buffer, sizeof(buffer), arq);
                  sscanf(buffer,"%s %d:%d,%d:%f,%f:%d:%d",nome,
                         &des, &quadX, &quadZ, &oX, &oZ, &oOri, &oPis);
                  oObj = MapSquares[posX][posZ].addObject(des==1, quadX,quadZ,
                                                          oOri,oX,oZ,oPis!=1,
                                                    objects->getObject(nome));
                  if(oObj->draw)
                  {
                     oObj->obj->incUsedFlag();
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
   maux = muros;
   int inix,iniz,maxx,maxz;
   int indiceMuro;
   Square* aux;
   
   while(maux)
   {
      inix = (int)floor(maux->x1 / SQUARE_SIZE);
      iniz = (int)floor(maux->z1 / SQUARE_SIZE);
      maxx = (int)floor(maux->x2 / SQUARE_SIZE);
      maxz = (int)floor(maux->z2 / SQUARE_SIZE);
      for(ax = inix;ax<=maxx;ax++)
      {
          for(az = iniz;az<=maxz;az++)
          {
              indiceMuro = 0;
              aux = relativeSquare(ax,az);
              while((aux!=NULL) && (indiceMuro < MAXMUROS) && 
                    (aux->muros[indiceMuro] != NULL))
              {
                 indiceMuro++;
              }
              if((aux!=NULL) && (indiceMuro < MAXMUROS))
              {
                 aux->muros[indiceMuro] = maux;
              }
              else if(indiceMuro >= MAXMUROS)
                 printf("Quad: %d %d has more walls than permitted: %d\n",ax,az,
                          indiceMuro);
          }
      }
      maux = maux->proximo;
   }

   return(1);
}


/************************************************************************
 *                     Create a new Map                                 *
 ************************************************************************/
void Map::newMap(int X, int Z)
{
   int auxX, auxZ;
   int i;

   printf("Beginning a new Map: %d,%d\n",X,Z);

   /* Alloc MapSquares */
   MapSquares = new Square*[X];//(Square***) malloc(X*sizeof(Square**));
   for(i = 0; i < X; i++)
   {
      MapSquares[i] = new Square[Z];//(Square**) malloc(Z*sizeof(Square*));
   } 

   /*for(auxX = 0; auxX < X; auxX++)
   {
      for(auxZ = 0; auxZ < Z; auxZ++)
      {
         MapSquares[auxX][auxZ] = new(Square);
      }
   }*/

   //roads = new mapRoad(X, Z);

   
   Square* saux;
   x = X;
   z = Z;
   int IDtextura = InserirTextura(this,
                                  "../data/texturas/chao_grama2.jpg", 
                                  "chao_grama2",54,102,49);

   for(auxZ = 0; auxZ < z; auxZ++)
   {
      for(auxX = 0; auxX < x; auxX++)
      {
          saux = relativeSquare(auxX,auxZ);
          saux->x1 = (auxX)*SQUARE_SIZE;
          saux->x2 = saux->x1+SQUARE_SIZE;
          saux->z1 = (auxZ)*SQUARE_SIZE;
          saux->z2 = saux->z1+SQUARE_SIZE; 
          saux->posX = auxX;
          saux->posZ = auxZ;
          saux->flags = PISAVEL;
          saux->textura = IDtextura;
          saux->R = 130;
          saux->G = 148;
          saux->B = 96;
      }
   }

   xInic = 1*SQUARE_SIZE;
   zInic = 1*SQUARE_SIZE;
   squareInic = relativeSquare(0,0);
}


/*******************************************************************
 *   Optimize map (wall sobreposition, object squares, etc)        *
 *******************************************************************/
void Map::optimize()
{
    /* Verify Wall Superposition */
    muro* maux = muros;
    while(maux != NULL)
    {
        muro* maux2 = muros;
        while(maux2 !=NULL)
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
            maux2 = maux2->proximo;
        }
        maux = maux->proximo;
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

   if(!(arq = fopen(arquivo.c_str(),"w")))
   {
      printf("Error while creating: %s to save\n",arquivo.c_str());
	return(0);
   }
   
   /* Write Dimensions */
   fprintf(arq,"T %dX%d\n",x,z);
   fprintf(arq,"# Made by DccNiTghtmare's MapEditor, v0.0.3\n");

   /* Write fog file name, if exists */
   if( !fog.fileName.empty())
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

   /* Write used objects */
   int i;
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
   texture* tex = (texture*)Texturas;
   while(tex)
   {
      fprintf(arq,"t %s %s %d %d %d\n",tex->nome.c_str(),tex->arqNome.c_str(),
                                       tex->R,tex->G,tex->B);
      tex = (texture*)tex->proximo;
   }

   /* Write Doors */
   door* porta = (door*)portas;
   while(porta != NULL)
   {
      fprintf(arq,"d %s %f,%f:%d\n",porta->obj->getFileName().c_str(),
                                    porta->x,porta->z,
                                    porta->orientacao);
      porta = porta->proximo;
   }
   
   /* Write Walls */
   muro* maux = (muro*)muros;
   int x1,z1,x2,z2;
   while(maux)
   {
      fprintf(arq,"muro %f,%f,%f,%f:%d,%d,%d\n",maux->x1,maux->z1,maux->x2,
                                                maux->z2,maux->dX,maux->dY,
                                                maux->dZ);
      fprintf(arq,"mt %s\n",NomeTextura(this, maux->textura).c_str());
      maux = (muro*)maux->proximo;
   }

   /* Write Half Walls */
   maux = (muro*)meiosFio;
   while(maux)
   {
      fprintf(arq,"meioFio %f,%f,%f,%f\n",maux->x1,maux->z1,maux->x2,maux->z2);
      fprintf(arq,"mt %s\n",NomeTextura(this, maux->textura).c_str());
      maux = (muro*)maux->proximo;
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
          fprintf(arq,"ut %s\n",NomeTextura(this, 
                      MapSquares[x1][z1].textura).c_str());
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
               x2 = (int)obj->x / SQUARE_SIZE;
               z2 = (int)obj->z / SQUARE_SIZE;
               fprintf(arq,"uo %s %d:%d,%d:%f,%f:%d:%d\n",
                       obj->obj->getFileName().c_str(),
                       obj->draw, x2 + 1, z2 + 1,
                       obj->x, obj->z, obj->orientation, !obj->colision);
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
   texture* tex = Texturas;
   texture* au;
   int i;
   for(i=0;i<numtexturas;i++)
   {
      au = tex;
      tex = tex->proximo;
      glDeleteTextures(1,&(au->indice));
      delete(au);
   }
   
   /* Delete all Walls */
   muro* m = muros;
   muro* am =NULL;
   while(m)
   {
      am = m;
      m = m->proximo;
      delete(am);
   }

   /* Deleting wall Doors */
   door* porta = portas;
   door* auxporta =NULL;
   while(porta)
   {
      auxporta = porta;
      porta = porta->proximo;
      delete(auxporta);
   }

   objects = NULL;
   /* Deleting all squares */
   int x1;
   for(x1 = 0; x1<x;x1++)
   {
      delete[](MapSquares[x1]);
   }
   delete[] (MapSquares);

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
                      MapSquares[X][Z].B);
          rectangle_Fill(img, x1,y1,x1+SQUAREMINISIZE-1,y1+SQUAREMINISIZE-1);
          x1+=SQUAREMINISIZE;
      }
      x1 = 0;
      y1+=SQUAREMINISIZE;
   }

   color_Set(1, 1, 1);
   rectangle_2Colors(img,0,0,sX*SQUAREMINISIZE-1,sZ*SQUAREMINISIZE-1,0,0,0);
   
   muro* maux = muros;
   while(maux!=NULL)
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
       color_Set(255,40,30);
       line_Draw(img, x1,y1,x2,y2);
       maux = maux->proximo;
   }

}

