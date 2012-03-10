/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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
}

/*********************************************************************
 *                             Destructor                            *
 *********************************************************************/
collision::~collision()
{
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
               if( ((colBox.max.y + sobj->y) - curHeight) <= 1.5)
               {
                  /* Can walk above the object */
                  varHeight = ((colBox.max.y+sobj->y)>varHeight)?
                     colBox.max.y+sobj->y:varHeight;
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
 *                           verifySquare                            *
 *********************************************************************/
bool collision::verifySquare(ray& acRay, Square* quad)
{
   boundingBox colBox;

   if(!(quad->flags & SQUARE_CAN_WALK))
   {
     /* Can't walk on square, collision! */
     return(false);
   }

   /* test with walls */
   int mur = 0;
   while((mur < MAX_WALLS ) && (quad->walls[mur] != NULL))
   {
      colBox.setMin(quad->walls[mur]->x1, 0, quad->walls[mur]->z1);
      colBox.setMax(quad->walls[mur]->x2, WALL_HEIGHT, 
            quad->walls[mur]->z2);
      if(colBox.intercepts(acRay))
      {
         return(false);
      }
      mur++;
   }

   /* test with objects */
   int ob = 0;
   objSquare* sobj = quad->getFirstObject();
   while( (ob < quad->getTotalObjects())) 
   {
      if(sobj->colision)
      {
         /* Get Bounding box */
         colBox = sobj->obj->scNode->getBoundingBox();

         if(colBox.intercepts(acRay))
         {
            return(false);
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
   return(canWalk(actor, actorBox, actor->scNode->getPosY(), perQuad, true, 
            varHeight, nx, nz));
}

/*********************************************************************
 *                              canWalk                              *
 *********************************************************************/
bool collision::canWalk(character* actor, boundingBox& actorBox, 
      GLfloat curPerY, Square* perQuad, bool checkChars, GLfloat& varHeight, 
      GLfloat& nx, GLfloat& nz)
{
   bool result = true;
   int i, j;
   Square* saux;
   boundingBox colBox;

   varHeight = 0.0;

   /* Test map limits */
   if( (actorBox.min.x < 2) || (actorBox.min.z < 2) || 
       (actorBox.max.x > actualMap->getSizeX()*actualMap->squareSize()-2) || 
       (actorBox.max.z > actualMap->getSizeZ()*actualMap->squareSize()-2))
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
   if(checkChars)
   {
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
   }

   nx = ((actorBox.min.x + actorBox.max.x) / 2);
   nz = ((actorBox.min.z + actorBox.max.z) / 2);
      
   return(result);
}


/*********************************************************************
 *                            verifyLine                             *
 *********************************************************************/
bool collision::canWalk(character* actor, GLfloat x, GLfloat y, GLfloat z, 
      GLfloat prevX, GLfloat prevY, GLfloat prevZ)
{
   boundingBox actorBox;

   /* Some unused returned values */
   GLfloat varHeight=0.0f, nX=0.0f, nZ=0.0f;

   /* Let's calculate the expanded BoundingBox */
   actorBox = actor->scNode->getModel()->getCrudeBoundingBox();
   /* Expand depending on direction */
   float dif;
   dif = fabs(x - prevX) / 2.0f;
   actorBox.min.x -= dif;
   actorBox.max.x += dif;
   dif = fabs(y - prevY) / 2.0f;
   actorBox.min.y -= dif;
   actorBox.max.y += dif;
   dif = fabs(z - prevZ) / 2.0f;
   actorBox.min.z -= dif;
   actorBox.max.z += dif;

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

   return(canWalk(actor, actorBox, midY, perQuad, true, varHeight, nX, nZ));
}

/*********************************************************************
 *                         characterAtSight                          *
 *********************************************************************/
bool collision::characterAtSight(character* actor, character* target)
{
   /* TODO: verify if actor is turned to the target */

   int x, z;
   Square* quad;

   bool result = true;

   ray sightRay;
   
   float difX = (target->scNode->getPosX() - actor->scNode->getPosX());
   float difZ = (target->scNode->getPosZ() - actor->scNode->getPosZ());
   sightRay.size = sqrt((difX*difX) + (difZ*difZ));

   sightRay.direction = vec3_t(difX, 0.0f, difZ);
   sightRay.direction.normalize();

   sightRay.origin = vec3_t(actor->scNode->getPosX(), 30.0f, 
         actor->scNode->getPosZ());

   /* Let's check each possible square */
   int initX = (int)floor(actor->scNode->getPosX() / (actualMap->squareSize()));
   int endX = (int)floor(target->scNode->getPosX() / (actualMap->squareSize()));
   int initZ = (int)floor(actor->scNode->getPosZ() / (actualMap->squareSize()));
   int endZ = (int)floor(target->scNode->getPosZ() / (actualMap->squareSize()));

#if 0
   glLineWidth(3);

     glBegin(GL_LINES);
        glVertex3f(sightRay.origin.x, sightRay.origin.y, sightRay.origin.z);
        glVertex3f(sightRay.origin.x+1000*sightRay.direction.x, 
                   sightRay.origin.y+1000*sightRay.direction.y,
                   sightRay.origin.z+1000*sightRay.direction.z);
     glEnd();
   glLineWidth(1);
   glFlush();
   SDL_GL_SwapBuffers();
   SDL_Delay(5000);
#endif

   if(endX < initX)
   {
      int tmp = initX;
      initX = endX;
      endX = tmp;
   }
   if(endZ < initZ)
   {
      int tmp = initZ;
      initZ = endZ;
      endZ = tmp;
   }

   /* FIXME: better if verify only in squares at the line, not at the 
    * rectangle as it is now... */
   for(x = initX; x <= endX; x++)
   {
      for(z = initZ; z <= endZ; z++)
      {
         quad = actualMap->relativeSquare(x, z);
         if(quad)
         {
            result &= verifySquare(sightRay, quad);
            if(!result) 
            {
               return(false);
            }
         }
      }
   }

   return(result);
}

/*********************************************************************
 *                         Static Members                            *
 *********************************************************************/
Map* collision::actualMap = NULL;
characterList* collision::NPCs = NULL;
characterList* collision::PCs = NULL;

