#include <iostream>
#include <string>
using namespace std;
#include "extensions.h"


/***********************************************************************
 *                        defineAllExtensions                          *
 ***********************************************************************/
void extensions::defineAllExtensions()
{
   /* Get Extension String */
   string ext = (char*)glGetString( GL_EXTENSIONS );
   
   /* Get point texture functions */
   definePointTexture(ext);

   /* Multi Texture */
   defineMultiTexture(ext);
  
   /* Shaders */
   defineShader(ext);

}

/***********************************************************************
 *                             showWarning                             *
 ***********************************************************************/
void extensions::showWarning(string functionName)
{
   cerr << "Warning: No " << functionName << " found!" << endl
        << "DNT will be in worse quality =^( " << endl;
}

/***********************************************************************
 *                          definePointTexture                         *
 ***********************************************************************/
void extensions::definePointTexture(string ext)
{
   if( ext.find("GL_ARB_point_sprite",0) != string::npos)
   {
      arbPointParameterf = (PFNGLPOINTPARAMETERFARBPROC)
                                  SDL_GL_GetProcAddress("glPointParameterfARB");
      arbPointParameterfv = (PFNGLPOINTPARAMETERFVARBPROC) 
                                 SDL_GL_GetProcAddress("glPointParameterfvARB");

      if( (arbPointParameterfv == NULL) || (arbPointParameterf == NULL) )
      {
         showWarning("glPointParameterfARB");
      }
   }
   else
   {
      showWarning("GL_ARB_point_sprite extension");
   }
}


/***********************************************************************
 *                         defineMultiTexture                          *
 ***********************************************************************/
void extensions::defineMultiTexture(string ext)
{
   if(ext.find("GL_ARB_multitexture") != string::npos)
   {
      arbActiveTexture = (PFNGLACTIVETEXTUREPROC) 
                                  SDL_GL_GetProcAddress("glActiveTextureARB");
      if(!arbActiveTexture)
      {
         showWarning("glActiveTextureARB");
      }
      arbClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC) 
                              SDL_GL_GetProcAddress("glClientActiveTextureARB");
      if(!arbClientActiveTexture)
      {
         showWarning("glClientActiveTextureARB");
      }
      arbMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC)
                                SDL_GL_GetProcAddress("glMultiTexCoord2dARB");
      arbMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)
                                SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
      arbMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC)
                                SDL_GL_GetProcAddress("glMultiTexCoord2dvARB");
      arbMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC)
                                SDL_GL_GetProcAddress("glMultiTexCoord2fvARB");
      if( (!arbMultiTexCoord2d) || (!arbMultiTexCoord2f) || 
          (!arbMultiTexCoord2dv) || (!arbMultiTexCoord2fv) )
      {
         showWarning("glMultiTextCoord*ARB");
      }
   }
   else
   {
      showWarning("GL_ARB_multitexture extension");
   }
}

/***********************************************************************
 *                            defineShader                             *
 ***********************************************************************/
void extensions::defineShader(string ext)
{
}

/***********************************************************************
 *                           hasPointTexture                           *
 ***********************************************************************/
bool extensions::hasPointTexture()
{
   return( (arbPointParameterf != NULL) && (arbPointParameterfv != NULL));
}

/***********************************************************************
 *                           hastMultiTexture                          *
 ***********************************************************************/
bool extensions::hasMultiTexture()
{
   return( (arbActiveTexture != NULL) &&
           (arbClientActiveTexture != NULL) &&
           (arbMultiTexCoord2f != NULL) && 
           (arbMultiTexCoord2fv != NULL) && 
           (arbMultiTexCoord2d != NULL) &&
           (arbMultiTexCoord2dv != NULL) );
}

/***********************************************************************
 *                             hasShader                               *
 ***********************************************************************/
bool extensions::hasShader()
{
   return( (arbCreateShaderObject != NULL) &&
           (arbCreateProgramObject != NULL) &&
           (arbDeleteObject != NULL) &&
           (arbShaderSourceObject != NULL) &&
           (arbCompileShader != NULL) &&
           (arbAttachObject != NULL) &&
           (arbLinkProgram != NULL) &&
           (arbUseProgram != NULL) &&
           (arbGetHandleObject != NULL) &&
           (arbGetUniformLocation != NULL) &&
           (arbGetObjectParameterfv != NULL) &&
           (arbGetObjectParamenteriv != NULL) &&
           (arbUniform1f != NULL) &&
           (arbUniform2f != NULL) &&
           (arbUniform3f != NULL) );
}

/***********************************************************************
 *                            Static Members                           *
 ***********************************************************************/
/* Texture per points functions */
PFNGLPOINTPARAMETERFARBPROC extensions::arbPointParameterf = NULL;   
PFNGLPOINTPARAMETERFVARBPROC extensions::arbPointParameterfv = NULL;

/* Multi Textures Functions */
PFNGLACTIVETEXTUREPROC extensions::arbActiveTexture = NULL;
PFNGLCLIENTACTIVETEXTUREPROC extensions::arbClientActiveTexture = NULL;
PFNGLMULTITEXCOORD2FPROC extensions::arbMultiTexCoord2f = NULL;
PFNGLMULTITEXCOORD2FVPROC extensions::arbMultiTexCoord2fv = NULL;
PFNGLMULTITEXCOORD2DPROC extensions::arbMultiTexCoord2d = NULL;
PFNGLMULTITEXCOORD2DVPROC extensions::arbMultiTexCoord2dv = NULL;

/* Shader Functions */
PFNGLCREATESHADEROBJECTARBPROC extensions::arbCreateShaderObject = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC extensions::arbCreateProgramObject = NULL;
PFNGLDELETEOBJECTARBPROC extensions::arbDeleteObject = NULL;
PFNGLSHADERSOURCEARBPROC extensions::arbShaderSourceObject = NULL;
PFNGLCOMPILESHADERARBPROC extensions::arbCompileShader = NULL;
PFNGLATTACHOBJECTARBPROC extensions::arbAttachObject = NULL;
PFNGLLINKPROGRAMARBPROC extensions::arbLinkProgram = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC extensions::arbUseProgram = NULL;
PFNGLGETHANDLEARBPROC extensions::arbGetHandleObject = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC extensions::arbGetUniformLocation = NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC extensions::arbGetObjectParameterfv = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC extensions::arbGetObjectParamenteriv = NULL;
PFNGLUNIFORM1FARBPROC extensions::arbUniform1f = NULL;
PFNGLUNIFORM2FARBPROC extensions::arbUniform2f = NULL;
PFNGLUNIFORM1FARBPROC extensions::arbUniform3f = NULL;

