/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "boundingbox.h"
#include "../engine/util.h"

/*! Delta value to render, since antialiasing
 *  * will do "glitch" effects if only render when visible */
#define ANTI_ALIASING_DELTA  20  

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
boundingBox::boundingBox()
{
}
boundingBox::boundingBox(float minV[3], float maxV[3])
{
   setMin(minV);
   setMax(maxV);
}


/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
boundingBox::~boundingBox()
{
}

/***********************************************************************
 *                             operator=                               *
 ***********************************************************************/
void boundingBox::operator=(const boundingBox& v)
{
   min.x = v.min.x;
   min.y = v.min.y;
   min.z = v.min.z;
   max.x = v.max.x;
   max.y = v.max.y;
   max.z = v.max.z;
}

/***********************************************************************
 *                                setMin                               *
 ***********************************************************************/
void boundingBox::setMin(float x, float y, float z)
{
   min.x = x;
   min.y = y;
   min.z = z;
}

/***********************************************************************
 *                                setMin                               *
 ***********************************************************************/
void boundingBox::setMin(float m[3])
{
   min.x = m[0];
   min.y = m[1];
   min.z = m[2];
}

/***********************************************************************
 *                                setMax                               *
 ***********************************************************************/
void boundingBox::setMax(float x, float y, float z)
{
   max.x = x;
   max.y = y;
   max.z = z;
}

/***********************************************************************
 *                                setMax                               *
 ***********************************************************************/
void boundingBox::setMax(float m[3])
{
   max.x = m[0];
   max.y = m[1];
   max.z = m[2];
}

/***********************************************************************
 *                                getMin                               *
 ***********************************************************************/
void boundingBox::getMin(float& x, float& y, float& z)
{
   x = min.x;
   y = min.y;
   z = min.z;
}

/***********************************************************************
 *                                getMax                               *
 ***********************************************************************/
void boundingBox::getMax(float& x, float& y, float& z)
{
   x = max.x;
   y = max.y;
   z = max.z;
}

/***********************************************************************
 *                               translate                             *
 ***********************************************************************/
void boundingBox::translate(float x, float y, float z)
{
   min.x += x;
   max.x += x;
   min.y += y;
   max.y += y;
   min.z += z;
   max.z += z;
}

/***********************************************************************
 *                                 notY                                *
 ***********************************************************************/
void boundingBox::notY()
{
   min.y = -min.y;
   max.y = -max.y;
}

/***********************************************************************
 *                                rotate                               *
 ***********************************************************************/
