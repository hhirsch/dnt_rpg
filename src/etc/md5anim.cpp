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

#include "md5anim.h"

#include "dirs.h"
#include <iostream>
#include <fstream>

/***********************************************************************
 *                               Constructor                           *
 ***********************************************************************/
md5Anim::md5Anim(int identifier)
{
   totalFrames=0;
   totalJoints=0;
   frameRate=0;

   id = identifier;

   skelFrames = NULL;
   retSkel = NULL;
   bboxes = NULL;

   curFrame = 0;
   nextFrame = 1;

   lastTime = 0.0;
   maxTime = 0.0;
}

/***********************************************************************
 *                                Destructor                           *
 ***********************************************************************/
md5Anim::~md5Anim()
{
   clean();
}

/***********************************************************************
 *                                   clean                             *
 ***********************************************************************/
void md5Anim::clean()
{
   int i;

   if(skelFrames)
   {
      /* Delete each animation data */
      for(i=0; i < totalFrames; i++)
      {
         delete[] skelFrames[i];
      }
      /* Delete all frames */
      delete[] skelFrames;
      skelFrames = NULL;
   }
   if(retSkel)
   {
      delete[] retSkel;
      retSkel = NULL;
   }
   if(bboxes)
   {
      /* delete all bounding boxes */
      delete[] bboxes;
      bboxes = NULL;
   }
}

/***********************************************************************
 *                                 load                                *
 ***********************************************************************/
