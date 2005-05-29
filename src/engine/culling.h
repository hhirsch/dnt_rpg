#ifndef _culling_h
#define _culling_h

#include <GL/gl.h>
#include <math.h>

/* Atualiza os planos do View Frustum atual*/
void AtualizaFrustum(GLfloat matriz[6][4]);

/* Testa se o quadrado está dentro do frustum */
int quadradoVisivel(GLfloat x1,GLfloat y1,GLfloat z1,GLfloat x2,GLfloat y2,GLfloat z2,GLfloat matriz[6][4]);


#endif