void boundingBox::rotate(float angleX, float angleY, float angleZ)
{
   float x[4], y[4], z[4];
   float X[4], Y[4], Z[4];
   float seno, cosseno;
   int aux;

   /* Rotate at Y axis */
   if(angleY != 0)
   {
      /* Populate the points X and Z */
      X[0] = min.x;
      Z[0] = min.z;
      X[1] = min.x;
      Z[1] = max.z;
      X[2] = max.x;
      Z[2] = max.z;
      X[3] = max.x;
      Z[3] = min.z;

      /* precalculate angles */
      seno = sin(deg2Rad(angleY));
      cosseno = cos(deg2Rad(angleY));

      /* Calculate new points */
      for(aux=0; aux < 4; aux++)
      {
         x[aux] = (Z[aux]*seno) + (X[aux]*cosseno);
         z[aux] = (Z[aux]*cosseno) - (X[aux]*seno);
      }
      /* Recalculate min and max */
      min.x = x[0]; max.x = x[0];
      min.z = z[0]; max.z = z[0];
      for(aux=1; aux < 4; aux++)
      {
         if(x[aux] < min.x)
         {
            min.x = x[aux];
         }
         if(x[aux] > max.x)
         {
            max.x = x[aux];
         }
         if(z[aux] < min.z)
         {
            min.z = z[aux];
         }
         if(z[aux] > max.z)
         {
            max.z = z[aux];
         }
      }
   }

   /* Rotate at X axis */
   if(angleX != 0)
   {
      /* Populate the points Y and Z */
      Y[0] = min.y;
      Z[0] = min.z;
      Y[1] = min.y;
      Z[1] = max.z;
      Y[2] = max.y;
      Z[2] = max.z;
      Y[3] = max.y;
      Z[3] = min.z;

      /* precalculate angles */
      seno = sin(deg2Rad(angleX));
      cosseno = cos(deg2Rad(angleX));

      /* Calculate new points */
      for(aux=0; aux < 4; aux++)
      {
         y[aux] = -(Z[aux]*seno) + (Y[aux]*cosseno);
         z[aux] = (Z[aux]*cosseno) + (Y[aux]*seno);
      }
      /* Recalculate min and max */
      min.y = y[0]; max.y = y[0];
      min.z = z[0]; max.z = z[0];
      for(aux=1; aux < 4; aux++)
      {
         if(y[aux] < min.y)
         {
            min.y = y[aux];
         }
         if(y[aux] > max.y)
         {
            max.y = y[aux];
         }
         if(z[aux] < min.z)
         {
            min.z = z[aux];
         }
         if(z[aux] > max.z)
         {
            max.z = z[aux];
         }
      }
   }

   /* Rotate at Z axis */
   if(angleZ != 0)
   {
      /* Populate the points X and Y */
      X[0] = min.x;
      Y[0] = min.y;
      X[1] = min.x;
      Y[1] = max.y;
      X[2] = max.x;
      Y[2] = max.y;
      X[3] = max.x;
      Y[3] = min.y;

      /* precalculate angles */
      seno = sin(deg2Rad(angleZ));
      cosseno = cos(deg2Rad(angleZ));

      /* Calculate new points */
      for(aux=0; aux < 4; aux++)
      {
         x[aux] = -(Y[aux]*seno) + (X[aux]*cosseno);
         y[aux] = (Y[aux]*cosseno) + (X[aux]*seno);
      }
      /* Recalculate min and max */
      min.x = x[0]; max.x = x[0];
      min.y = y[0]; max.y = y[0];
      for(aux=1; aux < 4; aux++)
      {
         if(x[aux] < min.x)
         {
            min.x = x[aux];
         }
         if(x[aux] > max.x)
         {
            max.x = x[aux];
         }
         if(y[aux] < min.y)
         {
            min.y = y[aux];
         }
         if(y[aux] > max.y)
         {
            max.y = y[aux];
         }
      }

   }

}

/***********************************************************************
 *                          multiplyShadow                             *
 ***********************************************************************/
void boundingBox::multiplyShadow(float* shadowMatrix)
{
   /* Note: no need to calc on Y, as it will project to Y=0, always. */
   float x[4], z[4];
   float X[4], Z[4];
   float nw;
   int i;

   /* Project on shadow all 4 "up" points of the bounding box */
   X[0] = min.x;
   Z[0] = min.z;
   X[1] = min.x;
   Z[1] = max.z;
   X[2] = max.x;
   Z[2] = max.z;
   X[3] = max.x;
   Z[3] = min.z;

   for(i=0; i < 4; i++)
   {
      /* Do Projection */
      x[i] = shadowMatrix[0]*X[i] + shadowMatrix[4]*max.y + shadowMatrix[8]*Z[i] +
         shadowMatrix[12];
      /*nmin.y = shadowMatrix[1]*min.x + shadowMatrix[5]*min.y + shadowMatrix[9]*min.z +
          shadowMatrix[13];*/
      z[i] = shadowMatrix[2]*X[i] + shadowMatrix[6]*max.y + shadowMatrix[10]*Z[i] +
         shadowMatrix[14];
      nw = shadowMatrix[3]*X[i] + shadowMatrix[7]*max.y + shadowMatrix[11]*Z[i] +
         shadowMatrix[15];
      if(nw != 0)
      {
         x[i] = x[i] / nw;
         z[i] = z[i] / nw;
      }

      /* Do min/max (conservative, as using previously min.x,max.x,min.z,max.z) */
      if(x[i] < min.x)
      {
         min.x = x[i];
      }
      if(x[i] > max.x)
      {
         max.x = x[i];
      }
      if(z[i] < min.z)
      {
         min.z = z[i];
      }
      if(z[i] > max.z)
      {
         max.z = z[i];
      }
   }

   /* Set Y, as always project to 0, set a delta to avoid glitches */
   min.y = 0;
   max.y = 4; /*< Delta to avoid glitches */
}