bool md5Anim::load(const std::string strFileName)
{
   dirs dir;
   std::ifstream file;
   std::string buff;
   char buf2[64];
   md5_joint_info_t* jointInfos = NULL;
   md5_baseframe_joint_t* baseFrame = NULL;
   float *animFrameData = NULL;
   int version;
   int numAnimatedComponents;
   int frame_index;
   int line, i;

   /* Define File */
   fileName = strFileName;

   /* Open it! */
   file.open(dir.getRealFile(fileName).c_str(), std::ios::in|std::ios::binary);

   if(!file)
   {
      std::cerr << "Error opening file: " << fileName << std::endl;
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
         /* Md5 animation version */
         //std::cerr << buff << std::endl;
         sscanf(buff.c_str(), "MD5Version %d", &version);
         
         /* Verify file version */
         if(version != 10)
         {
            /* Bad version */
            std::cerr << "Error: animation '" << fileName 
               << "' is of non supported version: " << version 
               << std::endl;
            file.close();
            return(false);
         }
      }
      else if(buff.compare(0, 9, "numFrames") == 0)
      {
         sscanf(buff.c_str(), "numFrames %d", &totalFrames);
         /* Allocate memory for skeleton frames and bounding boxes */
         if (totalFrames > 0)
         {
            skelFrames = new md5_joint_t*[totalFrames];
            bboxes = new md5_bbox_t[totalFrames];
         }
         //std::cerr <<"Created with totalFrames: " << totalFrames << std::endl;
      }
      else if(buff.compare(0, 9, "numJoints") == 0)
      {
         sscanf(buff.c_str(), "numJoints %d", &totalJoints);
      
         if(totalJoints > 0)
         {
            for(i = 0; i < totalFrames; ++i)
            {
               /* Allocate memory for joints of each frame */
               skelFrames[i] = new md5_joint_t[totalJoints];
            }
            /* Alloc return skeleton */
            retSkel = new md5_joint_t[totalJoints];

            /* Allocate temporary memory for building skeleton frames */
            jointInfos = new md5_joint_info_t[totalJoints];
            baseFrame = new md5_baseframe_joint_t[totalJoints];
         }
         //std::cerr <<"Created with totalFrames: " << totalFrames << std::endl;
      }
      else if(buff.compare(0, 9, "frameRate") == 0)
      {
         sscanf(buff.c_str(), "frameRate %d", &frameRate);
         maxTime = 1.0f / frameRate;
         //std::cerr << "FrameRate: " << frameRate << " maxTime: " << maxTime
         //          << std::endl;
      }
      else if(buff.compare(0, 21, "numAnimatedComponents") == 0)
      {
         sscanf(buff.c_str(), "numAnimatedComponents %d", 
               &numAnimatedComponents);
         if(numAnimatedComponents > 0)
         {
            /* Allocate memory for animation frame data */
            animFrameData = new float[numAnimatedComponents];
         }
         //std::cerr << "numAnimatedComponents: " 
         //          << numAnimatedComponents << std::endl;
      }
      else if(buff.compare(0, 11, "hierarchy {") == 0)
      {
         for (i = 0; i < totalJoints; ++i)
         {
            /* Read whole line */
            getline(file, buff);
            /* Clear leading spaces */
            buff.erase(0, buff.find_first_not_of(" \t"));

            /* Read joint info */
            sscanf(buff.c_str(), "%s %d %d %d", 
                  &buf2[0], &jointInfos[i].parent,
                  &jointInfos[i].flags, &jointInfos[i].startIndex);
            
            jointInfos[i].name = buf2;
            /* Remove name quotes */
            jointInfos[i].name.erase(0, 
                  jointInfos[i].name.find_first_of("\"")+1);
            jointInfos[i].name.erase(jointInfos[i].name.find_first_of("\""), 1);

            /*std::cerr << "Got Joint Info: " <<  jointInfos[i].name <<
                   " parent: " << jointInfos[i].parent << " flags " <<
                   jointInfos[i].flags << " startIndex: " 
                   << jointInfos[i].startIndex << std::endl;*/
         }
      }
      else if(buff.compare(0, 8, "bounds {") == 0)
      {
         for (i = 0; i < totalFrames; ++i)
         {
            /* Read whole line */
            getline(file, buff);
            /* Clear leading spaces */
            buff.erase(0, buff.find_first_not_of(" \t"));

            /* Read bounding box */
            sscanf(buff.c_str(), "( %f %f %f ) ( %f %f %f )",
                  &bboxes[i].min.x, &bboxes[i].min.y,
                  &bboxes[i].min.z, &bboxes[i].max.x,
                  &bboxes[i].max.y, &bboxes[i].max.z);
         }
      }
      else if(buff.compare(0, 11, "baseframe {") == 0)
      {
         for(i = 0; i < totalJoints; ++i)
         {
            /* Read whole line */
            getline(file, buff);
            /* Clear leading spaces */
            buff.erase(0, buff.find_first_not_of(" \t"));

            /* Read base frame joint */
            if(sscanf(buff.c_str(), " ( %f %f %f ) ( %f %f %f )",
                     &baseFrame[i].pos.x, &baseFrame[i].pos.y,
                     &baseFrame[i].pos.z, &baseFrame[i].orient.x,
                     &baseFrame[i].orient.y, &baseFrame[i].orient.z) == 6)
            {
             //std::cerr << " baseFrame[" << i << "] x: " << baseFrame[i].pos.x
             //     << " y: " << baseFrame[i].pos.y << " z: " 
             //   << baseFrame[i].pos.z << " orient (" << baseFrame[i].orient.x
             //     << " " << baseFrame[i].orient.y << " "
             //     << baseFrame[i].orient.z << ")" << std::endl;
               /* Compute the w component */
               baseFrame[i].orient.computeW();
            }
            else
            {
               std::cerr << "Error: Couldn't get baseFrame info for " 
                  << fileName << std::endl;
            }
         }
      }
      else if(buff.compare(0, 5, "frame") == 0)
      {
         sscanf(buff.c_str(), "frame %d", &frame_index);

         /* Read frame data */
         i = 0;
         while(i < numAnimatedComponents)
         {
            /* Read whole line */
            getline(file, buff);
            /* Clear leading spaces */
            buff.erase(0, buff.find_first_not_of(" \t"));

            while(sscanf(buff.c_str(), "%f", &animFrameData[i]) == 1)
            {
               /* Eat line up to next float */
               buff.erase(0, buff.find_first_of(" \t"));
               /* Clear leading spaces */
               buff.erase(0, buff.find_first_not_of(" \t"));

               i++;
            }
         }

         /* Build frame skeleton from the collected data */
         buildFrameSkeleton(jointInfos, baseFrame, animFrameData, frame_index);
      }
      
      /* Next list */
      getline(file, buff);
   }

   /* Close file */
   file.close();

   /* Clear temporary things */
   if(animFrameData)
   {
      delete[] animFrameData;
   }
   if(baseFrame)
   {
      delete[] baseFrame;
   }
   if(jointInfos)
   {
      delete[] jointInfos;
   }

   return(true);
}

/***********************************************************************
 *                         buildFrameSkeleton                          *
 ***********************************************************************/
