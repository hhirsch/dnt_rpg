/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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

#include "animodel.h"
#include "defparser.h"
#include "../engine/util.h"
#include "dirs.h"
#include "overlaps.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#ifdef __APPLE__
   #include <SDL_image/SDL_image.h>
#else
   #include <SDL/SDL_image.h>
#endif
using namespace std;

/**********************************************************************
 *                            Constructor                             *
 **********************************************************************/
aniModel::aniModel(int modelType)
{
   fileName = "";
   dY = 0.0f;
   faceCount = 0;
   faces = NULL;
   renderScale = 1.0f;
   curState = STATE_NONE;
   type = modelType;
   loadedTexture = false;
}

/**********************************************************************
 *                             Destructor                             *
 **********************************************************************/
aniModel::~aniModel()
{
}

/*********************************************************************
 *                            loadTexture                            *
 *********************************************************************/
GLuint aniModel::loadTexture(const string& strFilename)
{
   dirs dir;
   GLuint pId = 0;
 
   /* Load texture image from file */
   SDL_Surface* img = IMG_Load(dir.getRealFile(strFilename).c_str());
   if(!img)
   {
      cout << "Can't Open Texture" << strFilename << endl; 
      return(0);
   }

   GLint format = GL_RGB;
   GLint dFormat = DNT_IMAGE_FORMAT;

   if(img->format->BytesPerPixel == 4)
   {
      format = GL_RGBA;
      dFormat = DNT_IMAGE_FORMAT_A;
   }

   /* Show Warning if not power of 2 */
   if( (img->h != Farso::smallestPowerOfTwo(img->h)) ||
       (img->w != Farso::smallestPowerOfTwo(img->w)) )
   {
      cout << "Warning: image '" << strFilename 
           << "' is of non-power of two dimension '" 
           << img->w << "x" << img->h << "'" << endl;
   }

   
   glGenTextures(1, &pId);
   glBindTexture(GL_TEXTURE_2D, pId);
   glTexImage2D(GL_TEXTURE_2D,0,format,img->w,img->h, 
                0, dFormat, GL_UNSIGNED_BYTE, img->pixels);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                   GL_LINEAR_MIPMAP_LINEAR );
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   gluBuild2DMipmaps(GL_TEXTURE_2D, format, img->w,
                     img->h, dFormat, GL_UNSIGNED_BYTE, 
                     img->pixels);

   /* Free memory */
   SDL_FreeSurface(img);

   loadedTexture = true;

   return(pId);
}

/*********************************************************************
 *                          calculateDeltaY                          *
 *********************************************************************/
void aniModel::calculateDeltaY()
{
   int vertsCount = 0;
   vector3f_t* verts = getMeshVertices(0, vertsCount);
   int i;
   float mY = 5000;

   for(i=0; i < vertsCount; i++)
   {
      /* Verify minimum Y (2, as at blender coordinate system is Z) */
      if(verts[i][2] < mY)
      {
         mY = verts[i][2];
      }
   }

   /* Define the delta Y when model is bellow ground */
   if(mY < 0)
   {
      /* Delta to sum to Y to be above ground! */
      dY = -mY*renderScale;
   }
}

/*********************************************************************
 *                       getCrudeBoundingBox                         *
 *********************************************************************/
boundingBox aniModel::getCrudeBoundingBox()
{
   return(crudeBox);
}

/*********************************************************************
 *                        loadToGraphicMemory                        *
 *********************************************************************/
