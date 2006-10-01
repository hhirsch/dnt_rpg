#include "distance.h"

GLfloat distancePointLine(GLfloat x0, GLfloat z0, GLfloat x1, GLfloat z1,
                          GLfloat x2, GLfloat z2)
{
   /* Take the distance, preserving the signal, to know the direction,
    * formula deducted from GAAL book (!) */
   GLfloat num = /*fabs*/( ((x1 - x2)*(z1 - z0)) - ((x1 - x0)*(z1 - z2)) );
   GLfloat den = sqrt( ((x1-x2)*(x1-x2)) + ((z1-z2)*(z1-z2)) );

   return(num / den);
}
