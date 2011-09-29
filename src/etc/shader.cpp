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

#include "shader.h"
#include "dirs.h"
#include <fstream>
using namespace std;

/***********************************************************************
 *                           Constructor                               *
 ***********************************************************************/
shader::shader()
{
   program = 0;
   loaded = false;
}

/***********************************************************************
 *                           Destructor                                *
 ***********************************************************************/
shader::~shader()
{
   if(loaded)
   {
      clear();
   }
}

/***********************************************************************
 *                       printShaderInfoLog                            *
 ***********************************************************************/
void shader::printInfoLog(GLuint obj, bool prog)
{
   GLchar infoLog[1024];

   if(ext.hasShader())
   {
      /*if(prog)
      {
         ext.getProgramInfoLog(obj, 1024, NULL, infoLog);
         printOpenGLErrors("getProgramInfoLog");
      }
      else*/
      {
         ext.arbGetInfoLog(obj, 1024, NULL, infoLog); 
         printOpenGLErrors("arbGetInfoLog");
      }

      cout << "infoLog: " << infoLog << endl;
   }
}

/***********************************************************************
 *                              clear                                  *
 ***********************************************************************/
void shader::clear()
{
   if(ext.hasShader())
   {
      /* Cleanning Shaders */
      ext.arbDeleteObject(vertex);
      ext.arbDeleteObject(fragment);

      /* Cleanning Program */
      ext.arbDeleteObject(program);
   }
}

/***********************************************************************
 *                              enable                                 *
 ***********************************************************************/
void shader::enable()
{
   if((ext.hasShader()) && (loaded))
   {
      ext.arbUseProgram(program);
   }
}

/***********************************************************************
 *                              disable                                *
 ***********************************************************************/
void shader::disable()
{
   if(ext.hasShader())
   {
      ext.arbUseProgram(0);
   }
}

/***********************************************************************
 *                               load                                  *
 ***********************************************************************/
bool shader::load(string vShaderFileName, string fShaderFileName)
{
   GLint vertexCompiled, fragmentCompiled, linked;
   char buf[524288];
   string res;
   const char* str;

   if(ext.hasShader())
   {
      /* Create Shader Program */
      program = ext.arbCreateProgramObject();

      /* Create vertex and fragment shaders */
      vertex = ext.arbCreateShaderObject(GL_VERTEX_SHADER_ARB);
      fragment = ext.arbCreateShaderObject(GL_FRAGMENT_SHADER_ARB);

      /* Load Vertex and fragment shaders */
      res = parseFile(vShaderFileName);
      sprintf(buf,"%s", res.c_str());
      str = &res[0];
      ext.arbShaderSourceObject(vertex, 1, &str, NULL);
      res = parseFile(fShaderFileName).c_str();
      sprintf(buf,"%s", res.c_str());
      str = &res[0];
      ext.arbShaderSourceObject(fragment, 1, &str, NULL);

      /* Compile Vertex Shader */
      ext.arbCompileShader(vertex);
      printOpenGLErrors("arbCompileShader(vertex)");
      ext.arbGetObjectParameteriv(vertex, GL_OBJECT_COMPILE_STATUS_ARB, 
                                  &vertexCompiled);
      printInfoLog(vertex);
      if(!vertexCompiled)
      {
         cout << "ERROR: Can't compile vertex shader: " 
              << vShaderFileName << endl;
         return(false);
      }

      /* Compile Fragment shader */
      ext.arbCompileShader(fragment);
      printOpenGLErrors("arbCompileShader(fragment)");
      ext.arbGetObjectParameteriv(fragment, GL_OBJECT_COMPILE_STATUS_ARB, 
                                  &fragmentCompiled);
      printInfoLog(fragment);
      if(!fragmentCompiled)
      {
         cout << "ERROR: Can't compile fragment shader: " 
            << fShaderFileName << endl;
         return(false);
      }

      /* Attach shaders to the program */
      ext.arbAttachObject(program, fragment);
      ext.arbAttachObject(program, vertex);

      /* Finnaly, link them */
      ext.arbLinkProgram(program);
      printOpenGLErrors("arbLinkProgram");
      ext.arbGetObjectParameteriv(program, GL_OBJECT_LINK_STATUS_ARB, &linked);
      printOpenGLErrors("arbGetProgramiv");
      printInfoLog(program, true);
      if(!linked)
      {
         cerr << "ERROR: Can't link shaders: " << vShaderFileName 
            << " , " << fShaderFileName << endl;
         return(false);
      }

      loaded = true;

      return(true);

   }

   return(false);

}

/***********************************************************************
 *                           setUniformVariable                        *
 ***********************************************************************/
void shader::setUniformVariable(string variableName, 
                                GLfloat x, GLfloat y, GLfloat z)
{
   if(ext.hasShader())
   {
      ext.arbUniform3f(getUniformVariable(variableName), x, y, z);
   }
}

/***********************************************************************
 *                           setUniformVariable                        *
 ***********************************************************************/
void shader::setUniformVariable(string variableName, GLfloat x, GLfloat y)
{
   if(ext.hasShader())
   {
      ext.arbUniform2f(getUniformVariable(variableName), x, y);
   }
}

/***********************************************************************
 *                           setUniformVariable                        *
 ***********************************************************************/
void shader::setUniformVariable(string variableName, GLfloat x)
{
   if(ext.hasShader())
   {
      ext.arbUniform1f(getUniformVariable(variableName), x);
   }
}

/***********************************************************************
 *                           getUniformVariable                        *
 ***********************************************************************/
GLint shader::getUniformVariable(string variableName)
{
   if(ext.hasShader())
   {
      GLint loc;

      loc = ext.arbGetUniformLocation(program, variableName.c_str());

      if(loc == -1)
      {
         cerr << "ERROR: Can't find shader uniform variable: "
            << variableName << endl;
      }

      return(loc);
   }

   return(-1);
}

/***********************************************************************
 *                           setUniformVariable                        *
 ***********************************************************************/
void shader::setUniformVariable(string variableName, 
                                GLint x, GLint y, GLint z)
{
   if(ext.hasShader())
   {
      ext.arbUniform3i(getUniformVariable(variableName), x, y, z);
   }
}

/***********************************************************************
 *                           setUniformVariable                        *
 ***********************************************************************/
void shader::setUniformVariable(string variableName, GLint x, GLint y)
{
   if(ext.hasShader())
   {
      ext.arbUniform2i(getUniformVariable(variableName), x, y);
   }
}

/***********************************************************************
 *                           setUniformVariable                        *
 ***********************************************************************/
void shader::setUniformVariable(string variableName, GLint x)
{
   if(ext.hasShader())
   {
      ext.arbUniform1i(getUniformVariable(variableName), x);
   }
}


/***********************************************************************
 *                           getUniformVariable                        *
 ***********************************************************************/
string shader::parseFile(string fileName)
{
   dirs dir;
   string res = "";
   ifstream file;
   string buffer;

   /* Open File */
   file.open(dir.getRealFile(fileName).c_str(), ios::in | ios::binary);
   if(!file)
   {
      cerr << "Error opening file: " << fileName << endl;
      return("");
   }

   /* Get all file */
   while(!file.eof())
   {
      getline(file, buffer);
      if(!file.eof())
      {
         res += buffer + "\n";
      }
   }

   /* Close it, before return */
   file.close();

   return(res);
}