void aniModel::loadToGraphicMemory(bool useTexture)
{
   int count=0;
   vector3f_t* vertices=NULL;
   vector3f_t* normals=NULL;
   vector2f_t* uvs=NULL;

   aniModelMaterial* mat;

   /* Get all arrays
    * FIXME: using only the first mesh! */
   vertices = getMeshVertices(0, count);
   normals = getMeshNormals(0, count);
   uvs = getMeshTexCoords(0, count);
   faces = getMeshFaces(0, faceCount);
   
   /* Keep previous enable, before do the needed here */
   glPushAttrib(GL_ENABLE_BIT);

   /* set the global OpenGL states */
   glShadeModel(GL_SMOOTH);
   glDisable(GL_COLOR_MATERIAL);

   /* we will use vertex arrays, so enable them */
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);

   /* Set Material */
   mat = getMeshMaterial(0);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &mat->ambient[0]);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &mat->diffuse[0]);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &mat->specular[0]);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &mat->shininess);;
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &mat->emission[0]);

   /* set the vertex and normal buffers */
   glVertexPointer(3, GL_FLOAT, 0, &vertices[0][0]);
   glNormalPointer(GL_FLOAT, 0, &normals[0][0]);

   shader* normalMap = dntShaders.getShader(shaders::SHADER_BUMP_MAPPING);
   
   /* Set texture, when necessary */
   if( (useTexture) && (loadedTexture) && 
       ( (!mat->normalMap) || (normalMap == NULL) ||
         (!opt.getEnableBumpMapping()) ))
   {
      glEnable(GL_TEXTURE_2D);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glBindTexture(GL_TEXTURE_2D, mat->textureId);
      glTexCoordPointer(2, GL_FLOAT, 0, &uvs[0][0]);
   }
   /* Set Normal-map shader, if needed */
   else if( (useTexture) && (mat->normalMap) )
   {
      normalMap->enable();

      /* Set tangent attribute */
      tangentAttrib = normalMap->getAttrib("tangent");
      //tangentAttrib = normalMap->getAttrib("vTangent");
      normalMap->enableAttrib(tangentAttrib);
      normalMap->setAttrib(tangentAttrib, 3, GL_FLOAT,GL_FALSE, 0,
            getMeshTangents(0, count));

      /* Set color texture */
      ext.arbActiveTexture(GL_TEXTURE0);
      ext.arbClientActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, mat->textureId);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, 0, &uvs[0][0]);
      //normalMap->setUniformVariable("colorMap", (GLint)0);
      normalMap->setUniformVariable("diffuseTexture", (GLint)0);

      /* Set normal-map texture */
      ext.arbActiveTexture(GL_TEXTURE1);
      ext.arbClientActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, mat->normalTexId);
      /*glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR );
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
      //normalMap->setUniformVariable("normalMap", (GLint)1);
      normalMap->setUniformVariable("normalTexture", (GLint)1);

   }
}

/*********************************************************************
 *                       renderFromGraphicMemory                     *
 *********************************************************************/
void aniModel::renderFromGraphicMemory(float pX, float pY, float pZ, 
      float angleX, float angleY, float angleZ, bool inverted)
{
   glPushMatrix();
      glTranslatef(pX,(!inverted)?(pY+dY):(-pY-dY),pZ);
      glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
      glRotatef(angleX, 1.0f, 0.0f, 0.0f);
      glRotatef(angleY, 0.0f, 1.0f, 0.0f);
      if(inverted)
      {
         glScalef(1.0f, -1.0f, 1.0f);
      }
      renderFromGraphicMemory();
   glPopMatrix();
}

/*********************************************************************
 *                       renderFromGraphicMemory                     *
 *********************************************************************/
void aniModel::renderFromGraphicMemory(float pX, float pY, float pZ, 
      float angleX, float angleY, float angleZ, 
      float angle, float aX, float aY, float aZ, bool inverted)
{
   glPushMatrix();
      glTranslatef(pX, (!inverted)?pY+dY:-pY-dY, pZ);
      glRotatef(angle, aX, aY, aZ);
      glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
      glRotatef(angleX, 1.0f, 0.0f, 0.0f);
      glRotatef(angleY, 0.0f, 1.0f, 0.0f);
      if(inverted)
      {
         glScalef(1.0f, -1.0f, 1.0f);
      }
      renderFromGraphicMemory();
   glPopMatrix();
}

/*********************************************************************
 *                       renderFromGraphicMemory                     *
 *********************************************************************/
void aniModel::renderFromGraphicMemory()
{
   glPushMatrix();
   /* Correct from blender to dnt coordinates */
   glRotatef(180,0,1,0);
   glRotatef(-90,1,0,0);
   /* Scale, if needed */
   if(renderScale != 1.0)
   {
      glScalef(renderScale, renderScale, renderScale);
   }

   glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, &faces[0][0]);

   glPopMatrix();

   glColor3f(1.0f, 1.0f, 1.0f);
}

/*********************************************************************
 *                       removeFromgraphicMemory                     *
 *********************************************************************/
