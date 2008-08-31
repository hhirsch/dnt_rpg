
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

      GLuint vertex;    /**< The vertex shader Identifier */
      GLuint fragment;  /**< The fragment shader Identifier */
      GLuint program;   /**< The program Identifier */

      extensions ext;   /**< The openGL extensions controller */

      bool loaded;      /**< If the program was loaded or not */
};

#endif

