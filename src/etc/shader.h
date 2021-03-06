/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_shader_h
#define _dnt_shader_h

#include <SDL/SDL_opengl.h>
#include <string>
#include <iostream>

#include "extensions.h"

/*! An GLSL shader abstraction */
class shader
{
   public:
      /*! Constructor */
      shader();

      /*! Destructor */
      ~shader();

      /*! Load Shader
       * \param vShaderFileName -> vertex shader file name to load
       * \param fShaderFileName -> fragment shader file name to load 
       * \return -> true if successfull */
      bool load(std::string vShaderFileName, std::string fShaderFileName);

      /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value 
       * \param y -> y value
       * \param z -> z value */
      void setUniformVariable(std::string variableName, 
                              GLfloat x, GLfloat y, GLfloat z);
       /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value 
       * \param y -> y value */
      void setUniformVariable(std::string variableName, GLfloat x, GLfloat y);
       /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value */
      void setUniformVariable(std::string variableName, GLfloat x);

      /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value 
       * \param y -> y value
       * \param z -> z value */
      void setUniformVariable(std::string variableName, 
                              GLint x, GLint y, GLint z);
       /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value 
       * \param y -> y value */
      void setUniformVariable(std::string variableName, GLint x, GLint y);
       /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value */
      void setUniformVariable(std::string variableName, GLint x);
 
      /*! Get an vertex-attrib GL constant from shader */
      GLint getAttrib(std::string s);
      /*! Set vertex atrib pointer */
      void setAttrib(GLint att, GLint size, GLenum type,
            GLboolean normalized, GLsizei stride, const GLvoid* ptr);
      /*! Enable use of vertex attrib */
      void enableAttrib(GLuint att);
      /*! Disable use of vertex attrib */
      void disableAttrib(GLuint att);

      /*! Enable the shader */
      void enable();

      /*! Disable the shader */
      void disable();

      /*! Verify if the shader is enabled or not */
      bool isEnabled(){return(enabled);};
   
   protected:

      /*! Clear all created shader structures */
      void clear();

      /*! Parse the file, before send it to shader
       * \param fileName -> file to parse
       * \return -> string with file contents */
      std::string parseFile(std::string fileName);

      /*! Get uniform variable identifier
       * \param variableName -> uniform variable name */
      GLint getUniformVariable(std::string variableName);

      /*! Print the info log to the stdout
       * \param obj -> object to print info log from
       * \param prog -> if true obj is a program, if false is shader */
      void printInfoLog(GLuint obj, bool prog=false);

      GLuint vertex;    /**< The vertex shader Identifier */
      GLuint fragment;  /**< The fragment shader Identifier */
      GLuint program;   /**< The program Identifier */

      extensions ext;   /**< The openGL extensions controller */

      std::string fileName; /**< shader file name */

      bool loaded;      /**< If the program was loaded or not */
      bool enabled;     /**< If the shader is enabled or not */
};

/*! The loaded shaders */
class shaders
{
   public:
      /*! init and load all shaders */
      void init();

      /*! finish the shaders use */
      void finish();

      /*! Get a shader to use
       * \param id -> shaderId 
       * \return -> pointer to shader or NULL, if not defined */
      shader* getShader(int id);

      enum shaderId
      {
         SHADER_TEXTURE_SPLAT=0,
         SHADER_BUMP_MAPPING,
         TOTAL_SHADERS
      };

   protected:
      static shader* mShaders[TOTAL_SHADERS]; /**< All shaders */
};

#endif

