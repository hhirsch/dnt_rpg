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



/********************************************************************
 *                      Square Constructor                          *
 ********************************************************************/
Square::Square()
{
   flags = 0;
   h1 = h2 = h3 = h4 = 0;
   mapConection.active = false;
   mapConection.mapName = "Nothing";
   int aux;
   for(aux=0;aux<MAXOBJETOS;aux++)
   {
      objects[aux] = NULL;
      objectsDesenha[aux] = 0;
      quadObjetos[aux] = NULL;
      statusObj[aux] = 0;
      pisavelObj[aux] = 0;
   }
   for(aux=0;aux<MAXMUROS;aux++)
     muros[aux] = NULL;
   return;
}

/********************************************************************
 *                       Square Destructor                          *
 ********************************************************************/
Square::~Square()
{
   return;
}

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
                   GL_NEAREST_MIPMAP_LINEAR );
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

/********************************************************************
 *                          removeObject                            *
 ********************************************************************/
void Map::removeObject(GLfloat xObj, GLfloat zObj, object* obj)
{
   int Xaux, Zaux, o;
   for(Xaux = 0; Xaux < x; Xaux++)
   {
     for(Zaux = 0; Zaux < z; Zaux++)
     {
        for(o=0;o<MAXOBJETOS;o++)
        {
           if( (MapSquares[Xaux][Zaux].objects[o] == obj) && 
               (MapSquares[Xaux][Zaux].Xobjects[o] == xObj) &&
               (MapSquares[Xaux][Zaux].Zobjects[o] == zObj) )
           {
              MapSquares[Xaux][Zaux].objects[o] = NULL;
           }
        }
     }
   }
}

/********************************************************************
 *                          removeObject                            *
 ********************************************************************/
void Map::removeObject(GLfloat xObj, GLfloat zObj, string fileName)
{
   int Xaux, Zaux, o;
   for(Xaux = 0; Xaux < x; Xaux++)
   {
     for(Zaux = 0; Zaux < z; Zaux++)
     {
        for(o=0;o<MAXOBJETOS;o++)
        {
           if( (MapSquares[Xaux][Zaux].objects[o] != NULL) && 
               (MapSquares[Xaux][Zaux].Xobjects[o] == xObj) &&
               (MapSquares[Xaux][Zaux].Zobjects[o] == zObj) )
           {
              if(MapSquares[Xaux][Zaux].objects[o]->getName() == fileName)
              {
                 MapSquares[Xaux][Zaux].objects[o] = NULL;
              }
           }
        }
     }
   }
}

/******************************************************************
 *                          insertObject                          *
 ******************************************************************/
