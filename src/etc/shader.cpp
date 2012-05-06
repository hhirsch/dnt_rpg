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
   enabled = false;
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
         ext.arbGetInfoLog((GLhandleARB)obj, 1024, NULL, infoLog); 
         printOpenGLErrors("arbGetInfoLog");
      }

      std::string log = infoLog;
      if(!log.empty())
      {
         cout << "infoLog ('" << fileName << "')" << ": " << infoLog << endl;
      }
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
      ext.arbDeleteObject((GLhandleARB)vertex);
      ext.arbDeleteObject((GLhandleARB)fragment);

      /* Cleanning Program */
      ext.arbDeleteObject((GLhandleARB)program);
   }
}

/***********************************************************************
 *                              enable                                 *
 ***********************************************************************/
void shader::enable()
{
   if((ext.hasShader()) && (loaded))
   {
      ext.arbUseProgram((GLhandleARB)program);
      enabled = true;
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
      enabled = false;
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

   fileName = fShaderFileName.substr(0, vShaderFileName.length()-5);

   if(ext.hasShader())
   {
      /* Create Shader Program */
      program = (GLuint)(long)ext.arbCreateProgramObject();

      /* Create vertex and fragment shaders */
      vertex = (GLuint)(long)ext.arbCreateShaderObject(GL_VERTEX_SHADER_ARB);
      fragment = (GLuint)(long)ext.arbCreateShaderObject(GL_FRAGMENT_SHADER_ARB);

      /* Load Vertex and fragment shaders */
      res = parseFile(vShaderFileName);
      sprintf(buf,"%s", res.c_str());
      str = &res[0];
      ext.arbShaderSourceObject((GLhandleARB)vertex, 1, &str, NULL);
      res = parseFile(fShaderFileName).c_str();
      sprintf(buf,"%s", res.c_str());
      str = &res[0];
      ext.arbShaderSourceObject((GLhandleARB)fragment, 1, &str, NULL);

      /* Compile Vertex Shader */
      ext.arbCompileShader((GLhandleARB)vertex);
      printOpenGLErrors("arbCompileShader(vertex)");
      ext.arbGetObjectParameteriv((GLhandleARB)vertex, 
            GL_OBJECT_COMPILE_STATUS_ARB, &vertexCompiled);
      printInfoLog(vertex);
      if(!vertexCompiled)
      {
         cout << "ERROR: Can't compile vertex shader: " 
              << vShaderFileName << endl;
         return(false);
      }

      /* Compile Fragment shader */
      ext.arbCompileShader((GLhandleARB)fragment);
      printOpenGLErrors("arbCompileShader(fragment)");
      ext.arbGetObjectParameteriv((GLhandleARB)fragment, 
            GL_OBJECT_COMPILE_STATUS_ARB, &fragmentCompiled);
      printInfoLog(fragment);
      if(!fragmentCompiled)
      {
         cout << "ERROR: Can't compile fragment shader: " 
            << fShaderFileName << endl;
         return(false);
      }

      /* Attach shaders to the program */
      ext.arbAttachObject((GLhandleARB)program, (GLhandleARB)fragment);
      ext.arbAttachObject((GLhandleARB)program, (GLhandleARB)vertex);

      /* Finnaly, link them */
      ext.arbLinkProgram((GLhandleARB)program);
      printOpenGLErrors("arbLinkProgram");
      ext.arbGetObjectParameteriv((GLhandleARB)program, 
            GL_OBJECT_LINK_STATUS_ARB, &linked);
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

      loc = ext.arbGetUniformLocation((GLhandleARB)program, 
            variableName.c_str());

      if(loc == -1)
      {
         cerr << "ERROR: Can't find shader (" << fileName 
              << ") uniform variable: " << variableName << endl;
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
 *                                getAttrib                            *
 ***********************************************************************/
GLint shader::getAttrib(std::string s)
{
   GLint res;
   if(ext.hasShader())
   {
      res = ext.arbGetAttribLocation((GLhandleARB)program, s.c_str());
      if(res == -1)
      {
         cerr << "ERROR: Couldn't find shader (" << fileName 
              << ") vertex attribute: " << s << endl;
      }
      return(res);
   }
   return(-1);
}

/***********************************************************************
 *                                getAttrib                            *
 ***********************************************************************/
void shader::setAttrib(GLint att, GLint size, GLenum type,
      GLboolean normalized, GLsizei stride, const GLvoid* ptr)
{
   if(ext.hasShader())
   {
      ext.arbVertexAttribPointer(att, size, type, normalized, stride, ptr);
   }
}

/***********************************************************************
 *                              enableAttrib                           *
 ***********************************************************************/
void shader::enableAttrib(GLuint att)
{
   if(ext.hasShader())
   {
      ext.arbEnableVertexAttribArray(att);
   }
}

/***********************************************************************
 *                             disableAttrib                           *
 ***********************************************************************/
void shader::disableAttrib(GLuint att)
{
   if(ext.hasShader())
   {
      ext.arbDisableVertexAttribArray(att);
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

/***********************************************************************
 *                               init                                  *
 ***********************************************************************/
void shaders::init()
{
   extensions ext;
   if(ext.hasShader())
   {
      /* Load Texture Splatting shader */
      mShaders[SHADER_TEXTURE_SPLAT] = new shader();
      mShaders[SHADER_TEXTURE_SPLAT]->load("shaders/terrain_splat.vert",
            "shaders/terrain_splat.frag");
      /* Load Normal Map Shader */
      mShaders[SHADER_BUMP_MAPPING] = new shader();
      mShaders[SHADER_BUMP_MAPPING]->load("shaders/bump_mapping.vert",
            "shaders/bump_mapping.frag");
   }
}

/***********************************************************************
 *                                finish                               *
 ***********************************************************************/
void shaders::finish()
{
   int i;
   extensions ext;
   if(ext.hasShader())
   {
      for(i=0; i < TOTAL_SHADERS; i++)
      {
         delete(mShaders[i]);
      }
   }
}

/***********************************************************************
 *                                getShader                            *
 ***********************************************************************/
shader* shaders::getShader(int id)
{
   if((id >= 0) && (id < TOTAL_SHADERS))
   {
      return(mShaders[id]);
   }
   return(NULL);
}


/* The shaders */
shader* shaders::mShaders[shaders::TOTAL_SHADERS];


