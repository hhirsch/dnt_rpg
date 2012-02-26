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
   x1 = 0;
   z1 = 0;
   y1 = 0;
   x2 = 0;
   z2 = 0;
   y2 = 0;
}
boundingBox::boundingBox(float min[3], float max[3])
{
   setMin(min);
   setMax(max);
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
   x1 = v.x1;
   y1 = v.y1;
   z1 = v.z1;
   x2 = v.x2;
   y2 = v.y2;
   z2 = v.z2;
}

/***********************************************************************
 *                                setMin                               *
 ***********************************************************************/
void boundingBox::setMin(float x, float y, float z)
{
   x1 = x;
   y1 = y;
   z1 = z;
}

/***********************************************************************
 *                                setMin                               *
 ***********************************************************************/
void boundingBox::setMin(float m[3])
{
   x1 = m[0];
   y1 = m[1];
   z1 = m[2];
}

/***********************************************************************
 *                                setMax                               *
 ***********************************************************************/
void boundingBox::setMax(float x, float y, float z)
{
   x2 = x;
   y2 = y;
   z2 = z;
}

/***********************************************************************
 *                                setMax                               *
 ***********************************************************************/
void boundingBox::setMax(float m[3])
{
   x2 = m[0];
   y2 = m[1];
   z2 = m[2];
}

/***********************************************************************
 *                                getMin                               *
 ***********************************************************************/
void boundingBox::getMin(float& x, float& y, float& z)
{
   x = x1;
   y = y1;
   z = z1;
}

/***********************************************************************
 *                                getMax                               *
 ***********************************************************************/
void boundingBox::getMax(float& x, float& y, float& z)
{
   x = x2;
   y = y2;
   z = z2;
}

/***********************************************************************
 *                               translate                             *
 ***********************************************************************/
void boundingBox::translate(float x, float y, float z)
{
   x1 += x;
   x2 += x;
   y1 += y;
   y2 += y;
   z1 += z;
   z2 += z;
}

/***********************************************************************
 *                                 notY                                *
 ***********************************************************************/
