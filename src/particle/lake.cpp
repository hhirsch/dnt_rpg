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
         vertexBuffer[vPos] = x*VERT_POV;
         vertexBuffer[vPos+1] = -1;
         vertexBuffer[vPos+2] = z*VERT_POV; 

         /* Vertex 2 */
         vertexBuffer[vPos+3] = (x+1)*VERT_POV;
         vertexBuffer[vPos+4] = -1;
         vertexBuffer[vPos+5] = z*VERT_POV;

         /* Vertex 3 */
         vertexBuffer[vPos+6] = (x+1)*VERT_POV;
         vertexBuffer[vPos+7] = -1;
         vertexBuffer[vPos+8] = (z+1)*VERT_POV;

         /* Vertex 4 */
         vertexBuffer[vPos+9] = x*VERT_POV;
         vertexBuffer[vPos+10] = -1;
         vertexBuffer[vPos+11] = (z+1)*VERT_POV;

         /* Increment buffer position */
         vPos += 12;
      }
   }
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


