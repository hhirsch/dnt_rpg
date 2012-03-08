/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#include "collision.h"
#include "util.h"


/*********************************************************************
 *                            Constructor                            *
 *********************************************************************/
collision::collision()
{
   actualMap = NULL;
   NPCs = NULL;
   PCs = NULL;
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
collision::~collision()
{
   actualMap = NULL;
   NPCs = NULL;
   PCs = NULL;
}

/*********************************************************************
 *                             defineMap                             *
 *********************************************************************/
void collision::defineMap(Map* usedMap, characterList* npcs,
                          characterList* pcs)
{
   actualMap = usedMap;
   NPCs = npcs;
   PCs = pcs;
}

/*********************************************************************
 *                           verifySquare                            *
 *********************************************************************/
bool collision::verifySquare(boundingBox& actorBox, Square* quad,
                             GLfloat& varHeight, GLfloat curHeight)
{
   boundingBox colBox;

   Square* proxima = quad;
   if(!(proxima->flags & SQUARE_CAN_WALK))
   {
     /* Can't walk on square, collision! */
     return(false);
   }

   /* test with walls */
   int mur = 0;
   while((mur < MAX_WALLS ) && (proxima->walls[mur] != NULL))
   {
      colBox.setMin(proxima->walls[mur]->x1, 0, proxima->walls[mur]->z1);
      colBox.setMax(proxima->walls[mur]->x2, WALL_HEIGHT, 
            proxima->walls[mur]->z2);
      if(actorBox.intercepts(colBox))
      {
         return(false);
      }
      mur++;
   }

   /* test with objects */
   int ob = 0;
   objSquare* sobj = proxima->getFirstObject();
   while( (ob < proxima->getTotalObjects())) 
   {
      if(sobj->colision)
      {
         /* Get Bounding box */
         colBox = sobj->obj->scNode->getBoundingBox();

         if(actorBox.intercepts(colBox))
         {
            /* If the bounding boxes intercepts, we'll need to do a more 
             * depth collision verify, so it is */
            if(sobj->obj->depthCollision(sobj->angleX, sobj->angleY,
                     sobj->angleZ, sobj->x, 
                     sobj->y +
                     actualMap->getHeight(sobj->x,sobj->z),
                     sobj->z, actorBox))
            {
               /* So if the depth collision is true, verify if can go up
                * the position. if can't, the position is 'unwalkable' now! */
               if( ((colBox.y2 + sobj->y) - curHeight) <= 1.5)
               {
                  /* Can walk above the object */
                  varHeight = ((colBox.y2+sobj->y)>varHeight)?
                     colBox.y2+sobj->y:varHeight;
               }
               else
               {
                  return(false);
               }
            }
         }
      }
      ob++;
      sobj = (objSquare*)sobj->getNext();
   }

   return(true);
}

/*********************************************************************
 *                              canWalk                              *
 *********************************************************************/
bool collision::canWalk(character* actor, GLfloat varX, GLfloat varY,
                        GLfloat varZ, GLfloat varAlpha, 
                        GLfloat& varHeight, GLfloat& nx, 
                        GLfloat& nz, bool usePosition)
{
   boundingBox actorBox;

   /* Verify if there is an actor!  */
   if(!actor)
   {
      return(false);
   }

   /* set Character Positions */
   GLfloat perX = varX;
   GLfloat perY = varY;
   GLfloat perZ = varZ;
   GLfloat perOrientation = actor->scNode->getAngleY() + varAlpha;
   Square* perQuad = NULL;

   if(usePosition)
   {
      perX += actor->scNode->getPosX();
      perY += actor->scNode->getPosY();
      perZ += actor->scNode->getPosZ();
      perQuad = actor->ocSquare;
   }

   if(!actualMap)
   {
      return(false);
   }

   if(!perQuad)
   {
      /* Get the Relative Square */
      int posX =(int)floor( perX / (actualMap->squareSize()));
      int posZ =(int)floor( perZ / (actualMap->squareSize()));
      perQuad = actualMap->relativeSquare(posX, posZ);
      if(!perQuad)
      {
         return(false);
      }
   }

   /* Calculate the bounding box of the actor at the new position */
   actorBox = actor->scNode->getModel()->getCrudeBoundingBox();
   actorBox.rotate(0.0f, perOrientation, 0.0f);
   actorBox.translate(perX, perY, perZ);

   /* Do the check */
   return(canWalk(actor, actorBox, actor->scNode->getPosY(), perQuad, 
            varHeight, nx, nz));
}

/*********************************************************************
 *                              canWalk                              *
 *********************************************************************/
bool collision::canWalk(character* actor, boundingBox& actorBox, 
      GLfloat curPerY, Square* perQuad, GLfloat& varHeight, 
      GLfloat& nx, GLfloat& nz)
{
   bool result = true;
   int i, j;
   Square* saux;
   boundingBox colBox;

   varHeight = 0.0;

   /* Test map limits */
   if( (actorBox.x1 < 2) || (actorBox.z1 < 2) || 
       (actorBox.x2 > actualMap->getSizeX()*actualMap->squareSize()-2) || 
       (actorBox.z2 > actualMap->getSizeZ()*actualMap->squareSize()-2))
   {
      return(false);
   }

   /* Test Doors */
   door* door1 = actualMap->getFirstDoor();
   for(i=0; i < actualMap->getTotalDoors(); i++)
   {
      colBox = door1->obj->scNode->getBoundingBox();
      if(actorBox.intercepts(colBox))
      {
         return(false);
      }
      door1 = (door*)door1->getNext();
   }

   /* Test the actual square, since is BIG ! */
   colBox.setMin(perQuad->x1, 0, perQuad->z1);
   colBox.setMax(perQuad->x2, 400, perQuad->z2);
   if(actorBox.intercepts(colBox))
   {
      result &= verifySquare(actorBox, perQuad, varHeight, curPerY);
      if(!result)
      {
         return(false);
      }
   }

 
   /* Test near squares */
   saux = actualMap->relativeSquare(perQuad->posX+1, perQuad->posZ);
   if(saux) 
   { 
      /* lest */
      colBox.setMin(saux->x1, 0, saux->z1);
      colBox.setMax(saux->x2, 400, saux->z2);
      if(actorBox.intercepts(colBox) )
      {
         result &= verifySquare(actorBox, saux,varHeight, curPerY);
         if(!result)
         {
            return(false);
         }
      }
      /* Northest */
      saux = actualMap->relativeSquare(perQuad->posX+1, perQuad->posZ-1);
      if( saux )
      {
         colBox.setMin(saux->x1, 0, saux->z1);
         colBox.setMax(saux->x2, 400, saux->z2);
         if(actorBox.intercepts(colBox) )
         {
            result &= verifySquare(actorBox, saux,varHeight, curPerY);
            if(!result) 
            {
               return(false);
            }
         }
      }
      /* Southest */
      saux = actualMap->relativeSquare(perQuad->posX+1, perQuad->posZ+1);
      if( saux )
      {
         colBox.setMin(saux->x1, 0, saux->z1);
         colBox.setMax(saux->x2, 400, saux->z2);
         if(actorBox.intercepts(colBox) )
         {
            result &= verifySquare(actorBox,saux,varHeight, curPerY);
            if(!result) 
            {
               return(false);
            }
         }
      }
   }

   /* Test left walls */
   saux = actualMap->relativeSquare(perQuad->posX-1, perQuad->posZ);
   if( saux ) 
   { 
      /* west */
      colBox.setMin(saux->x1, 0, saux->z1);
      colBox.setMax(saux->x2, 400, saux->z2);
      if(actorBox.intercepts(colBox) )
      {
         result &= verifySquare(actorBox,saux,varHeight, curPerY);
         if(!result) 
         {
            return(false);
         }
      }

      /* Northest */
      saux = actualMap->relativeSquare(perQuad->posX-1, perQuad->posZ-1);
      if( saux )
      {
         colBox.setMin(saux->x1, 0, saux->z1);
         colBox.setMax(saux->x2, 400, saux->z2);
         if(actorBox.intercepts(colBox) )
         {
            result &= verifySquare(actorBox,saux,varHeight, curPerY);
            if(!result) 
            {
               return(false);
            }
         }
      }
      /* Southest */
      saux = actualMap->relativeSquare(perQuad->posX-1, perQuad->posZ+1);
      if( saux )
      {
         colBox.setMin(saux->x1, 0, saux->z1);
         colBox.setMax(saux->x2, 400, saux->z2);
         if(actorBox.intercepts(colBox) )
         {
            result &=verifySquare(actorBox,saux,varHeight, curPerY);
            if(!result) 
            {
               return(false);
            }
         }
      }
   }
  
   /* Test down squares */
   saux = actualMap->relativeSquare(perQuad->posX, perQuad->posZ+1);
   if( saux )
   {
      colBox.setMin(saux->x1, 0, saux->z1);
      colBox.setMax(saux->x2, 400, saux->z2);
      if(actorBox.intercepts(colBox) )
      { 
         /* south */
         result &= verifySquare(actorBox,saux,varHeight, curPerY);
         if(!result) 
         {
            return(false);
         }
      }
   }

   /* Up Squares */
   saux = actualMap->relativeSquare(perQuad->posX, perQuad->posZ-1);
   if( saux )
   {  
      colBox.setMin(saux->x1, 0, saux->z1);
      colBox.setMax(saux->x2, 400, saux->z2);
      if(actorBox.intercepts(colBox) )
      { 
         /* north */
         result &= verifySquare(actorBox,saux,varHeight, curPerY);
         if(!result) 
         {
            return(false);
         }
      }
   }

   /* Test colision with npcs and pcs */
   characterList* list = NPCs;
   for(i = 0; i <=1; i++)
   {
      if(list)
      {
         character* pers = (character*)list->getFirst();
         for(j = 0; j < list->getTotal(); j++)
         {
            if(pers != actor)
            {
               colBox = pers->scNode->getBoundingBox();
               if(actorBox.intercepts(colBox))
               {
                  /* Do a more depth colision verify */
                  if(pers->scNode->getModel()->depthCollision(
                           0.0f, pers->scNode->getAngleY(), 0.0f,
                           pers->scNode->getPosX(), 
                           pers->scNode->getPosY(),
                           pers->scNode->getPosZ(), actorBox))
                  {
                     return(false);
                  }
               }
            }
            pers = (character*)pers->getNext();
         }
      }
      /* Change from NPC List to PC list */
      list = PCs;
   }

   nx = ((actorBox.x1 + actorBox.x2) / 2);
   nz = ((actorBox.z1 + actorBox.z2) / 2);
      
   return(result);
}


/*********************************************************************
 *                            verifyLine                             *
 *********************************************************************/
bool collision::canWalk(character* actor, GLfloat x, GLfloat y, GLfloat z, 
      GLfloat prevX, GLfloat prevY, GLfloat prevZ)
{

#warning "Fix to pass on open doors!"

   boundingBox actorBox;

   /* Some unused returned values */
   GLfloat varHeight=0.0f, nX=0.0f, nZ=0.0f;

   /* Let's calculate the expanded BoundingBox */
   actorBox = actor->scNode->getModel()->getCrudeBoundingBox();
   /* Expand depending on direction */
   float dif;
   dif = fabs(x - prevX);
   actorBox.x1 -= dif;
   actorBox.x2 += dif;
   dif = fabs(y - prevY);
   actorBox.y1 -= dif;
   actorBox.y2 += dif;
   dif = fabs(z - prevZ);
   actorBox.z1 -= dif;
   actorBox.z2 += dif;

   /* Must do the check at the middle position */
   float midX = (x + prevX) / 2.0f;
   float midY = (y + prevY) / 2.0f;
   float midZ = (z + prevZ) / 2.0f;
   
   /* Now just rotate and set position */
   actorBox.rotate(0.0f, actor->scNode->getAngleY(), 0.0f);
   actorBox.translate(midX, midY, midZ);

   /* And Calculate square occupied */
   int posX =(int)floor(midX / (actualMap->squareSize()));
   int posZ =(int)floor(midZ / (actualMap->squareSize()));
   Square* perQuad = actualMap->relativeSquare(posX, posZ);
   if(!perQuad)
   {
      return(false);
   }

   return(canWalk(actor, actorBox, midY, perQuad, varHeight, nX, nZ));
#if 0
   GLfloat dX = fabs(x - prevX);
   GLfloat dY = fabs(y - prevY);
   GLfloat dZ = fabs(z - prevZ);
   int steps = 1;
   int i;
   GLfloat minVar = 4.0f;
   GLfloat pX, pY, pZ;

   /* First, calculate the number of steps */
   if( (dX >= dY) && (dX >= dZ) )
   {
      /* X is greater */
      steps = (int)floor(dX / minVar) + 1;
   }
   else if(dZ >= dY)
   {
      /* Z is greater */
      steps = (int)floor(dZ / minVar) + 1;
   }
   else
   {
      /* Y is greater */
      steps = (int)floor(dY / minVar) + 1;
   }

   printf("Steps: %d ", steps);

   /* Calculate Variation */
   dX = dX / steps;
   dY = dY / steps;
   dZ = dZ / steps;


   pX = prevX;
   pY = prevY;
   pZ = prevZ;
   for(i=1; i <= steps; i++)
   {
      if(i == steps)
      {
         /* Last step with the destiny */
         pX = x;
         pY = y;
         pZ = z;
      }

      if(!canWalk(actor, pX, pY, pZ, actor->scNode->getAngleY(), 
               varHeight, nX, nZ, false))
      {
         /* Can't walk, so no more need to test further */
         return(false);
      }


      pX += dX;
      pY += dY;
      pZ += dZ;
   }

   /* Could walk... finally! */
   return(true);
#endif
}

