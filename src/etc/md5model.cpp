/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
  
  Originally based on the work of David Henry <tfc_duke@club-internet.fr>
  Copyright (c) 2005-2007 David HENRY
 
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


#include "md5model.h"

#include "dirs.h"
#include "defparser.h"

#include <iostream>
#include <fstream>

#include <SDL/SDL_opengl.h>

/***********************************************************************
 *                               Constructor                           *
 ***********************************************************************/
md5Model::md5Model(): aniModel(aniModel::TYPE_MD5),
                    dntList(DNT_LIST_TYPE_ADD_AT_END)
{
   totalJoints = 0;
   totalMeshes = 0;

   meshes = NULL;
   baseSkel = NULL;
   curSkel = NULL;
   curAnimation = NULL;

   needToCalculate = true;
   needToCalculateBoundingBox = true;
}

/***********************************************************************
 *                                Destructor                           *
 ***********************************************************************/
md5Model::~md5Model()
{
   clean();
   clearList();
}

/***********************************************************************
 *                                   clean                             *
 ***********************************************************************/
void md5Model::clean()
{
   int i;

   if(loadedTexture)
   {
      /* Clear the texture */ 
      glDeleteTextures(1, &meshMaterial.textureId);
   }

   if(baseSkel)
   {
      /* Clear joints */
      delete[] baseSkel;
      baseSkel = NULL;
      totalJoints = 0;
   }
   if(meshes)
   {
      /* Clear each mesh data */
      for(i=0; i<totalMeshes; i++)
      {
         delete[] meshes[i].vertexArray;
         delete[] meshes[i].vertices;
         delete[] meshes[i].triangles;
         delete[] meshes[i].uvArray;
         delete[] meshes[i].normalArray;
         delete[] meshes[i].weights;
      }

      /* Clear meshes */
      delete[] meshes;
      meshes = NULL;
      totalMeshes = 0;
   }
}

/***********************************************************************
 *                             freeElement                             *
 ***********************************************************************/ 
void md5Model::freeElement(dntListElement* obj)
{
   if(obj)
   {
      md5ModelAnimation* m = (md5ModelAnimation*)obj;
      delete(m);
   }
}

/***********************************************************************
 *                         loadAnimationFile                           *
 ***********************************************************************/
bool md5Model::loadAnimFile(const std::string strFileName)
{
   md5ModelAnimation* mAnim = new md5ModelAnimation;
   if(mAnim->animation.load(strFileName))
   {
      insert(mAnim);
      return(true);
   }
   else
   {
      /* Couldn't load, delete the animation and ignore it */
      delete(mAnim);
   }
   return(false);
}

/***********************************************************************
 *                             loadMeshFile                            *
 ***********************************************************************/