void md5Anim::buildFrameSkeleton(md5_joint_info_t* jointInfos,
      md5_baseframe_joint_t *baseFrame, const float *animFrameData, 
      int frameNumber)
{
   int i;

   for(i = 0; i < totalJoints; ++i)
   {
      md5_baseframe_joint_t* baseJoint = &baseFrame[i];
      vec3_t animatedPos;
      quaternion animatedOrient;
      int j = 0;

      animatedPos = baseJoint->pos;
      animatedOrient = baseJoint->orient;

      if(jointInfos[i].flags & 1) /* Tx */
      {
         animatedPos.x = animFrameData[jointInfos[i].startIndex + j];
         ++j;
      }

      if(jointInfos[i].flags & 2) /* Ty */
      {
         animatedPos.y = animFrameData[jointInfos[i].startIndex + j];
         ++j;
      }

      if(jointInfos[i].flags & 4) /* Tz */
      {
         animatedPos.z = animFrameData[jointInfos[i].startIndex + j];
         ++j;
      }

      if(jointInfos[i].flags & 8) /* Qx */
      {
         animatedOrient.x = animFrameData[jointInfos[i].startIndex + j];
         ++j;
      }

      if(jointInfos[i].flags & 16) /* Qy */
      {
         animatedOrient.y = animFrameData[jointInfos[i].startIndex + j];
         ++j;
      }

      if(jointInfos[i].flags & 32) /* Qz */
      {
         animatedOrient.z = animFrameData[jointInfos[i].startIndex + j];
         ++j;
      }

      /* Compute orient quaternion's w value */
      animatedOrient.computeW();

      /* NOTE: we assume that this joint's parent has
         already been calculated, i.e. joint's ID should
         never be smaller than its parent ID. */
      struct md5_joint_t* thisJoint = &skelFrames[frameNumber][i];

      int parent = jointInfos[i].parent;
      thisJoint->parent = parent;
      thisJoint->name = jointInfos[i].name;

      /* Has parent? */
      if(thisJoint->parent < 0)
      {
         thisJoint->pos = animatedPos;
         thisJoint->orient = animatedOrient;
      }
      else
      {
         struct md5_joint_t *parentJoint = &skelFrames[frameNumber][parent];
         vec3_t rpos; /* Rotated position */

         /* Add positions */
         rpos = parentJoint->orient.rotatePoint(animatedPos);
         thisJoint->pos.x = rpos.x + parentJoint->pos.x;
         thisJoint->pos.y = rpos.y + parentJoint->pos.y;
         thisJoint->pos.z = rpos.z + parentJoint->pos.z;

         /* Concatenate rotations */
         thisJoint->orient = parentJoint->orient * animatedOrient;
         thisJoint->orient.normalize();
      }
   }
}

/***********************************************************************
 *                               reset                                 *
 ***********************************************************************/
void md5Anim::reset()
{
   lastTime = 0;
   curFrame = 0;
   nextFrame = 1;
}

/***********************************************************************
 *                               update                                *
 ***********************************************************************/
bool md5Anim::update(float delta, bool singleCycle)
{
   int maxFrames = totalFrames - 1;
   bool retVal = true;

   lastTime += delta;

   if(lastTime >= maxTime)
   {
      /* Frame time over, move to next frame */
      curFrame++;
      nextFrame++;
      lastTime = 0.0f;

      if(curFrame > maxFrames)
      {
         /* Reset cur frame */
         curFrame = 0;
         retVal = false;
      }

      if(nextFrame > maxFrames)
      {
         /* Reset next frame */
         nextFrame = 0;
      }
   }
   return(retVal);
}

/***********************************************************************
 *                            interpolate                              *
 ***********************************************************************/
md5_joint_t* md5Anim::interpolate()
{
   return(interpolate(skelFrames[curFrame], skelFrames[nextFrame]));
}

/***********************************************************************
 *                            interpolate                              *
 ***********************************************************************/
md5_joint_t* md5Anim::interpolate(md5_joint_t* skelA, md5_joint_t* skelB)
{
   int i;
   double interp = lastTime*frameRate;

   for(i = 0; i < totalJoints; ++i)
   {
      /* Copy parent index */
      retSkel[i].parent = skelA[i].parent;

      /* Linear interpolation for position */
      retSkel[i].pos.x = skelA[i].pos.x + interp * 
                                              (skelB[i].pos.x - skelA[i].pos.x);
      retSkel[i].pos.y = skelA[i].pos.y + interp * 
                                              (skelB[i].pos.y - skelA[i].pos.y);
      retSkel[i].pos.z = skelA[i].pos.z + interp * 
                                              (skelB[i].pos.z - skelA[i].pos.z);

      /* Spherical linear interpolation for orientation */
      retSkel[i].orient = skelA[i].orient.slerp(skelB[i].orient, interp);
   }

   return(retSkel);
}

/***********************************************************************
 *                       getCurrentBoundingBox                         *
 ***********************************************************************/
md5_bbox_t* md5Anim::getCurrentBoundingBox()
{
   return(&bboxes[curFrame]);
}

