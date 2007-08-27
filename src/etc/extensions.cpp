#include <stdio.h>
#include <string.h>
#include "extensions.h"


void extensions::defineAllExtensions()
{
   /* Point Functions */
   PointParameterf = NULL;
   PointParameterfv = NULL;

   char *ext = (char*)glGetString( GL_EXTENSIONS );

   if(strstr( ext, "GL_ARB_point_sprite" ) != NULL)
   {
      PointParameterf = (PFNGLPOINTPARAMETERFARBPROC)
                                     SDL_GL_GetProcAddress("glPointParameterfARB");
      PointParameterfv = (PFNGLPOINTPARAMETERFVARBPROC) 
                                    SDL_GL_GetProcAddress("glPointParameterfvARB");

      if( (PointParameterfv == NULL) || (PointParameterf == NULL) )
      {
         printf("Not found glPointParameterfARB: DNT will be in worse quality :^(\n");
      }
   }
   else
   {
      printf("Not Found GL_ARB_point_sprite extension. DNT will be in worse quality.\n");
   }

   /* Multi Texture */
   ARBActiveTexture = NULL;
   ARBMultiTexCoord2d = NULL;
   ARBMultiTexCoord2f = NULL;
   ARBMultiTexCoord2dv = NULL;
   ARBMultiTexCoord2fv = NULL;

   if(strstr( ext, "GL_ARB_multitexture") != NULL)
   {
      ARBActiveTexture = (PFNGLACTIVETEXTUREPROC) 
                                  SDL_GL_GetProcAddress("glActiveTextureARB");
      if(!ARBActiveTexture)
      {
         printf("Not Found glActiveTextureARB. DNT will be in worse quality.\n");
      }
      ARBClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC) 
                              SDL_GL_GetProcAddress("glClientActiveTextureARB");
      if(!ARBClientActiveTexture)
      {
         printf("Not Found glClientActiveTextureARB. "
                "DNT will be in worse quality.\n");
      }
      ARBMultiTexCoord2d = (PFNGLMULTITEXCOORD2DPROC)
                                SDL_GL_GetProcAddress("glMultiTexCoord2dARB");
      ARBMultiTexCoord2f = (PFNGLMULTITEXCOORD2FPROC)
                                SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
      ARBMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVPROC)
                                SDL_GL_GetProcAddress("glMultiTexCoord2dvARB");
      ARBMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVPROC)
                                SDL_GL_GetProcAddress("glMultiTexCoord2fvARB");
      if( (!ARBMultiTexCoord2d) || (!ARBMultiTexCoord2f) || 
          (!ARBMultiTexCoord2dv) || (!ARBMultiTexCoord2fv) )
      {
         printf("Not found glMultiTextCoord*ARB. "
                "DNT will be in worse quality!\n");
      }
   }

}


/* Texture per points functions */
PFNGLPOINTPARAMETERFARBPROC extensions::PointParameterf;   
PFNGLPOINTPARAMETERFVARBPROC extensions::PointParameterfv;

/* Multi Textures Functions */
PFNGLACTIVETEXTUREPROC extensions::ARBActiveTexture;
PFNGLCLIENTACTIVETEXTUREPROC extensions::ARBClientActiveTexture;
PFNGLMULTITEXCOORD2FPROC extensions::ARBMultiTexCoord2f;
PFNGLMULTITEXCOORD2FVPROC extensions::ARBMultiTexCoord2fv;
PFNGLMULTITEXCOORD2DPROC extensions::ARBMultiTexCoord2d;
PFNGLMULTITEXCOORD2DVPROC extensions::ARBMultiTexCoord2dv;