bool md5Model::loadMeshFile(const std::string strFileName)
{
   dirs dir;
   std::ifstream file;
   std::string buff;
   char buf2[64];
   int version;
   int curr_mesh = 0;
   int i;
   int line;

   /* Open it! */
   file.open(dir.getRealFile(strFileName).c_str(), 
         std::ios::in|std::ios::binary);

   if(!file)
   {
      std::cerr << "Error opening file: " << strFileName << std::endl;
      return(false);
   }

   /* Let's consume all file */
   getline(file, buff);
   for(line = 1; (!file.eof()); line++)
   {
      /* Clear leading spaces */
      buff.erase(0, buff.find_first_not_of(" \t"));

      /* Parse line */
      if(buff.empty())
      {
         /* empty line, must ignore! */
      }
      else if( buff.compare(0, 2, "//") == 0)
      {
         /* Comment line, must ignore too */
      }
      else if( buff.compare(0, 10, "MD5Version") == 0 )
      {
         /* Md5 model version */
         //std::cerr << buff << std::endl;
         sscanf(buff.c_str(), "MD5Version %d", &version);
         
         /* Verify file version */
         if(version != 10)
         {
            /* Bad version */
            std::cerr << "Error: Model '" << strFileName 
               << "' is of non supported model version: " << version 
               << std::endl;
            file.close();
            return(false);
         }
      }
      else if(buff.compare(0, 9, "numJoints") == 0)
      {
         /* Get number of joints */
         sscanf(buff.c_str(), " numJoints %d", &totalJoints);
         if(totalJoints > 0)
         {
            /* Allocate memory for base skeleton joints */
            baseSkel = new md5_joint_t[totalJoints];
            curSkel = baseSkel;
            //std::cerr << "Alloced " << totalJoints << " joints" << std::endl;
         }
      }
      else if(buff.compare(0, 9, "numMeshes") == 0)
      {
         /* Get number of meshes */
         sscanf(buff.c_str(), " numMeshes %d", &totalMeshes);
         if(totalMeshes > 0)
         {
            /* Allocate memory for meshes */
            meshes = new md5_mesh_t[totalMeshes];
            //std::cerr << "Alloced " << totalMeshes << " meshes" << std::endl;
         }
      }
      else if(buff.compare(0, 8, "joints {", 8) == 0)
      {
         /* Read each joint */
         for(i=0; i < totalJoints; ++i)
         {
            md5_joint_t* joint = &baseSkel[i];

            /* Get whole line */
            getline(file, buff);
            /* Clear leading spaces */
            buff.erase(0, buff.find_first_not_of(" \t"));

            /* Get joint date */
            if(sscanf(buff.c_str(), "%s %d ( %f %f %f ) ( %f %f %f )",
                     &buf2[0], &joint->parent, &joint->pos.x,
                     &joint->pos.y, &joint->pos.z, &joint->orient.x,
                     &joint->orient.y, &joint->orient.z) == 8)
            {
               joint->name = buf2;
               /* Remove quotes */
               joint->name.erase(0, joint->name.find_first_of("\"")+1);
               joint->name.erase(joint->name.find_first_of("\""), 1);

               //std::cerr << "Got Joint: " << joint->name << std::endl;
               joint->orient.computeW();
            }
         }
      }
      else if(buff.compare(0,6, "mesh {") == 0)
      {
         md5_mesh_t* mesh = &meshes[curr_mesh];
         int vert_index = 0;
         int tri_index = 0;
         int weight_index = 0;
         float fdata[4];
         int idata[3];

         while((buff[0] != '}') && (!file.eof()))
         {
            /* Get whole line */
            getline(file, buff);
            /* Clear leading spaces */
            buff.erase(0, buff.find_first_not_of(" \t"));

            if(buff.compare(0, 6, "shader") == 0)
            {
               /* Remove quotes */
               buff.erase(0, buff.find_first_of("\"")+1);
               buff.erase(buff.find_first_of("\""), 1);

               mesh->shader = buff;
               //std::cerr << "Defined shader as: " << mesh->shader << 
               //   std::endl;
            }
            else if(buff.compare(0, 8, "numverts") == 0)
            {
               /* Get number of vertices */
               sscanf(buff.c_str(), "numverts %d", &mesh->num_verts);
               if(mesh->num_verts > 0)
               {
                  /* Allocate memory for vertices */
                  mesh->vertices = new md5_vertex_t[mesh->num_verts];
                  //std::cerr << "Alloced " << mesh->num_verts << " vertices"
                  //          << std::endl;
               }

               /* Create the render arrays */
               mesh->vertexArray = new vector3f_t[mesh->num_verts];
               mesh->uvArray = new vector2f_t[mesh->num_verts];
               mesh->normalArray = new vector3f_t[mesh->num_verts];
            }
            else if(buff.compare(0, 7, "numtris") == 0)
            {
               sscanf(buff.c_str(), "numtris %d", &mesh->num_tris);
               if(mesh->num_tris > 0)
               {
                  /* Allocate memory for triangles */
                  mesh->triangles = new vector3i_t[mesh->num_tris];
                  //md5_triangle_t[mesh->num_tris];
                  //std::cerr << "Alloced " << mesh->num_tris << " triangles"
                  //          << std::endl;
               }
            }
            else if(buff.compare(0, 10, "numweights") == 0)
            {
               sscanf(buff.c_str(), "numweights %d", &mesh->num_weights);
               if(mesh->num_weights > 0)
               {
                  /* Allocate memory for vertex weights */
                  mesh->weights = new md5_weight_t[mesh->num_weights];
                  //std::cerr << "Alloced " << mesh->num_weights << " weights"
                  //          << std::endl;
               }
            }
            else if(buff.compare(0, 4, "vert") == 0)
            {
               sscanf(buff.c_str(), "vert %d ( %f %f ) %d %d", &vert_index,
                     &fdata[0], &fdata[1], &idata[0], &idata[1]);
               /* Copy vertex data */
               mesh->uvArray[vert_index][0] = fdata[0];
               mesh->uvArray[vert_index][1] = fdata[1];
               mesh->vertices[vert_index].start = idata[0];
               mesh->vertices[vert_index].count = idata[1];
            }
            else if(buff.compare(0, 3, "tri") == 0)
            {
               sscanf(buff.c_str(), "tri %d %d %d %d", &tri_index,
                     &idata[0], &idata[1], &idata[2]);
               /* Copy triangle data */
               mesh->triangles[tri_index][0] = idata[0];
               mesh->triangles[tri_index][1] = idata[1];
               mesh->triangles[tri_index][2] = idata[2];
            }
            else if(buff.compare(0, 6, "weight") == 0)
            {
               sscanf(buff.c_str(), "weight %d %d %f ( %f %f %f )",
                     &weight_index, &idata[0], &fdata[3],
                     &fdata[0], &fdata[1], &fdata[2]);
               /* Copy vertex data */
               mesh->weights[weight_index].joint  = idata[0];
               mesh->weights[weight_index].bias   = fdata[3];
               mesh->weights[weight_index].pos.x = fdata[0];
               mesh->weights[weight_index].pos.y = fdata[1];
               mesh->weights[weight_index].pos.z = fdata[2];
            }
         }

         curr_mesh++;
      }

      /* Get Next Line  */
      getline(file, buff);
   }

   /* End with file read */
   file.close();

   preCalculateNormals();

   return(true);
}

