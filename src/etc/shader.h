
/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#ifndef _dnt_shader_h
#define _dnt_shader_h

#include <SDL/SDL_opengl.h>
#include <string>
#include <iostream>
using namespace std;

#include "extensions.h"

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
      bool load(string vShaderFileName, string fShaderFileName);

      /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value 
       * \param y -> y value
       * \param z -> z value */
      void setUniformVariable(string variableName, 
                              GLfloat x, GLfloat y, GLfloat z);
       /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value 
       * \param y -> y value */
      void setUniformVariable(string variableName, GLfloat x, GLfloat y);
       /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value */
      void setUniformVariable(string variableName, GLfloat x);

      /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value 
       * \param y -> y value
       * \param z -> z value */
      void setUniformVariable(string variableName, 
                              GLint x, GLint y, GLint z);
       /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value 
       * \param y -> y value */
      void setUniformVariable(string variableName, GLint x, GLint y);
       /*! Set the uniform variable value 
       * \param variableName -> the uniform variable name
       * \param x -> x value */
      void setUniformVariable(string variableName, GLint x);

      /*! Enable the shader */
      void enable();

      /*! Disable the shader */
      void disable();
   
   protected:

      /*! Clear all created shader structures */
      void clear();

      /*! Parse the file, before send it to shader
       * \param fileName -> file to parse
       * \return -> string with file contents */
      string parseFile(string fileName);

      /*! Get uniform variable identifier
       * \param variableName -> uniform variable name */
      GLint getUniformVariable(string variableName);

      /*! Print the info log to the stdout
       * \param obj -> object to print info log from
       * \param prog -> if true obj is a program, if false is shader */
      void printInfoLog(GLuint obj, bool prog=false);

      GLuint vertex;    /**< The vertex shader Identifier */
      GLuint fragment;  /**< The fragment shader Identifier */
      GLuint program;   /**< The program Identifier */

      extensions ext;   /**< The openGL extensions controller */

      bool loaded;      /**< If the program was loaded or not */
};

#endif

