
#include "partaux.h"

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                part                                    //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

void partAux::updateMaxLive(int mLive)
{
   maxLive = mLive;
}

int partAux::getMaxLive()
{
   return(maxLive);
}

void partAux::updateMaxParticles(int mLive)
{
   maxParticles = mLive;
}

int partAux::getMaxParticles()
{
   return(maxParticles);
}

void partAux::updateCenterX(float center)
{
   centerX = center;
}

float partAux::getCenterX()
{
   return(centerX);
}

void partAux::updateCenterY(float center)
{
   centerY = center;
}

float partAux::getCenterY()
{
   return(centerY);
}

void partAux::updateCenterZ(float center)
{
   centerZ = center;
}

float partAux::getCenterZ()
{
   return(centerZ);
}


void partAux::updateGravity(float grv)
{
   gravity = grv;
}

float partAux::getGravity()
{
   return(gravity);
}


void partAux::updateInitR(float cor)
{
   initR = cor;
}

float partAux::getInitR()
{
  return(initR);
}

void partAux::updateInitG(float cor)
{
   initG = cor;
}

float partAux::getInitG()
{
  return(initG);
}

void partAux::updateInitB(float cor)
{
   initB = cor;
}

float partAux::getInitB()
{
  return(initB);
}

void partAux::updateFinalR(float cor)
{
   finalR = cor;
}

float partAux::getFinalR()
{
  return(finalR);
}

void partAux::updateFinalG(float cor)
{
   finalG = cor;
}

float partAux::getFinalG()
{
  return(finalG);
}

void partAux::updateFinalB(float cor)
{
   finalB = cor;
}

float partAux::getFinalB()
{
  return(finalB);
}

void partAux::updateAlpha(float a)
{
  alpha = a;
}

float partAux::getAlpha()
{
  return(alpha);
}

void partAux::updateDMultCenterX(float d)
{
   dMultCenter[0] = d;
}
float partAux::getDMultCenterX()
{
   return(dMultCenter[0]);
}

void partAux::updateDMultCenterY(float d)
{
   dMultCenter[1] = d;
}
float partAux::getDMultCenterY()
{
   return(dMultCenter[1]);
}

void partAux::updateDMultCenterZ(float d)
{
   dMultCenter[2] = d;
}
float partAux::getDMultCenterZ()
{
   return(dMultCenter[2]);
}

void partAux::updateDSumCenterX(float d)
{
   dSumCenter[0] = d;
}
float partAux::getDSumCenterX()
{
   return(dSumCenter[0]);
}

void partAux::updateDSumCenterY(float d)
{
   dSumCenter[1] = d;
}
float partAux::getDSumCenterY()
{
   return(dSumCenter[1]);
}

void partAux::updateDSumCenterZ(float d)
{
   dSumCenter[2] = d;
}
float partAux::getDSumCenterZ()
{
   return(dSumCenter[2]);
}

void partAux::updateDMultPosX(float d)
{
   dMultPos[0] = d;
}
float partAux::getDMultPosX()
{
   return(dMultPos[0]);
}

void partAux::updateDMultPosY(float d)
{
   dMultPos[1] = d;
}
float partAux::getDMultPosY()
{
   return(dMultPos[1]);
}

void partAux::updateDMultPosZ(float d)
{
   dMultPos[2] = d;
}
float partAux::getDMultPosZ()
{
   return(dMultPos[2]);
}

void partAux::updateDSumPosX(float d)
{
   dSumPos[0] = d;
}
float partAux::getDSumPosX()
{
   return(dSumPos[0]);
}

void partAux::updateDSumPosY(float d)
{
   dSumPos[1] = d;
}
float partAux::getDSumPosY()
{
   return(dSumPos[1]);
}

void partAux::updateDSumPosZ(float d)
{
   dSumPos[2] = d;
}
float partAux::getDSumPosZ()
{
   return(dSumPos[2]);
}

void partAux::updateDMultVelX(float d)
{
   dMultVel[0] = d;
}
float partAux::getDMultVelX()
{
   return(dMultVel[0]);
}

void partAux::updateDMultVelY(float d)
{
   dMultVel[1] = d;
}
float partAux::getDMultVelY()
{
   return(dMultVel[1]);
}

void partAux::updateDMultVelZ(float d)
{
   dMultVel[2] = d;
}
float partAux::getDMultVelZ()
{
   return(dMultVel[2]);
}

void partAux::updateDSumVelX(float d)
{
   dSumVel[0] = d;
}
float partAux::getDSumVelX()
{
   return(dSumVel[0]);
}

void partAux::updateDSumVelY(float d)
{
   dSumVel[1] = d;
}
float partAux::getDSumVelY()
{
   return(dSumVel[1]);
}

void partAux::updateDSumVelZ(float d)
{
   dSumVel[2] = d;
}
float partAux::getDSumVelZ()
{
   return(dSumVel[2]);
}

void partAux::updateDMultColorR(float d)
{
   dMultColor[0] = d;
}
float partAux::getDMultColorR()
{
   return(dMultColor[0]);
}

void partAux::updateDMultColorG(float d)
{
   dMultColor[1] = d;
}
float partAux::getDMultColorG()
{
   return(dMultColor[1]);
}

void partAux::updateDMultColorB(float d)
{
   dMultColor[2] = d;
}
float partAux::getDMultColorB()
{
   return(dMultColor[2]);
}

void partAux::updateDSumColorR(float d)
{
   dSumColor[0] = d;
}
float partAux::getDSumColorR()
{
   return(dSumColor[0]);
}

void partAux::updateDSumColorG(float d)
{
   dSumColor[1] = d;
}
float partAux::getDSumColorG()
{
   return(dSumColor[1]);
}

void partAux::updateDSumColorB(float d)
{
   dSumColor[2] = d;
}
float partAux::getDSumColorB()
{
   return(dSumColor[2]);
}
