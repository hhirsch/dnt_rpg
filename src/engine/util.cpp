#include "util.h"


/*********************************************************************
 *                   Loading Screen Atualization                     *
 *********************************************************************/
void showLoading(SDL_Surface* img, GLuint* texturaTexto, 
                 GLuint texturaCarga, const char* texto,
                 GLdouble proj[16], GLdouble modl[16],GLint viewPort[4])
{
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
   glDeleteTextures(1,texturaTexto);
   color_Set(0,0,0);
   rectangle_Fill(img,0,0,255,31);
   color_Set(200,20,20);
   defineFont(FMINI,ALIGN_CENTER,1);
   write(img,128,0,texto);
   setTextureRGBA(img,texturaTexto);
   
   textureToScreen(texturaCarga,proj,modl,viewPort,272,236,527,363,256,128,
                   0.01);
   textureToScreen(*texturaTexto,proj,modl,viewPort,272,365,527,396,256,128,
                   0.01);
   glFlush();
   SDL_GL_SwapBuffers();
}

/*********************************************************************
 *                Verify if two bounding boxes intercepts            *
 *********************************************************************/
int intercepts(GLfloat min1[3], GLfloat max1[3],
               GLfloat min2[3], GLfloat max2[3],
               int inverso)
{
   //testa o mínimo X do 2
   if( (min1[0] < min2[0]) && (max1[0] > min2[0]  ) )
   {
      //testa minimo Y
      //if( (min1[1] < min2[1]) && (max1[1] > min2[1]) )
      //{
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      //}
      //testa maximo Y
      /*if( (min1[1] < max2[1]) && (max1[1] > max2[1]) )
      {
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      }*/
   }
   
   //testa com o máximo X do 2
   if( (min1[0] < max2[0]) && (max1[0] > max2[0]) )
   {
      //testa minimo Y
      if( (min1[1] < min2[1]) && (max1[1] > min2[1]) )
      {
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      }
      //testa maximo Y
      if( (min1[1] < max2[1]) && (max1[1] > max2[1]) )
      {
         //testa minimoZ
         if( (min1[2] < min2[2]) && (max1[2] > min2[2]) )
         {
            return(1);
         }
         //testa maximoZ
         if( (min1[2] < max2[2]) && (max1[2] > max2[2]) )
         {
            return(1);
         }
      }
   }

   //testa casos de cruz + 
   if( (min2[0] < min1[0]) && (max2[0] > max1[0]) &&
       (min2[2] > min1[2]) && (min2[2] < max1[2]))
   {
      return(1);
   }
   if( (min2[2] < min1[2]) && (max2[2] > max1[2]) &&
       (min2[0] > min1[0]) && (min2[0] < max1[0]))
   {
      return(1);
   }

   if(inverso)
   {
      return( intercepts(min2, max2, min1, max1, 0));
   }
   else 
   {
      return(0);
   }
}

/*********************************************************************
 *                            actionInRange                          *
 *********************************************************************/
bool actionInRange(GLfloat posX, GLfloat posZ, 
                   GLfloat targX, GLfloat targZ,
                   GLfloat range)
{
   GLfloat dist = sqrt( (targX - posX) * (targX - posX) +
                        (targZ - posZ) * (targZ - posZ) );
   return( dist <= range);
}


/*********************************************************************
 *                Rotate and Translate a Bounding Box                *
 *********************************************************************/
void rotTransBoundingBox(GLfloat orientacao, GLfloat X[4], GLfloat Z[4],
                         GLfloat varX, GLfloat varMinY, GLfloat varMaxY, 
                         GLfloat varZ,
                         GLfloat min[3], GLfloat max[3])
{
   int aux;
   GLfloat x[4];
   GLfloat z[4];
   /* Rotaciona o bounding para a posicao correrta */
   GLfloat cosseno = cos(deg2Rad(orientacao));
   GLfloat seno = sin(deg2Rad(orientacao));
   for(aux = 0;aux<4;aux++)
   {
      x[aux] = (Z[aux]*seno) + (X[aux]*cosseno);
      z[aux] = (Z[aux]*cosseno) - (X[aux]*seno);
   }

   
   /* translada o bounding box para o local correto*/
   min[1] = varMinY;
   max[1] = varMaxY;
   for(aux=0;aux<4;aux++)
   {
     x[aux] += varX;
     z[aux] += varZ;
     if(aux == 0)
     {
        min[0] = x[0]; max[0] = x[0];
        min[2] = z[0]; max[2] = z[0];
     }
     else
     {
         if(x[aux] < min[0])
         {
            min[0] = x[aux];
         }
         if(x[aux] > max[0])
         {
            max[0] = x[aux];
         }
         if(z[aux] < min[2])
         {
            min[2] = z[aux];
         }
         if(z[aux] > max[2])
         {
            max[2] = z[aux];
         }

     }
   }
}