void Map::insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                       mapObject* obj, int qx, int qz)
{
   Square* saux = relativeSquare(qx,qz);
   int ob=0;
   if(saux)
   {
     while( (ob < MAXOBJETOS ) && (saux->objects[ob] != NULL))
     {
        ob++;
     }
     if(ob<MAXOBJETOS)
     {
        saux->objects[ob] = obj;
        saux->Xobjects[ob] = xReal;
        saux->Zobjects[ob] = zReal;
        saux->objectsOrientation[ob] = orObj;
        saux->objectsDesenha[ob] = 1;
        //printf("%d° Object Inserted on %d %d\n",ob,qx+1,qz+1);
                  
        //GLMmodel* modelo = (GLMmodel*)obj->modelo3d; 
        //
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
       minqx = (int)(X[0] + xReal) / SQUARESIZE;
       minqz = (int)(Z[0] + zReal) / SQUARESIZE;
       maxqx = (int)(X[1] + xReal) / SQUARESIZE;
       maxqz = (int)(Z[1] + zReal) / SQUARESIZE; 
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
                while( (ob < MAXOBJETOS ) && 
                       (qaux->objects[ob] != NULL))
                {
                   ob++;
                }
                if(ob < MAXOBJETOS)
                {
                   qaux->objects[ob] = obj;
                   qaux->Xobjects[ob] = xReal;
                   qaux->Zobjects[ob] = zReal;
                   qaux->objectsDesenha[ob] = 0;
                   qaux->objectsOrientation[ob] = orObj;
                   //printf("%d° Object Inserted on %d %d\n",ob,X1+1,Z1+1);
                }
             }
          }
       }
     }
     else
       printf("Objects Overflow on Square %d %d\n",qx+1,qz+1);
   }
   else
     printf("Out of Map's Limits!\n");
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

   /* For each square (a square is squarizeble!) */
   int k,l;
   GLfloat texX, texZ;
   GLfloat pX1, pX2, pZ1, pZ2, incPos, incTex;
   incTex = (1.0 / SQUARE_DIVISIONS);
   incPos = (GLfloat) SQUARESIZE / (GLfloat)SQUARE_DIVISIONS;

   glColor3f(1.0,1.0,1.0);

   textura = MapSquares[Xaux][Zaux].textura;
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, textura);
   

   /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );*/
   
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                   GL_NEAREST_MIPMAP_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   glBegin(GL_QUADS);
     /* Draw at horizon */
     if(outdoor)
     {
        glNormal3f(0,1,0);
        glTexCoord2f(0.0,0.0);
        glVertex3f(-FARVIEW,-1,-FARVIEW);
        glTexCoord2f(0.0,1.0);
        glVertex3f(-FARVIEW,-1,+FARVIEW);
        glTexCoord2f(1.0,1.0);
        glVertex3f(+FARVIEW,-1,+FARVIEW);
        glTexCoord2f(1.0,0.0);
        glVertex3f(+FARVIEW,-1,-FARVIEW);
        glColor3f(1.0,1.0,1.0);
     }

   for(Xaux = 0; Xaux < x; Xaux++)
   {
      for(Zaux = 0; Zaux < z; Zaux++)
      {
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
            /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );*/
            glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                            GL_NEAREST_MIPMAP_LINEAR );
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
            /*for(i=0;i<MAXOBJETOS;i++)
            {
               if(MapSquares[Xaux][Zaux].quadObjetos[i] != NULL)
               { 
                  MapSquares[Xaux][Zaux].quadObjetos[i]->visivel = 1;
               }
            }*/
            MapSquares[Xaux][Zaux].visivel = 1;

            /* Draw the Square as some squares */ 
            for(k = 0; k < SQUARE_DIVISIONS; k++)
            {
               texX = k*incTex;
               pX1 = (k*incPos) + MapSquares[Xaux][Zaux].x1;
               pX2 = (incPos) + pX1;
               for(l = 0; l < SQUARE_DIVISIONS; l++)
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
              GLfloat matriz[6][4])
{
        int textura = -1;
        int Xaux = 0, Zaux = 0;

        /* Actualize Lights */
        lights.actualize();

        glEnable(GL_COLOR_MATERIAL);
        drawFloor( cameraX, cameraY, cameraZ, matriz );

        textura = MapSquares[Xaux][Zaux].textura;
           glEnable(GL_TEXTURE_2D);
           glBindTexture(GL_TEXTURE_2D, textura);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glColor3f(1.0,1.0,1.0);

        /* Draw Walls */
        muro* maux = muros;
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
           if(quadradoVisivel(maux->x1,0,maux->z1,maux->x2,
                              altura,maux->z2,matriz))
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
              glVertex3f(maux->x2,-1,maux->z1);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,-1,maux->z1);
           /* Back Face */
              glNormal3i(0,0,-1);
              glTexCoord2f(0,Y);
              glVertex3f(maux->x1,altura,maux->z2);
              glTexCoord2f(X,Y);
              glVertex3f(maux->x2,altura,maux->z2);
              glTexCoord2f(X,0);
              glVertex3f(maux->x2,-1,maux->z2);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,-1,maux->z2);
           /* Left Face */
              glNormal3i(-1,0,0);
              glTexCoord2f(Y,0);
              glVertex3f(maux->x1,altura,maux->z1);
              glTexCoord2f(Y,Z);
              glVertex3f(maux->x1,altura,maux->z2);
              glTexCoord2f(0,Z);
              glVertex3f(maux->x1,-1,maux->z2);
              glTexCoord2f(0,0);
              glVertex3f(maux->x1,-1,maux->z1);
           /* Right Face */
              glNormal3i(1,0,0);
              glTexCoord2f(Y,0);
              glVertex3f(maux->x2,altura,maux->z1);
              glTexCoord2f(Y,Z);
              glVertex3f(maux->x2,altura,maux->z2);
              glTexCoord2f(0,Z);
              glVertex3f(maux->x2,-1,maux->z2);
              glTexCoord2f(0,0);
              glVertex3f(maux->x2,-1,maux->z1);
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

        /* Draw Roads */
        /*if(roads)
        {
           roads->draw();
        }*/


        /* Draw Doors */
        door* porta = portas;
        while(porta != NULL)
        {
           if(porta->object != NULL)
           {
              porta->object->draw(porta->x,porta->z,0,porta->orientacao);
           }
           porta = porta->proximo;
        }

        /* Draw objects */
        int o;
        GLfloat distancia;
        GLfloat deltaX, deltaZ;
        GLfloat deltaY2 = cameraY*cameraY;
        GLfloat min[3], max[3];
        GLfloat X[4], Z[4];
        boundingBox bound;

        for(Xaux = 0; Xaux < x; Xaux++)
        for(Zaux = 0; Zaux < z; Zaux++)
        {
           deltaX = (cameraX-MapSquares[Xaux][Zaux].x1+HALFSQUARESIZE);
           deltaZ = (cameraZ-MapSquares[Xaux][Zaux].z1+HALFSQUARESIZE);
           distancia = sqrt(deltaX*deltaX+deltaY2+deltaZ*deltaZ) 
                                                          / SQUARESIZE;
           for(o=0;o<MAXOBJETOS;o++)
           {
              if( (MapSquares[Xaux][Zaux].objects[o] != NULL) && 
                  (MapSquares[Xaux][Zaux].objectsDesenha[o] == 1))
              {
                 /* Do the Rotation of the Bounding Box */
                 bound = 
                 MapSquares[Xaux][Zaux].objects[o]->getBoundingBox();
                  X[0] = bound.x1;
                  Z[0] = bound.z1;
                  X[1] = bound.x1;
                  Z[1] = bound.z2; 
                  X[2] = bound.x2;
                  Z[2] = bound.z2;
                  X[3] = bound.x2;
                  Z[3] = bound.z1;
                  rotTransBoundingBox(
                                  MapSquares[Xaux][Zaux].objectsOrientation[o],
                                  X, Z, MapSquares[Xaux][Zaux].Xobjects[o], 
                                  bound.y1, bound.y2,
                                  MapSquares[Xaux][Zaux].Zobjects[o],
                                  min, max );

                  /* Verify ViewFrustum Culling */
                  if(quadradoVisivel(min[0],min[1],min[2],max[0],max[1],max[2],
                                     matriz))
                  {
                     glPushMatrix();
                     glTranslatef(0.0, 
                            getHeight(MapSquares[Xaux][Zaux].Xobjects[o],
                                      MapSquares[Xaux][Zaux].Zobjects[o]) +
                               MapSquares[Xaux][Zaux].objects[o]->posicaoLadoY,
                                  0.0);
                     MapSquares[Xaux][Zaux].objects[o]->draw(
                             MapSquares[Xaux][Zaux].Xobjects[o],
                             MapSquares[Xaux][Zaux].Zobjects[o],
                             distancia,
                             MapSquares[Xaux][Zaux].objectsOrientation[o]);
                     glPopMatrix();
                  }
              }
           }
           MapSquares[Xaux][Zaux].visivel = 0;
        }
      glDisable(GL_COLOR_MATERIAL);
      glColor3f(1.0,1.0,1.0);


      return(0);
}