void boundingBox::notY()
{
   y1 = -y1;
   y2 = -y2;
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
      X[0] = x1;
      Z[0] = z1;
      X[1] = x1;
      Z[1] = z2;
      X[2] = x2;
      Z[2] = z2;
      X[3] = x2;
      Z[3] = z1;

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
      x1 = x[0]; x2 = x[0];
      z1 = z[0]; z2 = z[0];
      for(aux=1; aux < 4; aux++)
      {
         if(x[aux] < x1)
         {
            x1 = x[aux];
         }
         if(x[aux] > x2)
         {
            x2 = x[aux];
         }
         if(z[aux] < z1)
         {
            z1 = z[aux];
         }
         if(z[aux] > z2)
         {
            z2 = z[aux];
         }
      }
   }

   /* Rotate at X axis */
   if(angleX != 0)
   {
      /* Populate the points Y and Z */
      Y[0] = y1;
      Z[0] = z1;
      Y[1] = y1;
      Z[1] = z2;
      Y[2] = y2;
      Z[2] = z2;
      Y[3] = y2;
      Z[3] = z1;

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
      y1 = y[0]; y2 = y[0];
      z1 = z[0]; z2 = z[0];
      for(aux=1; aux < 4; aux++)
      {
         if(y[aux] < y1)
         {
            y1 = y[aux];
         }
         if(y[aux] > y2)
         {
            y2 = y[aux];
         }
         if(z[aux] < z1)
         {
            z1 = z[aux];
         }
         if(z[aux] > z2)
         {
            z2 = z[aux];
         }
      }
   }

   /* Rotate at Z axis */
   if(angleZ != 0)
   {
      /* Populate the points X and Y */
      X[0] = x1;
      Y[0] = y1;
      X[1] = x1;
      Y[1] = y2;
      X[2] = x2;
      Y[2] = y2;
      X[3] = x2;
      Y[3] = y1;

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
      x1 = x[0]; x2 = x[0];
      y1 = y[0]; y2 = y[0];
      for(aux=1; aux < 4; aux++)
      {
         if(x[aux] < x1)
         {
            x1 = x[aux];
         }
         if(x[aux] > x2)
         {
            x2 = x[aux];
         }
         if(y[aux] < y1)
         {
            y1 = y[aux];
         }
         if(y[aux] > y2)
         {
            y2 = y[aux];
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
   X[0] = x1;
   Z[0] = z1;
   X[1] = x1;
   Z[1] = z2;
   X[2] = x2;
   Z[2] = z2;
   X[3] = x2;
   Z[3] = z1;

   for(i=0; i < 4; i++)
   {
      /* Do Projection */
      x[i] = shadowMatrix[0]*X[i] + shadowMatrix[4]*y2 + shadowMatrix[8]*Z[i] +
         shadowMatrix[12];
      /*ny1 = shadowMatrix[1]*x1 + shadowMatrix[5]*y1 + shadowMatrix[9]*z1 +
          shadowMatrix[13];*/
      z[i] = shadowMatrix[2]*X[i] + shadowMatrix[6]*y2 + shadowMatrix[10]*Z[i] +
         shadowMatrix[14];
      nw = shadowMatrix[3]*X[i] + shadowMatrix[7]*y2 + shadowMatrix[11]*Z[i] +
         shadowMatrix[15];
      if(nw != 0)
      {
         x[i] = x[i] / nw;
         z[i] = z[i] / nw;
      }

      /* Do min/max (conservative, as using previously x1,x2,z1,z2) */
      if(x[i] < x1)
      {
         x1 = x[i];
      }
      if(x[i] > x2)
      {
         x2 = x[i];
      }
      if(z[i] < z1)
      {
         z1 = z[i];
      }
      if(z[i] > z2)
      {
         z2 = z[i];
      }
   }

   /* Set Y, as always project to 0, set a delta to avoid glitches */
   y1 = 0;
   y2 = 4; /*< Delta to avoid glitches */
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
   if(! ((x1 <= bX1 && bX1 <= x2) ||
         (x1 <= bX2 && bX2 <= x2) ||
         (bX1 <= x1 && x1 <= bX2) ||
         (bX1 <= x2 && x2 <= bX2)))
   {
      return(false);;
   }

   /* Verify at Y */
   if(! ((y1 <= bY1 && bY1 <= y2) ||
         (y1 <= bY2 && bY2 <= y2) ||
         (bY1 <= y1 && y1 <= bY2) ||
         (bY1 <= y2 && y2 <= bY2)))
   {
      return(false);;
   }

   /* Verify at Z */
   if(! ((z1 <= bZ1 && bZ1 <= z2) ||
         (z1 <= bZ2 && bZ2 <= z2) ||
         (bZ1 <= z1 && z1 <= bZ2) ||
         (bZ1 <= z2 && z2 <= bZ2)))
   {
      return(false);;
   }

   return(true);
}

/***********************************************************************
 *                               isVisible                             *
 ***********************************************************************/
bool boundingBox::isVisible(GLfloat** matrix)
{
   /* Apply AntiAliasing Delta */
   float cx1 = x1 - ANTI_ALIASING_DELTA;
   float cy1 = y1 - ANTI_ALIASING_DELTA;
   float cz1 = z1 - ANTI_ALIASING_DELTA;
   float cx2 = x2 + ANTI_ALIASING_DELTA;
   float cy2 = y2 + ANTI_ALIASING_DELTA;
   float cz2 = z2 + ANTI_ALIASING_DELTA;

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
      glVertex3f(x1, y1, z1);
      glVertex3f(x1, y2, z1);
      glVertex3f(x2, y2, z1);
      glVertex3f(x2, y1, z1);
      glVertex3f(x1, y1, z1);
   glEnd();

   glBegin(GL_LINE_STRIP );
      /* Back */
      glVertex3f(x1, y1, z2);
      glVertex3f(x1, y2, z2);
      glVertex3f(x2, y2, z2);
      glVertex3f(x2, y1, z2);
   glEnd();
      
   glBegin(GL_LINE_STRIP );
      /* Top */
      glVertex3f(x1, y2, z1);
      glVertex3f(x1, y2, z2);
      glVertex3f(x2, y2, z2);
      glVertex3f(x2, y2, z1);
   glEnd();

   glBegin(GL_LINE_STRIP );
      /* Bottom */
      glVertex3f(x1, y1, z1);
      glVertex3f(x1, y1, z2);
      glVertex3f(x2, y1, z2);
      glVertex3f(x2, y1, z1);
   glEnd();
      
   glBegin(GL_LINE_STRIP );
      /* Left */
      glVertex3f(x1, y1, z1);
      glVertex3f(x1, y2, z1);
      glVertex3f(x1, y2, z2);
      glVertex3f(x1, y1, z2);
   glEnd();

   glBegin(GL_LINE_STRIP );
      /* Right */
      glVertex3f(x2, y1, z1);
      glVertex3f(x2, y2, z1);
      glVertex3f(x2, y2, z2);
      glVertex3f(x2, y1, z2);
   glEnd();

   glPopMatrix();
   glLineWidth(1);

   glPopAttrib();
}