void aniModel::removeFromGraphicMemory()
{
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisable(GL_TEXTURE_2D);
 
   glEnable(GL_COLOR_MATERIAL);

   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);

   aniModelMaterial* mat = getMeshMaterial(0);
   if( (mat->normalMap) && (opt.getEnableBumpMapping()) )
   {
      shader* normalMap = dntShaders.getShader(shaders::SHADER_BUMP_MAPPING);

      if(normalMap)
      {
         ext.arbActiveTexture(GL_TEXTURE1);
         ext.arbClientActiveTexture(GL_TEXTURE1);
         glDisable(GL_TEXTURE_2D);
         glDisableClientState(GL_TEXTURE_COORD_ARRAY);
         ext.arbActiveTexture(GL_TEXTURE0);
         ext.arbClientActiveTexture(GL_TEXTURE0);
         glDisable(GL_TEXTURE_2D);
         glDisableClientState(GL_TEXTURE_COORD_ARRAY);
         normalMap->disableAttrib(tangentAttrib);
         normalMap->disable();
      }
   }

   /* Back to previous enable bits */
   glPopAttrib();
}

/*********************************************************************
 *                             RenderShadow                          *
 *********************************************************************/
void aniModel::renderShadow(float pX, float pY, float pZ, float angleX,
            float angleY, float angleZ, GLfloat* shadowMatrix, float alpha)
{
   glPushAttrib(GL_ENABLE_BIT);

   /* Set Changes */
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_STENCIL_TEST);
   glStencilFunc(GL_EQUAL, 1, 0xffffffff);
   glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
   glPolygonOffset(-2.0f,-1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glDisable(GL_LIGHTING);
   glDisable(GL_FOG); /* << This will fix white shadows on some systems! */
   glDisable(GL_COLOR_MATERIAL);
   glColor4f(0.0f, 0.0f, 0.0f, alpha);
   shader* normalMap = dntShaders.getShader(shaders::SHADER_BUMP_MAPPING);
   if(normalMap->isEnabled())
   {
      normalMap->disable();
   }

   /* Render */
   glPushMatrix();
      glMultMatrixf(shadowMatrix);
      glPushMatrix();
         glTranslatef(pX, pY+dY, pZ);
         glRotatef(angleZ,0.0f,0.0f,1.0f);
         glRotatef(angleX,1.0f,0.0f,0.0f);
         glRotatef(angleY,0.0f,1.0f,0.0f);
         renderFromGraphicMemory();
     glPopMatrix();
   glPopMatrix();

   /* Unset Changes */
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
   glPopAttrib();
}

/*********************************************************************
 *                             RenderShadow                          *
 *********************************************************************/
void aniModel::renderShadow(float pX, float pY, float pZ, float angleX,
            float angleY, float angleZ, GLfloat* shadowMatrix, float alpha,
            float angle, float aX, float aY, float aZ)
{
   glPushAttrib(GL_ENABLE_BIT);
   /* Set Changes */
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_TEXTURE_2D);
   glEnable(GL_STENCIL_TEST);
   glStencilFunc(GL_EQUAL, 1, 0xffffffff);
   glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
   glPolygonOffset(-2.0f,-1.0f);
   glEnable(GL_POLYGON_OFFSET_FILL);
   glDisable(GL_LIGHTING);
   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_FOG);
   glColor4f(0.0f, 0.0f, 0.0f, alpha);
   shader* normalMap = dntShaders.getShader(shaders::SHADER_BUMP_MAPPING);
   if(normalMap->isEnabled())
   {
      normalMap->disable();
   }

   /* Render */
   glPushMatrix();
      glMultMatrixf(shadowMatrix);
      glPushMatrix();
         glTranslatef(pX, pY+dY, pZ);
         glRotatef(angle, aX, aY, aZ);
         glRotatef(angleZ,0.0f,0.0f,1.0f);
         glRotatef(angleX,1.0f,0.0f,0.0f);
         glRotatef(angleY,0.0f,1.0f,0.0f);
         renderFromGraphicMemory();
     glPopMatrix();
   glPopMatrix();

   /* Unset Changes */
   glPopAttrib();
   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}


/*********************************************************************
 *                           renderReflexion                         *
 *********************************************************************/
