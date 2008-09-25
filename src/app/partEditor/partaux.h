
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

      void setMaxLive(int mLive);
      int getMaxLive();

      void setMaxParticles(int mLive);
      int getMaxParticles();

      void setCenterX(float center);
      float getCenterX();
      void setCenterY(float center);
      float getCenterY();
      void setCenterZ(float center);
      float getCenterZ();
       
      void setGravity(float grv);
      float getGravity();

      void setInitR(float cor);
      float getInitR();
      void setInitG(float cor);
      float getInitG();
      void setInitB(float cor);
      float getInitB();

      void setFinalR(float cor);
      float getFinalR();
      void setFinalG(float cor);
      float getFinalG();
      void setFinalB(float cor);
      float getFinalB();

      void setAlpha(float a);
      float getAlpha();

      void setDMultCenterX(float d);
      float getDMultCenterX();
      void setDMultCenterY(float d);
      float getDMultCenterY();
      void setDMultCenterZ(float d);
      float getDMultCenterZ();
   
      void setDSumCenterX(float d);
      float getDSumCenterX();
      void setDSumCenterY(float d);
      float getDSumCenterY();
      void setDSumCenterZ(float d);
      float getDSumCenterZ();

      void setDMultPosX(float d);
      float getDMultPosX();
      void setDMultPosY(float d);
      float getDMultPosY();
      void setDMultPosZ(float d);
      float getDMultPosZ();
   
      void setDSumPosX(float d);
      float getDSumPosX();
      void setDSumPosY(float d);
      float getDSumPosY();
      void setDSumPosZ(float d);
      float getDSumPosZ();

      void setDMultColorR(float d);
      float getDMultColorR();
      void setDMultColorG(float d);
      float getDMultColorG();
      void setDMultColorB(float d);
      float getDMultColorB();
   
      void setDSumColorR(float d);
      float getDSumColorR();
      void setDSumColorG(float d);
      float getDSumColorG();
      void setDSumColorB(float d);
      float getDSumColorB();

      void setDMultVelX(float d);
      float getDMultVelX();
      void setDMultVelY(float d);
      float getDMultVelY();
      void setDMultVelZ(float d);
      float getDMultVelZ();
   
      void setDSumVelX(float d);
      float getDSumVelX();
      void setDSumVelY(float d);
      float getDSumVelY();
      void setDSumVelZ(float d);
      float getDSumVelZ();

};


#endif


