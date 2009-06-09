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

#include "lake.h"

#include <math.h>

#define VERT_POV 10.0

/***************************************************************
 *                        Constructor                          *
 ***************************************************************/
lake::lake()
{
   int i;

   vertexBuffer = NULL;
   numVertex = 0;

   for(i = 0; i < 4; i++)
   {
      pos[i] = -1;
   }

   color[0] = 0.71;
   color[1] = 0.85;
   color[2] = 0.96;
   color[3] = 0.4;
}

/***************************************************************
 *                         Destructor                          *
 ***************************************************************/
lake::~lake()
{
   if(vertexBuffer)
   {
      delete[] vertexBuffer;
   }
}

/***************************************************************
 *                          getColor                           *
 ***************************************************************/
void lake::getColor(GLfloat &r, GLfloat &g, GLfloat &b, GLfloat &a)
{
   r = color[0];
   g = color[1];
   b = color[2];
   a = color[3];
}

/***************************************************************
 *                        getPosition                          *
 ***************************************************************/
void lake::getPosition(GLfloat &x1, GLfloat &z1, GLfloat &x2, GLfloat &z2)
{
   x1 = pos[0];
   z1 = pos[1];
   x2 = pos[2];
   z2 = pos[3];
}

/***************************************************************
 *                        defineColor                          *
 ***************************************************************/
void lake::defineColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
   color[0] = r;
   color[1] = g;
   color[2] = b;
   color[3] = a;
}

/***************************************************************
 *                       definePosition                        *
 ***************************************************************/
void lake::definePosition(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2)
{
   /* Deallocate the buffer, if exists */
   if(vertexBuffer)
   {
      delete[] vertexBuffer;
   }

   /* Copy values */
   pos[0] = x1;
   pos[1] = z1;
   pos[2] = x2;
   pos[3] = z2;

   numVertex = 4;
   vertexBuffer = new GLfloat[numVertex*3];
   int vPos = 0;

         /* Vertex 1 */
         vertexBuffer[vPos] = x1;
         vertexBuffer[vPos+1] = -4;
         vertexBuffer[vPos+2] = z1; 

         /* Vertex 2 */
         vertexBuffer[vPos+3] = x2;
         vertexBuffer[vPos+4] = -4;
         vertexBuffer[vPos+5] = z1;

         /* Vertex 3 */
         vertexBuffer[vPos+6] = x2;
         vertexBuffer[vPos+7] = -4;
         vertexBuffer[vPos+8] = z2;

         /* Vertex 4 */
         vertexBuffer[vPos+9] = x1;
         vertexBuffer[vPos+10] = -4;
         vertexBuffer[vPos+11] = z2;

#if 0 
   /* Calculate number of vertex */
   int vX = ceil((x2 - x1) / VERT_POV);
   int vZ = ceil((z2 - z1) / VERT_POV);
   numVertex = vX*vZ*4;

   /* Allocate the buffer */
   vertexBuffer = new GLfloat[numVertex*3];

   /* Generate the Grid */
   int x, z;
   int vPos = 0;
   for(x = 0; x < vX; x++)
   {
      for(z = 0; z < vZ; z++)
      {
         /* Vertex 1 */
         vertexBuffer[vPos] = x1 + x*VERT_POV;
         vertexBuffer[vPos+1] = -4;
         vertexBuffer[vPos+2] = z1 + z*VERT_POV; 

         /* Vertex 2 */
         vertexBuffer[vPos+3] = x1 + (x+1)*VERT_POV;
         vertexBuffer[vPos+4] = -4;
         vertexBuffer[vPos+5] = z1 + z*VERT_POV;

         /* Vertex 3 */
         vertexBuffer[vPos+6] = x1 + (x+1)*VERT_POV;
         vertexBuffer[vPos+7] = -4;
         vertexBuffer[vPos+8] = z1 + (z+1)*VERT_POV;

         /* Vertex 4 */
         vertexBuffer[vPos+9] = x1 + x*VERT_POV;
         vertexBuffer[vPos+10] = -4;
         vertexBuffer[vPos+11] = z1 + (z+1)*VERT_POV;

         /* Increment buffer position */
         vPos += 12;
      }
   }
#endif   
}

/***************************************************************
 *                              draw                           *
 ***************************************************************/
void lake::draw()
{
   if(vertexBuffer)
   {
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
      glColor4fv(color);

      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, vertexBuffer);
      glDrawArrays(GL_QUADS, 0, numVertex);
      glDisableClientState(GL_VERTEX_ARRAY);

      glColor4f(1.0,1.0,1.0,1.0);
      glDisable(GL_BLEND);
   }
}


