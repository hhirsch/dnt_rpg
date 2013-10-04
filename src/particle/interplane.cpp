/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
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

#include "interplane.h"
#include "particle.h"

/****************************************************************************
 *                               Constuctor                                 *
 ****************************************************************************/
interPlaneList::interPlaneList():dntList(DNT_LIST_TYPE_ADD_AT_END)
{
}

/****************************************************************************
 *                               Destuctor                                  *
 ****************************************************************************/
interPlaneList::~interPlaneList()
{
   clearList();
}

/****************************************************************************
 *                              freeElement                                 *
 ****************************************************************************/
void interPlaneList::freeElement(dntListElement* obj)
{
   interPlane* pl = (interPlane*)obj;
   delete(pl);
}

/****************************************************************************
 *                            intersectPlanes                               *
 ****************************************************************************/
bool interPlaneList::intersectPlanes(particle* p, float* dX, float* dZ)
{
   int i;
   float yOnPlane = 0;
   float size;
   
   interPlane* ip = (interPlane*)getFirst();

   for(i = 0; i < getTotal(); i++)
   {
      if( (p->posX <= ip->x2) && 
          (p->posX >= ip->x1) &&
          (p->posZ <= ip->z2) && 
          (p->posZ >= ip->z1) )
      {
         switch(ip->inclination)
         { 
             case PLANE_NO_INCLINATION: 
                   yOnPlane = ip->y1;
             break;
             case PLANE_INCLINATION_X:
             {
                size = (ip->x2 - ip->x1);
                yOnPlane = ((ip->x2 - p->posX) / size) * ip->y1 +
                           ((p->posX - ip->x1) / size) * ip->y2;
             }
             break;
             case PLANE_INCLINATION_Z:
             {
                size = (ip->z2 - ip->z1);
                yOnPlane = ((ip->z2 - p->posZ) / size) * ip->y1 +
                           ((p->posZ - ip->z1) / size) * ip->y2;
             }
             break;
         }
         if( ((p->posY >= yOnPlane - 1) && (p->posY <= yOnPlane + 1)) )
         {
            *dX = ip->dX;
            *dZ = ip->dZ;
            return(true);
         }
      }
      ip = (interPlane*)ip->getNext();
   }
   return(false);
}

