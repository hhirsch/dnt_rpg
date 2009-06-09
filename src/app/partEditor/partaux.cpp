/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#include "partaux.h"

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//                                part                                    //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

int partAux::getDrawMode()
{
   return(drawMode);
}

void partAux::setMaxLive(int mLive)
{
   maxLive = mLive;
}

int partAux::getMaxLive()
{
   return(maxLive);
}

void partAux::setMaxParticles(int mLive)
{
   maxParticles = mLive;
}

int partAux::getMaxParticles()
{
   return(maxParticles);
}

void partAux::setCenterX(float center)
{
   centerX = center;
}

float partAux::getCenterX()
{
   return(centerX);
}

void partAux::setCenterY(float center)
{
   centerY = center;
}

float partAux::getCenterY()
{
   return(centerY);
}

void partAux::setCenterZ(float center)
{
   centerZ = center;
}

float partAux::getCenterZ()
{
   return(centerZ);
}


void partAux::setGravity(float grv)
{
   gravity = grv;
}

float partAux::getGravity()
{
   return(gravity);
}


void partAux::setInitR(float cor)
{
   initR = cor;
}

float partAux::getInitR()
{
  return(initR);
}

void partAux::setInitG(float cor)
{
   initG = cor;
}

float partAux::getInitG()
{
  return(initG);
}

void partAux::setInitB(float cor)
{
   initB = cor;
}

float partAux::getInitB()
{
  return(initB);
}

void partAux::setFinalR(float cor)
{
   finalR = cor;
}

float partAux::getFinalR()
{
  return(finalR);
}

void partAux::setFinalG(float cor)
{
   finalG = cor;
}

float partAux::getFinalG()
{
  return(finalG);
}

void partAux::setFinalB(float cor)
{
   finalB = cor;
}

float partAux::getFinalB()
{
  return(finalB);
}

void partAux::setAlpha(float a)
{
  alpha = a;
}

float partAux::getAlpha()
{
  return(alpha);
}

void partAux::setDMultCenterX(float d)
{
   dMultCenter[0] = d;
}
float partAux::getDMultCenterX()
{
   return(dMultCenter[0]);
}

void partAux::setDMultCenterY(float d)
{
   dMultCenter[1] = d;
}
float partAux::getDMultCenterY()
{
   return(dMultCenter[1]);
}

void partAux::setDMultCenterZ(float d)
{
   dMultCenter[2] = d;
}
float partAux::getDMultCenterZ()
{
   return(dMultCenter[2]);
}

void partAux::setDSumCenterX(float d)
{
   dSumCenter[0] = d;
}
float partAux::getDSumCenterX()
{
   return(dSumCenter[0]);
}

void partAux::setDSumCenterY(float d)
{
   dSumCenter[1] = d;
}
float partAux::getDSumCenterY()
{
   return(dSumCenter[1]);
}

void partAux::setDSumCenterZ(float d)
{
   dSumCenter[2] = d;
}
float partAux::getDSumCenterZ()
{
   return(dSumCenter[2]);
}

void partAux::setDMultPosX(float d)
{
   dMultPos[0] = d;
}
float partAux::getDMultPosX()
{
   return(dMultPos[0]);
}

void partAux::setDMultPosY(float d)
{
   dMultPos[1] = d;
}
float partAux::getDMultPosY()
{
   return(dMultPos[1]);
}

void partAux::setDMultPosZ(float d)
{
   dMultPos[2] = d;
}
float partAux::getDMultPosZ()
{
   return(dMultPos[2]);
}

void partAux::setDSumPosX(float d)
{
   dSumPos[0] = d;
}
float partAux::getDSumPosX()
{
   return(dSumPos[0]);
}

void partAux::setDSumPosY(float d)
{
   dSumPos[1] = d;
}
float partAux::getDSumPosY()
{
   return(dSumPos[1]);
}

void partAux::setDSumPosZ(float d)
{
   dSumPos[2] = d;
}
float partAux::getDSumPosZ()
{
   return(dSumPos[2]);
}

void partAux::setDMultVelX(float d)
{
   dMultVel[0] = d;
}
float partAux::getDMultVelX()
{
   return(dMultVel[0]);
}

void partAux::setDMultVelY(float d)
{
   dMultVel[1] = d;
}
float partAux::getDMultVelY()
{
   return(dMultVel[1]);
}

void partAux::setDMultVelZ(float d)
{
   dMultVel[2] = d;
}
float partAux::getDMultVelZ()
{
   return(dMultVel[2]);
}

void partAux::setDSumVelX(float d)
{
   dSumVel[0] = d;
}
float partAux::getDSumVelX()
{
   return(dSumVel[0]);
}

void partAux::setDSumVelY(float d)
{
   dSumVel[1] = d;
}
float partAux::getDSumVelY()
{
   return(dSumVel[1]);
}

void partAux::setDSumVelZ(float d)
{
   dSumVel[2] = d;
}
float partAux::getDSumVelZ()
{
   return(dSumVel[2]);
}

void partAux::setDMultColorR(float d)
{
   dMultColor[0] = d;
}
float partAux::getDMultColorR()
{
   return(dMultColor[0]);
}

void partAux::setDMultColorG(float d)
{
   dMultColor[1] = d;
}
float partAux::getDMultColorG()
{
   return(dMultColor[1]);
}

void partAux::setDMultColorB(float d)
{
   dMultColor[2] = d;
}
float partAux::getDMultColorB()
{
   return(dMultColor[2]);
}

void partAux::setDSumColorR(float d)
{
   dSumColor[0] = d;
}
float partAux::getDSumColorR()
{
   return(dSumColor[0]);
}

void partAux::setDSumColorG(float d)
{
   dSumColor[1] = d;
}
float partAux::getDSumColorG()
{
   return(dSumColor[1]);
}

void partAux::setDSumColorB(float d)
{
   dSumColor[2] = d;
}
float partAux::getDSumColorB()
{
   return(dSumColor[2]);
}

void partAux::setInitVelX(float d)
{
   initVelX = d;
}
float partAux::getInitVelX()
{
   return(initVelX);
}

void partAux::setInitVelY(float d)
{
   initVelY = d;
}
float partAux::getInitVelY()
{
   return(initVelY);
}

void partAux::setInitVelZ(float d)
{
   initVelZ = d;
}
float partAux::getInitVelZ()
{
   return(initVelZ);
}

