#include "shadow.h"
#include "util.h"

/*******************************************************************
 *                          Constructor                            *
 *******************************************************************/
shadow::shadow()
{
   avaible = false;
   char *ext = (char*)glGetString( GL_EXTENSIONS );

   if(strstr( ext, "GL_ARB_shadow" ) != NULL)
   {
      avaible = true;
   }
   else
   {
      printf("Not Found GL_ARB_shadow extension. No shadows on DNT!\n");
      avaible = false;
   }

   if(strstr( ext, "GL_ARB_depth_texture" ) == NULL)
   {
      printf("Not Found GL_ARB_depth_texture extension. No shadows on DNT!\n");
      avaible = false;
   }

   biasMatrix[0] = 0.5f;
   biasMatrix[1] = 0.0f;
   biasMatrix[2] = 0.0f;
   biasMatrix[3] = 0.0f;

   biasMatrix[4] = 0.0f;
   biasMatrix[5] = 0.5f;
   biasMatrix[6] = 0.0f;
   biasMatrix[7] = 0.0f;

   biasMatrix[8] = 0.0f;
   biasMatrix[9] = 0.0f;
   biasMatrix[10] = 0.5f;
   biasMatrix[11] = 0.0f;

   biasMatrix[12] = 0.5f;
   biasMatrix[13] = 0.5f;
   biasMatrix[14] = 0.5f;
   biasMatrix[15] = 1.0f;

   //FIXME -> the shadow is too slow and not working at all
   avaible = false;
   enable = avaible;

}

/*******************************************************************
 *                           Destructor                            *
 *******************************************************************/
shadow::~shadow()
{
}

/*******************************************************************
 *                              Init                               *
 *******************************************************************/
void shadow::init()
{
   if(avaible)
   {
      

      glGenTextures(1, &shadowMapTexture);
      glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                   SHADOW_MAP_SIZE, SHADOW_MAP_SIZE, 0, GL_DEPTH_COMPONENT,
                   GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   }
}

/*******************************************************************
 *                         defineLightView                         *
 *******************************************************************/
void shadow::defineLightView(GLfloat pX, GLfloat pY, GLfloat pZ)
{
   if(avaible)
   {
      /*glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(lightProjectionMatrix);

      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixf(lightViewMatrix);*/

      glLoadIdentity();
         gluPerspective(45.0f, 1.0f, 2.0f, 8.0f);
         glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix);

      glLoadIdentity();
         gluLookAt(pX, pY, pZ,
                   0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
      glGetFloatv(GL_MODELVIEW_MATRIX, lightViewMatrix);

      glViewport(0, 0, SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
   }
}

/*******************************************************************
 *                         defineLightView                         *
 *******************************************************************/
void shadow::defineCameraView(camera& cam, GLdouble proj[16],GLdouble modl[16])
{
   if(avaible)
   {
      glClear(GL_DEPTH_BUFFER_BIT);
      glMatrixMode(GL_PROJECTION);
      glLoadMatrixf((GLfloat*)proj);

      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixf((GLfloat*)modl);

      glViewport(0, 0, SCREEN_X, SCREEN_Y);
   }
}

/*******************************************************************
 *                             isEnable                            *
 *******************************************************************/
bool shadow::isEnable()
{
   return(enable);
}

/*******************************************************************
 *                           setEnable                             *
 *******************************************************************/
void shadow::setEnable(bool en)
{
   enable = ( (en) && (avaible));
}

void shadow::saveShadowMap()
{
   glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
   glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 
                       SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);

}

void shadow::beginShadowMap()
{
   GLfloat res1[16];
   GLfloat res2[16];

   mulMatrix(biasMatrix, lightProjectionMatrix, res1);
   mulMatrix(res1, lightViewMatrix, res2);

   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGenfv(GL_S, GL_EYE_PLANE, &res2[0]);
   glEnable(GL_TEXTURE_GEN_S);

   glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGenfv(GL_T, GL_EYE_PLANE, &res2[4]);
   glEnable(GL_TEXTURE_GEN_T);
   
   glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGenfv(GL_R, GL_EYE_PLANE, &res2[8]);
   glEnable(GL_TEXTURE_GEN_R);

   glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGenfv(GL_Q, GL_EYE_PLANE, &res2[12]);
   glEnable(GL_TEXTURE_GEN_Q);

   /* Bind & enable shadow map texture */
   glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
   glEnable(GL_TEXTURE_2D);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, 
                   GL_COMPARE_R_TO_TEXTURE);

   /* Shadow comparison should be true (ie not in shadow) if r<=texture */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);

   /* Shadow comparison should generate an INTENSITY result */
   glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

   /* Set alpha test to discard false comparisons */
   glAlphaFunc(GL_GEQUAL, 0.99f);
   glEnable(GL_ALPHA_TEST);
}

void shadow::endShadowMap()
{
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_TEXTURE_GEN_S);
   glDisable(GL_TEXTURE_GEN_T);
   glDisable(GL_TEXTURE_GEN_R);
   glDisable(GL_TEXTURE_GEN_Q);
   glDisable(GL_ALPHA_TEST);
}

void shadow::mulMatrix(GLfloat a[16], GLfloat b[16], GLfloat result[16])
{
   int row;
   int col;
   int acRow;
   for(row=0; row<4; row++)
   {
      acRow = row*4;
      for(col=0; col<4; col++)
      {
         result[acRow+col] = (a[acRow]*b[col]) + 
                             (a[acRow]*b[4+col]) +
                             (a[acRow]*b[8 + col]) +
                             (a[acRow]*b[12 + col]);
      }
   }
}