/***********************************************************************
 *                            setAnimation                             *
 ***********************************************************************/
void md5Model::setAnimation(int animationId)
{
   if(curAnimation)
   {
      /* TODO: blend it with the next! */
   }
   curAnimation = (md5ModelAnimation*)get(animationId);
   if(!curAnimation)
   {
      needToCalculate = true;
      needToCalculateBoundingBox = true;
   }
}

/***********************************************************************
 *                               update                                *
 ***********************************************************************/
void md5Model::update(float delta)
{
   if(curAnimation)
   {
      /* Always need to calculate when animated */
      needToCalculate = true;
      /* Calculate current and next frames */
      curAnimation->animation.update(delta);
      /* Interpolate skeletons between two frames */
      curSkel = curAnimation->animation.interpolate();
      calculateMeshes();
   }
   else
   {
      /* No animation, just use the base skeleton */
      curSkel = baseSkel;
      /* Only do a single calculation for non-animation */
      if(needToCalculate)
      {
         calculateMeshes();
         needToCalculate = false;
      }
   }
}

/***********************************************************************
 *                           renderSkeleton                            *
 ***********************************************************************/
void md5Model::renderSkeleton()
{
   int i;

   glPushAttrib(GL_ENABLE_BIT);
   glDisable(GL_FOG);
   glDisable(GL_LIGHTING);


   glPushMatrix();
  
   glScalef(10.0f, 10.0f, 10.0f);
   /* Correct from blender to dnt coordinates */
   glRotatef(180,0,1,0);
   glRotatef(-90,1,0,0);

   /* Draw each joint */
   glPointSize (5.0f);
   glColor3f (1.0f, 0.0f, 0.0f);
   glBegin(GL_POINTS);
   for(i = 0; i < totalJoints; ++i)
   {
      glVertex3f(curSkel[i].pos.x, curSkel[i].pos.y, curSkel[i].pos.z);
   }
   glEnd();
   glPointSize (1.0f);

   /* Draw each bone */
   glColor3f(0.0f, 1.0f, 0.0f);
   glBegin(GL_LINES);
   for (i = 0; i < totalJoints; ++i)
   {
      if(curSkel[i].parent != -1)
      {
         glVertex3f(curSkel[curSkel[i].parent].pos.x,
                    curSkel[curSkel[i].parent].pos.y,
                    curSkel[curSkel[i].parent].pos.z);
         glVertex3f(curSkel[i].pos.x, curSkel[i].pos.y, curSkel[i].pos.z);
      }
   }
   glEnd();

   glPopAttrib();

   glPopMatrix();
}

