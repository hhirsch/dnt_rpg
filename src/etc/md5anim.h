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

#ifndef _dnt_md5_anim_h
#define _dnt_md5_anim_h

#include <string>
#include "quaternion.h"
#include "list.h"

/*! Joint (skeleton bone) */
class md5_joint_t
{
   public:
      std::string name; /**< Joint name  */
      int parent;       /**< Parent ID */

      vec3_t pos;         /**< Joint position (local space) */
      quaternion orient;  /**< Joint orientation (local space) */
};

/* Joint info */
class md5_joint_info_t
{
   public:
      std::string name;
      int parent;
      int flags;
      int startIndex;
};

/* Base frame joint */
class md5_baseframe_joint_t
{
   public:
      vec3_t pos;
      quaternion orient;
};


/*! Bounding box */
class md5_bbox_t
{
   public:
      vec3_t min;
      vec3_t max;
};

/*! The MD5 anim is the abstraction of a single animation */
class md5Anim
{
   public:

      /* Constructor */
      md5Anim(int identifier);

      /*! Destructor */
      ~md5Anim();

      /*! Load a .md5anim file */
      bool load(std::string strFileName);

      /*! Update the animation, delta seconds after previous update
       * \param delta -> time after previous call (in seconds)
       * \param singleCycle -> if animation is just a single cycle call
       * \return false when animation ended! */
      bool update(float delta, bool singleCycle);

      /*! Reset the animation state */
      void reset();

      /*! Get the animation id */
      int getId(){return(id);};

      /*! Interpolate current and next skeletons 
       * \note the return value will be overriden on successive calls */
      md5_joint_t* interpolate();

      /*! Interpolate 2 skeletons
       * \note the return value will be overriden on successive calls */
      md5_joint_t* interpolate(md5_joint_t* skelA, md5_joint_t* skelB);

      /*! Get bounding box for current frame */
      md5_bbox_t* getCurrentBoundingBox();

   protected:
      /*! Delete all created structures */
      void clean();

      /*! Create the skeleton of frame (frameNumber) */
      void buildFrameSkeleton(md5_joint_info_t* jointInfos,
            md5_baseframe_joint_t *baseFrame, const float *animFrameData, 
            int frameNumber);

      std::string fileName; /**< Animation fileName */

      int id;    /**< Animation identifier */

      int totalFrames;    /**< Total frames in the animation */
      int totalJoints;    /**< Total joints in the animation */
      int frameRate;      /**< Animation frame rate */

      md5_joint_t** skelFrames;  /**< Each frame skeleton data */
      md5_bbox_t* bboxes;        /**< Each frame precomputed bounding box */

      md5_joint_t* retSkel; /**< Skeleton used to return */

      int curFrame;    /**< Current animation frame */
      int nextFrame;   /**< Next animation frame */

      double lastTime; /**< Last animation time */
      double maxTime;  /**< Max animation time */
};

#endif


