#ifndef _partSystem_h
#define _partSystem_h

#include "particle.h"
#include "part1.h"
#include "part2.h"
#include "part3.h"
#include "part4.h"
#include "part5.h"
#include "part6.h"
#include "part7.h"
#include <string>
using namespace std;

#define PART_WATERFALL     1
#define PART_FIRE          2
#define PART_WATER_SURFACE 3
#define PART_SMOKE         4
#define PART_BLOOD         5
#define PART_LIGHTNING     6
#define PART_SNOW          7


#define MAX_WATERFALL     2
#define MAX_FIRE          5
#define MAX_SMOKE         5
#define MAX_WATER_SURFACE 3
#define MAX_BLOOD         10
#define MAX_SNOW          1
#define MAX_LIGHTNING     3

class partSystem
{
   public:
      partSystem();
      ~partSystem();

      void actualizeAll();
      particle* addParticle(int type, GLfloat X, GLfloat Y, GLfloat Z,
                            string fileName );
      void removeParticle(int type, particle* part);
      int numParticles();

   private:
      part1* waterfall[MAX_WATERFALL];
      part2* fire[MAX_FIRE];
      part3* waterSurface[MAX_WATER_SURFACE];
      part4* smoke[MAX_SMOKE];
      part5* blood[MAX_BLOOD];
      part6* lightning[MAX_LIGHTNING];
      part7* snow[MAX_SNOW];

};


#endif