/***********************************************************************
 *                              intercepts                             *
 ***********************************************************************/
bool boundingBox::intercepts(boundingBox& b)
{
   float bX1=0.0f, bY1=0.0f, bZ1=0.0f;
   float bX2=0.0f, bY2=0.0f, bZ2=0.0f;

   b.getMin(bX1, bY1, bZ1);
   b.getMax(bX2, bY2, bZ2);

   /* Verify at X */
   if(! ((min.x <= bX1 && bX1 <= max.x) ||
         (min.x <= bX2 && bX2 <= max.x) ||
         (bX1 <= min.x && min.x <= bX2) ||
         (bX1 <= max.x && max.x <= bX2)))
   {
      return(false);;
   }

   /* Verify at Y */
   if(! ((min.y <= bY1 && bY1 <= max.y) ||
         (min.y <= bY2 && bY2 <= max.y) ||
         (bY1 <= min.y && min.y <= bY2) ||
         (bY1 <= max.y && max.y <= bY2)))
   {
      return(false);;
   }

   /* Verify at Z */
   if(! ((min.z <= bZ1 && bZ1 <= max.z) ||
         (min.z <= bZ2 && bZ2 <= max.z) ||
         (bZ1 <= min.z && min.z <= bZ2) ||
         (bZ1 <= max.z && max.z <= bZ2)))
   {
      return(false);;
   }

   return(true);
}

/***********************************************************************
 *                              intercepts                             *
 ***********************************************************************/
bool boundingBox::intercepts(const ray& r, float* d)
{
   /* Note: from Ogre3D source code. */
   float lowt = 0.0f;
   float t;
   bool hit = false;
   vec3_t hitpoint;

   vec3_t rayorig = r.origin;
   vec3_t raydir = r.direction;

   // Check origin inside first
   if( (rayorig[0] > min[0]) && 
       (rayorig[1] > min[1]) &&
       (rayorig[2] > min[2]) &&
       (rayorig[0] < max[0]) &&
       (rayorig[1] < max[1]) &&
       (rayorig[2] < max[2]) )
   {
      if(d)
      {
         *d = 0;
      }
      return(true);
   }

   // Check each face in turn, only check closest 3
   // Min x
   if (rayorig.x <= min.x && raydir.x > 0)
   {
      t = (min.x - rayorig.x) / raydir.x;
      if (t >= 0)
      {
         // Substitute t back into ray and check bounds and dist
         hitpoint = rayorig + raydir * t;
         if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
               hitpoint.z >= min.z && hitpoint.z <= max.z &&
               (!hit || t < lowt))
         {
            hit = true;
            lowt = t;
         }
      }
   }
   // Max x
   if (rayorig.x >= max.x && raydir.x < 0)
   {
      t = (max.x - rayorig.x) / raydir.x;
      if (t >= 0)
      {
         // Substitute t back into ray and check bounds and dist
         hitpoint = rayorig + raydir * t;
         if (hitpoint.y >= min.y && hitpoint.y <= max.y &&
               hitpoint.z >= min.z && hitpoint.z <= max.z &&
               (!hit || t < lowt))
         {
            hit = true;
            lowt = t;
         }
      }
   }
   // Min y
   if (rayorig.y <= min.y && raydir.y > 0)
   {
      t = (min.y - rayorig.y) / raydir.y;
      if (t >= 0)
      {
         // Substitute t back into ray and check bounds and dist
         hitpoint = rayorig + raydir * t;
         if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
               hitpoint.z >= min.z && hitpoint.z <= max.z &&
               (!hit || t < lowt))
         {
            hit = true;
            lowt = t;
         }
      }
   }
   // Max y
   if (rayorig.y >= max.y && raydir.y < 0)
   {
      t = (max.y - rayorig.y) / raydir.y;
      if (t >= 0)
      {
         // Substitute t back into ray and check bounds and dist
         hitpoint = rayorig + raydir * t;
         if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
               hitpoint.z >= min.z && hitpoint.z <= max.z &&
               (!hit || t < lowt))
         {
            hit = true;
            lowt = t;
         }
      }
   }
   // Min z
   if (rayorig.z <= min.z && raydir.z > 0)
   {
      t = (min.z - rayorig.z) / raydir.z;
      if (t >= 0)
      {
         // Substitute t back into ray and check bounds and dist
         hitpoint = rayorig + raydir * t;
         if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
               hitpoint.y >= min.y && hitpoint.y <= max.y &&
               (!hit || t < lowt))
         {
            hit = true;
            lowt = t;
         }
      }
   }
   // Max z
   if (rayorig.z >= max.z && raydir.z < 0)
   {
      t = (max.z - rayorig.z) / raydir.z;
      if (t >= 0)
      {
         // Substitute t back into ray and check bounds and dist
         hitpoint = rayorig + raydir * t;
         if (hitpoint.x >= min.x && hitpoint.x <= max.x &&
               hitpoint.y >= min.y && hitpoint.y <= max.y &&
               (!hit || t < lowt))
         {
            hit = true;
            lowt = t;
         }
      }
   }

   if(d)
   {
      *d = lowt;
   }

   if( (hit) && (r.size != DNT_INFINITE_RAY_SIZE) )
   {
      hit = ((lowt >= 0) && (lowt <= r.size)); 
   }
   return(hit);
}