void aniModel::renderReflexion(float pX, float pY, float pZ, float angleX,
      float angleY, float angleZ)
{
   glPushAttrib(GL_ENABLE_BIT);
   glEnable(GL_STENCIL_TEST);
   glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
   glEnable(GL_NORMALIZE);

   renderFromGraphicMemory(pX, pY, pZ, angleX, angleY, angleZ, true);

   glPopAttrib();
}

/*********************************************************************
 *                           renderReflexion                         *
 *********************************************************************/
void aniModel::renderReflexion(float pX, float pY, float pZ, float angleX,
      float angleY, float angleZ,
      float angle, float aX, float aY, float aZ)
{
   glPushAttrib(GL_ENABLE_BIT);
   glEnable(GL_STENCIL_TEST);
   glEnable(GL_STENCIL_TEST);
   glStencilFunc(GL_EQUAL, 1, 0xffffffff);  /* draw if ==1 */
   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
   glEnable(GL_NORMALIZE);

   renderFromGraphicMemory(pX, pY, pZ, angleX, angleY, angleZ, 
         angle, aX, aY, aZ, true);

   glPopAttrib();
}

/*********************************************************************
 *                               setState                            *
 *********************************************************************/
void aniModel::setState(int state)
{
   /* Verify if animation is defined */
   if( (state > ANIMODEL_MAX_ANIMATIONS) || (state < 0) )
   {
      return;
   }

   /* Only change animation if not actually on it */
   if(state != curState)
   {
      if(type == TYPE_CAL3D)
      {
         /* FIXME: remove this specific case. */
         curState = state;
         setAnimation(curState);

      }
      else
      {
         switch(state)
         {
            case STATE_DIE:
            {
               /* Action action that will became dead after */
               curState = STATE_DEAD;
               setAnimation(STATE_DEAD);
               /* No merge, as dead will be after die, not previously */
               callActionAnimation(STATE_DIE, false);
            }
            break;
            case STATE_ATTACK_MEELE:
            case STATE_HIT:
            case STATE_GUN_USE:
            {
               /* Action action wich go back to previous */
               callActionAnimation(state);
            }
            break;
            default:
            {
               /* Normal cycled action */
               curState = state;
               setAnimation(curState);
            }
            break;
         }
      }
   }
}

/*********************************************************************
 *                               getState                            *
 *********************************************************************/
int aniModel::getState()
{
   return(curState);
}

/*********************************************************************
 *                                update                             *
 *********************************************************************/
void aniModel::update(GLfloat delta, float angleY, 
      float pX, float pY, float pZ, bool isVisible)
{
   //curPos = delta;
   update(delta, isVisible);

   if(isVisible)
   {
      updateKeyVertex(leftHand, angleY, pX, pY+dY, pZ);
      updateKeyVertex(rightHand, angleY, pX, pY+dY, pZ);
      updateKeyVertex(head, angleY, pX, pY+dY, pZ);
   }
}

/*********************************************************************
 *                                Render                             *
 *********************************************************************/
void aniModel::render()
{
   loadToGraphicMemory();
   renderFromGraphicMemory();
   removeFromGraphicMemory();
}

/*********************************************************************
 *                             renderNormals                         *
 *********************************************************************/
void aniModel::renderNormals()
{
   int i;
   int total=0;
   GLfloat factor = 5.0f;
   vector3f_t* normals = getMeshNormals(0, total);
   vector3f_t* verts = getMeshVertices(0, total);

   glPushAttrib(GL_ENABLE_BIT);
   
   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);
   glColor3f( 1.0f, 1.0f, 0.0f );

   glPushMatrix();
   
   /* Correct from blender to dnt coordinates */
   glRotatef(180,0,1,0);
   glRotatef(-90,1,0,0);
   /* Scale, if needed */
   if(renderScale != 1.0)
   {
      glScalef(renderScale, renderScale, renderScale);
      factor = 5.0f / renderScale;
   }

   glBegin( GL_LINES );
   for(i=0; i < total; i++)
   {
      glVertex3fv(verts[i]);
      glVertex3f(verts[i][0] + normals[i][0]*factor,
            verts[i][1] + normals[i][1]*factor, 
            verts[i][2] + normals[i][2]*factor);
   }
   glEnd();

   glPopMatrix();

   glPopAttrib();
}