/*********************************************************************
 *                               rotatePoint                         *
 *********************************************************************/
void rotatePoint(GLfloat srcX, GLfloat srcY, GLfloat srcZ, GLfloat theta,
                 GLfloat p1X, GLfloat p1Y, GLfloat p1Z, 
                 GLfloat p2X, GLfloat p2Y, GLfloat p2Z, 
                 GLfloat& resX, GLfloat& resY, GLfloat& resZ)
{
   GLfloat costheta,sintheta;
   GLfloat rX, rY, rZ;

   /* Calculate the axys vector */
   resX = 0; resY = 0; resZ = 0;
   rX = p2X - p1X;
   rY = p2Y - p1Y;
   rZ = p2Z - p1Z;
   srcX -= p1X;
   srcY -= p1Y;
   srcZ -= p1Z;

   /* Normalize the vector */
   normalize(rX, rY, rZ);

   /* Define the in and cos, to avoid calculating it all the time */
   costheta = cos(theta);
   sintheta = sin(theta);

   resX += (costheta + (1 - costheta) * rX * rX) * srcX;
   resX += ((1 - costheta) * rX * rY - rZ * sintheta) * srcY;
   resX += ((1 - costheta) * rX * rZ + rY * sintheta) * srcZ;

   resY += ((1 - costheta) * rX * rY + rZ * sintheta) * srcX;
   resY += (costheta + (1 - costheta) * rY * rY) * srcY;
   resY += ((1 - costheta) * rY * rZ - rX * sintheta) * srcZ;

   resZ += ((1 - costheta) * rX * rZ - rY * sintheta) * srcX;
   resZ += ((1 - costheta) * rY * rZ + rX * sintheta) * srcY;
   resZ += (costheta + (1 - costheta) * rZ * rZ) * srcZ;

   resX += p1X;
   resY += p1Y;
   resZ += p1Z;
}

/*********************************************************************
 *                               normalize                           *
 *********************************************************************/
void normalize (GLfloat& nx, GLfloat& ny, GLfloat& nz)
{
    // calculate the length of the vector
    GLfloat len = (GLfloat)(sqrt((nx * nx) + (ny * ny) + (nz * nz)));

    // avoid division by 0
    if (len == 0.0f)
        len = 1.0f;

    // reduce to unit size
    nx /= len;
    ny /= len;
    nz /= len;
}

/*********************************************************************
 *                                normal                             *
 *********************************************************************/
void normal (GLfloat x1, GLfloat y1, GLfloat z1,
             GLfloat x2, GLfloat y2, GLfloat z2,
             GLfloat x3, GLfloat y3, GLfloat z3, 
             GLfloat& nx, GLfloat &ny, GLfloat& nz)
{
    GLfloat ax, ay, az , bx, by, bz;

    // calculate the vectors A and B
    // note that v[3] is defined with counterclockwise winding in mind
    // a
    ax = x1 - x2;
    ay = y1 - y2;
    az = z1 - z2;
    // b
    bx = x2 - x3;
    by = y2 - y3;
    bz = z2 - z3;

    // calculate the cross product and place the resulting vector
    // into the address specified by vertex_t *normal
    nx = (ay * bz) - (az * by);
    ny = (az * bx) - (ax * bz);
    nz = (ax * by) - (ay * bx);

    // normalize
    normalize(nx, ny, nz);
}

/*********************************************************************
 *                             draw2DMode                            *
 *********************************************************************/
void draw2DMode()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble) SCREEN_X, 0.0, (GLdouble) SCREEN_Y);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/*********************************************************************
 *                             draw3DMode                            *
 *********************************************************************/
void draw3DMode(int actualFarView)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, SCREEN_X / (float)SCREEN_Y, 1.0, actualFarView);
   glMatrixMode (GL_MODELVIEW);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


