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

#include <iostream>
#include <string>
using namespace std;
#include "extensions.h"

/***********************************************************************
 *                         printOpenGLErrors                           *
 ***********************************************************************/
void printOpenGLErrors(string where)
{
   GLenum errorCode;
   while( (errorCode = glGetError()) != GL_NO_ERROR)
   {
      cerr << "OpenGL Error: " << gluErrorString(errorCode);
      if(!where.empty())
      {
         cerr << " at " << where;
      }
      cerr << endl;
   }
}

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
 
   /* Frame Buffer Objects */
   defineFrameBuffer(ext);

   /* Shaders */
   defineShader(ext);

   /* Anisotropic */
   defineAnisotropic(ext);
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
 *                              getFunction                            *
 ***********************************************************************/
void* extensions::getFunction(string functionName)
{
   void* func = SDL_GL_GetProcAddress(functionName.c_str());
   if(!func)
   {
      showWarning(functionName);
   }

   return(func);
}

/***********************************************************************
 *                          definePointTexture                         *
 ***********************************************************************/
void extensions::definePointTexture(string ext)
{
   if( ext.find("GL_ARB_point_sprite",0) != string::npos)
   {
      arbPointParameterf = (PFNGLPOINTPARAMETERFARBPROC)
                                            getFunction("glPointParameterfARB");
      arbPointParameterfv = (PFNGLPOINTPARAMETERFVARBPROC) 
                                           getFunction("glPointParameterfvARB");
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
      arbActiveTexture = (PFNGLACTIVETEXTUREARBPROC) 
                                              getFunction("glActiveTextureARB");
      arbClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREARBPROC) 
                                        getFunction("glClientActiveTextureARB");
      arbMultiTexCoord2d = (PFNGLMULTITEXCOORD2DARBPROC)
                                            getFunction("glMultiTexCoord2dARB");
      arbMultiTexCoord2f = (PFNGLMULTITEXCOORD2FARBPROC)
                                            getFunction("glMultiTexCoord2fARB");
      arbMultiTexCoord2dv = (PFNGLMULTITEXCOORD2DVARBPROC)
                                           getFunction("glMultiTexCoord2dvARB");
      arbMultiTexCoord2fv = (PFNGLMULTITEXCOORD2FVARBPROC)
                                           getFunction("glMultiTexCoord2fvARB");
   }
   else
   {
      showWarning("GL_ARB_multitexture extension");
   }
}

/***********************************************************************
 *                          defineFrameBuffer                          *
 ***********************************************************************/
void extensions::defineFrameBuffer(string ext)
{
   if(ext.find("GL_EXT_framebuffer_object") != string::npos)
   {
      extGenFramebuffers = (PFNGLGENFRAMEBUFFERSEXTPROC)
                                            getFunction("glGenFramebuffersEXT");
      extBindFramebuffer = (PFNGLBINDFRAMEBUFFEREXTPROC)
                                            getFunction("glBindFramebufferEXT");
      extFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)
                                       getFunction("glFramebufferTexture2DEXT");
      extDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSEXTPROC)
                                         getFunction("glDeleteFramebuffersEXT");
      extCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)
                                     getFunction("glCheckFramebufferStatusEXT");
   
      extDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSEXTPROC)
                                        getFunction("glDeleteRenderbuffersEXT");
      extGenRenderbuffers = (PFNGLGENRENDERBUFFERSEXTPROC)
                                           getFunction("glGenRenderbuffersEXT");
      extBindRenderbuffer = (PFNGLBINDRENDERBUFFEREXTPROC)
                                           getFunction("glBindRenderbufferEXT");
      extRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEEXTPROC)
                                        getFunction("glRenderbufferStorageEXT");
      extFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)
                                    getFunction("glFramebufferRenderbufferEXT");
   }
   else
   {
      showWarning("GL_EXT_framebuffer_object");
   }

}

/***********************************************************************
 *                            defineShader                             *
 ***********************************************************************/