/***********************************************************************
 *                           calculateMeshes                           *
 ***********************************************************************/
void md5Model::calculateMeshes()
{
   int m,i,j;
   md5_weight_t* weight;
   md5_joint_t* joint;
   md5_vertex_t* vert;

   /* Calculate each mesh */
   for(m=0; m < totalMeshes; m++)
   {
      for(i=0; i < meshes[m].num_verts; i++)
      {
         vec3_t finalVertex;
         vec3_t finalNormal;

         /* Calculate final vertex with weights */
         for (j = 0; j < meshes[m].vertices[i].count; ++j)
         {
            weight = &meshes[m].weights[meshes[m].vertices[i].start + j];
            vert = &meshes[m].vertices[i];
            joint = &curSkel[weight->joint];

            /* Calculate transformed vertex for this weight */
            vec3_t wv;
            wv = joint->orient.rotatePoint(weight->pos);

            /* The sum of all weight->bias should be 1.0 */
            finalVertex.x += (joint->pos.x + wv.x) * weight->bias;
            finalVertex.y += (joint->pos.y + wv.y) * weight->bias;
            finalVertex.z += (joint->pos.z + wv.z) * weight->bias;

            finalNormal = finalNormal + 
               (joint->orient.rotatePoint(vert->normal) * weight->bias);
         }

         meshes[m].vertexArray[i][0] = finalVertex.x;
         meshes[m].vertexArray[i][1] = finalVertex.y;
         meshes[m].vertexArray[i][2] = finalVertex.z;

         meshes[m].normalArray[i][0] = finalNormal.x;
         meshes[m].normalArray[i][1] = finalNormal.y;
         meshes[m].normalArray[i][2] = finalNormal.z;
      }
   }
}

/***********************************************************************
 *                        preCalculateNormals                          *
 ***********************************************************************/
void md5Model::preCalculateNormals()
{
   int i,j, meshId;

   /* First, let's compute the vertices positions at "pose" state */
   curSkel = baseSkel;
   calculateMeshes();

   /* Now, let's calculate weighted-normals for all meshes */
   for(meshId=0; meshId < totalMeshes; meshId++)
   {
      md5_mesh_t* curMesh = &meshes[meshId];

      /* Calculate each face normal */
      for(i=0; i < curMesh->num_tris; i++)
      {
         md5_vertex_t* vert0 = &curMesh->vertices[curMesh->triangles[i][0]];
         md5_vertex_t* vert1 = &curMesh->vertices[curMesh->triangles[i][1]];
         md5_vertex_t* vert2 = &curMesh->vertices[curMesh->triangles[i][2]];

         vec3_t v0 = vec3_t(curMesh->vertexArray[curMesh->triangles[i][0]]);
         vec3_t v1 = vec3_t(curMesh->vertexArray[curMesh->triangles[i][1]]);
         vec3_t v2 = vec3_t(curMesh->vertexArray[curMesh->triangles[i][2]]);

         vec3_t dif1 = v2 - v0;
         vec3_t n = dif1.cross(v1 - v0);
         
         vert0->normal = vert0->normal + n;
         vert1->normal = vert1->normal + n;
         vert2->normal = vert2->normal + n;
      }

      /* Finally, normalize normals */
      for(i=0; i < curMesh->num_verts; i++)
      {
         md5_vertex_t* vert = &curMesh->vertices[i];

         /* normal.normalize is incredible! */
         vec3_t normal = vert->normal;
         normal.normalize();

         /* Let's return back to joint-local space */
         vert->normal = vec3_t();
         for(j=0; j < vert->count; j++)
         {
            md5_weight_t* w = &curMesh->weights[vert->start + j];
            md5_joint_t* j = &baseSkel[w->joint];
            vec3_t wv;
            wv = j->orient.rotatePoint(normal);

            vert->normal = vert->normal +  (wv * w->bias);

         }
      }
   }
}

