
#ifndef _dnt_part_editor_part_aux
#define _dnt_part_editor_part_aux


#include "../../particle/particle.h"


/*! The partAux class represents a particleSystem. It is defined
 * to have access to some particleSystem methods, only usefull for
 * partEditor use (i.e. no need to be implemented at the 
 * particleSystem class itself). */
class partAux: public particleSystem
{
   public:
      /*! Constructor */
      partAux():particleSystem(0,0){};
      /*! Destructor */
      ~partAux(){};

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
       
      void updateGravity(float grv);
      float getGravity();

      void updateInitR(float cor);
      float getInitR();
      void updateInitG(float cor);
      float getInitG();
      void updateInitB(float cor);
      float getInitB();

      void updateFinalR(float cor);
      float getFinalR();
      void updateFinalG(float cor);
      float getFinalG();
      void updateFinalB(float cor);
      float getFinalB();

      void updateAlpha(float a);
      float getAlpha();

      void updateDMultCenterX(float d);
      float getDMultCenterX();
      void updateDMultCenterY(float d);
      float getDMultCenterY();
      void updateDMultCenterZ(float d);
      float getDMultCenterZ();
   
      void updateDSumCenterX(float d);
      float getDSumCenterX();
      void updateDSumCenterY(float d);
      float getDSumCenterY();
      void updateDSumCenterZ(float d);
      float getDSumCenterZ();

      void updateDMultPosX(float d);
      float getDMultPosX();
      void updateDMultPosY(float d);
      float getDMultPosY();
      void updateDMultPosZ(float d);
      float getDMultPosZ();
   
      void updateDSumPosX(float d);
      float getDSumPosX();
      void updateDSumPosY(float d);
      float getDSumPosY();
      void updateDSumPosZ(float d);
      float getDSumPosZ();

      void updateDMultColorR(float d);
      float getDMultColorR();
      void updateDMultColorG(float d);
      float getDMultColorG();
      void updateDMultColorB(float d);
      float getDMultColorB();
   
      void updateDSumColorR(float d);
      float getDSumColorR();
      void updateDSumColorG(float d);
      float getDSumColorG();
      void updateDSumColorB(float d);
      float getDSumColorB();

      void updateDMultVelX(float d);
      float getDMultVelX();
      void updateDMultVelY(float d);
      float getDMultVelY();
      void updateDMultVelZ(float d);
      float getDMultVelZ();
   
      void updateDSumVelX(float d);
      float getDSumVelX();
      void updateDSumVelY(float d);
      float getDSumVelY();
      void updateDSumVelZ(float d);
      float getDSumVelZ();

};


#endif