/*********************************************************************
 *                             renderTangents                        *
 *********************************************************************/
void aniModel::renderTangents()
{
   int i;
   int total=0;
   GLfloat factor = 5.0f;
   vector3f_t* tangents = getMeshTangents(0, total);
   vector3f_t* verts = getMeshVertices(0, total);

   if(!tangents)
   {
      /* No tangents to render */
      return;
   }

   glPushAttrib(GL_ENABLE_BIT);
   
   glDisable(GL_LIGHTING);
   glDisable(GL_FOG);
   glColor3f( 1.0f, 0.2f, 0.6f );

   glPushMatrix();
   
   /* Correct from blender to dnt coordinates */
   glRotatef(180,0,1,0);
   glRotatef(-90,1,0,0);
   /* Scale, if needed */
   if(renderScale != 1.0)
   {
      glScalef(renderScale, renderScale, renderScale);
      factor = 5.0f / renderScale;
   }

   glBegin( GL_LINES );
   for(i=0; i < total; i++)
   {
      glVertex3fv(verts[i]);
      glVertex3f(verts[i][0] + tangents[i][0]*factor,
            verts[i][1] + tangents[i][1]*factor, 
            verts[i][2] + tangents[i][2]*factor);
   }
   glEnd();

   glPopMatrix();

   glPopAttrib();
}

/*********************************************************************
 *                         renderBoundingBox                         *
 *********************************************************************/
void aniModel::renderBoundingBox()
{
   crudeBox.render();
}

/*********************************************************************
 *                           depthColision                           *
 *********************************************************************/