/***********************************************************************
 *                               isVisible                             *
 ***********************************************************************/
bool boundingBox::isVisible(GLfloat** matrix)
{
   /* Apply AntiAliasing Delta */
   float cx1 = min.x - ANTI_ALIASING_DELTA;
   float cy1 = min.y - ANTI_ALIASING_DELTA;
   float cz1 = min.z - ANTI_ALIASING_DELTA;
   float cx2 = max.x + ANTI_ALIASING_DELTA;
   float cy2 = max.y + ANTI_ALIASING_DELTA;
   float cz2 = max.z + ANTI_ALIASING_DELTA;

   if(!matrix)
   {
      /* Allways visible, if no matrix passed */
      return(true);
   }

   /* If all points are outter planes, is out! */
   if(((matrix[0][0]*cx1+matrix[0][1]*cy1+matrix[0][2]*cz1+matrix[0][3] <= 0) &&
       (matrix[0][0]*cx1+matrix[0][1]*cy1+matrix[0][2]*cz2+matrix[0][3] <= 0) &&
       (matrix[0][0]*cx1+matrix[0][1]*cy2+matrix[0][2]*cz1+matrix[0][3] <= 0) &&
       (matrix[0][0]*cx1+matrix[0][1]*cy2+matrix[0][2]*cz2+matrix[0][3] <= 0) &&
       (matrix[0][0]*cx2+matrix[0][1]*cy1+matrix[0][2]*cz1+matrix[0][3] <= 0) &&
       (matrix[0][0]*cx2+matrix[0][1]*cy1+matrix[0][2]*cz2+matrix[0][3] <= 0) &&
       (matrix[0][0]*cx2+matrix[0][1]*cy2+matrix[0][2]*cz1+matrix[0][3] <= 0) &&
       (matrix[0][0]*cx2+matrix[0][1]*cy2+matrix[0][2]*cz2+matrix[0][3] <= 0))||
      ((matrix[1][0]*cx1+matrix[1][1]*cy1+matrix[1][2]*cz1+matrix[1][3] <= 0) &&
       (matrix[1][0]*cx1+matrix[1][1]*cy1+matrix[1][2]*cz2+matrix[1][3] <= 0) &&
       (matrix[1][0]*cx1+matrix[1][1]*cy2+matrix[1][2]*cz1+matrix[1][3] <= 0) &&
       (matrix[1][0]*cx1+matrix[1][1]*cy2+matrix[1][2]*cz2+matrix[1][3] <= 0) &&
       (matrix[1][0]*cx2+matrix[1][1]*cy1+matrix[1][2]*cz1+matrix[1][3] <= 0) &&
       (matrix[1][0]*cx2+matrix[1][1]*cy1+matrix[1][2]*cz2+matrix[1][3] <= 0) &&
       (matrix[1][0]*cx2+matrix[1][1]*cy2+matrix[1][2]*cz1+matrix[1][3] <= 0) &&
       (matrix[1][0]*cx2+matrix[1][1]*cy2+matrix[1][2]*cz2+matrix[1][3] <= 0))||
      ((matrix[2][0]*cx1+matrix[2][1]*cy1+matrix[2][2]*cz1+matrix[2][3] <= 0) &&
       (matrix[2][0]*cx1+matrix[2][1]*cy1+matrix[2][2]*cz2+matrix[2][3] <= 0) &&
       (matrix[2][0]*cx1+matrix[2][1]*cy2+matrix[2][2]*cz1+matrix[2][3] <= 0) &&
       (matrix[2][0]*cx1+matrix[2][1]*cy2+matrix[2][2]*cz2+matrix[2][3] <= 0) &&
       (matrix[2][0]*cx2+matrix[2][1]*cy1+matrix[2][2]*cz1+matrix[2][3] <= 0) &&
       (matrix[2][0]*cx2+matrix[2][1]*cy1+matrix[2][2]*cz2+matrix[2][3] <= 0) &&
       (matrix[2][0]*cx2+matrix[2][1]*cy2+matrix[2][2]*cz1+matrix[2][3] <= 0) &&
       (matrix[2][0]*cx2+matrix[2][1]*cy2+matrix[2][2]*cz2+matrix[2][3] <= 0))||
      ((matrix[3][0]*cx1+matrix[3][1]*cy1+matrix[3][2]*cz1+matrix[3][3] <= 0) &&
       (matrix[3][0]*cx1+matrix[3][1]*cy1+matrix[3][2]*cz2+matrix[3][3] <= 0) &&
       (matrix[3][0]*cx1+matrix[3][1]*cy2+matrix[3][2]*cz1+matrix[3][3] <= 0) &&
       (matrix[3][0]*cx1+matrix[3][1]*cy2+matrix[3][2]*cz2+matrix[3][3] <= 0) &&
       (matrix[3][0]*cx2+matrix[3][1]*cy1+matrix[3][2]*cz1+matrix[3][3] <= 0) &&
       (matrix[3][0]*cx2+matrix[3][1]*cy1+matrix[3][2]*cz2+matrix[3][3] <= 0) &&
       (matrix[3][0]*cx2+matrix[3][1]*cy2+matrix[3][2]*cz1+matrix[3][3] <= 0) &&
       (matrix[3][0]*cx2+matrix[3][1]*cy2+matrix[3][2]*cz2+matrix[3][3] <= 0))||
      ((matrix[4][0]*cx1+matrix[4][1]*cy1+matrix[4][2]*cz1+matrix[4][3] <= 0) &&
       (matrix[4][0]*cx1+matrix[4][1]*cy1+matrix[4][2]*cz2+matrix[4][3] <= 0) &&
       (matrix[4][0]*cx1+matrix[4][1]*cy2+matrix[4][2]*cz1+matrix[4][3] <= 0) &&
       (matrix[4][0]*cx1+matrix[4][1]*cy2+matrix[4][2]*cz2+matrix[4][3] <= 0) &&
       (matrix[4][0]*cx2+matrix[4][1]*cy1+matrix[4][2]*cz1+matrix[4][3] <= 0) &&
       (matrix[4][0]*cx2+matrix[4][1]*cy1+matrix[4][2]*cz2+matrix[4][3] <= 0) &&
       (matrix[4][0]*cx2+matrix[4][1]*cy2+matrix[4][2]*cz1+matrix[4][3] <= 0) &&
       (matrix[4][0]*cx2+matrix[4][1]*cy2+matrix[4][2]*cz2+matrix[4][3] <= 0))||
      ((matrix[5][0]*cx1+matrix[5][1]*cy1+matrix[5][2]*cz1+matrix[5][3] <= 0) &&
       (matrix[5][0]*cx1+matrix[5][1]*cy1+matrix[5][2]*cz2+matrix[5][3] <= 0) &&
       (matrix[5][0]*cx1+matrix[5][1]*cy2+matrix[5][2]*cz1+matrix[5][3] <= 0) &&
       (matrix[5][0]*cx1+matrix[5][1]*cy2+matrix[5][2]*cz2+matrix[5][3] <= 0) &&
       (matrix[5][0]*cx2+matrix[5][1]*cy1+matrix[5][2]*cz1+matrix[5][3] <= 0) &&
       (matrix[5][0]*cx2+matrix[5][1]*cy1+matrix[5][2]*cz2+matrix[5][3] <= 0) &&
       (matrix[5][0]*cx2+matrix[5][1]*cy2+matrix[5][2]*cz1+matrix[5][3] <= 0) &&
       (matrix[5][0]*cx2+matrix[5][1]*cy2+matrix[5][2]*cz2+matrix[5][3] <= 0)))

   {
      return(false);
   }

   /* if a point is inner, so its visible */
   return(true);
}