/***********************************************************************
 *                           getMeshVertices                           *
 ***********************************************************************/
vector3f_t* md5Model::getMeshVertices(int meshId, int& count)
{
   if((meshId >= 0) && (meshId < totalMeshes))
   {
      count = meshes[meshId].num_verts;
      return(&meshes[meshId].vertexArray[0]);
   }

   /* No mesh with meshId */
   count = 0;
   return(NULL);
}

/***********************************************************************
 *                           getMeshVertices                           *
 ***********************************************************************/
vector3i_t* md5Model::getMeshFaces(int meshId, int& count)
{
   if((meshId >= 0) && (meshId < totalMeshes))
   {
      count = meshes[meshId].num_tris;
      return(&meshes[meshId].triangles[0]);
   }

   /* No mesh with meshId */
   count = 0;
   return(NULL);
}

/***********************************************************************
 *                          getMeshTexCoords                           *
 ***********************************************************************/
vector2f_t* md5Model::getMeshTexCoords(int meshId, int& count)
{
    if((meshId >= 0) && (meshId < totalMeshes))
   {
      count = meshes[meshId].num_tris;
      return(&meshes[meshId].uvArray[0]);
   }

   /* No mesh with meshId */
   count = 0;
   return(NULL);
}

/***********************************************************************
 *                           getMeshNormals                            *
 ***********************************************************************/
