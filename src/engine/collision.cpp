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
bool collision::verifySquare(GLfloat min[3], GLfloat max[3], Square* quad,
                             GLfloat& varHeight, GLfloat curHeight)
{
   GLfloat min2[3];
   GLfloat max2[3];

   Square* proxima = quad;
   if(proxima->flags != SQUARE_CAN_WALK)
   {
     return(false);
   }
   
   /* test with walls */
   {
      int mur = 0;
      while((mur < MAX_WALLS ) && (proxima->walls[mur] != NULL))
      {
         min2[0] = proxima->walls[mur]->x1; 
         min2[1] = 0; 
         min2[2] = proxima->walls[mur]->z1;
         max2[0] = proxima->walls[mur]->x2; 
         max2[1] = WALL_HEIGHT; 
         max2[2] = proxima->walls[mur]->z2;
         if(intercepts(min,max,min2,max2))
         {
           return(false);
         }
         mur++;
      }
   }
   /* test with objects */
   {
      int ob = 0;
      boundingBox bounding;
      GLfloat X[4], Z[4];
      objSquare* sobj = proxima->getFirstObject();
      while( (ob < proxima->getTotalObjects())) 
      {
        if(sobj->colision)
        {
          bounding = sobj->obj->getBoundingBox();
          X[0] = bounding.x1;
          Z[0] = bounding.z1;
          X[1] = bounding.x1;
          Z[1] = bounding.z2;
          X[2] = bounding.x2;
          Z[2] = bounding.z2;
          X[3] = bounding.x2;
          Z[3] = bounding.z1;
          
          rotTransBoundingBox(sobj->orientation, X, Z, sobj->x, 
                              sobj->y+bounding.y1, 
                              sobj->y+bounding.y2, sobj->z, min2, max2);
          if(intercepts(min,max,min2,max2))
          {
             cout << "collided: " << sobj->obj->name << endl;
             /* If the bounding boxes intercepts, we'll need to do a more 
              * depth collision verify, so it is */
             if(sobj->obj->depthCollision(sobj->orientation, sobj->x, 
                                          sobj->y +
                                          actualMap->getHeight(sobj->x,sobj->z),
                                          sobj->z,min,max))
             {
                cout << "still collided: " << sobj->obj->name << endl;
                /* So if the depth collision is true, verify if can go up
                 * the position. if can't, the position is 'unwalkable' now! */
                if( ((bounding.y2+sobj->y) - curHeight) <= 1.5)
                {
                   /* Can walk above the object */
                   varHeight = ((bounding.y2+sobj->y)>varHeight)?
                                                  bounding.y2+sobj->y:varHeight;
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
   bool result = true;
   int i, j;
   Square* saux;

   varHeight = 0.0;

   /* Verify if there is an actor!  */
   if(!actor)
   {
      return(false);
   }

   /* set Character Positions */
   GLfloat perX = varX;
   GLfloat perY = varY;
   GLfloat perZ = varZ;
   GLfloat perOrientation = actor->orientation + varAlpha;
   Square* perQuad = NULL;

   if(usePosition)
   {
      perX += actor->xPosition;
      perY += actor->yPosition;
      perZ += actor->zPosition;
      perQuad = actor->ocSquare;
   }

   GLfloat min[3],min2[3];
   GLfloat max[3],max2[3];

   GLfloat x[4],z[4];

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


   x[0] = actor->min[0];
   z[0] = actor->min[2];

   x[1] = actor->min[0];
   z[1] = actor->max[2]; 

   x[2] = actor->max[0];
   z[2] = actor->max[2];

   x[3] = actor->max[0];
   z[3] = actor->min[2];

   /* Rotate and translate the Bounding Box */
   rotTransBoundingBox(perOrientation, x, z, perX, actor->min[1] + perY, 
                       actor->max[1] + perY, perZ, min, max );

   /* Test map limits */
   if( (min[0]<2) || (min[2]<2) || 
       (max[0]>actualMap->getSizeX()*actualMap->squareSize()-2) || 
       (max[2]>actualMap->getSizeZ()*actualMap->squareSize()-2))
   {
      return(false);
   }

   /* Test Doors */
   door* door1 = actualMap->getFirstDoor();
   for(i=0; i < actualMap->getTotalDoors(); i++)
   {
      GLfloat minObj[3], maxObj[3];
      boundingBox boundPorta = door1->obj->getBoundingBox();
      GLfloat XA[4]; GLfloat ZA[4];
      XA[0] = boundPorta.x1;
      ZA[0] = boundPorta.z1;

      XA[1] = boundPorta.x1;
      ZA[1] = boundPorta.z2; 

      XA[2] = boundPorta.x2;
      ZA[2] = boundPorta.z2;

      XA[3] = boundPorta.x2;
      ZA[3] = boundPorta.z1;
      rotTransBoundingBox(door1->orientation+door1->delta, XA, ZA,
                          door1->x, 0.0,0.0,door1->z, 
                          minObj, maxObj);
      if(intercepts( min, max, minObj, maxObj))
      {
         return(false);
      }
      door1 = (door*)door1->getNext();
   }

   /* Test the actual square, since is BIG ! */
   min2[0] = perQuad->x1;
   min2[1] = 0;
   min2[2] = perQuad->z1;
   max2[0] = perQuad->x2;
   max2[1] = 400;
   max2[2] = perQuad->z2;
   if(intercepts(min,max,min2,max2))
   {
      result &= verifySquare(min,max,perQuad, varHeight, actor->yPosition);
      if(!result)
      {
         return(false);
      }
   }

 
   /* Test right walls */
   saux = actualMap->relativeSquare(perQuad->posX+1, perQuad->posZ);
   if(saux) 
   { 
      /* lest */
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(intercepts(min,max,min2,max2) )
      {
         result &= verifySquare(min,max,saux,varHeight, actor->yPosition);
         if(!result)
         {
            return(false);
         }
      }
      /* Northest */
      saux = actualMap->relativeSquare(perQuad->posX+1, perQuad->posZ-1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(intercepts(min,max,min2,max2) )
         {
            result &= verifySquare(min,max,saux,varHeight, actor->yPosition);
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
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(intercepts(min,max,min2,max2))
         {
            result &= verifySquare(min,max,saux,varHeight, actor->yPosition);
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
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(intercepts(min,max,min2,max2))
      {
         result &= verifySquare(min,max,saux,varHeight, actor->yPosition);
         if(!result) 
         {
            return(false);
         }
      }

      /* Northest */
      saux = actualMap->relativeSquare(perQuad->posX-1, perQuad->posZ-1);
      if( saux )
      {
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(intercepts(min,max,min2,max2) )
         {
            result &= verifySquare(min,max,saux,varHeight, actor->yPosition);
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
         min2[0] = saux->x1;
         min2[2] = saux->z1;
         max2[0] = saux->x2;
         max2[2] = saux->z2;
         if(intercepts(min,max,min2,max2))
         {
            result &=verifySquare(min,max,saux,varHeight, actor->yPosition);
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
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(intercepts(min,max,min2,max2) )
      { 
         /* south */
         result &= verifySquare(min,max,saux,varHeight, actor->yPosition);
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
      min2[0] = saux->x1;
      min2[2] = saux->z1;
      max2[0] = saux->x2;
      max2[2] = saux->z2;
      if(intercepts(min,max,min2,max2) )
      { 
         /* nort */
         result &= verifySquare(min,max,saux,varHeight, actor->yPosition);
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
               x[0] = pers->min[0];
               z[0] = pers->min[2];

               x[1] = pers->min[0];
               z[1] = pers->max[2]; 

               x[2] = pers->max[0];
               z[2] = pers->max[2];
 
               x[3] = pers->max[0];
               z[3] = pers->min[2];

               rotTransBoundingBox(pers->orientation, x, z,
                                   pers->xPosition, 
                                   pers->min[1]+pers->yPosition,
                                   pers->max[1]+pers->yPosition, 
                                   pers->zPosition, min2, max2 );

               if(intercepts( min, max, min2, max2))
               {
                  /* Do a more depth colision verify */
                  if(pers->depthCollision(pers->orientation, pers->xPosition, 
                                          pers->yPosition,
                                          pers->zPosition, min, max))
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

   nx = ((min[0] + max[0]) / 2);
   nz = ((min[2] + max[2]) / 2);
      
   return(result);
}