bool aniModel::depthCollision(GLfloat angleX, GLfloat angleY, GLfloat angleZ,
      GLfloat pX, GLfloat pY, GLfloat pZ, boundingBox& colBox)
{
   /* Calculate the sin and cos of the angles, to do less calculations */
   float sinAngleX = sinf(deg2Rad(angleX));
   float cosAngleX = cosf(deg2Rad(angleX));
   float sinAngleY = sinf(deg2Rad(angleY));
   float cosAngleY = cosf(deg2Rad(angleY));
   float sinAngleZ = sinf(deg2Rad(angleZ));
   float cosAngleZ = cosf(deg2Rad(angleZ));

   int vertCount=0;
   int triCount=0;
   vector3i_t* triangles = NULL;
   vector3f_t* vertices=NULL;

   /* Apply delta */
   pY += dY;

   /* get the number of meshes */
   int meshCount;
   meshCount = getTotalMeshes();

   /* verify all meshes of the model */
   int meshId;
   for(meshId = 0; meshId < meshCount; meshId++)
   {
      /* get the transformed vertices of the mesh */
      vertices = getMeshVertices(meshId, vertCount);

      /* get faces */
      triangles = getMeshFaces(meshId, triCount);

      /* Verify each model face with each bounding box face */
      int f;
      float V0[3], V1[3], V2[3];
      float U0[3], U1[3], U2[3];
      int index0=0, index1=0, index2=0;
      for(f = 0; f < triCount; f++)
      {

         /* Define Triangle Vertex index */
         index0 = triangles[f][0];
         index1 = triangles[f][1];
         index2 = triangles[f][2];

         /* Translate and rotate the coordinates.
          * NOTE: Do not forget that if the blender coordinate system is
          * (x,y,z), the DNT system is (-x,z,y) */
         rotatePoint(-vertices[index0][0]*renderScale,
               vertices[index0][2]*renderScale,
               vertices[index0][1]*renderScale,
               angleX, angleY, angleZ, sinAngleX, cosAngleX, 
               sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
               V0[0], V0[1], V0[2]);
         V0[0] += pX;
         V0[1] += pY;
         V0[2] += pZ;

         rotatePoint(-vertices[index1][0]*renderScale,
               vertices[index1][2]*renderScale,
               vertices[index1][1]*renderScale,
               angleX, angleY, angleZ, sinAngleX, cosAngleX, 
               sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
               V1[0], V1[1], V1[2]);
         V1[0] += pX;
         V1[1] += pY;
         V1[2] += pZ;

         rotatePoint(-vertices[index2][0]*renderScale,
               vertices[index2][2]*renderScale,
               vertices[index2][1]*renderScale,
               angleX, angleY, angleZ, sinAngleX, cosAngleX, 
               sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
               V2[0], V2[1], V2[2]);
         V2[0] += pX;
         V2[1] += pY;
         V2[2] += pZ;

         /* Bounding Box Faces */

         /* Upper Face A */
         U0[0] = colBox.min.x;
         U0[1] = colBox.max.y;   
         U0[2] = colBox.min.z;

         U1[0] = colBox.max.x;
         U1[1] = colBox.max.y;
         U1[2] = colBox.min.z;

         U2[0] = colBox.min.x;
         U2[1] = colBox.max.y;
         U2[2] = colBox.max.z;

         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Upper Face A! */
            return(true);
         }

         /* Upper Face B */
         U0[0] = colBox.max.x;
         U0[1] = colBox.max.y;
         U0[2] = colBox.max.z;
         //U1 = same as upper face A
         //U2 = same as upper face A


         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Upper Face B! */
            return(true);
         }

         /* Lower Face A */
         U0[0] = colBox.min.x;
         U0[1] = colBox.min.y;   
         U0[2] = colBox.min.z;

         U1[0] = colBox.max.x;
         U1[1] = colBox.min.y;
         U1[2] = colBox.min.z;

         U2[0] = colBox.min.x;
         U2[1] = colBox.min.y;
         U2[2] = colBox.max.z;

         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Lower Face A! */
            return(true);
         }

         /* Lower Face B */
         U0[0] = colBox.max.x;
         U0[1] = colBox.min.y;
         U0[2] = colBox.max.z;
         //U1 = same as lower face A
         //U2 = same as lower face A


         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Lower Face B! */
            return(true);
         }


         /* Left Face A */
         U0[0] = colBox.min.x;
         U0[1] = colBox.min.y;   
         U0[2] = colBox.min.z;

         U1[0] = colBox.min.x;
         U1[1] = colBox.min.y;
         U1[2] = colBox.max.z;

         U2[0] = colBox.min.x;
         U2[1] = colBox.max.y;
         U2[2] = colBox.min.z;

         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Left Face A! */  
            return(true);
         }

         /* Left Face B */
         U0[0] = colBox.min.x;
         U0[1] = colBox.max.y;
         U0[2] = colBox.max.z;
         //U1 = same as left face A
         //U2 = same as left face A

         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Left Face B! */
            return(true);
         }

         /* Right Face A */
         U0[0] = colBox.max.x;
         U0[1] = colBox.min.y;   
         U0[2] = colBox.min.z;

         U1[0] = colBox.max.x;
         U1[1] = colBox.min.y;
         U1[2] = colBox.max.z;

         U2[0] = colBox.max.x;
         U2[1] = colBox.max.y;
         U2[2] = colBox.min.z;

         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Right Face A! */
            return(true);
         }

         /* Right Face B */
         U0[0] = colBox.max.x;
         U0[1] = colBox.max.y;
         U0[2] = colBox.max.z;
         //U1 = same as right face A
         //U2 = same as right face A

         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Right Face B! */
            return(true);
         }


         /* Front Face A */
         U0[0] = colBox.min.x;
         U0[1] = colBox.min.y;   
         U0[2] = colBox.min.z;

         U1[0] = colBox.max.x;
         U1[1] = colBox.min.y;
         U1[2] = colBox.min.z;

         U2[0] = colBox.min.x;
         U2[1] = colBox.max.y;
         U2[2] = colBox.min.z;

         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Front Face A! */
            return(true);
         }

         /* Front Face B */
         U0[0] = colBox.max.x;
         U0[1] = colBox.max.y;
         U0[2] = colBox.min.z;
         //U1 = same as front face A
         //U2 = same as front face A

         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Front Face B! */
            return(true);
         }

         /* Back Face A */
         U0[0] = colBox.min.x;
         U0[1] = colBox.min.y;   
         U0[2] = colBox.max.z;

         U1[0] = colBox.max.x;
         U1[1] = colBox.min.y;
         U1[2] = colBox.max.z;

         U2[0] = colBox.min.x;
         U2[1] = colBox.max.y;
         U2[2] = colBox.max.z;

         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Back Face A! */
            return(true);
         }

         /* Back Face B */
         U0[0] = colBox.max.x;
         U0[1] = colBox.max.y;
         U0[2] = colBox.max.z;
         //U1 = same as back face A
         //U2 = same as back face A

         if(NoDivTriTriIsect(V0, V1, V2, U0, U1, U2))
         {
            /* Detected Collision at Back Face B! */
            return(true);
         }
      }
   }
   /* If got here, no collision occurs */
   return(false);
}

