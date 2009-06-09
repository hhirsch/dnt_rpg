/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#include "maproad.h"
#include "map.h"

#define HALFSIZE 32
#define FULLSIZE 32

/****************************************************************
 *                         Constructor                          *
 ****************************************************************/
mapRoad::mapRoad(int numX, int numZ)
{
   SDL_Surface* img;
   int i,z;
   maxX = numX+1;
   maxZ = numZ+1;
   typeOfRoad = (int**) malloc(maxX*sizeof(int*));
   for(i=0; i < maxX; i++)
   {
      typeOfRoad[i] = (int*) malloc(maxZ*sizeof(int));
      for(z=0; z < maxZ; z++)
      {
        typeOfRoad[i][z] = MAPROAD_NONE;
      }
   }

   img = IMG_Load("../data/texturas/roads/dirt2.png");
   glGenTextures(1,&roadTexture);
   glBindTexture(GL_TEXTURE_2D, roadTexture);
   if(!img)
   {
      printf("Fail: ../data/texturas/roads/dirt.png\nShould Crash Soon.\n");
   }
   else
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 
                   0, GL_RGBA, GL_UNSIGNED_BYTE, 
                   img->pixels);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_LINEAR );
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

   
      gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->w,
                        img->h, GL_RGBA, GL_UNSIGNED_BYTE, 
                        img->pixels );
      SDL_FreeSurface(img);
   }
   
   img = IMG_Load("../data/texturas/roads/dirt_curve2.png");
   glGenTextures(1,&roadCurveTexture);
   glBindTexture(GL_TEXTURE_2D, roadCurveTexture);
   if(!img)
   {
      printf("Fail: ../data/texturas/roads/dirt_curve.png\nWill Crash Soon.\n");
   }
   else
   {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 
                   0, GL_RGBA, GL_UNSIGNED_BYTE, 
                   img->pixels);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

      glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_LINEAR );
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

   
      gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->w,
                        img->h, GL_RGBA, GL_UNSIGNED_BYTE, 
                        img->pixels );
      SDL_FreeSurface(img);
   }

}

/****************************************************************
 *                          Destructor                          *
 ****************************************************************/
mapRoad::~mapRoad()
{
   int i;
   for(i=0; i < maxX; i++)
   {
      free(typeOfRoad[i]);
   }
   free(typeOfRoad);
   glDeleteTextures(1, &roadTexture);
   glDeleteTextures(1, &roadCurveTexture);
}

/****************************************************************
 *                           setRoad                            *
 ****************************************************************/
void mapRoad::setRoad(int posX, int posZ, int direction)
{
   if( (posX > 0) && (posX < maxX) &&
       (posZ > 0) && (posZ < maxZ))
   {
      typeOfRoad[posX][posZ] = direction;
   }
}

/****************************************************************
 *                          unsetRoad                           *
 ****************************************************************/
void mapRoad::unsetRoad(int posX, int posZ)
{
   if( (posX > 0) && (posX < maxX) &&
       (posZ > 0) && (posZ < maxZ))
   {
      typeOfRoad[posX][posZ] = MAPROAD_NONE;
   }
}

/****************************************************************
 *                             draw                             *
 ****************************************************************/