/***********************************************************************
 *                                render                               *
 ***********************************************************************/
void boundingBox::render()
{
   glPushAttrib(GL_ENABLE_BIT);

   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);

   glLineWidth(4);
   glColor4f(0.0f, 0.5f, 0.8f, 1.0f);
   glPushMatrix();
   
   glBegin(GL_LINE_STRIP );
      /* Front */
      glVertex3f(min.x, min.y, min.z);
      glVertex3f(min.x, max.y, min.z);
      glVertex3f(max.x, max.y, min.z);
      glVertex3f(max.x, min.y, min.z);
      glVertex3f(min.x, min.y, min.z);
   glEnd();

   glBegin(GL_LINE_STRIP );
      /* Back */
      glVertex3f(min.x, min.y, max.z);
      glVertex3f(min.x, max.y, max.z);
      glVertex3f(max.x, max.y, max.z);
      glVertex3f(max.x, min.y, max.z);
   glEnd();
      
   glBegin(GL_LINE_STRIP );
      /* Top */
      glVertex3f(min.x, max.y, min.z);
      glVertex3f(min.x, max.y, max.z);
      glVertex3f(max.x, max.y, max.z);
      glVertex3f(max.x, max.y, min.z);
   glEnd();

   glBegin(GL_LINE_STRIP );
      /* Bottom */
      glVertex3f(min.x, min.y, min.z);
      glVertex3f(min.x, min.y, max.z);
      glVertex3f(max.x, min.y, max.z);
      glVertex3f(max.x, min.y, min.z);
   glEnd();
      
   glBegin(GL_LINE_STRIP );
      /* Left */
      glVertex3f(min.x, min.y, min.z);
      glVertex3f(min.x, max.y, min.z);
      glVertex3f(min.x, max.y, max.z);
      glVertex3f(min.x, min.y, max.z);
   glEnd();

   glBegin(GL_LINE_STRIP );
      /* Right */
      glVertex3f(max.x, min.y, min.z);
      glVertex3f(max.x, max.y, min.z);
      glVertex3f(max.x, max.y, max.z);
      glVertex3f(max.x, min.y, max.z);
   glEnd();

   glPopMatrix();
   glLineWidth(1);

   glPopAttrib();
}