void extensions::defineShader(string ext)
{
   if( (ext.find("GL_ARB_fragment_shader") != string::npos) &&
       (ext.find("GL_ARB_vertex_shader") != string::npos) )
   {
      arbCreateShaderObject = (PFNGLCREATESHADEROBJECTARBPROC)
                                         getFunction("glCreateShaderObjectARB");
      arbCreateProgramObject = (PFNGLCREATEPROGRAMOBJECTARBPROC)
                                        getFunction("glCreateProgramObjectARB");
      arbDeleteObject = (PFNGLDELETEOBJECTARBPROC)
                                               getFunction("glDeleteObjectARB");
      arbShaderSourceObject = (PFNGLSHADERSOURCEARBPROC)
                                               getFunction("glShaderSourceARB");
      arbCompileShader = (PFNGLCOMPILESHADERARBPROC)
                                              getFunction("glCompileShaderARB");
      arbAttachObject = (PFNGLATTACHOBJECTARBPROC)
                                               getFunction("glAttachObjectARB");
      arbLinkProgram = (PFNGLLINKPROGRAMARBPROC)
                                                getFunction("glLinkProgramARB");
      arbUseProgram = (PFNGLUSEPROGRAMOBJECTARBPROC)
                                           getFunction("glUseProgramObjectARB");
      arbGetObjectParameteriv = (PFNGLGETOBJECTPARAMETERIVARBPROC)
                                       getFunction("glGetObjectParameterivARB");
      arbGetInfoLog = (PFNGLGETINFOLOGARBPROC)getFunction("glGetInfoLogARB");
      arbGetHandle = (PFNGLGETHANDLEARBPROC)getFunction("glGetHandleARB");
      arbGetUniformLocation = (PFNGLGETUNIFORMLOCATIONARBPROC)
                                         getFunction("glGetUniformLocationARB");
      arbGetObjectParameterfv = (PFNGLGETOBJECTPARAMETERFVARBPROC)
                                       getFunction("glGetObjectParameterfvARB");
      arbGetObjectParamenteriv = (PFNGLGETOBJECTPARAMETERIVARBPROC)
                                       getFunction("glGetObjectParameterivARB");
      arbUniform1f = (PFNGLUNIFORM1FARBPROC)getFunction("glUniform1fARB");
      arbUniform2f = (PFNGLUNIFORM2FARBPROC)getFunction("glUniform2fARB");
      arbUniform3f = (PFNGLUNIFORM3FARBPROC)getFunction("glUniform3fARB");
      arbUniform1i = (PFNGLUNIFORM1IARBPROC)getFunction("glUniform1iARB");
      arbUniform2i = (PFNGLUNIFORM2IARBPROC)getFunction("glUniform2iARB");
      arbUniform3i = (PFNGLUNIFORM3IARBPROC)getFunction("glUniform3iARB");
      arbVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERARBPROC)
         getFunction("glVertexAttribPointerARB");
      arbGetAttribLocation = (PFNGLGETATTRIBLOCATIONARBPROC)
         getFunction("glGetAttribLocationARB");
      arbEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)
         getFunction("glEnableVertexAttribArrayARB");
      arbDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)
         getFunction("glDisableVertexAttribArrayARB");
   }
   else
   {
      showWarning("GL_ARB_*_shader extension");
   }
}

/***********************************************************************
 *                          defineAnisotropic                          *
 ***********************************************************************/
void extensions::defineAnisotropic(string ext)
{
   if(ext.find("GL_EXT_texture_filter_anisotropic") != string::npos)
   {
      anisotropic = true;
   }
   else
   {
      showWarning("GL_EXT_texture_filter_anisotropic");
   }
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
 *                           hastFrameBuffer                           *
 ***********************************************************************/
bool extensions::hasFrameBuffer()
{
   return( (extGenFramebuffers != NULL) &&
           (extBindFramebuffer != NULL) &&
           (extFramebufferTexture2D != NULL) &&
           (extDeleteFramebuffers != NULL) && 
           (extCheckFramebufferStatus != NULL) &&
           (extDeleteRenderbuffers != NULL) &&
           (extGenRenderbuffers != NULL) &&
           (extBindRenderbuffer != NULL) &&
           (extRenderbufferStorage != NULL) &&
           (extFramebufferRenderbuffer != NULL) );
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
           (arbGetHandle != NULL) &&
           (arbGetObjectParamenteriv != NULL) &&
           (arbGetUniformLocation != NULL) &&
           (arbGetObjectParameterfv != NULL) &&
           (arbGetObjectParamenteriv != NULL) &&
           (arbUniform1f != NULL) &&
           (arbUniform2f != NULL) &&
           (arbUniform3f != NULL) &&
           (arbUniform1i != NULL) &&
           (arbUniform2i != NULL) &&
           (arbUniform3i != NULL) );
}

