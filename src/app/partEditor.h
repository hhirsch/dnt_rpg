#ifndef _partEditor_h
#define _partEditor_h

#include "../gui/farso.h"
#include "../particle/particle.h"
#include "../particle/part1.h"
#include "../particle/part2.h"
#include "../particle/part3.h"
#include "../particle/part4.h"
#include "../particle/part5.h"
#include "../particle/part6.h"
#include "../engine/culling.h"
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class part: public particleSystem
{
   public:
      part():particleSystem(0,0){};
      ~part(){};

      void updateMaxLive(int mLive);
      int getMaxLive();

      void updateMaxParticles(int mLive);
      int getMaxParticles();

      void updateCenterX(float center);
      float getCenterX();
      void updateCenterY(float center);
      float getCenterY();
      void updateCenterZ(float center);
      float getCenterZ();



#if 0     
      int maxParticles;        /* Max number of particles */
      float* vertexArray;      /* Vertex array to draw */
      float* colorArray;       /* Color array to draw */
      float centerX,           /* X initial position */
            centerY,           /* Y initial position */
            centerZ;           /* Z initial position */
      float gravity;           /* Force of Gravity */
      float initR,             /* Initial Red Color */ 
            initG,             /* Initial Green Color */
            initB;             /* Initial Blue Color */
      float finalR,            /* Final Red Color */
            finalG,            /* Final Green Color */
            finalB;            /* Final Blue Color */
      float alpha;             /* Global Alpha Value */

      /* Probability Values */
 
      float dMultCenter[3];    /* Multiply values to Center Position */
      float dSumCenter[3];     /* Sum Values to Center Position */

      float dMultPos[3];       /* Multiply values to Actual Position */
      float dSumPos[3];        /* Sum Values to Actual Position */

      float dMultColor[3];     /* Multiply values to inital Color */
      float dSumColor[3];      /* Sum Values to initial Color */

      float dMultVel[3];       /* Multiply values to Actual Velocity */
      float dSumVel[3]
#endif

};

#endif
 