/*********************************************************************
 *                           depthColision                           *
 *********************************************************************/
bool aniModel::depthCollision(GLfloat angleX, GLfloat angleY, GLfloat angleZ,
      GLfloat pX, GLfloat pY, GLfloat pZ, ray& colRay)
{
   /* Calculate the sin and cos of the angles, to do less calculations */
   float sinAngleX = sinf(deg2Rad(angleX));
   float cosAngleX = cosf(deg2Rad(angleX));
   float sinAngleY = sinf(deg2Rad(angleY));
   float cosAngleY = cosf(deg2Rad(angleY));
   float sinAngleZ = sinf(deg2Rad(angleZ));
   float cosAngleZ = cosf(deg2Rad(angleZ));

   int vertCount=0;
   int triCount=0;
   vector3i_t* triangles = NULL;
   vector3f_t* vertices=NULL;

   float t=0.0f, u=0.0f, v=0.0f;

   /* Get ray info */
   float orig[3];
   float dir[3];

   orig[0] = colRay.origin.x;
   orig[1] = colRay.origin.y;
   orig[2] = colRay.origin.z;

   dir[0] = colRay.direction.x;
   dir[1] = colRay.direction.y;
   dir[2] = colRay.direction.z;

   /* Apply delta */
   pY += dY;

   /* get the number of meshes */
   int meshCount;
   meshCount = getTotalMeshes();

   /* verify all meshes of the model */
   int meshId;
   for(meshId = 0; meshId < meshCount; meshId++)
   {
      /* get the transformed vertices of the mesh */
      vertices = getMeshVertices(meshId, vertCount);

      /* get faces */
      triangles = getMeshFaces(meshId, triCount);

      /* Verify each model face */
      int f;
      float V0[3], V1[3], V2[3];
      int index0=0, index1=0, index2=0;
      for(f = 0; f < triCount; f++)
      {

         /* Define Triangle Vertex index */
         index0 = triangles[f][0];
         index1 = triangles[f][1];
         index2 = triangles[f][2];

         /* Translate and rotate the coordinates.
          * NOTE: Do not forget that if the blender coordinate system is
          * (x,y,z), the DNT system is (-x,z,y) */
         rotatePoint(-vertices[index0][0]*renderScale,
               vertices[index0][2]*renderScale,
               vertices[index0][1]*renderScale,
               angleX, angleY, angleZ, sinAngleX, cosAngleX, 
               sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
               V0[0], V0[1], V0[2]);
         V0[0] += pX;
         V0[1] += pY;
         V0[2] += pZ;

         rotatePoint(-vertices[index1][0]*renderScale,
               vertices[index1][2]*renderScale,
               vertices[index1][1]*renderScale,
               angleX, angleY, angleZ, sinAngleX, cosAngleX, 
               sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
               V1[0], V1[1], V1[2]);
         V1[0] += pX;
         V1[1] += pY;
         V1[2] += pZ;

         rotatePoint(-vertices[index2][0]*renderScale,
               vertices[index2][2]*renderScale,
               vertices[index2][1]*renderScale,
               angleX, angleY, angleZ, sinAngleX, cosAngleX, 
               sinAngleY, cosAngleY, sinAngleZ, cosAngleZ,
               V2[0], V2[1], V2[2]);
         V2[0] += pX;
         V2[1] += pY;
         V2[2] += pZ;

         /* Verify ray triangle intersection */
         if(intersect_triangle(orig, dir, V0, V1, V2, &t, &u, &v) )
         {
            if(colRay.size != DNT_INFINITE_RAY_SIZE)
            {
               /* Collided with ray, must verify extension */
               if(t <= colRay.size)
               {
                  /* Lenght lesser: collided! */
                  return(true);
               }
            }
            else
            {
               /* Collided with infinite ray */
               return(true);
            }
         }
      }
   }
   /* If got here, no collision occurs */
   return(false);
}


