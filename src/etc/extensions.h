#ifndef _extensions_h
#define _extensions_h

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

class extensions
{
   public:
      /* Texture per points functions */
      static PFNGLPOINTPARAMETERFARBPROC PointParameterf;   
      static PFNGLPOINTPARAMETERFVARBPROC PointParameterfv;

      /* Multi Textures Functions */
      static PFNGLACTIVETEXTUREPROC ARBActiveTexture;
      static PFNGLMULTITEXCOORD2FPROC ARBMultiTexCoord2f;
      static PFNGLMULTITEXCOORD2FVPROC ARBMultiTexCoord2fv;
      static PFNGLMULTITEXCOORD2DPROC ARBMultiTexCoord2d;
      static PFNGLMULTITEXCOORD2DVPROC ARBMultiTexCoord2dv;

      /*! Gets all Extended Functions that DNT uses. Usually call this after
       * the init of the Engine. */
      void defineAllExtensions();
};

#endif

