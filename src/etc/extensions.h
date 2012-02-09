/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_extensions_h
#define _dnt_extensions_h

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

/*! Print all openGL errors on cache */
void printOpenGLErrors(std::string where="");

/* some declarations for AmigaOS4 */
#if defined (__amigaos4__)
   typedef void (APIENTRYP PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
   typedef void (APIENTRYP PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
   typedef void (APIENTRYP PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, 
                                                         GLfloat s, GLfloat t);
   typedef void (APIENTRYP PFNGLMULTITEXCOORD2FVARBPROC) (GLenum target, 
                                                          const GLfloat *v);
   typedef void (APIENTRYP PFNGLMULTITEXCOORD2DARBPROC) (GLenum target, 
                                                        GLdouble s, GLdouble t);
   typedef void (APIENTRYP PFNGLMULTITEXCOORD2DVARBPROC) (GLenum target, 
                                                          const GLdouble *v);
#endif


/*! The extensons class load, if avalaible, openGL extensions
 * functions used by DNT. */
class extensions
{
   public:
      /* Texture per points functions */
      static PFNGLPOINTPARAMETERFARBPROC arbPointParameterf;   
      static PFNGLPOINTPARAMETERFVARBPROC arbPointParameterfv;

      /* Multi Textures Functions */
      static PFNGLACTIVETEXTUREARBPROC arbActiveTexture;
      static PFNGLCLIENTACTIVETEXTUREARBPROC arbClientActiveTexture;
      static PFNGLMULTITEXCOORD2FARBPROC arbMultiTexCoord2f;
      static PFNGLMULTITEXCOORD2FVARBPROC arbMultiTexCoord2fv;
      static PFNGLMULTITEXCOORD2DARBPROC arbMultiTexCoord2d;
      static PFNGLMULTITEXCOORD2DVARBPROC arbMultiTexCoord2dv;

      /* Render Frame Buffer Object Functions */
      static PFNGLGENFRAMEBUFFERSEXTPROC extGenFramebuffers;
      static PFNGLBINDFRAMEBUFFEREXTPROC extBindFramebuffer;
      static PFNGLFRAMEBUFFERTEXTURE2DEXTPROC extFramebufferTexture2D;
      static PFNGLDELETEFRAMEBUFFERSEXTPROC extDeleteFramebuffers;
      static PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC extCheckFramebufferStatus;
      static PFNGLDELETERENDERBUFFERSEXTPROC extDeleteRenderbuffers;
      static PFNGLGENRENDERBUFFERSEXTPROC extGenRenderbuffers;
      static PFNGLBINDRENDERBUFFEREXTPROC extBindRenderbuffer;
      static PFNGLRENDERBUFFERSTORAGEEXTPROC extRenderbufferStorage;
      static PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC extFramebufferRenderbuffer;

      /* Shader functions */
      static PFNGLCREATESHADEROBJECTARBPROC arbCreateShaderObject;
      static PFNGLCREATEPROGRAMOBJECTARBPROC arbCreateProgramObject;
      static PFNGLDELETEOBJECTARBPROC arbDeleteObject;
      static PFNGLSHADERSOURCEARBPROC arbShaderSourceObject;
      static PFNGLCOMPILESHADERARBPROC arbCompileShader;
      static PFNGLATTACHOBJECTARBPROC arbAttachObject;
      static PFNGLLINKPROGRAMARBPROC arbLinkProgram;
      static PFNGLUSEPROGRAMOBJECTARBPROC arbUseProgram;
      static PFNGLGETHANDLEARBPROC arbGetHandle;
      static PFNGLGETOBJECTPARAMETERIVARBPROC arbGetObjectParameteriv;
      //static PFNGLGETSHADERIVPROC getShaderiv;
      static PFNGLGETINFOLOGARBPROC arbGetInfoLog;
      //static PFNGLGETPROGRAMINFOLOGPROC getProgramInfoLog;
      //static PFNGLGETPROGRAMIVARBPROC arbGetProgramiv;
      static PFNGLGETUNIFORMLOCATIONARBPROC arbGetUniformLocation;
      static PFNGLGETOBJECTPARAMETERFVARBPROC arbGetObjectParameterfv;
      static PFNGLGETOBJECTPARAMETERIVARBPROC arbGetObjectParamenteriv;
      static PFNGLUNIFORM1FARBPROC arbUniform1f;
      static PFNGLUNIFORM2FARBPROC arbUniform2f;
      static PFNGLUNIFORM3FARBPROC arbUniform3f;
      static PFNGLUNIFORM1IARBPROC arbUniform1i;
      static PFNGLUNIFORM2IARBPROC arbUniform2i;
      static PFNGLUNIFORM3IARBPROC arbUniform3i;

      /*! Gets all Extended Functions that DNT uses. Usually call this after
       * the init of the Engine. */
      void defineAllExtensions();

      /*! Verify if the machine have the point texture extension
       * \return true if the extension is available */
      bool hasPointTexture();
      /*! Verify if the machine have the multi texture extension
       * \return true if the extension is available */
      bool hasMultiTexture();
      /*! Verify if the machine have the frame buffer extension
       * \return -> true if the extension is available */
      bool hasFrameBuffer();
      /*! Verify if the machine have the shader extension
       * \return true if the extension is available */
      bool hasShader();

      /*! Verify if the machine have anisotropic extension avalaible
       * \return true if avalaible */
      bool hasAnisotropic();

   protected:
      /*! Define Point per Texture functions
       * \param ext -> extensions string */
      void definePointTexture(std::string ext);
      /*! Define MultiTexture functions
       * \param ext -> extensions string */
      void defineMultiTexture(std::string ext);
      /*! Define Frame Buffer Object functions
       * \param ext -> extensions string */
      void defineFrameBuffer(std::string ext);
      /*! Define shader functions
       * \param ext -> extensions string */
      void defineShader(std::string ext);
      /*! Define anisotropic
       * \param ext -> extensions string */
      void defineAnisotropic(std::string ext);

      /*! Get OpenGL Function
       * \param functionName -> function to get
       * \return -> pointer to the function found or NULL */
      void* getFunction(std::string functionName);

      /*! Sow an warning about not found some extension function 
       * \param functionName -> string wit function name to displa warning of*/
      void showWarning(std::string functionName);

      static bool anisotropic;   /**< True when anisotropic is avalaible */

};

#endif