void mapRoad::draw()
{
   int x,z;

   GLfloat x1, x2, x3, x4, z1, z2, z3, z4;
   GLfloat dX, dZ;
   
   glColor4f(1,1,1,1);
   glPushMatrix();
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_TEXTURE_2D);
   
   for(x=0; x<maxX; x++)
   {
      for(z=0;z<maxZ; z++)
      {
       if(typeOfRoad[x][z] != MAPROAD_NONE)
       {
         switch(typeOfRoad[x][z])
         {
            case MAPROAD_DOWN_UP:
               glBindTexture(GL_TEXTURE_2D, roadTexture);
               x1 = 0; z1 = 0;
               x2 = 0; z2 = 1;
               x3 = 1; z3 = 1;
               x4 = 1; z4 = 0;
               dX = FULLSIZE;
               dZ = HALFSIZE;
            break;
            case MAPROAD_CURVE_DOWN_LEFT:
               glBindTexture(GL_TEXTURE_2D, roadCurveTexture);
               x1 = 1; z1 = 1;
               x2 = 1; z2 = 0;
               x3 = 0; z3 = 0;
               x4 = 0; z4 = 1;
               dX = FULLSIZE;
               dZ = HALFSIZE;
            break;
            case MAPROAD_CURVE_DOWN_RIGHT:
               glBindTexture(GL_TEXTURE_2D, roadCurveTexture);
               x1 = 0; z1 = 1;
               x2 = 0; z2 = 0;
               x3 = 1; z3 = 0;
               x4 = 1; z4 = 1;
               dX = FULLSIZE;
               dZ = HALFSIZE;
            break;
            case MAPROAD_CURVE_UP_LEFT:
               glBindTexture(GL_TEXTURE_2D, roadCurveTexture);
               x1 = 1; z1 = 0;
               x2 = 1; z2 = 1;
               x3 = 0; z3 = 1;
               x4 = 0; z4 = 0;
               dX = FULLSIZE;
               dZ = HALFSIZE;
            break;
            case MAPROAD_CURVE_UP_RIGHT:
               glBindTexture(GL_TEXTURE_2D, roadCurveTexture);
               x1 = 0; z1 = 0;
               x2 = 0; z2 = 1;
               x3 = 1; z3 = 1;
               x4 = 1; z4 = 0;
               dX = FULLSIZE;
               dZ = HALFSIZE;
            break;
            case MAPROAD_LEFT_RIGHT:
               glBindTexture(GL_TEXTURE_2D, roadTexture);
               x1 = 1; z1 = 0;
               x2 = 0; z2 = 0;
               x3 = 0; z3 = 1;
               x4 = 1; z4 = 1;
               dX = HALFSIZE;
               dZ = FULLSIZE;
            break;
            case MAPROAD_CURVE_LEFT_DOWN:
               glBindTexture(GL_TEXTURE_2D, roadCurveTexture);
               x1 = 1; z1 = 1;
               x2 = 0; z2 = 1;
               x3 = 0; z3 = 0;
               x4 = 1; z4 = 0;
               dX = HALFSIZE;
               dZ = FULLSIZE;
            break;
            case MAPROAD_CURVE_LEFT_UP:
               glBindTexture(GL_TEXTURE_2D, roadCurveTexture);
               x1 = 0; z1 = 1;
               x2 = 1; z2 = 1;
               x3 = 1; z3 = 0;
               x4 = 0; z4 = 0;
               dX = HALFSIZE;
               dZ = FULLSIZE;
            break;
            case MAPROAD_CURVE_RIGHT_DOWN:
               glBindTexture(GL_TEXTURE_2D, roadCurveTexture);
               x1 = 1; z1 = 0;
               x2 = 0; z2 = 0;
               x3 = 0; z3 = 1;
               x4 = 1; z4 = 1;
               dX = HALFSIZE;
               dZ = FULLSIZE;
            break;
            case MAPROAD_CURVE_RIGHT_UP:
               glBindTexture(GL_TEXTURE_2D, roadCurveTexture);
               x1 = 0; z1 = 0;
               x2 = 1; z2 = 0;
               x3 = 1; z3 = 1;
               x4 = 0; z4 = 1;
               dX = HALFSIZE;
               dZ = FULLSIZE;
            break;
            case MAPROAD_INTERSECTION:
               /* FIXME use intersection texture! */
               glBindTexture(GL_TEXTURE_2D, roadCurveTexture);
               x1 = 0; z1 = 0;
               x2 = 0; z2 = 1;
               x3 = 1; z3 = 1;
               x4 = 1; z4 = 0;
               dX = FULLSIZE;
               dZ = FULLSIZE;
            break;
            default:
               x1 = 0; z1 = 0;
               x2 = 0; z2 = 1;
               x3 = 1; z3 = 1;
               x4 = 1; z4 = 0;
               dX = FULLSIZE;
               dZ = HALFSIZE;
            break;
         }
         glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                         GL_NEAREST_MIPMAP_LINEAR );
         glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


         //FIXME make the road follow the map height!

         /* Draw the Surface */
         /*glBegin(GL_QUADS);
            glTexCoord2f(x1,z1);
            glVertex3f((SQUARE_SIZE*x)-dX, 0.15, (SQUARE_SIZE*z)-dZ);
            glTexCoord2f(x2,z2);
            glVertex3f((SQUARE_SIZE*x)-dX, 0.15, (SQUARE_SIZE*z)+dZ);
            glTexCoord2f(x3,z3);
            glVertex3f((SQUARE_SIZE*x)+dX, 0.15, (SQUARE_SIZE*z)+dZ);
            glTexCoord2f(x4,z4);
            glVertex3f((SQUARE_SIZE*x)+dX, 0.15, (SQUARE_SIZE*z)-dZ);
         glEnd();*/
       }
      }
   }
   
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glPopMatrix();
}