vector3f_t* md5Model::getMeshNormals(int meshId, int& count)
{
    if((meshId >= 0) && (meshId < totalMeshes))
   {
      count = meshes[meshId].num_tris;
      return(&meshes[meshId].normalArray[0]);
   }

   /* No mesh with meshId */
   count = 0;
   return(NULL);
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool md5Model::load(const std::string& strFileName)
{
   defParser def;
   std::string key="", data="";

   /* Load the definitions */
   if(!def.load(strFileName))
   {
      return(false);
   }
   fileName = strFileName;

   /* Parse all tuples */
   while(def.getNextTuple(key, data))
   {
      if(key == "path")
      {
         /* Set default model path */
         path = data;
      }
      else if(key == "scale")
      {
         /* set rendering scale factor */
         std::stringstream sconv;
         sconv << data;
         sconv >> renderScale;
      }
      else if(key == "mesh")
      {
         if(meshes == NULL)
         {
            /* Load the mesh file */
            loadMeshFile(path+data);
         }
         else
         {
            /* Already loaded a mesh, error! */
            std::cerr << "Warning: DNT doesn't support multiple md5 models "
                << "declared at the same definition for file ('" 
                << fileName << "')" << std::endl;
         }
      }
      else if( (key == "anim") || (key == "animation") )
      {
         /* Load a new animation */
         loadAnimFile(path+data);
      }
      else if(key == "deltaY")
      {
         /* Define the model's deltaY */
         sscanf(data.c_str(), "%f", &dY);
      }
      /* Bellow are material related things */
      else if(key == "diffuse")
      {
         if(sscanf(data.c_str(), "%f %f %f %f", 
                  &meshMaterial.diffuse[0], &meshMaterial.diffuse[1],
                  &meshMaterial.diffuse[2], &meshMaterial.diffuse[3]) != 4)
         {
            std::cerr << "Warning: invalid material data '" << data 
               << "' on model " << fileName << std::endl;
         }
      }
      else if(key == "ambient")
      {
         if(sscanf(data.c_str(), "%f %f %f %f", 
                  &meshMaterial.ambient[0], &meshMaterial.ambient[1],
                  &meshMaterial.ambient[2], &meshMaterial.ambient[3]) != 4)
         {
            std::cerr << "Warning: invalid material data '" << data 
               << "' on model " << fileName << std::endl;
         }
      }
      else if(key == "specular")
      {
         if(sscanf(data.c_str(), "%f %f %f %f", 
                  &meshMaterial.ambient[0], &meshMaterial.ambient[1],
                  &meshMaterial.ambient[2], &meshMaterial.ambient[3]) != 4)
         {
            std::cerr << "Warning: invalid material data '" << data 
               << "' on model " << fileName << std::endl;
         }
      }
      else if(key == "shininess")
      {
         sscanf(data.c_str(), "%f", &meshMaterial.shininess);
      }
      else if( (key == "texture") || (key == "image"))
      {
         meshMaterial.textureId = loadTexture(path+data);
      }
   }

   /* Must do a first model update, to a random position */
   if(getTotal() > 0)
   {
      /* Set initial animation to IDLE */
      setAnimation(STATE_IDLE);
   }
   int curPos =  11 + (int)(30 * (rand() / (RAND_MAX + 1.0)));
   update(curPos);

   /* Must define all initial bounding box */
   calculateCrudeBoundingBox();
   
   return(true);
}

/***********************************************************************
 *                         callActionAnimation                         *
 ***********************************************************************/
void md5Model::callActionAnimation(int aniId)
{
   /* TODO: not yet on md5Model! */
}

/***********************************************************************
 *                      calculateCrudeBoundingBox                      *
 ***********************************************************************/
void md5Model::calculateCrudeBoundingBox()
{
   vec3_t min, max;
   int i,v;

   /* NOTE: Do not forget that if the blender coordinate system is
    * (x,y,z), the DNT system is (-x,z,y) */

   if(curAnimation)
   {
      /* Get bounding box from animation frame */
      md5_bbox_t* bbox = curAnimation->animation.getCurrentBoundingBox();
      min = bbox->min;
      max = bbox->max;
   }
   else if(needToCalculateBoundingBox)
   {
      needToCalculateBoundingBox = false;
      if(needToCalculate)
      {
         std::cerr << "Warning: called to calculate crudeBoundingBox on a non "
            << "defined mesh!" << std::endl;
      }
      /* Set initial */
      min = meshes[0].vertexArray[0];
      max = meshes[0].vertexArray[0];

      /* Let's verify all meshes vertices */
      for(i=0; i<totalMeshes; i++)
      {
         for(v=0; v<meshes[i].num_verts; v++)
         {
            /* Verify X */
            if(meshes[i].vertexArray[v][0] < min.x)
            {
               min.x = meshes[i].vertexArray[v][0];
            }
            if(meshes[i].vertexArray[v][0] > max.x)
            {
               max.x = meshes[i].vertexArray[v][0];
            }
            /* Verify Y */
            if(meshes[i].vertexArray[v][1] < min.y)
            {
               min.y = meshes[i].vertexArray[v][1];
            }
            if(meshes[i].vertexArray[v][1] > max.y)
            {
               max.y = meshes[i].vertexArray[v][1];
            }
            /* Verify Z */
            if(meshes[i].vertexArray[v][2] < min.z)
            {
               min.z = meshes[i].vertexArray[v][2];
            }
            if(meshes[i].vertexArray[v][2] > max.z)
            {
               max.z = meshes[i].vertexArray[v][2];
            }
         }
      }
   }
   else
   {
      /* No need to update bounding box */
      return;
   }

   /* Do the Scale to the bounding box */
   min = min*renderScale;
   max = max*renderScale;

   /* Apply the delta (to Z, as not yet converted to DNT coord system) */
   min.z += dY;
   max.z += dY;

   /* Finally, set bounding box, converting to DNT system */
   crudeBox.setMin(-max.x, min.z, min.y);
   crudeBox.setMax(-min.x, max.z, max.y);
}

/***********************************************************************
 *                           updateKeyVertex                           *
 ***********************************************************************/
void md5Model::updateKeyVertex(vertexInfo& v,
      float angleY, float pX, float pY, float pZ)
{
   /* TODO: not implemented for md5Model yet. */
}

/***********************************************************************
 *                           getMeshMaterial                           *
 ***********************************************************************/
aniModelMaterial* md5Model::getMeshMaterial(int meshId)
{
   return(&meshMaterial);
}

/***********************************************************************
 *                              getBoneId                              *
 ***********************************************************************/
int md5Model::getBoneId(std::string bName)
{
   /* TODO: not yet. */
   return(-1);
}