/***********************************************************************
 *                           hasAnisotropic                            *
 ***********************************************************************/
bool extensions::hasAnisotropic()
{
   return(anisotropic);
}

/***********************************************************************
 *                            Static Members                           *
 ***********************************************************************/
/* Texture per points functions */
PFNGLPOINTPARAMETERFARBPROC extensions::arbPointParameterf = NULL;   
PFNGLPOINTPARAMETERFVARBPROC extensions::arbPointParameterfv = NULL;

/* Multi Textures Functions */
PFNGLACTIVETEXTUREARBPROC extensions::arbActiveTexture = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC extensions::arbClientActiveTexture = NULL;
PFNGLMULTITEXCOORD2FARBPROC extensions::arbMultiTexCoord2f = NULL;
PFNGLMULTITEXCOORD2FVARBPROC extensions::arbMultiTexCoord2fv = NULL;
PFNGLMULTITEXCOORD2DARBPROC extensions::arbMultiTexCoord2d = NULL;
PFNGLMULTITEXCOORD2DVARBPROC extensions::arbMultiTexCoord2dv = NULL;

/* Render Frame Buffer Object Functions */
PFNGLGENFRAMEBUFFERSEXTPROC extensions::extGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC extensions::extBindFramebuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC extensions::extFramebufferTexture2D = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC extensions::extDeleteFramebuffers = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC extensions::extCheckFramebufferStatus = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC extensions::extDeleteRenderbuffers = NULL;
PFNGLGENRENDERBUFFERSEXTPROC extensions::extGenRenderbuffers = NULL;
PFNGLBINDRENDERBUFFEREXTPROC extensions::extBindRenderbuffer = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC extensions::extRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC extensions::extFramebufferRenderbuffer=NULL;


/* Shader Functions */
PFNGLCREATESHADEROBJECTARBPROC extensions::arbCreateShaderObject = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC extensions::arbCreateProgramObject = NULL;
PFNGLDELETEOBJECTARBPROC extensions::arbDeleteObject = NULL;
PFNGLSHADERSOURCEARBPROC extensions::arbShaderSourceObject = NULL;
PFNGLCOMPILESHADERARBPROC extensions::arbCompileShader = NULL;
PFNGLATTACHOBJECTARBPROC extensions::arbAttachObject = NULL;
PFNGLLINKPROGRAMARBPROC extensions::arbLinkProgram = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC extensions::arbUseProgram = NULL;
PFNGLGETHANDLEARBPROC extensions::arbGetHandle = NULL;
//PFNGLGETSHADERIVPROC extensions::getShaderiv = NULL;
//PFNGLGETPROGRAMIVARBPROC extensions::arbGetProgramiv = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC extensions::arbGetObjectParameteriv = NULL;
PFNGLGETINFOLOGARBPROC extensions::arbGetInfoLog = NULL;
//PFNGLGETPROGRAMINFOLOGPROC extensions::getProgramInfoLog = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC extensions::arbGetUniformLocation = NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC extensions::arbGetObjectParameterfv = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC extensions::arbGetObjectParamenteriv = NULL;
PFNGLUNIFORM1FARBPROC extensions::arbUniform1f = NULL;
PFNGLUNIFORM2FARBPROC extensions::arbUniform2f = NULL;
PFNGLUNIFORM3FARBPROC extensions::arbUniform3f = NULL;
PFNGLUNIFORM1IARBPROC extensions::arbUniform1i = NULL;
PFNGLUNIFORM2IARBPROC extensions::arbUniform2i = NULL;
PFNGLUNIFORM3IARBPROC extensions::arbUniform3i = NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC extensions::arbVertexAttribPointer = NULL;
PFNGLGETATTRIBLOCATIONARBPROC extensions::arbGetAttribLocation = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC extensions::arbEnableVertexAttribArray=NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC 
   extensions::arbDisableVertexAttribArray = NULL;

bool extensions::anisotropic = false;

