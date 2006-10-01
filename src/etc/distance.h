#ifndef _distance_h
#define _distance_h

#include <math.h>
#include <SDL/SDL_opengl.h>

/*! Returns the distance from point Po to line */
GLfloat distancePointLine(GLfloat x0, GLfloat z0, GLfloat x1, GLfloat z1,
                          GLfloat x2, GLfloat z2);

#endif