/********************************************************************
 *                          Map Constructor                         *
 ********************************************************************/
Map::Map()
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
   objects = new(lMapObject);
   x = z = 0;
   xInic = zInic = 0;
   SQUAREMINISIZE = 4;
   SQUAREMINIDIV = (SQUARESIZE / SQUAREMINISIZE);
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
   int posX =(int)floor( nx / (SQUARESIZE));
   int posZ =(int)floor( nz / (SQUARESIZE)); 

   Square* saux = relativeSquare(posX, posZ);

   return(getHeight(nx, nz, saux));
}

GLfloat Map::getHeight(GLfloat nx, GLfloat nz, Square* saux)
{
   if(!saux)
   {
      /* No positon on map, so no height! */
      return(0.0);
   }

   /* Do Interpolation to define the Height on position */
   GLfloat dx1 = fabs(nx - saux->x1) / SQUARESIZE;
   GLfloat dz1 = fabs(nz - saux->z1) / SQUARESIZE;
   GLfloat dx2 = fabs(saux->x2 - nx) / SQUARESIZE;
   GLfloat dz2 = fabs(saux->z2 - nz) / SQUARESIZE;

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
mapObject* Map::insertMapObject(string arquivo, modelList& mdlList)
{
   return(objects->insertMapObject(arquivo, mdlList));
}

/********************************************************************
 *                       Open Map File                              *
 ********************************************************************/
int Map::open(string arquivo, modelList& mdlList)
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
   int numObjetosAtual = 0;
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
   MapSquares = new Square*[x];//(Square***) malloc(x*sizeof(Square**));
   for(i = 0; i < x; i++)
   {
      MapSquares[i] = new Square[z];//(Square**) malloc(z*sizeof(Square*));
   } 

   /*for(posX = 0; posX < x; posX++)
   {
      for(posZ = 0; posZ < z; posZ++)
      {
         MapSquares[posX][posZ] = new(Square);
      }
   }*/

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
            porta->object = objects->getMapObject(nome);
            if(porta->object == NULL)
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
               int posX =(int)floor( xInic / (SQUARESIZE));
               int posZ =(int)floor( zInic / (SQUARESIZE));
               squareInic = relativeSquare(posX,posZ);
            }
            break;
         }
         case 'o': /* Insert Object */
         {
             fgets(buffer, sizeof(buffer), arq); 
             sscanf(buffer, "%s",nomeArq);
             objects->insertMapObject(nomeArq, mdlList);
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
            numObjetosAtual = 0;
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
            MapSquares[posX][posZ].x1 = (posX) * SQUARESIZE;
            MapSquares[posX][posZ].x2 = MapSquares[posX][posZ].x1+SQUARESIZE;
            MapSquares[posX][posZ].z1 = (posZ) * SQUARESIZE;
            MapSquares[posX][posZ].z2 = MapSquares[posX][posZ].z1+SQUARESIZE; 
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
                  if(numObjetosAtual >= MAXOBJETOS)
                  {
                     printf("Overflow of objects per Square\n");
                  }
                  else
                  {
                     fgets(buffer, sizeof(buffer), arq);
                     sscanf(buffer,"%s %d:%d,%d:%f,%f:%d:%d",nome,
                       &MapSquares[posX][posZ].objectsDesenha[numObjetosAtual],
                       &MapSquares[posX][posZ].quadXobjects[numObjetosAtual],
                       &MapSquares[posX][posZ].quadZobjects[numObjetosAtual],
                       &MapSquares[posX][posZ].Xobjects[numObjetosAtual],
                       &MapSquares[posX][posZ].Zobjects[numObjetosAtual],
                       &MapSquares[posX][posZ].objectsOrientation[numObjetosAtual],
                       &MapSquares[posX][posZ].pisavelObj[numObjetosAtual]);
                     MapSquares[posX][posZ].objects[numObjetosAtual] = 
                                                   objects->getMapObject(nome);
                     MapSquares[posX][posZ].quadObjetos[i] = relativeSquare(
                         MapSquares[posX][posZ].quadXobjects[numObjetosAtual],
                         MapSquares[posX][posZ].quadZobjects[numObjetosAtual]);
                     numObjetosAtual++;
                  }
                  break;
               }
               default:
                       printf("What the Hell: %s on %s\n",buffer,arquivo.c_str());
                       break;
            }
            break; 
         }
         case '#': //ignore comentaires
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
      inix = (int)floor(maux->x1 / SQUARESIZE);
      iniz = (int)floor(maux->z1 / SQUARESIZE);
      maxx = (int)floor(maux->x2 / SQUARESIZE);
      maxz = (int)floor(maux->z2 / SQUARESIZE);
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
          saux->x1 = (auxX)*SQUARESIZE;
          saux->x2 = saux->x1+SQUARESIZE;
          saux->z1 = (auxZ)*SQUARESIZE;
          saux->z2 = saux->z1+SQUARESIZE; 
          saux->posX = auxX;
          saux->posZ = auxZ;
          saux->flags = PISAVEL;
          saux->textura = IDtextura;
          saux->R = 130;
          saux->G = 148;
          saux->B = 96;
      }
   }

   xInic = 1*SQUARESIZE;
   zInic = 1*SQUARESIZE;
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
   /*if(!lights.empty())
   {
      fprintf(arq,"light %s\n",lights.c_str());
   }*/

   /* Write if is outdoor or not */
   fprintf(arq,"Outdoor: %d\n", outdoor);

   /* Write used objects */
   int i;
   if(objects->total>0)
   {
      mapObject* objAux = objects->first;
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
      fprintf(arq,"d %s %f,%f:%d\n",porta->object->getFileName().c_str(),
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
          for(aux=0;aux<MAXOBJETOS;aux++)
          {
            if(MapSquares[x1][z1].objects[aux])
            {
               x2 = (int)MapSquares[x1][z1].Xobjects[aux] / SQUARESIZE;
               z2 = (int)MapSquares[x1][z1].Zobjects[aux] / SQUARESIZE;
               fprintf(arq,"uo %s %d:%d,%d:%f,%f:%d:%d\n",
                       MapSquares[x1][z1].objects[aux]->getFileName().c_str(),
                       MapSquares[x1][z1].objectsDesenha[aux],
                       x2+1,z2+1,
                       MapSquares[x1][z1].Xobjects[aux],
                       MapSquares[x1][z1].Zobjects[aux],
                       MapSquares[x1][z1].objectsOrientation[aux],
                       MapSquares[x1][z1].pisavelObj[aux]);
            }
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



   /* Deleting all objects */
   delete(objects);
  
   /* Deleting all squares */
   //int x1,z1;
   /*for(x1 = 0; x1<x; x1++)
   {
      for(z1 = 0; z1<z;z1++)
      {
         delete (MapSquares[x1][z1]);
      }
   }*/
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
          cor_Definir(MapSquares[X][Z].R,
                      MapSquares[X][Z].G,
                      MapSquares[X][Z].B);
          retangulo_Colorir(img, x1, y1, 
                            x1+SQUAREMINISIZE-1, y1+SQUAREMINISIZE-1, 0);
          x1+=SQUAREMINISIZE;
      }
      x1 = 0;
      y1+=SQUAREMINISIZE;
   }

   cor_Definir(1, 1, 1);
   retangulo_2Cores(img,0,0,sX*SQUAREMINISIZE-1,sZ*SQUAREMINISIZE-1,0,0,0,0);
   
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
       cor_Definir(255,40,30);
       linha_Desenhar(img, x1,y1,x2,y2, 0);
       maux = maux->proximo;
   }

}

